#include "SaplingRegistry.h"

#include "BlockRegistry.h"
#include "../Client/ModSaplingTile.h"
#include "../Common/ModPaths.h"
#include "../Host/RubyLauncherHost.h"
#include "../WorldGen/TreeFeatureRegistry.h"
#include "Item.h"
#include "Loader.h"
#include "Tile.h"
#include "TileItem.h"
#include "Registry/Item/ItemRegistry.h"
#include "Registry/IDs.h"

#if defined(_WINDOWS64)
#include "Windows64Media/strings.h"
#else
#include "DurangoMedia/loc/strings.h"
#endif

int SaplingRegistry::registerSapling(const std::string &modId, const std::string &saplingId, const std::string &name, const std::wstring &modPath, const std::string &texturePath, const std::string &shape, const std::string &trunk, const std::string &leaves, int height)
{
	const std::string context = modId + ":" + saplingId;

	Feature *treeFeature = TreeFeatureRegistry::buildTreeFeature(shape, trunk, leaves, height, context);
	if (treeFeature == nullptr) return -1;

	const int tileId = BlockRegistry::nextItemId();
	if (tileId == -1)
	{
		Loader::_debugPrint("out of block ids, sapling '" + context + "' was not registered");
		delete treeFeature;
		return -1;
	}

	std::wstring icon;
	std::string iconName;
	if (!texturePath.empty())
	{
		RubyModTexture request;
		request.iconName = rubyModTextureIconName(modId, saplingId);
		request.filePath = RubyPaths::resolveModTexturePath(RubyPaths::toNarrow(modPath), texturePath);
		request.fallback = "sapling";
		request.block = true;

		RubyLoader::addModTexture(request);
		icon = RubyPaths::toWide(request.iconName);
	}
	else
	{
		icon = L"sapling";
	}

	const int nameId = ItemRegistry::nextItemNameId();

	Tile *tile = new ModSaplingTile(tileId, icon, treeFeature);
	tile->setDescriptionId(nameId);

	Item::items[tileId] = (new TileItem(tileId - 512))
		->setIconName(icon.empty() ? L"sapling" : icon)
		->setDescriptionId(nameId)
		->setBaseItemTypeAndMaterial(Item::eBaseItemType_structblock, Item::eMaterial_undefined)
		->setUseDescriptionId(IDS_DESC_STONE);

	const std::wstring wname(name.begin(), name.end());
	ItemRegistry::langList[nameId] = wname;
	IDMapping::get()->add(modId, saplingId, true, tileId);

	RubyCreative::addEntry(tileId, 0, RubyCreativeGroup_Decoration);
	Loader::_debugPrint("registered sapling " + context + " as tile " + std::to_string(tileId) + " (grows a " + shape + " tree)");

	return tileId;
}
