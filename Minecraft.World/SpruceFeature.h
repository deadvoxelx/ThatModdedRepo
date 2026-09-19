#pragma once
#include "Feature.h"
#include "net.minecraft.world.level.tile.h"

class SpruceFeature : public Feature
{
private:
	const int baseHeight;
	const int trunkType;
	const int leafType;
	const int trunkTile;
	const int leafTile;

public:
	SpruceFeature(bool doUpdate);
	SpruceFeature(bool doUpdate, int baseHeight, int trunkType, int leafType, int trunkTile = Tile::treeTrunk_Id, int leafTile = Tile::leaves_Id);
    virtual bool place(Level *level, Random *random, int x, int y, int z);
};
