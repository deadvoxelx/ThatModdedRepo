#include "RubyUndergroundLevelSource.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "HeavyTile.h"
#include "HellSpringFeature.h"
#include "LargeHellCaveFeature.h"
#include "Level.h"
#include "LevelChunk.h"
#include "PerlinNoise.h"
#include "Random.h"
#include "Tile.h"
#include <cmath>
#include <cstring>

namespace
{
	const double kPi = 3.14159265358979323846;

	const int kLavaLevel = 32;
	const int kLavaWaterHeight = Level::genDepth - 64;
}

RubyUndergroundLevelSource::RubyUndergroundLevelSource(Level *level, int64_t seed, int xzSize, int stoneId, int surfaceId, int fillerId)
{
	m_level = level;

	m_XZSize = xzSize > 0 ? xzSize : LEVEL_MAX_WIDTH;

	m_stoneId = stoneId;
	m_surfaceId = surfaceId;
	m_fillerId = fillerId;

	m_caveFeature = new LargeHellCaveFeature();

	m_random = new Random(seed);
	m_pprandom = new Random(seed);
	m_lperlinNoise1 = new PerlinNoise(m_random, 16);
	m_lperlinNoise2 = new PerlinNoise(m_random, 16);
	m_perlinNoise1 = new PerlinNoise(m_random, 8);
	m_perlinNoise2 = new PerlinNoise(m_random, 4);
	m_perlinNoise3 = new PerlinNoise(m_random, 4);

	m_scaleNoise = new PerlinNoise(m_random, 10);
	m_depthNoise = new PerlinNoise(m_random, 16);
}

RubyUndergroundLevelSource::~RubyUndergroundLevelSource()
{
	delete m_caveFeature;

	delete m_random;
	delete m_pprandom;
	delete m_lperlinNoise1;
	delete m_lperlinNoise2;
	delete m_perlinNoise1;
	delete m_perlinNoise2;
	delete m_perlinNoise3;
	delete m_scaleNoise;
	delete m_depthNoise;
}

void RubyUndergroundLevelSource::prepareHeights(int xOffs, int zOffs, byteArray blocks, unsigned short *tiles)
{
	const int xChunks = 16 / CHUNK_WIDTH;

	const int xSize = xChunks + 1;
	const int ySize = Level::genDepth / CHUNK_HEIGHT + 1;
	const int zSize = xChunks + 1;
	doubleArray buffer;
	buffer = getHeights(buffer, xOffs * xChunks, 0, zOffs * xChunks, xSize, ySize, zSize);

	for (int xc = 0; xc < xChunks; xc++)
	{
		for (int zc = 0; zc < xChunks; zc++)
		{
			for (int yc = 0; yc < Level::genDepth / CHUNK_HEIGHT; yc++)
			{
				const double yStep = 1 / (double) CHUNK_HEIGHT;
				double s0 = buffer[((xc + 0) * zSize + (zc + 0)) * ySize + (yc + 0)];
				double s1 = buffer[((xc + 0) * zSize + (zc + 1)) * ySize + (yc + 0)];
				double s2 = buffer[((xc + 1) * zSize + (zc + 0)) * ySize + (yc + 0)];
				double s3 = buffer[((xc + 1) * zSize + (zc + 1)) * ySize + (yc + 0)];

				double s0a = (buffer[((xc + 0) * zSize + (zc + 0)) * ySize + (yc + 1)] - s0) * yStep;
				double s1a = (buffer[((xc + 0) * zSize + (zc + 1)) * ySize + (yc + 1)] - s1) * yStep;
				double s2a = (buffer[((xc + 1) * zSize + (zc + 0)) * ySize + (yc + 1)] - s2) * yStep;
				double s3a = (buffer[((xc + 1) * zSize + (zc + 1)) * ySize + (yc + 1)] - s3) * yStep;

				for (int y = 0; y < CHUNK_HEIGHT; y++)
				{
					const double xStep = 1 / (double) CHUNK_WIDTH;

					double _s0 = s0;
					double _s1 = s1;
					const double _s0a = (s2 - s0) * xStep;
					const double _s1a = (s3 - s1) * xStep;

					for (int x = 0; x < CHUNK_WIDTH; x++)
					{
						int offs = (x + xc * CHUNK_WIDTH) << Level::genDepthBitsPlusFour | (0 + zc * CHUNK_WIDTH) << Level::genDepthBits | (yc * CHUNK_HEIGHT + y);
						const int step = 1 << Level::genDepthBits;
						const double zStep = 1 / (double) CHUNK_WIDTH;

						double val = _s0;
						double vala = (_s1 - _s0) * zStep;

						for (int z = 0; z < CHUNK_WIDTH; z++)
						{
							int tileId = 0;
							if (yc * CHUNK_HEIGHT + y < kLavaLevel)
							{
								tileId = Tile::calmLava_Id;
							}
							if (val > 0)
							{
								tileId = m_stoneId;
							}

							placeTile(blocks, tiles, offs, tileId);

							offs += step;
							val += vala;
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

	delete [] buffer.data;
}

void RubyUndergroundLevelSource::buildSurfaces(int xOffs, int zOffs, byteArray blocks, unsigned short *tiles)
{
	const int waterHeight = kLavaWaterHeight;

	const double s = 1 / 32.0;

	doubleArray sandBuffer(16 * 16);
	doubleArray gravelBuffer(16 * 16);
	doubleArray depthBuffer(16 * 16);

	sandBuffer = m_perlinNoise2->getRegion(sandBuffer, xOffs * 16, zOffs * 16, 0, 16, 16, 1, s, s, 1);
	gravelBuffer = m_perlinNoise2->getRegion(gravelBuffer, xOffs * 16, 109, zOffs * 16, 16, 1, 16, s, 1, s);
	depthBuffer = m_perlinNoise3->getRegion(depthBuffer, xOffs * 16, zOffs * 16, 0, 16, 16, 1, s * 2, s * 2, s * 2);

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			const bool sand = (sandBuffer[x + z * 16] + m_random->nextDouble() * 0.2) > 0;
			const bool gravel = (gravelBuffer[x + z * 16] + m_random->nextDouble() * 0.2) > 0;
			const int runDepth = (int) (depthBuffer[x + z * 16] / 3 + 3 + m_random->nextDouble() * 0.25);

			int run = -1;

			int top = m_stoneId;
			int material = m_stoneId;

			for (int y = Level::genDepthMinusOne; y >= 0; y--)
			{
				int offs = (z * 16 + x) * Level::genDepth + y;

				bool blockSet = false;
				if (xOffs <= -(m_XZSize / 2))
				{
					if (z - m_random->nextInt(4) <= 0 || xOffs < -(m_XZSize / 2))
					{
						placeTile(blocks, tiles, offs, Tile::unbreakable_Id);
						blockSet = true;
					}
				}
				if (zOffs <= -(m_XZSize / 2))
				{
					if (x - m_random->nextInt(4) <= 0 || zOffs < -(m_XZSize / 2))
					{
						placeTile(blocks, tiles, offs, Tile::unbreakable_Id);
						blockSet = true;
					}
				}
				if (xOffs >= (m_XZSize / 2) - 1)
				{
					if (z + m_random->nextInt(4) >= 15 || xOffs > (m_XZSize / 2))
					{
						placeTile(blocks, tiles, offs, Tile::unbreakable_Id);
						blockSet = true;
					}
				}
				if (zOffs >= (m_XZSize / 2) - 1)
				{
					if (x + m_random->nextInt(4) >= 15 || zOffs > (m_XZSize / 2))
					{
						placeTile(blocks, tiles, offs, Tile::unbreakable_Id);
						blockSet = true;
					}
				}
				if (blockSet) continue;

				if (y >= Level::genDepthMinusOne - m_random->nextInt(5) || y <= 0 + m_random->nextInt(5))
				{
					placeTile(blocks, tiles, offs, Tile::unbreakable_Id);
				}
				else
				{
					const int old = tiles[offs];

					if (old == 0)
					{
						run = -1;
					}
					else if (old == m_stoneId)
					{
						if (run == -1)
						{
							if (runDepth <= 0)
							{
								top = 0;
								material = m_stoneId;
							}
							else if (y >= waterHeight - 4 && y <= waterHeight + 1)
							{
								top = m_fillerId;
								material = m_fillerId;
								if (gravel) top = m_fillerId;
								if (gravel) material = m_stoneId;
								if (sand) top = m_surfaceId;
								if (sand) material = m_surfaceId;
							}
							else if (y >= waterHeight - 30 && y <= waterHeight)
							{
								top = m_fillerId;
								material = m_fillerId;
								if (gravel) top = m_stoneId;
								if (gravel) material = m_stoneId;
								if (sand) top = m_surfaceId;
								if (sand) material = m_surfaceId;
							}
							else if (y >= waterHeight && y <= waterHeight + 52)
							{
								top = m_fillerId;
								material = m_fillerId;
								if (gravel) top = m_fillerId;
								if (gravel) material = m_fillerId;
							}
							else if (y >= waterHeight - 42 && y <= waterHeight - 32)
							{
								top = m_stoneId;
								material = m_stoneId;
							}

							run = runDepth;
							placeTile(blocks, tiles, offs, (y >= waterHeight - 1 || sand) ? top : material);
						}
						else if (run > 0)
						{
							run--;
							placeTile(blocks, tiles, offs, material);
						}
					}
				}
			}
		}
	}

	delete [] sandBuffer.data;
	delete [] gravelBuffer.data;
	delete [] depthBuffer.data;
}

doubleArray RubyUndergroundLevelSource::getHeights(doubleArray buffer, int x, int y, int z, int xSize, int ySize, int zSize)
{
	if (buffer.data == nullptr)
	{
		buffer = doubleArray(xSize * ySize * zSize);
	}

	const double s = 1 * 684.412;
	const double hs = 1 * 684.412 * 3;

	doubleArray pnr, ar, br, sr, dr;

	sr = m_scaleNoise->getRegion(sr, x, y, z, xSize, 1, zSize, 1.0, 0, 1.0);
	dr = m_depthNoise->getRegion(dr, x, y, z, xSize, 1, zSize, 100.0, 0, 100.0);

	pnr = m_perlinNoise1->getRegion(pnr, x, y, z, xSize, ySize, zSize, s / 80.0, hs / 60.0, s / 80.0);
	ar = m_lperlinNoise1->getRegion(ar, x, y, z, xSize, ySize, zSize, s, hs, s);
	br = m_lperlinNoise2->getRegion(br, x, y, z, xSize, ySize, zSize, s, hs, s);

	int p = 0;
	int pp = 0;
	doubleArray yoffs = doubleArray(ySize);
	for (int yy = 0; yy < ySize; yy++)
	{
		yoffs[yy] = cos(yy * kPi * 6 / (double) ySize) * 2;

		double dd = yy;
		if (yy > ySize / 2)
		{
			dd = (ySize - 1) - yy;
		}
		if (dd < 4)
		{
			dd = 4 - dd;
			yoffs[yy] -= dd * dd * dd * 10;
		}
	}

	for (int xx = 0; xx < xSize; xx++)
	{
		for (int zz = 0; zz < zSize; zz++)
		{
			double scale = ((sr[pp] + 256.0) / 512);
			if (scale > 1) scale = 1;

			const double floating = 0;

			double depth = (dr[pp] / 8000.0);
			if (depth < 0) depth = -depth;
			depth = depth * 3.0 - 3.0;

			if (depth < 0)
			{
				depth = depth / 2;
				if (depth < -1) depth = -1;
				depth = depth / 1.4;
				depth /= 2;
				scale = 0;
			}
			else
			{
				if (depth > 1) depth = 1;
				depth = depth / 6;
			}
			scale = (scale) + 0.5;
			depth = depth * ySize / 16;
			pp++;

			for (int yy = 0; yy < ySize; yy++)
			{
				double val = 0;

				const double yOffs = yoffs[yy];

				const double bb = ar[p] / 512;
				const double cc = br[p] / 512;

				const double v = (pnr[p] / 10 + 1) / 2;
				if (v < 0) val = bb;
				else if (v > 1) val = cc;
				else val = bb + (cc - bb) * v;
				val -= yOffs;

				if (yy > ySize - 4)
				{
					const double slide = (yy - (ySize - 4)) / (4 - 1.0f);
					val = val * (1 - slide) + -10 * slide;
				}

				if (yy < floating)
				{
					double slide = (floating - yy) / (4);
					if (slide < 0) slide = 0;
					if (slide > 1) slide = 1;
					val = val * (1 - slide) + -10 * slide;
				}

				buffer[p] = val;
				p++;
			}
		}
	}

	delete [] pnr.data;
	delete [] ar.data;
	delete [] br.data;
	delete [] sr.data;
	delete [] dr.data;
	delete [] yoffs.data;

	return buffer;
}

LevelChunk *RubyUndergroundLevelSource::create(int x, int z)
{
	return getChunk(x, z);
}

LevelChunk *RubyUndergroundLevelSource::getChunk(int xOffs, int zOffs)
{
	m_random->setSeed(xOffs * 341873128712l + zOffs * 132897987541l);

	const unsigned int blocksSize = Level::genDepth * 16 * 16;
	byte *tileData = new byte[blocksSize];
	memset(tileData, 0, blocksSize);
	byteArray blocks = byteArray(tileData, blocksSize);

	unsigned short *tiles = new unsigned short[blocksSize];
	memset(tiles, 0, blocksSize * sizeof(unsigned short));

	prepareHeights(xOffs, zOffs, blocks, tiles);
	buildSurfaces(xOffs, zOffs, blocks, tiles);

	byte *beforeCaves = new byte[blocksSize];
	memcpy(beforeCaves, tileData, blocksSize);

	m_caveFeature->apply(this, m_level, xOffs, zOffs, blocks);

	for (unsigned int i = 0; i < blocksSize; i++)
	{
		int tileId = tiles[i];

		if (blocks[i] != beforeCaves[i])
		{
			tileId = blocks[i];
		}

		const int mapped = remap(tileId);
		if (mapped != tileId) tileId = mapped;

		if ((unsigned short) tileId != tiles[i]) tiles[i] = (unsigned short) tileId;
		if ((byte) tileId != blocks[i]) blocks[i] = (byte) tileId;
	}

	delete [] beforeCaves;

	LevelChunk *levelChunk = new LevelChunk(m_level, blocks, xOffs, zOffs);
	levelChunk->setCheckAllLight();
	delete [] tileData;

	levelChunk->setBlockData16(tiles, blocksSize);
	delete [] tiles;

	return levelChunk;
}

void RubyUndergroundLevelSource::lightChunk(LevelChunk *lc)
{
	lc->recalcHeightmap();
}

bool RubyUndergroundLevelSource::hasChunk(int x, int y)
{
	return true;
}

void RubyUndergroundLevelSource::postProcess(ChunkSource *parent, int xt, int zt)
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

	for (int i = 0; i < 8; i++)
	{
		const int x = xo + m_pprandom->nextInt(16) + 8;
		const int y = m_pprandom->nextInt(Level::genDepth - 8) + 4;
		const int z = zo + m_pprandom->nextInt(16) + 8;

		HellSpringFeature(Tile::lava_Id, false).place(m_level, m_pprandom, x, y, z);
	}

	HeavyTile::instaFall = false;
}

bool RubyUndergroundLevelSource::save(bool force, ProgressListener *progressListener)
{
	return true;
}

bool RubyUndergroundLevelSource::tick()
{
	return false;
}

bool RubyUndergroundLevelSource::shouldSave()
{
	return true;
}

wstring RubyUndergroundLevelSource::gatherStats()
{
	return L"RubyUndergroundLevelSource";
}

vector<Biome::MobSpawnerData *> *RubyUndergroundLevelSource::getMobsAt(MobCategory *mobCategory, int x, int y, int z)
{
	Biome *biome = m_level->getBiome(x, z);
	if (biome == nullptr) return nullptr;

	return biome->getMobs(mobCategory);
}

TilePos *RubyUndergroundLevelSource::findNearestMapFeature(Level *level, const wstring& featureName, int x, int y, int z)
{
	return nullptr;
}

void RubyUndergroundLevelSource::recreateLogicStructuresForChunk(int chunkX, int chunkZ)
{
}
