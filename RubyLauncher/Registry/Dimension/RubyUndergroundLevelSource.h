#pragma once

#include "ChunkSource.h"
#include "Tile.h"

class Level;
class PerlinNoise;
class ProgressListener;
class LargeHellCaveFeature;

class RubyUndergroundLevelSource : public ChunkSource
{
public:
	static const int CHUNK_HEIGHT = 8;
	static const int CHUNK_WIDTH = 4;

	RubyUndergroundLevelSource(Level *level, int64_t seed, int xzSize, int stoneId, int surfaceId, int fillerId);
	virtual ~RubyUndergroundLevelSource();

	virtual LevelChunk *create(int x, int z);
	virtual LevelChunk *getChunk(int xOffs, int zOffs);
	virtual void lightChunk(LevelChunk *lc);
	virtual bool hasChunk(int x, int y);
	virtual void postProcess(ChunkSource *parent, int xt, int zt);
	virtual bool save(bool force, ProgressListener *progressListener);
	virtual bool tick();
	virtual bool shouldSave();
	virtual wstring gatherStats();
	virtual vector<Biome::MobSpawnerData *> *getMobsAt(MobCategory *mobCategory, int x, int y, int z);
	virtual TilePos *findNearestMapFeature(Level *level, const wstring& featureName, int x, int y, int z);
	virtual void recreateLogicStructuresForChunk(int chunkX, int chunkZ);

private:
	void prepareHeights(int xOffs, int zOffs, byteArray blocks, unsigned short *tiles);
	void buildSurfaces(int xOffs, int zOffs, byteArray blocks, unsigned short *tiles);
	doubleArray getHeights(doubleArray buffer, int x, int y, int z, int xSize, int ySize, int zSize);

	void placeTile(byteArray &blocks, unsigned short *tiles, int offs, int tileId)
	{
		blocks[offs] = (byte) tileId;
		tiles[offs] = (unsigned short) tileId;
	}

	int remap(int tileId) const
	{
		switch (tileId)
		{
		case Tile::netherRack_Id:
		case Tile::lavaRock_Id:
			return m_stoneId;
		case Tile::netherSoil_Id:
		case Tile::gravel_Id:
			return m_fillerId;
		case Tile::soulsand_Id:
			return m_surfaceId;
		default:
			return tileId;
		}
	}

	Level *m_level;
	Random *m_random;
	Random *m_pprandom;

	PerlinNoise *m_lperlinNoise1;
	PerlinNoise *m_lperlinNoise2;
	PerlinNoise *m_perlinNoise1;
	PerlinNoise *m_perlinNoise2;
	PerlinNoise *m_perlinNoise3;
	PerlinNoise *m_scaleNoise;
	PerlinNoise *m_depthNoise;

	LargeHellCaveFeature *m_caveFeature;

	int m_stoneId;
	int m_surfaceId;
	int m_fillerId;
};
