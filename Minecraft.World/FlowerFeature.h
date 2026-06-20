#pragma once
#include "Feature.h"

class Level;

class FlowerFeature : public Feature
{
private:
	int tile;
	int data;

public:
	FlowerFeature (int tile, int data);
	bool place(Level *level, Random *random, int x, int y, int z);
};
