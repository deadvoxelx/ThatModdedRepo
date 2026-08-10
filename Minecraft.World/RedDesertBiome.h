#pragma once

#include "Biome.h"

class RedDesertBiome : public Biome
{
public:
	RedDesertBiome(int id);
	virtual void decorate(Level *level, Random *random, int xo, int zo);
};