#pragma once
#include "Feature.h"
#include "net.minecraft.world.level.tile.h"

class Level;

class BirchFeature : public Feature
{
private:
	const int baseHeight;
	const int trunkType;
	const int leafType;
	const int trunkTile;
	const int leafTile;

public:
	BirchFeature(bool doUpdate);
	BirchFeature(bool doUpdate, int baseHeight, int trunkType, int leafType, int trunkTile = Tile::treeTrunk_Id, int leafTile = Tile::leaves_Id);
	bool branch(Level *level, Random *random, int x, int y, int z, int slant);
    virtual bool place(Level *level, Random *random, int x, int y, int z);
};
