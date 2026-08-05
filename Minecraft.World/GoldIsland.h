#pragma once
#include "Feature.h"

class Level;

class GoldIsland : public Feature
{
public:
	GoldIsland(int blockId);
    virtual bool place(Level *level, Random *random, int x, int y, int z);
	virtual bool generate(Level *level, Random *random, int x, int y, int z, int l);
	bool generateBlob(Level *level, Random *random, int x, int y, int z, int l);

protected:
	Feature *whiteFlowerFeature;
	Feature *purpleFlowerFeature;
	Feature *grassFeature;
};
