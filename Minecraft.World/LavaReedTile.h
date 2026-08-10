#pragma once
using namespace std;

#include "Tile.h"
#include "Definitions.h"

class Random;

class LavaReedTile : public Tile 
{
	friend class Tile;

protected:
	LavaReedTile(int id);

public:
    virtual void updateDefaultShape();
	void tick(Level *level, int x, int y, int z, Random* random);
	bool mayPlace(Level *level, int x, int y, int z);
	void neighborChanged(Level *level, int x, int y, int z, int type);

protected:
	const void checkAlive(Level *level, int x, int y, int z);

public:
	bool canSurvive(Level *level, int x, int y, int z);
	AABB *getAABB(Level *level, int x, int y, int z);
	int getResource(int data, Random *random, int playerBonusLevel);
	bool blocksLight();
	bool isSolidRender(bool isServerLevel = false);
	bool isCubeShaped();
	int getRenderShape();
	virtual int cloneTileId(Level *level, int x, int y, int z);
	virtual bool shouldTileTick(Level *level, int x,int y,int z);
};
