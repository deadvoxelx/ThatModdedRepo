#pragma once
#include "Feature.h"
#include "net.minecraft.world.level.tile.h"

class TreeFeature : public Feature
{
private:
	const int baseHeight;
	const bool addJungleFeatures;
	const int trunkType;
	const int leafType;
	const int trunkTile;
	const int leafTile;

public:
	TreeFeature(bool doUpdate);
	TreeFeature(bool doUpdate, int baseHeight, int trunkType, int leafType, bool addJungleFeatures, int trunkTile = Tile::treeTrunk_Id, int leafTile = Tile::leaves_Id);

	virtual bool place(Level *level, Random *random, int x, int y, int z);

private:
	void addVine(Level *level, int xx, int yy, int zz, int dir);
};
