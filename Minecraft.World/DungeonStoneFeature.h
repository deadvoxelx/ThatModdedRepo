#pragma once
#include "Feature.h"


class DungeonStoneFeature : public Feature
{
private:
	int tile;

public:
	DungeonStoneFeature(int tile);

    virtual bool place(Level *level, Random *random, int x, int y, int z);
};