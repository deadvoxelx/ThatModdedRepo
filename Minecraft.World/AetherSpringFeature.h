#pragma once
#include "Feature.h"


class AetherSpringFeature : public Feature
{
private:
	int tile;

public:
	AetherSpringFeature(int tile);

    virtual bool place(Level *level, Random *random, int x, int y, int z);
};