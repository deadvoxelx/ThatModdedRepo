#pragma once

#include "ChunkSource.h"
class PerlinNoise;

class AetherFarlandsLevelSource : public ChunkSource
{
public:
	static const int CHUNK_HEIGHT = 4;
	static const int CHUNK_WIDTH = 8;
private:
	Random *random;
	Random *pprandom;

	LargeFeature *caveFeature;

	PerlinNoise *lperlinNoise1;
	PerlinNoise *lperlinNoise2;
	PerlinNoise *perlinNoise1;
	PerlinNoise *perlinNoise3;
public:
	PerlinNoise *scaleNoise;
	PerlinNoise *depthNoise;
private:
	PerlinNoise *floatingIslandScale;
	PerlinNoise *floatingIslandNoise;
	PerlinNoise *carvingNoise;
	PerlinNoise *terrainHeightNoise;

	Level *level;

public:
	AetherFarlandsLevelSource(Level *level, int64_t seed);
	~AetherFarlandsLevelSource();

	void prepareHeights(int xOffs, int zOffs, byteArray blocks, BiomeArray biomes);
	void buildSurfaces(int xOffs, int zOffs, byteArray blocks, BiomeArray biomes);

public:
	virtual LevelChunk *create(int x, int z);
	virtual LevelChunk *getChunk(int xOffs, int zOffs);

private:
	doubleArray getHeights(doubleArray buffer, int x, int y, int z, int xSize, int ySize, int zSize);
	void calcWaterDepths(ChunkSource *parent, int xt, int zt);

public:
	virtual bool hasChunk(int x, int y);
	virtual void lightChunk(LevelChunk *lc);
	virtual void postProcess(ChunkSource *parent, int xt, int zt);
	virtual bool save(bool force, ProgressListener *progressListener);
	virtual bool tick();
	virtual bool shouldSave();
	virtual wstring gatherStats();

public:
	virtual vector<Biome::MobSpawnerData *> *getMobsAt(MobCategory *mobCategory, int x, int y, int z);
	virtual TilePos *findNearestMapFeature(Level *level, const wstring& featureName, int x, int y, int z);
	void recreateLogicStructuresForChunk(int chunkX, int chunkZ);
};
