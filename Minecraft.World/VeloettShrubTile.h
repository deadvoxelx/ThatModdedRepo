#pragma once
#include "Bush.h"

class Random;

class VeloettShrubTile : public Bush
{
	friend class Tile;
protected:
	VeloettShrubTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);
public:
	virtual bool mayPlace(Level *level, int x, int y, int z);
	virtual bool canSurvive(Level *level, int x, int y, int z);
    virtual void updateDefaultShape();
    virtual int getResource(int data, Random *random, int playerBonusLevel);
	virtual int getResourceCount(Random *random);
	virtual int getResourceCountForLootBonus(int bonusLevel, Random *random);
	virtual void spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel);
};
