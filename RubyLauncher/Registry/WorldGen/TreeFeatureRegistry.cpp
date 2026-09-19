#include "TreeFeatureRegistry.h"

#include "OreFeatureRegistry.h"
#include "Loader.h"
#include "Registry/IDs.h"

#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "Random.h"
#include "Tile.h"
#include "Biome.h"
#include "TreeFeature.h"
#include "SpruceFeature.h"
#include "BirchFeature.h"
#include "MegaTreeFeature.h"
#include "SwampTreeFeature.h"

#include <utility>
#include <vector>
#include <string>

namespace
{
	struct PendingTree
	{
		std::string modId;
		std::string treeId;
		std::string shape;
		std::string trunk;
		std::string leaves;
		std::string biome;
		int count = 1;
		int height = 0;
	};

	struct PlacedTree
	{
		Feature *feature = nullptr;
		Biome *biome = nullptr;
		int count = 1;
	};

	std::vector<PendingTree> g_pending;
	std::vector<PlacedTree> g_placed;

	std::string describe(const PendingTree &pending)
	{
		return pending.modId + ":" + pending.treeId;
	}

	bool resolveTrunkOrLeaves(const std::string &name, int &tile, int &aux, const std::string &context, const std::string &role)
	{
		IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
		if (mapped.id == 0)
		{
			Loader::_debugPrint("tree feature " + context + ": unknown " + role + " '" + name + "'");
			return false;
		}
		if (Tile::tiles[mapped.id] == nullptr)
		{
			Loader::_debugPrint("tree feature " + context + ": " + role + " '" + name + "' has no Tile entry (id " + std::to_string(mapped.id) + ")");
			return false;
		}
		tile = mapped.id;
		aux = mapped.aux;
		return true;
	}

	Biome *resolveBiome(const std::string &biomeName, const std::string &context)
	{
		if (biomeName.empty()) return nullptr;

		std::string name = biomeName;
		const std::string prefix = "minecraft:";
		if (name.rfind(prefix, 0) == 0) name = name.substr(prefix.size());

		auto normalize = [](std::string s)
		{
			std::string out;
			for (char c : s) { if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a'; if (c != ' ' && c != '_') out += c; }
			return out;
		};
		const std::string wanted = normalize(biomeName.substr(biomeName.rfind(':') + 1));

		static const struct { const char *name; Biome **slot; } kBiomes[] =
		{
			{ "ocean", &Biome::ocean },
			{ "plains", &Biome::plains },
			{ "desert", &Biome::desert },
			{ "extremehills", &Biome::extremeHills },
			{ "forest", &Biome::forest },
			{ "taiga", &Biome::taiga },
			{ "swampland", &Biome::swampland },
			{ "river", &Biome::river },
			{ "hell", &Biome::hell },
			{ "sky", &Biome::sky },
			{ "frozenocean", &Biome::frozenOcean },
			{ "frozenriver", &Biome::frozenRiver },
			{ "iceflats", &Biome::iceFlats },
			{ "icemountains", &Biome::iceMountains },
			{ "mushroomisland", &Biome::mushroomIsland },
			{ "mushroomislandshore", &Biome::mushroomIslandShore },
			{ "beaches", &Biome::beaches },
			{ "deserthills", &Biome::desertHills },
			{ "foresthills", &Biome::forestHills },
			{ "taigahills", &Biome::taigaHills },
			{ "smallerextremehills", &Biome::smallerExtremeHills },
			{ "jungle", &Biome::jungle },
			{ "junglehills", &Biome::jungleHills },
			{ "outerislands", &Biome::outerIslands },
			{ "aetherislands", &Biome::aether },
			{ "birchforest", &Biome::birchForest },
			{ "lavenderforest", &Biome::lavenderForest },
			{ "nulands", &Biome::nulands },
			{ "reddesert", &Biome::redDesert },
		};

		for (const auto &entry : kBiomes)
		{
			if (wanted == entry.name) return *entry.slot;
		}

		Loader::_debugPrint("tree feature " + context + ": unknown biome '" + biomeName + "'");
		return nullptr;
	}
}

bool TreeFeatureRegistry::registerTree(const std::string &modId, const std::string &treeId, const std::string &shape, const std::string &trunk, const std::string &leaves, const std::string &biome, int count, int height)
{
	const std::string context = modId + ":" + treeId;

	if (shape != "oak" && shape != "spruce" && shape != "birch" && shape != "hugeJungle" && shape != "swamp")
	{
		Loader::_debugPrint("tree feature " + context + ": shape must be \"oak\", \"spruce\", \"birch\", \"hugeJungle\" or \"swamp\" (got '" + shape + "')");
		return false;
	}
	if (count <= 0)
	{
		Loader::_debugPrint("tree feature " + context + ": count must be positive");
		return false;
	}
	if (height < 0)
	{
		Loader::_debugPrint("tree feature " + context + ": height cant be negative");
		return false;
	}

	PendingTree pending;
	pending.modId = modId;
	pending.treeId = treeId;
	pending.shape = shape;
	pending.trunk = trunk;
	pending.leaves = leaves;
	pending.biome = biome;
	pending.count = count;
	pending.height = height;

	g_pending.push_back(std::move(pending));
	return true;
}

Feature *TreeFeatureRegistry::buildTreeFeature(const std::string &shape, const std::string &trunk, const std::string &leaves, int height, const std::string &context)
{
	if (shape != "oak" && shape != "spruce" && shape != "birch" && shape != "hugeJungle" && shape != "swamp")
	{
		Loader::_debugPrint("tree feature " + context + ": shape must be \"oak\", \"spruce\", \"birch\", \"hugeJungle\" or \"swamp\" (got '" + shape + "')");
		return nullptr;
	}

	int trunkTile = Tile::treeTrunk_Id, trunkAux = 0;
	int leafTile = Tile::leaves_Id, leafAux = 0;
	if (shape == "spruce")
	{
		trunkAux = TreeTile::DARK_TRUNK;
		leafAux = LeafTile::EVERGREEN_LEAF;
	}
	else if (shape == "birch")
	{
		trunkAux = TreeTile::BIRCH_TRUNK;
		leafAux = LeafTile::BIRCH_LEAF;
	}
	else if (shape == "hugeJungle")
	{
		trunkAux = TreeTile::JUNGLE_TRUNK;
		leafAux = LeafTile::JUNGLE_LEAF;
	}
	else if (shape == "swamp")
	{
		trunkAux = 0;	// oak trunk
		leafAux = LeafTile::NORMAL_LEAF;
	}

	if (!trunk.empty() && !resolveTrunkOrLeaves(trunk, trunkTile, trunkAux, context, "trunk")) return nullptr;
	if (!leaves.empty() && !resolveTrunkOrLeaves(leaves, leafTile, leafAux, context, "leaves")) return nullptr;

	if (shape == "spruce")
	{
		const int baseHeight = (height > 0) ? height : 6;
		return new SpruceFeature(true, baseHeight, trunkAux, leafAux, trunkTile, leafTile);
	}
	else if (shape == "birch")
	{
		const int baseHeight = (height > 0) ? height : 9;
		return new BirchFeature(true, baseHeight, trunkAux, leafAux, trunkTile, leafTile);
	}
	else if (shape == "hugeJungle")
	{
		const int baseHeight = (height > 0) ? height : 10;
		return new MegaTreeFeature(true, baseHeight, trunkAux, leafAux, trunkTile, leafTile);
	}
	else if (shape == "swamp")
	{
		const int baseHeight = (height > 0) ? height : 5;
		return new SwampTreeFeature(true, baseHeight, trunkAux, leafAux, trunkTile, leafTile);
	}
	else	// "oak"
	{
		const int baseHeight = (height > 0) ? height : 4;
		return new TreeFeature(true, baseHeight, trunkAux, leafAux, false, trunkTile, leafTile);
	}
}

void TreeFeatureRegistry::finalize()
{
	if (g_pending.empty()) return;

	int added = 0;
	for (const PendingTree &pending : g_pending)
	{
		const std::string context = describe(pending);

		Feature *feature = buildTreeFeature(pending.shape, pending.trunk, pending.leaves, pending.height, context);
		if (feature == nullptr) continue;

		PlacedTree placed;
		placed.feature = feature;
		placed.count = pending.count;
		placed.biome = resolveBiome(pending.biome, context);

		g_placed.push_back(placed);
		++added;
	}

	g_pending.clear();

	if (added > 0)
	{
		Loader::_debugPrint("TreeFeatureRegistry: " + std::to_string(added) + " tree feature(s) registered");
	}
	else
	{
		Loader::_debugPrint("TreeFeatureRegistry: no tree features could be registered");
	}
}

void TreeFeatureRegistry::reset()
{
	for (const PlacedTree &tree : g_placed)
	{
		delete tree.feature;
	}
	g_placed.clear();
	g_pending.clear();
}

void TreeFeatureRegistry::decorateChunk(Level *level, Random *random, int xo, int zo, Biome *biome)
{
	if (g_placed.empty()) return;

	for (const PlacedTree &tree : g_placed)
	{
		if (tree.biome != nullptr && tree.biome != biome) continue;

		for (int i = 0; i < tree.count; i++)
		{
			const int x = xo + random->nextInt(16) + 8;
			const int z = zo + random->nextInt(16) + 8;
			const int y = level->getHeightmap(x, z);

			tree.feature->place(level, random, x, y, z);
		}
	}
}
