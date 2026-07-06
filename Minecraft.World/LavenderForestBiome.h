#pragma once
#include "Biome.h"

class LavenderBiome : public Biome
{
public:
	LavenderBiome(int id);

	virtual Feature *getTreeFeature(Random *random);
	Feature *getGrassFeature(Random *random);
};