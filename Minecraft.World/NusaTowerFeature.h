#pragma once
#include "Feature.h"
#include "Material.h"

class WeighedTreasure;

class NusaTowerFeature : public Feature
{
public:
    NusaTowerFeature(int blockId);

    virtual bool place(Level *level, Random *random, int x, int y, int z);

private:
	static const int TREASURE_ITEMS_COUNT = 24;
	static WeighedTreasure *nusaTowerTreasure[TREASURE_ITEMS_COUNT];
};
