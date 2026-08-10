#pragma once
#include "Feature.h"

class Level;

class GoldIsland : public Feature
{
public:
	GoldIsland(int blockId);
    virtual bool place(Level *level, Random *random, int x, int y, int z);
	bool place(Level *level, Random *random, int x, int y, int z, bool spawnBoss);
	virtual bool generate(Level *level, Random *random, int x, int y, int z, int l, bool spawnBoss = true);
	bool generateBlob(Level *level, Random *random, int x, int y, int z, int l);

protected:
	Feature *whiteFlowerFeature;
	Feature *purpleFlowerFeature;
	Feature *grassFeature;
};
