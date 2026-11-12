#pragma once

#include "Biome.h"
#include <string>

class RubyBiome : public Biome
{
public:
	RubyBiome(int id);

	void applyDefinition(const std::wstring &name, int surface, int filler, float depth, float scale, float temperature, float downfall, bool snow, bool rain, int grassColor, int foliageColor, int waterColor, int skyColor);
	void setVegetationCounts(int grass, int flowers);
	void setGrassBlock(int tileId);
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
