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
#include "Client/ModTileShapes.h"
#include "StoneSlabTileItem.h"
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

	int baseItemTypeForName(const std::string &name)
	{
		if (name == "planks") return Item::eBaseItemType_structwoodstuff;
		return -1;
	}

	int lightLevelFor(const std::string &context, const char *option, int value, int fallback)
	{
		if (value >= 0 && value <= 15) return value;

		Loader::_debugPrint("block '" + context + "': " + option + " must be from 0 to 15, using " + std::to_string(fallback));
		return fallback;
	}

	ModTileTraits traitsFor(const BlockDefinition &def, const std::string &context)
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

	if (def.baseItemType >= 0) traits.craftingCategory = def.baseItemType;

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

		const int defaultBlocking = def.transparent ? 1 : 15;
		traits.lightEmission = lightLevelFor(context, "lightEmission", def.lightEmission, 0);
		traits.lightBlocking = (def.lightBlocking < 0) ? defaultBlocking : lightLevelFor(context, "lightBlocking", def.lightBlocking, defaultBlocking);

		return traits;
	}

	enum EBlockShape
	{
		BlockShape_Block = 0,
		BlockShape_Slab,
		BlockShape_Stairs,
		BlockShape_Fence,
		BlockShape_FenceGate,
	};

	EBlockShape shapeForName(const std::string &name)
	{
		if (name.empty() || name == "block" || name == "cube")
		{
			return BlockShape_Block;
		}
		if (name == "slab" || name == "halfSlab" || name == "half_slab")
		{
			return BlockShape_Slab;
		}
		if (name == "stairs" || name == "stair")
		{
			return BlockShape_Stairs;
		}
		if (name == "fence")
		{
			return BlockShape_Fence;
		}
		if (name == "fenceGate" || name == "fence_gate" || name == "gate")
		{
			return BlockShape_FenceGate;
		}

		Loader::_debugPrint("unknown block shape '" + name + "' (expected block, slab, stairs, fence or fenceGate); using block");
		return BlockShape_Block;
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

	sol::optional<std::string> baseItemValue = table["baseItem"];
	if (baseItemValue) baseItemType = baseItemTypeForName(baseItemValue.value());

	sol::optional<std::string> shapeValue = table["shape"];
	if (shapeValue) shape = shapeValue.value();

	sol::optional<int> lightEmissionValue = table["lightEmission"];
	if (lightEmissionValue) lightEmission = lightEmissionValue.value();

	sol::optional<int> lightBlockingValue = table["lightBlocking"];
	if (lightBlockingValue) lightBlocking = lightBlockingValue.value();
}

int BlockRegistry::maxBlockCount() {
	return TILE_ID_MAX - TILE_ID_MIN + 1;
}

bool BlockRegistry::isBlockIdFree(int id) {
	return id >= TILE_ID_MIN && id <= TILE_ID_MAX && Tile::tiles[id] == nullptr && Item::items[id] == nullptr;
}

int BlockRegistry::nextItemId(const std::string &key) {
	IDMapping *mapping = IDMapping::get();
	do {
		tileIdMax += 1;
	} while (tileIdMax <= TILE_ID_MAX && (Tile::tiles[tileIdMax] != nullptr || Item::items[tileIdMax] != nullptr || mapping->isReservedByOther(tileIdMax, key)));

	return (tileIdMax > TILE_ID_MAX) ? -1 : tileIdMax;
}

int BlockRegistry::registerBlock(const std::wstring& path, const std::string& id, const std::string& name, std::string modId, const std::string& texturePath, const BlockDefinition& def) {
	int nameId = ItemRegistry::nextItemNameId();

	const std::string context = modId + ":" + id;
	int tileId = IDMapping::get()->pinnedId(context);
	if (tileId != -1 && !isBlockIdFree(tileId))
	{
		Loader::_debugPrint("block '" + context + "' was pinned to id " + std::to_string(tileId) + " but that slot is already taken; allocating a new id");
		tileId = -1;
	}

	const bool pinned = (tileId != -1);
	if (!pinned) tileId = BlockRegistry::nextItemId(context);

	if (tileId == -1) {
		Loader::_debugPrint("out of block ids (" + std::to_string(TILE_ID_MIN) + "-" + std::to_string(TILE_ID_MAX) + "), '" + context + "' was not registered");
		return -1;
	}

	std::wstring wname(name.begin(), name.end());

	BlockDefinition defCopy = def;
	defCopy.finalizeDrops(modId, modId + ":" + id);
	if (defCopy.dropPending)
	{
		defCopy.dropId = -1;
		PendingBlockDrop pending;
		pending.sink = nullptr;
		pending.drop = defCopy.drop;
		pending.context = modId + ":" + id;
		pendingDrops.push_back(pending);
	}
	ModTileTraits traits = traitsFor(defCopy, context);

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

	ModDropSink *dropSink = nullptr;
	Item *item = nullptr;

	switch (shapeForName(def.shape))
	{
	case BlockShape_Slab:
	{
		const std::string fullKey = context + "_double";
		int fullTileId = IDMapping::get()->pinnedId(fullKey);

		if (fullTileId != -1 && !isBlockIdFree(fullTileId))
		{
			Loader::_debugPrint("slab '" + context + "': its double form was pinned to id " + std::to_string(fullTileId) + " but that slot is already taken; allocating a new id");
			fullTileId = -1;
		}

		if (fullTileId == -1) fullTileId = nextItemId(fullKey);

		if (fullTileId == -1)
		{
			Loader::_debugPrint("out of block ids (" + std::to_string(TILE_ID_MIN) + "-" + std::to_string(TILE_ID_MAX) + "), slab '" + context + "' needs a second id for its double form and was not registered");
			return -1;
		}

		ModSlabTile *halfTile = new ModSlabTile(tileId, false, tileId, fullTileId, traits);
		ModSlabTile *fullTile = new ModSlabTile(fullTileId, true, tileId, fullTileId, traits);

		halfTile->setDescriptionId(nameId);
		fullTile->setDescriptionId(nameId);

		IDMapping::get()->add(modId, id + "_double", true, fullTileId);

		dropSink = halfTile;
		item = (new StoneSlabTileItem(tileId - 512, halfTile, fullTile, false))
			->setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName)
			->setDescriptionId(nameId)
			->setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial)
			->setUseDescriptionId(IDS_DESC_STONE);
		break;
	}
	case BlockShape_Stairs:
	{
		ModStairTile *stairTile = new ModStairTile(tileId, traits);
		stairTile->setDescriptionId(nameId);
		dropSink = stairTile;
		break;
	}
	case BlockShape_Fence:
	{
		ModFenceTile *fenceTile = new ModFenceTile(tileId, traits);
		fenceTile->setDescriptionId(nameId);
		dropSink = fenceTile;
		break;
	}
	case BlockShape_FenceGate:
	{
		ModFenceGateTile *gateTile = new ModFenceGateTile(tileId, traits);
		gateTile->setDescriptionId(nameId);
		dropSink = gateTile;
		break;
	}
	case BlockShape_Block:
	default:
	{
		ModTile *modTile = new ModTile(tileId, traits);
		modTile->setDescriptionId(nameId);
		dropSink = modTile;
		break;
	}
	}

	if (item == nullptr)
	{
		item = (new TileItem(tileId - 512))
			->setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName)
			->setDescriptionId(nameId)
			->setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial)
			->setUseDescriptionId(IDS_DESC_STONE);
	}

	Item::items[tileId] = item;

	if (defCopy.dropPending && !pendingDrops.empty())
	{
		pendingDrops.back().sink = dropSink;
	}

	ItemRegistry::langList[nameId] = wname;
	IDMapping::get()->add(modId, id, true, tileId);

	RubyCreative::addEntry(tileId, 0, RubyCreativeGroup_BuildingBlocks);
	Loader::_debugPrint("registered block " + context + " as tile " + std::to_string(tileId) + (pinned ? " (pinned)" : "") + " (hardness " + std::to_string(def.hardness) + ")" + (texturePath.empty() ? "" : ", texture '" + texturePath + "'"));

	return tileId;
}

void BlockRegistry::finalizeDrops()
{
	for (const PendingBlockDrop &pending : pendingDrops)
	{
		if (pending.sink == nullptr) continue;

		IDMapping::MappedItem mapped = IDMapping::get()->getID(pending.drop);
		if (mapped.id != 0)
		{
			pending.sink->setDropOverride(mapped.id, mapped.aux);
		}
		else
		{
			Loader::_debugPrint("block '" + pending.context + "': unknown drop '" + pending.drop + "'; dropping itself instead");
		}
	}
	pendingDrops.clear();
}
