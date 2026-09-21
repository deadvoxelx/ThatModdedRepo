#include "Host/RubyLauncherHost.h"
#include "Loader.h"
#include "Registry/IDs.h"
#include "Registry/Item/ItemRegistry.h"
#include "Registry/Recipe/RecipeRegistry.h"
#include "Registry/Block/BlockRegistry.h"
#include "Registry/WorldGen/OreFeatureRegistry.h"
#include "Registry/WorldGen/TreeFeatureRegistry.h"
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

#include "../Server/Events/Item/ItemTickEvent.h"
#include "Item.h"
#include "Mob.h"
#include "ServerLevel.h"
#include "ServerPlayer.h"
#include "TileItem.h"

#include "json/json.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>

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
	g_loader->executeClientScripts("main");

	onStringTableReloaded();

	RecipeRegistry::finalize();
	BlockRegistry::finalizeDrops();
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
}

void RubyLoader::onModNetPacket(ServerPlayer *player, const std::string &channel, const std::string &data)
{
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

	g_loader->registerServerFunctions(server);
	g_loader->refreshServerScripts();
	g_loader->executeServerScripts("main");

	OreFeatureRegistry::finalize();
	TreeFeatureRegistry::finalize();
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
