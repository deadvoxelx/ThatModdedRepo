#pragma once
#include "Feature.h"

class Level;

class GoldDungeon : public Feature
{
public:
	GoldDungeon(int blockId);
    virtual bool place(Level *level, Random *random, int x, int y, int z);
	virtual bool generate(Level *level, Random *random, int x, int y, int z, int r, bool spawnBoss = true);
	
private:
	static const int TREASURE_ITEMS_COUNT = 30;
	static WeighedTreasure *goldDungeonTreasure[TREASURE_ITEMS_COUNT];
};
