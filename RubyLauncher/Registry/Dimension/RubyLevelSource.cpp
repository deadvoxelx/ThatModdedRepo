#include "RubyLevelSource.h"
#include "net.minecraft.world.level.h"
#include "ChunkSource.h"
#include "HeavyTile.h"
#include "Registry/IDs.h"
#include "Level.h"
#include "LevelChunk.h"
#include "PerlinNoise.h"
#include "Random.h"
#include "Tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include <cstring>
#include <string>

namespace
{
	const int kVerticalFalloff = 3;
	const double kNoiseBase = 684.412;

	int resolveBlock(const std::string &name, int fallback)
	{
		if (name.empty()) return fallback;

		IDMapping *mapping = IDMapping::get();
		if (mapping == nullptr) return fallback;

		IDMapping::MappedItem mapped = mapping->getID(name);
		return mapped.id > 0 ? mapped.id : fallback;
	}
}

RubyLevelSource::RubyLevelSource(Level *level, int64_t seed, const DimensionDefinition &definition) : m_definition(definition)
{
	m_level = level;

	const int width = definition.xzSize > 0 ? definition.xzSize : LEVEL_MAX_WIDTH;
	m_XZSize = width > 0 ? width : LEVEL_MAX_WIDTH;

	m_random = new Random(seed);
	m_pprandom = new Random(seed);
	m_heightNoise = new PerlinNoise(m_random, 1);
	m_detailNoise = new PerlinNoise(m_random, 1);
	m_depthVariationNoise = new PerlinNoise(m_random, 1);
	m_selectorNoise = new PerlinNoise(m_random, 8);
	m_terrainNoise1 = new PerlinNoise(m_random, 16);
	m_terrainNoise2 = new PerlinNoise(m_random, 16);

	m_stoneId = resolveBlock(definition.terrain.stone, Tile::stone_Id);
	m_surfaceId = resolveBlock(definition.terrain.surface, 0);
	m_fillerId = resolveBlock(definition.terrain.filler, 0);
}

RubyLevelSource::~RubyLevelSource()
{
	delete m_random;
	delete m_pprandom;
	delete m_heightNoise;
	delete m_detailNoise;
	delete m_depthVariationNoise;
	delete m_selectorNoise;
	delete m_terrainNoise1;
	delete m_terrainNoise2;
}

Biome *RubyLevelSource::biomeAt(int xOffs, int zOffs)
{
	return m_level->getBiome(xOffs * 16 + 8, zOffs * 16 + 8);
}

void RubyLevelSource::prepareHeights(int xOffs, int zOffs, byteArray blocks, unsigned short *tiles)
{
	const DimensionTerrain &terrain = m_definition.terrain;

	doubleArray heights;
	doubleArray detail;
	doubleArray depthVariation;

	heights = m_heightNoise->getRegion(heights, xOffs * 16, zOffs * 16, 16, 16, 1 / 64.0, 1 / 64.0, 0.5);
	detail = m_detailNoise->getRegion(detail, xOffs * 16, zOffs * 16, 16, 16, 1 / 24.0, 1 / 24.0, 0.5);
	depthVariation = m_depthVariationNoise->getRegion(depthVariation, xOffs * 16, zOffs * 16, 16, 16, 1 / 192.0, 1 / 192.0, 0.5);

	Biome *biome = biomeAt(xOffs, zOffs);

	const int stoneId = m_stoneId > 0 ? m_stoneId : Tile::stone_Id;
	const int surfaceId = m_surfaceId > 0 ? m_surfaceId : (biome != nullptr ? biome->topMaterial : Tile::grass_Id);
	const int fillerId = m_fillerId > 0 ? m_fillerId : (biome != nullptr ? biome->material : Tile::dirt_Id);

	float scale = (biome != nullptr ? biome->scale : 0.5f) * 0.9f + 0.1f;
	float depth = ((biome != nullptr ? biome->depth : 0.1f) * 4 - 1) / 8.0f;

	int columnTop[16 * 16];
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			const int index = x * 16 + z;

			int top = terrain.baseHeight
				+ (int) ((depth + depthVariation[index] * 0.5f) * terrain.heightVariation)
				+ (int) ((heights[index] * 14.0 + detail[index] * 4.0) * terrain.relief * scale);

			if (top < terrain.baseDepth + 2) top = terrain.baseDepth + 2;
			if (top > Level::genDepth - terrain.ceilingMargin) top = Level::genDepth - terrain.ceilingMargin;

			columnTop[index] = top;
		}
	}

	delete [] heights.data;
	delete [] detail.data;
	delete [] depthVariation.data;

	const int xChunks = 16 / CHUNK_WIDTH;
	const int yChunks = Level::genDepth / CHUNK_HEIGHT;
	const int xSize = xChunks + 1;
	const int ySize = yChunks + 1;
	const int zSize = xChunks + 1;

	doubleArray selector;
	doubleArray terrain1;
	doubleArray terrain2;

	selector = m_selectorNoise->getRegion(selector, xOffs * xChunks, 0, zOffs * xChunks, xSize, ySize, zSize, kNoiseBase / 160.0, kNoiseBase / 160.0, kNoiseBase / 160.0);
	terrain1 = m_terrainNoise1->getRegion(terrain1, xOffs * xChunks, 0, zOffs * xChunks, xSize, ySize, zSize, kNoiseBase, kNoiseBase, kNoiseBase);
	terrain2 = m_terrainNoise2->getRegion(terrain2, xOffs * xChunks, 0, zOffs * xChunks, xSize, ySize, zSize, kNoiseBase, kNoiseBase, kNoiseBase);

	doubleArray density;
	density = doubleArray(xSize * ySize * zSize);

	for (int p = 0; p < xSize * ySize * zSize; p++)
	{
		double a = terrain1[p] / 512;
		double b = terrain2[p] / 512;
		double pick = (selector[p] / 10 + 1) / 2;
		if (pick < 0) density[p] = a;
		else if (pick > 1) density[p] = b;
		else density[p] = a + (b - a) * pick;
	}

	delete [] selector.data;
	delete [] terrain1.data;
	delete [] terrain2.data;

	const double yStep = 1 / (double) CHUNK_HEIGHT;
	const double xStep = 1 / (double) CHUNK_WIDTH;
	const double zStep = 1 / (double) CHUNK_WIDTH;

	for (int xc = 0; xc < xChunks; xc++)
	{
		for (int zc = 0; zc < xChunks; zc++)
		{
			for (int yc = 0; yc < yChunks; yc++)
			{
				double s0 = density[((xc + 0) * zSize + (zc + 0)) * ySize + (yc + 0)];
				double s1 = density[((xc + 0) * zSize + (zc + 1)) * ySize + (yc + 0)];
				double s2 = density[((xc + 1) * zSize + (zc + 0)) * ySize + (yc + 0)];
				double s3 = density[((xc + 1) * zSize + (zc + 1)) * ySize + (yc + 0)];

				double s0a = (density[((xc + 0) * zSize + (zc + 0)) * ySize + (yc + 1)] - s0) * yStep;
				double s1a = (density[((xc + 0) * zSize + (zc + 1)) * ySize + (yc + 1)] - s1) * yStep;
				double s2a = (density[((xc + 1) * zSize + (zc + 0)) * ySize + (yc + 1)] - s2) * yStep;
				double s3a = (density[((xc + 1) * zSize + (zc + 1)) * ySize + (yc + 1)] - s3) * yStep;

				for (int y = 0; y < CHUNK_HEIGHT; y++)
				{
					double _s0 = s0;
					double _s1 = s1;
					double _s0a = (s2 - s0) * xStep;
					double _s1a = (s3 - s1) * xStep;

					for (int x = 0; x < CHUNK_WIDTH; x++)
					{
						const int bx = xc * CHUNK_WIDTH + x;
						double value = _s0;
						double valueStep = (_s1 - _s0) * zStep;
						value -= valueStep;

						for (int z = 0; z < CHUNK_WIDTH; z++)
						{
							value += valueStep;

							const int bz = zc * CHUNK_WIDTH + z;
							const int by = yc * CHUNK_HEIGHT + y;
							const int column = bx * 16 + bz;

							double yOffs = (by - columnTop[column]) * kVerticalFalloff / scale;
							if (yOffs < 0) yOffs *= 4;

							double fill = value - yOffs;

							if (by > Level::genDepthMinusOne - 16)
							{
								double slide = (by - (Level::genDepthMinusOne - 16)) / 16.0;
								fill = fill * (1 - slide) + (-10) * slide;
							}

							if (by <= terrain.baseDepth || fill > 0)
							{
								const int offs = column * Level::genDepth + by;
								tiles[offs] = (unsigned short) stoneId;
								blocks[offs] = (byte) Tile::stone_Id;
							}
						}

						_s0 += _s0a;
						_s1 += _s1a;
					}

					s0 += s0a;
					s1 += s1a;
					s2 += s2a;
					s3 += s3a;
				}
			}
		}
	}

	delete [] density.data;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			const int offs = (x * 16 + z) * Level::genDepth;
			int run = 0;
			for (int y = Level::genDepthMinusOne; y >= 0; y--)
			{
				if (tiles[offs + y] == 0)
				{
					run = 0;
					continue;
				}
				if (run == 0)
				{
					tiles[offs + y] = (unsigned short) surfaceId;
					run = 1;
				}
				else if (run < terrain.soilDepth)
				{
					tiles[offs + y] = (unsigned short) fillerId;
					run++;
				}
			}
		}
	}
}

LevelChunk *RubyLevelSource::create(int x, int z)
{
	return getChunk(x, z);
}

LevelChunk *RubyLevelSource::getChunk(int xOffs, int zOffs)
{
	m_random->setSeed(xOffs * 341873128712l + zOffs * 132897987541l);

	const unsigned int blocksSize = Level::genDepth * 16 * 16;
	byte *tileData = new byte[blocksSize];
	memset(tileData, 0, blocksSize);
	byteArray blocks = byteArray(tileData, blocksSize);

	unsigned short *tiles = new unsigned short[blocksSize];
	memset(tiles, 0, blocksSize * sizeof(unsigned short));

	prepareHeights(xOffs, zOffs, blocks, tiles);

	LevelChunk *levelChunk = new LevelChunk(m_level, blocks, xOffs, zOffs);
	delete [] tileData;

	levelChunk->setBlockData16(tiles, blocksSize);
	delete [] tiles;

	levelChunk->recalcHeightmap();

	return levelChunk;
}

bool RubyLevelSource::hasChunk(int x, int y)
{
	return true;
}

void RubyLevelSource::postProcess(ChunkSource *parent, int xt, int zt)
{
	HeavyTile::instaFall = true;

	const int xo = xt * 16;
	const int zo = zt * 16;

	m_pprandom->setSeed(m_level->getSeed());
	int64_t xScale = m_pprandom->nextLong() / 2 * 2 + 1;
	int64_t zScale = m_pprandom->nextLong() / 2 * 2 + 1;
	m_pprandom->setSeed(((xt * xScale) + (zt * zScale)) ^ m_level->getSeed());

	if (Biome *biome = m_level->getBiome(xo + 16, zo + 16))
	{
		biome->decorate(m_level, m_pprandom, xo, zo);
	}

	if (m_definition.terrain.lakes && m_pprandom->nextInt(4) == 0)
	{
		const int x = xo + m_pprandom->nextInt(16) + 8;
		const int y = m_pprandom->nextInt(Level::genDepth);
		const int z = zo + m_pprandom->nextInt(16) + 8;

		LakeFeature calmWater(Tile::calmWater_Id);
		calmWater.place(m_level, m_pprandom, x, y, z);
	}

	HeavyTile::instaFall = false;
}

bool RubyLevelSource::save(bool force, ProgressListener *progressListener)
{
	return true;
}

bool RubyLevelSource::tick()
{
	return false;
}

bool RubyLevelSource::shouldSave()
{
	return true;
}

wstring RubyLevelSource::gatherStats()
{
	return L"RubyLevelSource";
}

vector<Biome::MobSpawnerData *> *RubyLevelSource::getMobsAt(MobCategory *mobCategory, int x, int y, int z)
{
	Biome *biome = m_level->getBiome(x, z);
	if (biome == nullptr) return nullptr;

	return biome->getMobs(mobCategory);
}

TilePos *RubyLevelSource::findNearestMapFeature(Level *level, const wstring& featureName, int x, int y, int z)
{
	return nullptr;
}

void RubyLevelSource::recreateLogicStructuresForChunk(int chunkX, int chunkZ)
{
}
