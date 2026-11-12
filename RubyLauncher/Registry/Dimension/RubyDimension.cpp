#include "RubyDimension.h"
#include "Registry/Biome/BiomeRegistry.h"
#include "RubyLevelSource.h"
#include "RubyUndergroundLevelSource.h"
#include "RubyFloatingIslandLevelSource.h"
#include "RubyTerrainPalette.h"
#include "TilePos.h"
#include "Biome.h"
#include "ChunkSource.h"
#include "FixedBiomeSource.h"
#include "Level.h"
#include "Tile.h"

RubyDimension::RubyDimension(const DimensionDefinition &definition) : m_definition(definition)
{
}

void RubyDimension::init()
{
	id = m_definition.slot;
	hasCeiling = m_definition.hasCeiling;
	ultraWarm = m_definition.ultraWarm;

	if (!m_definition.biome.empty())
	{
		if (Biome *biome = BiomeRegistry::findBiome(m_definition.biome))
		{
			biomeSource = new FixedBiomeSource(biome, 0.5f, 0.5f);
			return;
		}
	}

	Dimension::init();
}

ChunkSource *RubyDimension::createRandomLevelSource() const
{
	if (level == nullptr) return nullptr;

	const std::string generator = m_definition.generator.empty() ? std::string("surface") : m_definition.generator;
	const int xzSize = m_definition.xzSize > 0 ? m_definition.xzSize : LEVEL_MAX_WIDTH;
	const Biome *biome = m_definition.biome.empty() ? nullptr : BiomeRegistry::findBiome(m_definition.biome);
	const RubyTerrainPalette palette(m_definition.terrain, biome);

	if (generator == "surface")
	{
		return new RubyLevelSource(level, level->getSeed(), m_definition);
	}
	if (generator == "underground")
	{
		return new RubyUndergroundLevelSource(level, level->getSeed(), xzSize, palette.stone, palette.surface, palette.filler);
	}
	if (generator == "floatingIslands")
	{
		return new RubyFloatingIslandLevelSource(level, level->getSeed(), xzSize, palette.stone, palette.surface, palette.filler);
	}
	return Dimension::createRandomLevelSource();
}

bool RubyDimension::isValidSpawn(int x, int z) const
{
	int topTile = level->getTopTile(x, z);

	if (topTile == 0 || Tile::tiles[topTile] == nullptr) return false;

	return Tile::tiles[topTile]->material->blocksMotion();
}

bool RubyDimension::mayRespawn() const
{
	return m_definition.mayRespawn;
}

Pos *RubyDimension::getSpawnPos()
{
	return new Pos(m_definition.spawnX, m_definition.spawnY, m_definition.spawnZ);
}

int RubyDimension::getXZSize()
{
	if (m_definition.xzSize > 0) return m_definition.xzSize;

	return Dimension::getXZSize();
}

int RubyDimension::getSeaLevelOverride()
{
	return m_definition.seaLevel >= 0 ? m_definition.seaLevel : -1;
}

bool RubyDimension::isNaturalDimension()
{
	return m_definition.natural;
}

float RubyDimension::getTimeOfDay(int64_t time, float a) const
{
	if (!m_definition.natural) return 0.0f;

	return Dimension::getTimeOfDay(time, a);
}

float *RubyDimension::getSunriseColor(float td, float a)
{
	if (!m_definition.natural) return nullptr;

	return Dimension::getSunriseColor(td, a);
}
