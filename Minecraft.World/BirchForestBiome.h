#pragma once
#include "Biome.h"

class BirchForestBiome : public Biome
{
public:
    BirchForestBiome(int id);

	virtual Feature *getTreeFeature(Random *random);
	Feature *getGrassFeature(Random *random);
};
