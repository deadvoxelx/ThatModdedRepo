#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.levelgen.h"
#include "net.minecraft.world.level.levelgen.synth.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.levelgen.structure.h"
#include "net.minecraft.world.entity.h"
#include "BiomeSource.h"
#include "AetherLevelSource.h"

AetherLevelSource::AetherLevelSource(Level *level, int64_t seed)
{
	m_XZSize = AETHER_LEVEL_MIN_WIDTH;

	this->level = level;

	random = new Random(seed);
	pprandom = new Random(seed);

	lperlinNoise1 = new PerlinNoise(random, 16);
	lperlinNoise2 = new PerlinNoise(random, 16);
	perlinNoise1 = new PerlinNoise(random, 8);
	scaleNoise = new PerlinNoise(random, 10);
	depthNoise = new PerlinNoise(random, 16);

	floatingIslandNoise = new PerlinNoise(random, 4);
	carvingNoise = new PerlinNoise(random, 6);
}

AetherLevelSource::~AetherLevelSource()
{
	delete random;
	delete pprandom;
	delete lperlinNoise1;
	delete lperlinNoise2;
	delete perlinNoise1;
	delete scaleNoise;
	delete depthNoise;

	delete floatingIslandNoise;
	delete carvingNoise;
}

void AetherLevelSource::prepareHeights(int xOffs, int zOffs, byteArray blocks, BiomeArray biomes)
{
	doubleArray buffer;

	int xChunks = 16 / CHUNK_WIDTH;

	int waterHeight = 0;

	int xSize = xChunks + 1;
	int ySize = Level::genDepth / CHUNK_HEIGHT + 1;
	int zSize = xChunks + 1;
	buffer = getHeights(buffer, xOffs * xChunks, 0, zOffs * xChunks, xSize, ySize, zSize);

	for (int xc = 0; xc < xChunks; xc++)
	{
		for (int zc = 0; zc < xChunks; zc++)
		{
			for (int yc = 0; yc < Level::genDepth / CHUNK_HEIGHT; yc++)
			{
				double yStep = 1 / (double) CHUNK_HEIGHT;
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
					double xStep = 1 / (double) CHUNK_WIDTH;

					double _s0 = s0;
					double _s1 = s1;
					double _s0a = (s2 - s0) * xStep;
					double _s1a = (s3 - s1) * xStep;

					for (int x = 0; x < CHUNK_WIDTH; x++)
					{
						int offs = (x + xc * CHUNK_WIDTH) << Level::genDepthBitsPlusFour | (0 + zc * CHUNK_WIDTH) << Level::genDepthBits | (yc * CHUNK_HEIGHT + y);
						int step = 1 << Level::genDepthBits;
						double zStep = 1 / (double) CHUNK_WIDTH;

						double val = _s0;
						double vala = (_s1 - _s0) * zStep;
						for (int z = 0; z < CHUNK_WIDTH; z++)
						{
							int tileId = 0;
							if (val > 0)
							{
								tileId = Tile::holystone_Id;
							}

							blocks[offs] = (byte) tileId;
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

void AetherLevelSource::buildSurfaces(int xOffs, int zOffs, byteArray blocks, BiomeArray biomes)
{
	int waterHeight = Level::genDepth - 64;

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			int run = -1;
			int runDepth = 3;

			byte top = (byte) Tile::aetherGrass_Id;
			byte mid = (byte) Tile::aetherDirt_Id;
			byte base = (byte) Tile::holystone_Id;

			for (int y = Level::genDepthMinusOne; y >= 0; y--)
			{
				int offs = (z * 16 + x) * Level::genDepth + y;

				int old = blocks[offs];

				if (old == 0)
				{
					run = -1;
				}
				else if (old == Tile::holystone_Id)
				{
					if (run == -1)
					{
						if (runDepth <= 0)
						{
							top = 0;
							mid = base;
						}

						run = runDepth;
						blocks[offs] = top;
					}
					else if (run > 0)
					{
						run--;
						blocks[offs] = mid;
					}
				}
			}
		}
	}
}

LevelChunk *AetherLevelSource::create(int x, int z)
{
	return getChunk(x, z);
}

LevelChunk *AetherLevelSource::getChunk(int xOffs, int zOffs)
{
	random->setSeed(xOffs * 341873128712l + zOffs * 132897987541l);

	BiomeArray biomes;
	// 4J - now allocating this with a physical alloc & bypassing general memory management so that it will get cleanly freed
	unsigned int blocksSize = Level::genDepth * 16 * 16;
	byte *tileData = static_cast<byte *>(XPhysicalAlloc(blocksSize, MAXULONG_PTR, 4096, PAGE_READWRITE));
	XMemSet128(tileData,0,blocksSize);
	byteArray blocks = byteArray(tileData,blocksSize);
	//    byteArray blocks = byteArray(16 * level->depth * 16);

	//    LevelChunk *levelChunk = new LevelChunk(level, blocks, xOffs, zOffs);			// 4J moved below
	level->getBiomeSource()->getBiomeBlock(biomes, xOffs * 16, zOffs * 16, 16, 16, true);

	prepareHeights(xOffs, zOffs, blocks, biomes);
	buildSurfaces(xOffs, zOffs, blocks, biomes);

	// 4J - this now creates compressed block data from the blocks array passed in, so moved it until after the blocks are actually finalised. We also
	// now need to free the passed in blocks as the LevelChunk doesn't use the passed in allocation anymore.
	LevelChunk *levelChunk = new LevelChunk(level, blocks, xOffs, zOffs);
	XPhysicalFree(tileData);

	levelChunk->recalcHeightmap();

	//delete blocks.data; // Don't delete the blocks as the array data is actually owned by the chunk now
	delete biomes.data;

	return levelChunk;
}

void AetherLevelSource::lightChunk(LevelChunk *lc)
{
	lc->recalcHeightmap();
}

doubleArray AetherLevelSource::getHeights(doubleArray buffer, int x, int y, int z, int xSize, int ySize, int zSize)
{
	if (buffer.data == nullptr)
	{
		buffer = doubleArray(xSize * ySize * zSize);
	}

	double s = 1 * 684.412;
	double hs = 1 * 684.412;

	doubleArray pnr, ar, br, sr, dr;

	sr = scaleNoise->getRegion(sr, x, z, xSize, zSize, 1.121, 1.121, 0.5);
	dr = depthNoise->getRegion(dr, x, z, xSize, zSize, 200.0, 200.0, 0.5);

	s *= 2;

	pnr = perlinNoise1->getRegion(pnr, x, y, z, xSize, ySize, zSize, s / 80.0, hs / 160.0, s / 80.0);
	ar = lperlinNoise1->getRegion(ar, x, y, z, xSize, ySize, zSize, s, hs, s);
	br = lperlinNoise2->getRegion(br, x, y, z, xSize, ySize, zSize, s, hs, s);

	doubleArray inr;
	inr = floatingIslandNoise->getRegion(inr, x, z, xSize, zSize, 0.35, 0.35, 0.5);

	doubleArray cnr;
	cnr = carvingNoise->getRegion(cnr, x, y, z, xSize, ySize, zSize, s / 30.0, hs / 30.0, s / 30.0);

	float worldHalf = (float)m_XZSize;

	int p = 0;
	int pp = 0;

	for (int xx = 0; xx < xSize; xx++)
	{
		for (int zz = 0; zz < zSize; zz++)
		{
			double scale = ((sr[pp] + 256.0) / 512);
			if (scale > 1) scale = 1;

			double depth = (dr[pp] / 8000.0);
			if (depth < 0) depth = -depth * 0.3;
			depth = depth * 3.0 - 2.0;

			float islandVal = (float)inr[pp];
			float doffs = islandVal * 100.0f - 60.0f;

			float xd = (float)(xx + x);
			float zd = (float)(zz + z);
			float edgeDist = sqrt(xd * xd + zd * zd);
			float fadeStart = worldHalf * 0.85f;
			float edgeFade;
			if (edgeDist < fadeStart)
				edgeFade = 80.0f;
			else
				edgeFade = 80.0f - ((edgeDist - fadeStart) / (worldHalf - fadeStart)) * 180.0f;
			if (edgeFade < -100) edgeFade = -100;

			if (edgeFade < doffs) doffs = edgeFade;

			if (doffs < -100) doffs = -100;
			if (doffs > 80) doffs = 80;

			if (depth > 2) depth = 2;
			if (depth < -2) depth = -2;
			double depthShift = depth * 1.5;

			if (scale < 0) scale = 0;
			scale = (scale) + 0.5;

			pp++;

			double yCenter = ySize / 2.0 + depthShift;

			for (int yy = 0; yy < ySize; yy++)
			{
				double val = 0;
				double yOffs = (yy - yCenter) * 8 / scale;

				if (yOffs < 0) yOffs *= -1;

				double bb = ar[p] / 512;
				double cc = br[p] / 512;

				double v = (pnr[p] / 10 + 1) / 2;
				if (v < 0) val = bb;
				else if (v > 1) val = cc;
				else val = bb + (cc - bb) * v;
				val -= 8;
				val += doffs;

				double carve = cnr[p] / 384.0;
				if (val > 0 && carve < -6.0)
				{
					val += (carve + 6.0) * 2.0;
				}

				int r = 2;
				if (yy > ySize / 2 - r)
				{
					double slide = (yy - (ySize / 2 - r)) / (64.0f);
					if (slide < 0) slide = 0;
					if (slide > 1) slide = 1;
					val = val * (1 - slide) + -3000 * slide;
				}
				r = 10;
				if (yy < r)
				{
					double slide = (r - yy) / (r - 1.0f);
					val = val * (1 - slide) + -30 * slide;
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
	delete [] inr.data;
	delete [] cnr.data;

	return buffer;
}

void AetherLevelSource::calcWaterDepths(ChunkSource *parent, int xt, int zt)
{
	int xo = xt * 16;
	int zo = zt * 16;
	for (int x = 0; x < 16; x++)
	{
		int y = level->getSeaLevel();
		for (int z = 0; z < 16; z++)
		{
			int xp = xo + x + 7;
			int zp = zo + z + 7;
			int h = level->getHeightmap(xp, zp);
			if (h <= 0)
			{
				if (level->getHeightmap(xp - 1, zp) > 0 || level->getHeightmap(xp + 1, zp) > 0 || level->getHeightmap(xp, zp - 1) > 0 || level->getHeightmap(xp, zp + 1) > 0)
				{
					bool hadWater = false;
					if (hadWater || (level->getTile(xp - 1, y, zp) == Tile::calmWater_Id && level->getData(xp - 1, y, zp) < 7)) hadWater = true;
					if (hadWater || (level->getTile(xp + 1, y, zp) == Tile::calmWater_Id && level->getData(xp + 1, y, zp) < 7)) hadWater = true;
					if (hadWater || (level->getTile(xp, y, zp - 1) == Tile::calmWater_Id && level->getData(xp, y, zp - 1) < 7)) hadWater = true;
					if (hadWater || (level->getTile(xp, y, zp + 1) == Tile::calmWater_Id && level->getData(xp, y, zp + 1) < 7)) hadWater = true;
					if (hadWater)
					{
						for (int x2 = -5; x2 <= 5; x2++)
						{
							for (int z2 = -5; z2 <= 5; z2++)
							{
								int d = (x2 > 0 ? x2 : -x2) + (z2 > 0 ? z2 : -z2);

								if (d <= 5)
								{
									d = 6 - d;
									if (level->getTile(xp + x2, y, zp + z2) == Tile::calmWater_Id)
									{
										int od = level->getData(xp + x2, y, zp + z2);
										if (od < 7 && od < d)
										{
											level->setData(xp + x2, y, zp + z2, d, Tile::UPDATE_CLIENTS);
										}
									}
								}
							}
						}
						if (hadWater)
						{
							level->setTileAndData(xp, y, zp, Tile::calmWater_Id, 7, Tile::UPDATE_CLIENTS);
							for (int y2 = 0; y2 < y; y2++)
							{
								level->setTileAndData(xp, y2, zp, Tile::calmWater_Id, 8, Tile::UPDATE_CLIENTS);
							}
						}
					}
				}
			}
		}
	}
}

bool AetherLevelSource::hasChunk(int x, int y)
{
	return true;
}

void AetherLevelSource::postProcess(ChunkSource *parent, int xt, int zt)
{
	HeavyTile::instaFall = true;
	int xo = xt * 16;
	int zo = zt * 16;

	int64_t xScale = pprandom->nextLong() / 2 * 2 + 1;
	int64_t zScale = pprandom->nextLong() / 2 * 2 + 1;
	pprandom->setSeed(((xt * xScale) + (zt * zScale)) ^ level->getSeed());

	Biome *biome = level->getBiome(xo + 16, zo + 16);
	biome->decorate(level, pprandom, xo, zo);

	PIXBeginNamedEvent(0,"Lakes");
	if (pprandom->nextInt(4) == 0)
	{
		int x = xo + pprandom->nextInt(16) + 8;
		int y = pprandom->nextInt(Level::genDepth);
		int z = zo + pprandom->nextInt(16) + 8;

		LakeFeature calmWater(Tile::calmWater_Id);
		calmWater.place(level, pprandom, x, y, z);
	}
	PIXEndNamedEvent();

	HeavyTile::instaFall = false;

	app.processSchematics(parent->getChunk(xt,zt));
}

bool AetherLevelSource::save(bool force, ProgressListener *progressListener)
{
	return true;
}

bool AetherLevelSource::tick()
{
	return false;
}

bool AetherLevelSource::shouldSave()
{
	return true;
}

wstring AetherLevelSource::gatherStats()
{
	return L"RandomLevelSource";
}

vector<Biome::MobSpawnerData *> *AetherLevelSource::getMobsAt(MobCategory *mobCategory, int x, int y, int z)
{
	Biome *biome = level->getBiome(x, z);
	if (biome == nullptr)
	{
		return nullptr;
	}
	return biome->getMobs(mobCategory);
}

TilePos *AetherLevelSource::findNearestMapFeature(Level *level, const wstring& featureName, int x, int y, int z)
{
	return nullptr;
}

void AetherLevelSource::recreateLogicStructuresForChunk(int chunkX, int chunkZ)
{
}
