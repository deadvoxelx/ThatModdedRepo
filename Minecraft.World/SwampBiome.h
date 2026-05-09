#pragma once
#include "Biome.h"
class LevelSource;

class SwampBiome : public Biome
{
public:
	SwampBiome(int id);

	virtual Feature *getTreeFeature(Random *random);
};