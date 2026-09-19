#include "OreFeatureRegistry.h"

#include "Loader.h"
#include "Registry/IDs.h"

#include "net.minecraft.world.level.h"
#include "Dimension.h"
#include "Random.h"
#include "Tile.h"
#include "OreFeature.h"

#include <utility>

namespace
{
	struct PendingOre
	{
		std::string modId;
		std::string oreId;
		std::string block;
		std::string target;
		int size = 8;
		int dimension = -1;
		int yMin = 0;
		int yMax = 256;
		int count = 8;
	};

	struct PlacedOre
	{
		OreFeature *feature = nullptr;
		int dimension = -1;
		int yMin = 0;
		int yMax = 256;
		int count = 8;
	};

	std::vector<PendingOre> g_pending;
	std::vector<PlacedOre> g_placed;

	std::string describe(const PendingOre &pending)
	{
		return pending.modId + ":" + pending.oreId;
	}

	int resolveBlock(const std::string &name, const std::string &context, const std::string &role)
	{
		IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
		if (mapped.id == 0)
		{
			Loader::_debugPrint("ore feature " + context + ": unknown " + role + " '" + name + "'");
			return -1;
		}
		if (Tile::tiles[mapped.id] == nullptr)
		{
			Loader::_debugPrint("ore feature " + context + ": " + role + " '" + name + "' has no Tile entry (id " + std::to_string(mapped.id) + ")");
			return -1;
		}
		return mapped.id;
	}
}

bool OreFeatureRegistry::registerOre(const std::string &modId, const std::string &oreId, const std::string &block, int size, const std::string &target, int dimension, int yMin, int yMax, int count)
{
	const std::string context = modId + ":" + oreId;

	if (block.empty())
	{
		Loader::_debugPrint("ore feature " + context + ": no block was given");
		return false;
	}
	if (size <= 0)
	{
		Loader::_debugPrint("ore feature " + context + ": size must be positive");
		return false;
	}
	if (count <= 0)
	{
		Loader::_debugPrint("ore feature " + context + ": count must be positive");
		return false;
	}
	if (yMin < 0 || yMax <= yMin)
	{
		Loader::_debugPrint("ore feature " + context + ": yMin must be >= 0 and yMax must be greater than yMin");
		return false;
	}

	PendingOre pending;
	pending.modId = modId;
	pending.oreId = oreId;
	pending.block = block;
	pending.target = target;
	pending.size = size;
	pending.dimension = dimension;
	pending.yMin = yMin;
	pending.yMax = yMax;
	pending.count = count;

	g_pending.push_back(std::move(pending));
	return true;
}

void OreFeatureRegistry::finalize()
{
	if (g_pending.empty()) return;

	int added = 0;
	for (const PendingOre &pending : g_pending)
	{
		const std::string context = describe(pending);

		const int blockId = resolveBlock(pending.block, context, "block");
		if (blockId == -1) continue;

		const std::string targetName = pending.target.empty() ? "minecraft:stone" : pending.target;
		const int targetId = resolveBlock(targetName, context, "target");
		if (targetId == -1) continue;

		PlacedOre placed;
		placed.feature = new OreFeature(blockId, IDMapping::get()->getID(pending.block).aux, pending.size, targetId);
		placed.dimension = pending.dimension;
		placed.yMin = pending.yMin;
		placed.yMax = pending.yMax;
		placed.count = pending.count;

		g_placed.push_back(placed);
		++added;
	}

	g_pending.clear();

	if (added > 0)
	{
		Loader::_debugPrint("OreFeatureRegistry: " + std::to_string(added) + " ore feature(s) registered");
	}
	else
	{
		Loader::_debugPrint("OreFeatureRegistry: no ore features could be registered");
	}
}

void OreFeatureRegistry::reset()
{
	for (const PlacedOre &ore : g_placed)
	{
		delete ore.feature;
	}
	g_placed.clear();
	g_pending.clear();
}

void OreFeatureRegistry::decorateChunk(Level *level, Random *random, int xo, int zo, Biome *biome)
{
	if (g_placed.empty()) return;

	const int dim = level->dimension->id;
	for (const PlacedOre &ore : g_placed)
	{
		if (ore.dimension != DIMENSION_ALL && ore.dimension != dim) continue;

		for (int i = 0; i < ore.count; i++)
		{
			int x = xo + random->nextInt(16);
			int y = ore.yMin + random->nextInt(ore.yMax - ore.yMin);
			int z = zo + random->nextInt(16);
			ore.feature->place(level, random, x, y, z);
		}
	}
}
