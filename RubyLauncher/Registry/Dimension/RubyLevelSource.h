#pragma once

#include "ChunkSource.h"
#include "DimensionRegistry.h"

class PerlinNoise;
class ProgressListener;

class RubyLevelSource : public ChunkSource
{
public:
	static const int CHUNK_HEIGHT = 4;
	static const int CHUNK_WIDTH = 8;

	RubyLevelSource(Level *level, int64_t seed, const DimensionDefinition &definition);
	virtual ~RubyLevelSource();

	virtual LevelChunk *create(int x, int z);
	virtual LevelChunk *getChunk(int xOffs, int zOffs);
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
	Biome *biomeAt(int xOffs, int zOffs);

	Level *m_level;
	Random *m_random;
	Random *m_pprandom;

	PerlinNoise *m_heightNoise;
	PerlinNoise *m_detailNoise;
	PerlinNoise *m_depthVariationNoise;
	PerlinNoise *m_selectorNoise;
	PerlinNoise *m_terrainNoise1;
	PerlinNoise *m_terrainNoise2;

	const DimensionDefinition m_definition;

	int m_stoneId;
	int m_surfaceId;
	int m_fillerId;
};
