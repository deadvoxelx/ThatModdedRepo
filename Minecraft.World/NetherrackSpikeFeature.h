#pragma once
#include "Feature.h"
#include "Material.h"

class Level;

class NetherrackSpikeFeature : public Feature
{
public:
	NetherrackSpikeFeature(int blockId);
	virtual bool place(Level *level, Random *random, int x, int y, int z);
};
