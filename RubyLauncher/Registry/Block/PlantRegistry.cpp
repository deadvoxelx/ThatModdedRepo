#include "PlantRegistry.h"
#include "BlockRegistry.h"
#include "../Client/ModPlantTile.h"
#include "../Common/ModPaths.h"
#include "../Host/RubyLauncherHost.h"
#include "Item.h"
#include "Loader.h"
#include "Registry/IDs.h"
#include "Registry/Item/ItemRegistry.h"
#include "Tile.h"
#include "TileItem.h"

#if defined(_WINDOWS64)
#include "Windows64Media/strings.h"
#else
#include "DurangoMedia/loc/strings.h"
#endif

namespace
{
	struct PendingPlant
	{
		ModPlantTile *tile = nullptr;
		PlantDefinition definition;
	};

	std::vector<PendingPlant> g_pending;

	std::string describe(const PlantDefinition &definition)
	{
		return definition.modId + ":" + definition.plantId;
	}
}

int PlantRegistry::registerPlant(const PlantDefinition &definition)
{
	const std::string context = describe(definition);

	if (definition.modId.empty() || definition.plantId.empty())
	{
		Loader::_debugPrint("plant " + context + ": modId and plantId are required");
		return -1;
	}

	int tileId = IDMapping::get()->pinnedId(context);
	if (tileId != -1 && !BlockRegistry::isBlockIdFree(tileId))
	{
		Loader::_debugPrint("plant '" + context + "' was pinned to id " + std::to_string(tileId) + " but that slot is already taken; allocating a new id");
		tileId = -1;
	}
	if (tileId == -1) tileId = BlockRegistry::nextItemId(context);

	if (tileId == -1)
	{
		Loader::_debugPrint("out of block ids, plant '" + context + "' was not registered");
		return -1;
	}

	std::wstring icon;
	if (!definition.texturePath.empty())
	{
		RubyModTexture request;
		request.iconName = rubyModTextureIconName(definition.modId, definition.plantId);
		request.filePath = RubyPaths::resolveModTexturePath(RubyPaths::toNarrow(definition.modPath), definition.texturePath);
		request.fallback = "tallgrass";
		request.block = true;

		RubyLoader::addModTexture(request);
		icon = RubyPaths::toWide(request.iconName);
	}
	else
	{
		icon = L"tallgrass";
	}

	const int nameId = ItemRegistry::nextItemNameId();

	ModPlantTile *tile = new ModPlantTile(tileId, icon);
	tile->setDescriptionId(nameId);
	tile->setDropNothing(definition.dropNothing);

	Item::items[tileId] = (new TileItem(tileId - 512))
		->setIconName(icon)
		->setDescriptionId(nameId)
		->setBaseItemTypeAndMaterial(Item::eBaseItemType_structblock, Item::eMaterial_undefined)
		->setUseDescriptionId(IDS_DESC_STONE);

	const std::wstring wname(definition.name.begin(), definition.name.end());
	ItemRegistry::langList[nameId] = wname;
	IDMapping::get()->add(definition.modId, definition.plantId, true, tileId);

	RubyCreative::addEntry(tileId, 0, RubyCreativeGroup_Decoration);
	g_pending.push_back({ tile, definition });

	Loader::_debugPrint("registered plant " + context + " as tile " + std::to_string(tileId) + (definition.texturePath.empty() ? "" : ", texture '" + definition.texturePath + "'"));

	return tileId;
}

void PlantRegistry::finalize()
{
	for (const PendingPlant &pending : g_pending)
	{
		if (pending.tile == nullptr) continue;

		const std::string context = describe(pending.definition);

		if (!pending.definition.soil.empty())
		{
			std::vector<int> soil;
			for (const std::string &name : pending.definition.soil)
			{
				IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
				if (mapped.id == 0 || Tile::tiles[mapped.id] == nullptr)
				{
					Loader::_debugPrint("plant " + context + ": unknown soil block '" + name + "', ignored");
					continue;
				}
				soil.push_back(mapped.id);
			}

			if (soil.empty())
			{
				Loader::_debugPrint("plant " + context + ": no usable soil blocks, so it keeps the default grass and dirt");
			}
			else
			{
				pending.tile->setSoil(soil);
			}
		}

		if (!pending.definition.drop.empty())
		{
			IDMapping::MappedItem mapped = IDMapping::get()->getID(pending.definition.drop);
			if (mapped.id == 0)
			{
				Loader::_debugPrint("plant " + context + ": unknown drop '" + pending.definition.drop + "'; dropping itself instead");
			}
			else
			{
				pending.tile->setDrop(mapped.id, mapped.aux);
			}
		}
	}

	g_pending.clear();
}
