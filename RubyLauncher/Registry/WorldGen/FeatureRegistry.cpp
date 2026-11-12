#include "FeatureRegistry.h"
#include "Loader.h"
#include "Registry/Biome/BiomeRegistry.h"
#include "Registry/Dimension/DimensionRegistry.h"
#include "Registry/IDs.h"
#include "Level.h"
#include "Random.h"
#include "Tile.h"
#include "Biome.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include <algorithm>
#include <map>
#include <utility>

namespace
{
	struct PlacedFeature
	{
		std::string name;
		std::vector<Biome *> biomes;
		std::vector<int> dimensions;
		bool allDimensions = true;
		int count = 1;
		float chance = 1.0f;
		FeaturePlacement placement;
		std::vector<int> ground;
		std::function<bool(FeatureContext &)> builder;
	};

	std::vector<FeatureDefinition> g_pending;
	std::vector<PlacedFeature> g_placed;
	std::map<std::string, int> g_blockIds;

	std::string describe(const FeatureDefinition &definition)
	{
		return definition.modId + ":" + definition.featureId;
	}

	int resolveGround(const std::string &name, const std::string &context)
	{
		IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
		if (mapped.id <= 0 || Tile::tiles[mapped.id] == nullptr)
		{
			Loader::_debugPrint("feature " + context + ": unknown ground block '" + name + "'");
			return -1;
		}
		return mapped.id;
	}

	bool isNumeric(const std::string &value)
	{
		if (value.empty()) return false;
		return value.find_first_not_of("-0123456789") == std::string::npos;
	}

	bool exceedsWorldHeight(const FeatureContext &context)
	{
		for (const FeatureBlock &block : context.blocks())
		{
			if (block.y >= Level::genDepth) return true;
		}
		return false;
	}

	void applyBlocks(Level *level, const FeatureContext &context)
	{
		const std::vector<FeatureBlock> &blocks = context.blocks();
		if (blocks.empty()) return;

		for (const FeatureBlock &block : blocks)
		{
			if (block.y < 0 || block.y >= Level::genDepth) continue;
			if (block.airOnly && level->getTile(block.x, block.y, block.z) != 0) continue;

			level->setTileAndData(block.x, block.y, block.z, block.tile, block.aux, Tile::UPDATE_CLIENTS);
		}
	}
}

FeatureContext::FeatureContext(Level *level, Random *random, int x, int y, int z) : m_level(level), m_random(random), m_x(x), m_y(y), m_z(z)
{
}

void FeatureContext::set(int dx, int dy, int dz, int tile, int aux)
{
	if (tile <= 0) return;

	FeatureBlock block;
	block.x = m_x + dx;
	block.y = m_y + dy;
	block.z = m_z + dz;
	block.tile = tile;
	block.aux = aux;
	block.airOnly = false;
	m_blocks.push_back(block);
}

void FeatureContext::replace(int dx, int dy, int dz, int tile, int aux)
{
	if (tile <= 0) return;

	FeatureBlock block;
	block.x = m_x + dx;
	block.y = m_y + dy;
	block.z = m_z + dz;
	block.tile = tile;
	block.aux = aux;
	block.airOnly = true;
	m_blocks.push_back(block);
}

void FeatureContext::fill(int x0, int y0, int z0, int x1, int y1, int z1, int tile, int aux)
{
	const int minX = std::min<int>(x0, x1);
	const int maxX = std::max<int>(x0, x1);
	const int minY = std::min<int>(y0, y1);
	const int maxY = std::max<int>(y0, y1);
	const int minZ = std::min<int>(z0, z1);
	const int maxZ = std::max<int>(z0, z1);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			for (int z = minZ; z <= maxZ; z++)
			{
				set(x, y, z, tile, aux);
			}
		}
	}
}

int FeatureContext::get(int dx, int dy, int dz) const
{
	return m_level->getTile(m_x + dx, m_y + dy, m_z + dz);
}

bool FeatureContext::isAir(int dx, int dy, int dz) const
{
	return get(dx, dy, dz) == 0;
}

int FeatureContext::height(int dx, int dz) const
{
	return m_level->getHeightmap(m_x + dx, m_z + dz);
}

int FeatureContext::randomInt(int minInclusive, int maxInclusive)
{
	if (maxInclusive < minInclusive) return minInclusive;

	return minInclusive + m_random->nextInt(maxInclusive - minInclusive + 1);
}

float FeatureContext::randomFloat()
{
	return m_random->nextFloat();
}

bool FeatureRegistry::registerFeature(const FeatureDefinition &definition)
{
	const std::string context = describe(definition);

	if (definition.modId.empty() || definition.featureId.empty())
	{
		Loader::_debugPrint("feature: modId and featureId are required");
		return false;
	}
	if (!definition.builder)
	{
		Loader::_debugPrint("feature " + context + ": a place function is required");
		return false;
	}
	if (definition.count <= 0)
	{
		Loader::_debugPrint("feature " + context + ": count must be positive");
		return false;
	}
	if (definition.chance <= 0.0f || definition.chance > 1.0f)
	{
		Loader::_debugPrint("feature " + context + ": chance has to be above 0 and at most 1");
		return false;
	}

	g_pending.push_back(definition);
	return true;
}

void FeatureRegistry::finalize()
{
	if (g_pending.empty()) return;

	int added = 0;

	for (const FeatureDefinition &pending : g_pending)
	{
		const std::string context = describe(pending);

		PlacedFeature placed;
		placed.name = context;
		placed.count = pending.count;
		placed.chance = pending.chance;
		placed.placement = pending.placement;
		placed.builder = pending.builder;

		bool valid = true;

		for (const std::string &biomeName : pending.biomes)
		{
			Biome *biome = BiomeRegistry::findBiome(biomeName);
			if (biome == nullptr)
			{
				Loader::_debugPrint("feature " + context + ": no biome '" + biomeName + "' (register the biome first)");
				valid = false;
				break;
			}
			placed.biomes.push_back(biome);
		}
		if (!valid) continue;

		if (!pending.dimensions.empty())
		{
			placed.allDimensions = false;
			for (const std::string &dimension : pending.dimensions)
			{
				int slot = DIMENSION_ALL;
				if (isNumeric(dimension)) slot = std::stoi(dimension);
				else slot = DimensionRegistry::slotForName(dimension);

				if (slot == DIMENSION_ALL)
				{
					placed.allDimensions = true;
					break;
				}
				if (slot == DimensionRegistry::NO_SLOT)
				{
					Loader::_debugPrint("feature " + context + ": no dimension '" + dimension + "'");
					valid = false;
					break;
				}
				placed.dimensions.push_back(slot);
			}
		}
		if (!valid) continue;

		for (const std::string &ground : pending.placement.ground)
		{
			const int tile = resolveGround(ground, context);
			if (tile < 0)
			{
				valid = false;
				break;
			}
			placed.ground.push_back(tile);
		}
		if (!valid) continue;

		g_placed.push_back(placed);
		++added;
	}

	g_pending.clear();
	g_blockIds.clear();

	if (added > 0)
	{
		Loader::_debugPrint("FeatureRegistry: " + std::to_string(added) + " feature(s) registered");
	}
	else
	{
		Loader::_debugPrint("FeatureRegistry: no features could be registered");
	}
}

void FeatureRegistry::reset()
{
	g_placed.clear();
	g_pending.clear();
	g_blockIds.clear();
}

void FeatureRegistry::decorateChunk(Level *level, Random *random, int xo, int zo, Biome *biome)
{
	if (level == nullptr || random == nullptr || g_placed.empty()) return;

	const int dimension = level->dimension != nullptr ? level->dimension->id : 0;

	for (const PlacedFeature &feature : g_placed)
	{
		if (!feature.allDimensions)
		{
			const bool allowed = std::find(feature.dimensions.begin(), feature.dimensions.end(), dimension) != feature.dimensions.end();
			if (!allowed) continue;
		}

		if (!feature.biomes.empty())
		{
			const bool allowed = std::find(feature.biomes.begin(), feature.biomes.end(), biome) != feature.biomes.end();
			if (!allowed) continue;
		}

		for (int i = 0; i < feature.count; i++)
		{
			if (feature.chance < 1.0f && random->nextFloat() >= feature.chance) continue;

				const int x = xo + random->nextInt(16);
				const int z = zo + random->nextInt(16);

			int y = 0;
			switch (feature.placement.mode)
			{
			case FeaturePlacement::RANGE:
				{
					const int span = std::max<int>(1, feature.placement.maxY - feature.placement.minY);
					y = feature.placement.minY + random->nextInt(span);
				}
				break;
			case FeaturePlacement::FIXED:
				y = feature.placement.minY;
				break;
			case FeaturePlacement::SURFACE:
			default:
				y = level->getHeightmap(x, z);
				break;
			}

				if (y <= 0) continue;

				if (!feature.ground.empty())
				{
					const int below = level->getTile(x, y - 1, z);
					if (std::find(feature.ground.begin(), feature.ground.end(), below) == feature.ground.end()) continue;
				}

			FeatureContext context(level, random, x, y, z);
			if (!feature.builder(context)) continue;
			if (exceedsWorldHeight(context)) continue;

			applyBlocks(level, context);
		}
	}
}

int FeatureRegistry::blockId(const std::string &name)
{
	std::map<std::string, int>::const_iterator cached = g_blockIds.find(name);
	if (cached != g_blockIds.end()) return cached->second;

	int id = -1;
	IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
	if (mapped.id > 0 && Tile::tiles[mapped.id] != nullptr)
	{
		id = mapped.id;
	}
	else
	{
		Loader::_debugPrint("feature: unknown block '" + name + "'");
	}

	g_blockIds[name] = id;
	return id;
}
