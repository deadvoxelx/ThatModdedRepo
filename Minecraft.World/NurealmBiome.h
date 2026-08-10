#pragma once
#include "Biome.h"

class NurealmBiome : public Biome
{
public:
    NurealmBiome(int id);
	virtual Feature *getTreeFeature(Random *random);
	virtual Feature *getGrassFeature(Random *random);
	virtual int getSkyColor(float temp);
};
