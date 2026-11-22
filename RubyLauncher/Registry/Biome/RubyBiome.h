#pragma once

#include "Biome.h"
#include <string>
#include <vector>

class MobCategory;

struct RubyMobSpawn
{
	eINSTANCEOF type = eTYPE_NOTSET;
	MobCategory *category = nullptr;
	int weight = 1;
	int minCount = 1;
	int maxCount = 1;
};

struct RubyDecoratorCounts
{
	int grass = 1;
	int flowers = 2;
	int deadBushes = 0;
	int cacti = 0;
	int reeds = 0;
	int mushrooms = 0;
	int waterlilies = 0;
	int sand = 3;
	int clay = 1;
	int gravel = 1;
};

class RubyBiome : public Biome
{
public:
	RubyBiome(int id);

	void applyDefinition(const std::wstring &name, int surface, int filler, float depth, float scale, float temperature, float downfall, bool snow, bool rain, int grassColor, int foliageColor, int waterColor, int skyColor);
	void setVegetationCounts(const RubyDecoratorCounts &counts);
	void setGrassBlock(int tileId);
	void setMobSpawns(const std::vector<RubyMobSpawn> &spawns);
	virtual Feature *getGrassFeature(Random *random);
	virtual int getGrassColor();
	virtual int getFolageColor();
	virtual int getWaterColor();
	virtual int getSkyColor(float temp);

private:
	int m_grassRGB;
	int m_foliageRGB;
	int m_waterRGB;
	int m_skyRGB;
	int m_grassTile = -1;
};
