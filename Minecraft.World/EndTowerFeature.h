#pragma once
#include "Feature.h"
#include "Material.h"

class WeighedTreasure;

class EndTowerFeature : public Feature
{
public:
    EndTowerFeature(int blockId);

    virtual bool place(Level *level, Random *random, int x, int y, int z);

private:
	static const int TREASURE_ITEMS_COUNT = 19;
	static WeighedTreasure *endTowerTreasure[TREASURE_ITEMS_COUNT];
};
