#include "BlockRegistry.h"

#include "Item.h"
#include "Loader.h"
#include "Material.h"
#include "Registry/IDs.h"
#include "Tile.h"
#include "TileItem.h"
#include "Common/ModPaths.h"

#include <algorithm>

#include "Client/ModTile.h"
#include "Common/ModPaths.h"
#include "Host/RubyLauncherHost.h"
#include "Registry/Item/ItemRegistry.h"

#if defined(_WINDOWS64)
#include "Windows64Media/strings.h"
#else
#include "DurangoMedia/loc/strings.h"
#endif

std::vector<PendingBlockDrop> BlockRegistry::pendingDrops;

namespace
{
	const Tile::SoundType *soundTypeForName(const std::string &name)
	{
		if (name == "normal")	return Tile::SOUND_NORMAL;
		if (name == "stone")	return Tile::SOUND_STONE;
		if (name == "wood")		return Tile::SOUND_WOOD;
		if (name == "gravel")	return Tile::SOUND_GRAVEL;
		if (name == "grass")	return Tile::SOUND_GRASS;
		if (name == "metal")	return Tile::SOUND_METAL;
		if (name == "glass")	return Tile::SOUND_GLASS;
		if (name == "cloth")	return Tile::SOUND_CLOTH;
		if (name == "sand")		return Tile::SOUND_SAND;
		if (name == "snow")		return Tile::SOUND_SNOW;
		if (name == "ladder")	return Tile::SOUND_LADDER;
		if (name == "anvil")	return Tile::SOUND_ANVIL;
		return nullptr;
	}

	const int TILE_ID_MIN = 269;	// Dark Nustone Stairs; this has to be changed whenever i add new blocks to the base
	const int TILE_ID_MAX = 511;

	int tileIdMax = TILE_ID_MIN - 1;

	ModTileTraits traitsFor(const BlockDefinition &def)
	{
		ModTileTraits traits;

		switch (def.tool)
		{
		case BlockTool_None:
			traits.material = Material::dirt;
			traits.sound = Tile::SOUND_GRAVEL;
			traits.placeholderIconName = L"dirt";
			traits.craftingCategory = Item::eBaseItemType_structblock;
			traits.itemMaterial = Item::eMaterial_undefined;
			break;
		case BlockTool_Hatchet:
			traits.material = Material::wood;
			traits.sound = Tile::SOUND_WOOD;
			traits.placeholderIconName = L"planks_oak";
			traits.craftingCategory = Item::eBaseItemType_structwoodstuff;
			traits.itemMaterial = Item::eMaterial_wood;
			break;
		case BlockTool_Shovel:
			traits.material = Material::dirt;
			traits.sound = Tile::SOUND_SAND;
			traits.placeholderIconName = L"sand";
			traits.craftingCategory = Item::eBaseItemType_structblock;
			traits.itemMaterial = Item::eMaterial_sand;
			break;
		case BlockTool_Hoe:
			traits.material = Material::dirt;
			traits.sound = Tile::SOUND_GRASS;
			traits.placeholderIconName = L"dirt";
			traits.craftingCategory = Item::eBaseItemType_structblock;
			traits.itemMaterial = Item::eMaterial_undefined;
			break;
		case BlockTool_Pickaxe:
		default:
			traits.material = Material::stone;
			traits.sound = Tile::SOUND_STONE;
			traits.placeholderIconName = L"stone";
			traits.craftingCategory = Item::eBaseItemType_structblock;
			traits.itemMaterial = Item::eMaterial_stone;
			break;
		}

	traits.hardness = def.hardness;
		traits.resistance = def.resistance;

		if (!def.sound.empty())
		{
			const Tile::SoundType *soundType = soundTypeForName(def.sound);
			if (soundType != nullptr)
			{
				traits.sound = soundType;
			}
			else
			{
				Loader::_debugPrint("unknown block sound '" + def.sound + "' (expected normal, stone, wood, gravel, grass, metal, glass, cloth, sand, snow, ladder or anvil); using the tool default");
			}
		}

		traits.dropId = def.dropId;
		traits.dropAux = def.dropAux;
		traits.dropMin = def.dropMin;
		traits.dropMax = def.dropMax;
		traits.dropChance = def.dropChance;
		traits.dropNothing = def.dropNothing;

		return traits;
	}
}

void BlockDefinition::finalizeDrops(const std::string &modId, const std::string &context)
{
	if (dropNothing || drop.empty())
	{
		return;
	}

	IDMapping::MappedItem mapped = IDMapping::get()->getID(drop);
	if (mapped.id != 0)
	{
		dropId = mapped.id;
		dropAux = mapped.aux;
		return;
	}

	dropPending = true;
}

BlockDefinition::BlockDefinition(sol::table table)
{
	sol::optional<float> hardnessValue = table["hardness"];
	if (hardnessValue) hardness = hardnessValue.value();

	sol::optional<float> resistanceValue = table["resistance"];
	if (resistanceValue) resistance = resistanceValue.value();

	sol::optional<EBlockTool> toolValue = table["tool"];
	if (toolValue) tool = toolValue.value();

	sol::optional<std::string> topTextureValue = table["topTexture"];
	if (topTextureValue) topTexture = topTextureValue.value();

	sol::optional<std::string> bottomTextureValue = table["bottomTexture"];
	if (bottomTextureValue) bottomTexture = bottomTextureValue.value();

	sol::optional<bool> transparentValue = table["transparent"];
	if (transparentValue) transparent = transparentValue.value();

	sol::optional<std::string> soundValue = table["sound"];
	if (soundValue) sound = soundValue.value();

	sol::optional<std::string> dropValue = table["drop"];
	if (dropValue) drop = dropValue.value();

	sol::optional<int> dropMinValue = table["dropMin"];
	if (dropMinValue) dropMin = dropMinValue.value();

	sol::optional<int> dropMaxValue = table["dropMax"];
	if (dropMaxValue) dropMax = dropMaxValue.value();

	sol::optional<float> dropChanceValue = table["dropChance"];
	if (dropChanceValue) dropChance = dropChanceValue.value();

	sol::optional<bool> dropNothingValue = table["dropNothing"];
	if (dropNothingValue) dropNothing = dropNothingValue.value();
	if (drop == "nothing") dropNothing = true;
}

int BlockRegistry::maxBlockCount() {
	return TILE_ID_MAX - TILE_ID_MIN + 1;
}

int BlockRegistry::nextItemId() {
	do {
		tileIdMax += 1;
	} while (tileIdMax <= TILE_ID_MAX && (Tile::tiles[tileIdMax] != nullptr || Item::items[tileIdMax] != nullptr));

	return (tileIdMax > TILE_ID_MAX) ? -1 : tileIdMax;
}

int BlockRegistry::registerBlock(const std::wstring& path, const std::string& id, const std::string& name, std::string modId, const std::string& texturePath, const BlockDefinition& def) {
	int nameId = ItemRegistry::nextItemNameId();
	int tileId = BlockRegistry::nextItemId();

	if (tileId == -1) {
		Loader::_debugPrint("out of block ids (" + std::to_string(TILE_ID_MIN) + "-" + std::to_string(TILE_ID_MAX) + "), '" + id + "' was not registered");
		return -1;
	}

	std::wstring wname(name.begin(), name.end());

	BlockDefinition defCopy = def;
	defCopy.finalizeDrops(modId, modId + ":" + id);
	if (defCopy.dropPending)
	{
		defCopy.dropId = -1;
		PendingBlockDrop pending;
		pending.tile = nullptr;
		pending.drop = defCopy.drop;
		pending.context = modId + ":" + id;
		pendingDrops.push_back(pending);
	}
	ModTileTraits traits = traitsFor(defCopy);

	if (!texturePath.empty())
	{
		RubyModTexture request;
		request.iconName = rubyModTextureIconName(modId, id);
		request.filePath = RubyPaths::resolveModTexturePath(RubyPaths::toNarrow(path), texturePath);
		request.fallback = RubyPaths::toNarrow(traits.placeholderIconName);
		request.block = true;

		RubyLoader::addModTexture(request);
		traits.textureIconName = RubyPaths::toWide(request.iconName);

		if (!def.topTexture.empty())
		{
			RubyModTexture topRequest;
			topRequest.iconName = request.iconName + "_top";
			topRequest.filePath = RubyPaths::resolveModTexturePath(RubyPaths::toNarrow(path), def.topTexture);
			topRequest.fallback = request.iconName;
			topRequest.block = true;

			RubyLoader::addModTexture(topRequest);
			traits.topTextureIconName = RubyPaths::toWide(topRequest.iconName);
		}

		if (!def.bottomTexture.empty())
		{
			RubyModTexture bottomRequest;
			bottomRequest.iconName = request.iconName + "_bottom";
			bottomRequest.filePath = RubyPaths::resolveModTexturePath(RubyPaths::toNarrow(path), def.bottomTexture);
			bottomRequest.fallback = request.iconName + (def.topTexture.empty() ? "" : "_top");
			bottomRequest.block = true;

			RubyLoader::addModTexture(bottomRequest);
			traits.bottomTextureIconName = RubyPaths::toWide(bottomRequest.iconName);
		}
	}

	traits.transparent = def.transparent;

	Tile *tile = new ModTile(tileId, traits);
	tile->setDescriptionId(nameId);

	if (defCopy.dropPending && !pendingDrops.empty())
	{
		pendingDrops.back().tile = static_cast<ModTile *>(tile);
	}

	Item::items[tileId] = (new TileItem(tileId - 512))
		->setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName)
		->setDescriptionId(nameId)
		->setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial)
		->setUseDescriptionId(IDS_DESC_STONE);

	ItemRegistry::langList[nameId] = wname;
	IDMapping::get()->add(modId, id, true, tileId);

	RubyCreative::addEntry(tileId, 0, RubyCreativeGroup_BuildingBlocks);
	Loader::_debugPrint("registered block " + modId + ":" + id + " as tile " + std::to_string(tileId) + " (hardness " + std::to_string(def.hardness) + ")" + (texturePath.empty() ? "" : ", texture '" + texturePath + "'"));

	return tileId;
}

void BlockRegistry::finalizeDrops()
{
	for (const PendingBlockDrop &pending : pendingDrops)
	{
		if (pending.tile == nullptr) continue;

		IDMapping::MappedItem mapped = IDMapping::get()->getID(pending.drop);
		if (mapped.id != 0)
		{
			pending.tile->setDropOverride(mapped.id, mapped.aux);
		}
		else
		{
			Loader::_debugPrint("block '" + pending.context + "': unknown drop '" + pending.drop + "'; dropping itself instead");
		}
	}
	pendingDrops.clear();
}
