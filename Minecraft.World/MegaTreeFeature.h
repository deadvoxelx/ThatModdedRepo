#pragma once

#include "Feature.h"
#include "net.minecraft.world.level.tile.h"

class MegaTreeFeature : public Feature
{
private:
	const int baseHeight;
	const int trunkType;
	const int leafType;
	const int trunkTile;
	const int leafTile;

public:
	MegaTreeFeature(bool doUpdate, int baseHeight, int trunkType, int leafType, int trunkTile = Tile::treeTrunk_Id, int leafTile = Tile::leaves_Id);

	bool place(Level *level, Random *random, int x, int y, int z);

private:
	void placeLeaves(Level *level, int x, int z, int topPosition, int baseRadius, Random *random);
};