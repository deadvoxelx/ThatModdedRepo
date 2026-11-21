#include "Host/RubyLauncherHost.h"
#include "Loader.h"
#include "Registry/IDs.h"
#include "Registry/Item/ItemRegistry.h"
#include "Registry/Recipe/RecipeRegistry.h"
#include "Registry/Block/BlockRegistry.h"
#include "Registry/WorldGen/OreFeatureRegistry.h"
#include "Registry/WorldGen/TreeFeatureRegistry.h"
#include "Registry/WorldGen/FeatureRegistry.h"
#include "Registry/Biome/BiomeRegistry.h"
#include "Registry/Block/PlantRegistry.h"
#include "Registry/Dimension/DimensionRegistry.h"
#include "Registry/Dimension/PortalRegistry.h"
#include "Common/EventSystem/EventBus.h"
#include "Common/ModNetBus.h"
#include "Common/ModPaths.h"
#include "Server/Events/Item/ItemCompleteUseEvent.h"
#include "Server/Events/Item/ItemInteractEvent.h"
#include "Server/Events/Item/ItemInteractEntityEvent.h"
#include "Server/Events/Player/PlayerBlockBreakEvent.h"
#include "Server/Events/Player/PlayerBlockPlaceEvent.h"
#include "Server/Events/Player/PlayerConnectionEvent.h"
#include "Server/Events/Player/PlayerFlightEndedEvent.h"
#include "Server/Events/Player/PlayerFlightStartedEvent.h"
#include "Server/Events/Player/PlayerJoinEvent.h"
#include "Server/Events/Player/PlayerHurtEvent.h"

#include "../../Minecraft.Client/ServerLevel.h"
#include "../Server/Events/Block/BlockTickEvent.h"
#include "../Server/Events/Block/BlockUseEvent.h"
#include "../Server/Events/Item/ItemTickEvent.h"
#include "Item.h"
#include "Mob.h"
#include "ServerLevel.h"
#include "ServerPlayer.h"
#include "TileItem.h"
#include "ClientConnection.h"
#include "CustomPayloadPacket.h"
#include "DisconnectPacket.h"
#include "Minecraft.h"
#include "PlayerConnection.h"

#include "json/json.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

namespace
{
	Loader *g_loader = nullptr;
	std::vector<RubyCreativeEntry> g_creativeEntries;
	std::vector<RubyModTexture> g_modTextures;

	struct ArmorSet
	{
		std::string name;
		std::string modFolder;
		bool hasOverlay;
	};

	const int ARMOR_SET_FIRST_INDEX = 9;
	std::vector<ArmorSet> g_armorSets;

	const char *REGISTRY_CHANNEL = "registry";
	const char *REGISTRY_REPLY_CHANNEL = "registry_reply";
	const char *REGISTRY_HEADER = "ruby-registry 1";

	using RegistryEntries = std::map<std::string, std::pair<int, std::string>>;

	std::string registrySnapshot()
	{
		std::vector<std::string> lines;

		for (const auto &entry : IDMapping::get()->stringToID)
		{
			lines.push_back(entry.first + "\t" + std::to_string(entry.second.id) + "\t" + (entry.second.isBlock ? "block" : "item"));
		}

		std::sort(lines.begin(), lines.end());

		std::string snapshot = std::string(REGISTRY_HEADER) + "\n";

		for (const std::string &line : lines) snapshot += line + "\n";

		return snapshot;
	}

	bool parseRegistrySnapshot(const std::string &snapshot, RegistryEntries &entries)
	{
		entries.clear();

		std::istringstream stream(snapshot);
		std::string line;

		if (!std::getline(stream, line)) return false;

		if (line != REGISTRY_HEADER) return false;

		while (std::getline(stream, line))
		{
			const size_t firstTab = line.find('\t');
			if (firstTab == std::string::npos) continue;

			const size_t secondTab = line.find('\t', firstTab + 1);
			if (secondTab == std::string::npos) continue;

			try
			{
				entries[line.substr(0, firstTab)] = { std::stoi(line.substr(firstTab + 1, secondTab - firstTab - 1)), line.substr(secondTab + 1) };
			}
			catch (const std::exception &)
			{
			}
		}
		return true;
	}

	std::string registryMismatch(const RegistryEntries &remote, const RegistryEntries &local)
	{
		std::vector<std::string> differences;

		for (const auto &entry : local)
		{
			auto found = remote.find(entry.first);

			if (found == remote.end()) differences.push_back("'" + entry.first + "' only exists here");
			else if (found->second.first != entry.second.first) differences.push_back("'" + entry.first + "' is id " + std::to_string(entry.second.first) + " here but " + std::to_string(found->second.first) + " there");
			else if (found->second.second != entry.second.second) differences.push_back("'" + entry.first + "' is a " + entry.second.second + " here but a " + found->second.second + " there");
		}

		for (const auto &entry : remote)
		{
			if (local.find(entry.first) == local.end()) differences.push_back("'" + entry.first + "' only exists on the other side");
		}

		if (differences.empty()) return std::string();

		const size_t limit = 8;
		std::string description;

		for (size_t i = 0; i < differences.size() && i < limit; ++i)
		{
			if (i > 0) description += "; ";
			description += differences[i];
		}

		if (differences.size() > limit) description += "; and " + std::to_string(differences.size() - limit) + " more";

		return description;
	}

	ClientConnection *primaryConnection(Minecraft *minecraft)
	{
		if (minecraft == nullptr || minecraft->localplayers[0] == nullptr) return nullptr;

		return minecraft->getConnection(0);
	}

	void sendRegistrySnapshot(PlayerConnection &connection, const char *channel, const std::string &snapshot)
	{
		connection.send(std::make_shared<CustomPayloadPacket>(RubyPaths::toWide(std::string("ruby:") + channel), stringToByteArray(snapshot)));
	}

	RegistryEntries localRegistryEntries()
	{
		RegistryEntries entries;

		parseRegistrySnapshot(registrySnapshot(), entries);

		return entries;
	}

	void checkClientRegistry(ServerPlayer *player, const std::string &clientSnapshot)
	{
		RegistryEntries clientEntries;

		if (!parseRegistrySnapshot(clientSnapshot, clientEntries))
		{
			Loader::_debugPrint("ignoring a mod id payload without the '" + std::string(REGISTRY_HEADER) + "' header");
			return;
		}

		const RegistryEntries serverEntries = localRegistryEntries();

		if (serverEntries.empty() || clientEntries.empty())
		{
			Loader::_debugPrint(std::string("skipping the mod id check, ") + (serverEntries.empty() ? "this server" : "the client") + " registered no mod ids");
			return;
		}

		const std::string mismatch = registryMismatch(clientEntries, serverEntries);

		if (mismatch.empty())
		{
			Loader::_debugPrint("client mod ids match the server's");
			return;
		}

		Loader::_debugPrint("refusing a client whose mod ids do not match: " + mismatch);

		if (player->connection == nullptr) return;

		sendRegistrySnapshot(*player->connection, REGISTRY_REPLY_CHANNEL, registrySnapshot());
		player->connection->disconnect(DisconnectPacket::eDisconnect_Kicked);
	}

	void checkServerRegistry(const std::string &serverSnapshot)
	{
		RegistryEntries serverEntries;

		if (!parseRegistrySnapshot(serverSnapshot, serverEntries))
		{
			Loader::_debugPrint("ignoring a mod id payload without the '" + std::string(REGISTRY_HEADER) + "' header");
			return;
		}

		const RegistryEntries clientEntries = localRegistryEntries();

		if (serverEntries.empty() || clientEntries.empty())
		{
			Loader::_debugPrint(std::string("skipping the mod id check, ") + (clientEntries.empty() ? "this client" : "the server") + " registered no mod ids");
			return;
		}

		const std::string mismatch = registryMismatch(serverEntries, clientEntries);

		if (mismatch.empty())
		{
			Loader::_debugPrint("this client's mod ids match the server's");
			return;
		}

		Loader::_debugPrint("this client does not match the server's mod ids: " + mismatch);

		ClientConnection *connection = primaryConnection(Minecraft::GetInstance());

		if (connection != nullptr && !connection->isClosed()) connection->onDisconnect(DisconnectPacket::eDisconnect_Kicked, nullptr);
	}

	std::string toForwardSlashes(const std::string &path)
	{
		std::string result = path;
		std::replace(result.begin(), result.end(), '\\', '/');
		return result;
	}
}

void RubyCreative::addEntry(int id, int aux, RubyCreativeGroup group)
{
	g_creativeEntries.push_back({ id, aux, group });
}

void RubyCreative::removeEntry(int id)
{
	g_creativeEntries.erase(std::remove_if(g_creativeEntries.begin(), g_creativeEntries.end(),
		[id](const RubyCreativeEntry &entry) { return entry.id == id; }), g_creativeEntries.end());
}

std::vector<RubyCreativeEntry> RubyLoader::getCreativeEntries()
{
	return g_creativeEntries;
}

void RubyLoader::addModTexture(const RubyModTexture &texture)
{
	g_modTextures.push_back(texture);
}

const std::vector<RubyModTexture> &RubyLoader::getModTextures()
{
	return g_modTextures;
}

int RubyLoader::registerArmorSet(const std::string &setName, const std::string &modFolderName, int vanillaFallback)
{
	if (setName.empty()) return vanillaFallback;

	for (size_t i = 0; i < g_armorSets.size(); ++i)
	{
		if (g_armorSets[i].name == setName && g_armorSets[i].modFolder == modFolderName) return ARMOR_SET_FIRST_INDEX + static_cast<int>(i);
	}

	const bool hasOverlay = !RubyPaths::resolveModTexturePath(modFolderName, "armor/" + setName + "_1_b.png").empty() || !RubyPaths::resolveModTexturePath(modFolderName, setName + "_1_b.png").empty();

	g_armorSets.push_back(ArmorSet{ setName, modFolderName, hasOverlay });
	return ARMOR_SET_FIRST_INDEX + static_cast<int>(g_armorSets.size()) - 1;
}

std::wstring RubyLoader::getArmorSetName(int modelIndex)
{
	const int i = modelIndex - ARMOR_SET_FIRST_INDEX;

	if (i < 0 || i >= static_cast<int>(g_armorSets.size())) return std::wstring();

	return RubyPaths::toWide(g_armorSets[i].name);
}

bool RubyLoader::armorSetHasOverlay(int modelIndex)
{
	const int i = modelIndex - ARMOR_SET_FIRST_INDEX;

	if (i < 0 || i >= static_cast<int>(g_armorSets.size())) return false;

	return g_armorSets[i].hasOverlay;
}

bool RubyLoader::resolveArmorTexture(const std::wstring &relativeName, std::wstring &drive, std::wstring &texturePath)
{
	const std::string name = RubyPaths::toNarrow(relativeName);

	if (name.compare(0, 6, "armor/") != 0) return false;

	const std::string bare = name.substr(6);

	for (const ArmorSet &set : g_armorSets)
	{
		if (bare.compare(0, set.name.size() + 1, set.name + "_") != 0) continue;

		drive = RubyPaths::modsRootW();
		drive += L"/";
		drive += RubyPaths::toWide(toForwardSlashes(set.modFolder));
		drive += L"/";

		if (!RubyPaths::resolveModTexturePath(set.modFolder, name).empty())
		{
			texturePath = L"/" + relativeName;
			return true;
		}

		if (!RubyPaths::resolveModTexturePath(set.modFolder, bare).empty())
		{
			texturePath = RubyPaths::toWide(bare);
			texturePath.insert(texturePath.begin(), L'/');
			return true;
		}
	}

	return false;
}

void RubyLoader::onClientBoot()
{
	if (g_loader != nullptr) return;

	g_loader = new Loader();
	g_creativeEntries.clear();
	g_modTextures.clear();
	g_armorSets.clear();
	ModNetBus::Get().clearClient();

	IDMapping::get()->init();

	g_loader->collectMods();
	g_loader->refreshClientScripts();
	g_loader->executeClientScripts("main", true);
    g_loader->refreshStartupScripts();
    g_loader->executeStartupScripts("main", true);

	onStringTableReloaded();

	RecipeRegistry::finalize();
	BlockRegistry::finalizeDrops();
	PlantRegistry::finalize();
	BiomeRegistry::finalize();
	DimensionRegistry::finalize();
	PortalRegistry::finalize();

	IDMapping::get()->saveRegistry();
}

void RubyLoader::onStringTableReloaded()
{
	if (g_loader == nullptr || app.getStringTable() == nullptr) return;

	ItemRegistry::changeLang(*app.getStringTable());
}

void RubyLoader::onServerTick(MinecraftServer *server)
{
	if (g_loader == nullptr) return;
	g_loader->tickServer(server);
}

void RubyLoader::onClientTick()
{
	if (g_loader == nullptr) return;
	g_loader->tickClient();

	static bool registrySent = false;

	Minecraft *minecraft = Minecraft::GetInstance();
	ClientConnection *connection = primaryConnection(minecraft);

	if (minecraft == nullptr || minecraft->level == nullptr || connection == nullptr || connection->isClosed())
	{
		registrySent = false;
		return;
	}

	if (registrySent) return;

	connection->send(std::make_shared<CustomPayloadPacket>(RubyPaths::toWide(std::string("ruby:") + REGISTRY_CHANNEL), stringToByteArray(registrySnapshot())));
	registrySent = true;
}

void RubyLoader::onModNetPacket(ServerPlayer *player, const std::string &channel, const std::string &data)
{
	if (player != nullptr && channel == REGISTRY_CHANNEL)
	{
		checkClientRegistry(player, data);
		return;
	}

	if (player == nullptr && channel == REGISTRY_REPLY_CHANNEL)
	{
		checkServerRegistry(data);
		return;
	}

	if (player != nullptr)
	{
		ModNetBus::Get().fireServer(player, channel, data);
	}
	else
	{
		ModNetBus::Get().fireClient(channel, data);
	}
}

void RubyLoader::onServerStart(MinecraftServer *server)
{
	if (g_loader == nullptr) return;

	EventBus::Get().clearServerListeners();
	ModNetBus::Get().clearServer();
	OreFeatureRegistry::reset();
	TreeFeatureRegistry::reset();
	FeatureRegistry::reset();

	g_loader->registerServerFunctions(server);
	g_loader->refreshServerScripts();
	g_loader->executeServerScripts("main", true);

	BiomeRegistry::finalize();
	DimensionRegistry::finalize();
	PortalRegistry::finalize();
	PlantRegistry::finalize();
	OreFeatureRegistry::finalize();
	TreeFeatureRegistry::finalize();
	FeatureRegistry::finalize();

	IDMapping::get()->saveRegistry();
}

int RubyLoader::getModCount()
{
	return (g_loader == nullptr) ? 0 : static_cast<int>(g_loader->mods_.size());
}

const wchar_t *RubyLoader::getModsFolder()
{
	return RubyPaths::modsRootW().c_str();
}

namespace
{
	namespace fs = std::filesystem;

	unsigned long long folderSize(const fs::path &path)
	{
		unsigned long long total = 0;
		std::error_code error;

		for (const auto &entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied, error))
		{
			if (entry.is_regular_file(error))
			{
				total += entry.file_size(error);
			}
		}
		return total;
	}

	bool isModLoaded(const std::string &folderName)
	{
		if (g_loader == nullptr) return false;

		for (const auto &entry : g_loader->mods_)
		{
			if (std::string(entry.second.getPathName()) == folderName) return true;
		}
		return false;
	}
}

std::vector<RubyModInfo> RubyLoader::getInstalledMods()
{
	std::vector<RubyModInfo> mods;
	std::error_code error;

	if (!fs::exists(RubyPaths::modsRoot(), error)) return mods;

	for (const auto &entry : fs::directory_iterator(RubyPaths::modsRoot(), error))
	{
		if (!entry.is_directory(error)) continue;

		RubyModInfo info;
		info.folderName = entry.path().filename().string();
		info.size = folderSize(entry.path());

		fs::path manifestPath = entry.path() / "manifest.json";

		if (fs::exists(manifestPath, error))
		{
			try
			{
				std::ifstream file(manifestPath);
				nlohmann::json json;
				file >> json;

				info.hasManifest = true;
				info.name = json.value("name", info.folderName);
				info.version = json.value("version", std::string());
				info.modId = json.value("modId", std::string());
			}
			catch (const std::exception &e)
			{
				info.error = std::string("invalid manifest.json: ") + e.what();
			}
		}
		else
		{
			info.name = info.folderName;
			info.error = "no manifest.json";
		}

		if (info.name.empty()) info.name = info.folderName;

		info.loaded = isModLoaded(info.folderName);

		if (!info.loaded && info.error.empty()) info.error = "not loaded (installed after the game started, restart to load it)";

		mods.push_back(info);
	}

	std::sort( mods.begin(), mods.end(), [](const RubyModInfo &a, const RubyModInfo &b) { return a.name < b.name; } );

	return mods;
}

bool RubyLoader::firePlayerBlockBreak(ServerPlayer *player, int x, int y, int z, int blockId, int blockAux)
{
	PlayerBlockBreakEvent event(player, x, y, z, blockId, blockAux);
	return EventBus::Get().fire(event);
}

bool RubyLoader::firePlayerBlockPlace(ServerPlayer *player, int x, int y, int z, ItemInstance *item, int targetTile)
{
	int blockId = targetTile;

	if (item != nullptr)
	{
		if (Item *held = item->getItem())
		{
			if (TileItem *tileItem = dynamic_cast<TileItem *>(held))
			{
				blockId = tileItem->getTileId();
			}
		}
	}

	PlayerBlockPlaceEvent event(player, x, y, z, blockId);
	return EventBus::Get().fire(event);
}

bool RubyLoader::firePlayerHurt(ServerPlayer* plr, ServerLevel* level, DamageSource* src, int dmg)
{
    if (plr == nullptr || level == nullptr) return false;

    PlayerHurtEvent event(plr, level, src, dmg);
    return EventBus::Get().fire(event);
}

void RubyLoader::fireItemCompleteUse(ItemInstance *item, ServerLevel *level, ServerPlayer *player)
{
	if (item == nullptr || player == nullptr) return;

	ItemCompleteUseEvent event(item, level, player, IDMapping::get()->getByID(item->id, item->getAuxValue()));
    EventBus::Get().fire(event);
}

void RubyLoader::fireItemInteract(ItemInstance *item, ServerLevel *level, ServerPlayer *player)
{
	if (item == nullptr) return;

	ItemInteractEvent event(item, level, player);
	EventBus::Get().fire(event);
}

void RubyLoader::fireItemTick(ItemInstance *item, ServerLevel *level, ServerPlayer *player, int slot)
{
    if (item == nullptr) return;

    ItemTickEvent event(item, level, player, slot);
    EventBus::Get().fire(event);
}

bool RubyLoader::hasBlockTickListeners()
{
    return EventBus::Get().hasListeners(BlockTickEvent::NAME);
}

void RubyLoader::fireBlockTick(int x, int y, int z, int blockId, ServerLevel* level)
{
    if (level == nullptr) return;

    BlockTickEvent event(x, y, z, blockId, level);
    EventBus::Get().fire(event);
}

void RubyLoader::fireBlockUse(int x, int y, int z, int blockId, ServerLevel* level, ServerPlayer* player) {
    if (level == nullptr || player == nullptr) return;

    BlockUseEvent event(x, y, z, blockId, level, player);
    EventBus::Get().fire(event);
}

void RubyLoader::fireItemInteractEntity(ItemInstance *item, Entity *entity)
{
	Mob *mob = dynamic_cast<Mob *>(entity);

	if (item == nullptr || mob == nullptr) return;

	ItemInteractEntityEvent event(item, mob);
	EventBus::Get().fire(event);
}

void RubyLoader::firePlayerConnection(ServerPlayer *player)
{
	PlayerConnectionEvent event(player);
	EventBus::Get().fire(event);
}

void RubyLoader::firePlayerJoin(ServerPlayer *player)
{
	PlayerJoinEvent event(player);
	EventBus::Get().fire(event);
}

void RubyLoader::fireFlightStarted(ServerPlayer *player)
{
	PlayerFlightStartedEvent event(player);
	EventBus::Get().fire(event);
}

void RubyLoader::fireFlightEnded(ServerPlayer *player)
{
	PlayerFlightEndedEvent event(player);
	EventBus::Get().fire(event);
}
