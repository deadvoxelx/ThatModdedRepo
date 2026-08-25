#pragma once
#include "TransparentTile.h"

class LeafTile : public TransparentTile
{
	friend class Tile;
public:
	static const int REQUIRED_WOOD_RANGE = 8;
    //static const int UPDATE_LEAF_BIT = 8;
	//static const int PERSISTENT_LEAF_BIT = 4;

	static const int NORMAL_LEAF = 0;
	static const int EVERGREEN_LEAF = 1;
	static const int BIRCH_LEAF = 2;
	static const int JUNGLE_LEAF = 3;
	static const int PURPLE_LEAF = 4;

	static const int LEAF_NAMES_LENGTH = 5;

	static const unsigned int LEAF_NAMES[LEAF_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

	int *checkBuffer;

private:
	Icon** icons;

protected:
	LeafTile(int id);
	virtual ~LeafTile();
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual int getResource(int data, Random *random, int playerBonusLevel);
	virtual int getResourceCountForLootBonus(int bonusLevel, Random *random);
	virtual int getResourceCount(Random *random);
	virtual void spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel);
	virtual void playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data);

	virtual Icon* getTexture(int face, int data);
	void registerIcons(IconRegister* iconRegister);
	virtual bool isSolidRender(bool isServerLevel = false);
	
	virtual int getColor() const;
	virtual int getColor(int data);
	virtual int getColor(LevelSource *level, int x, int y, int z);
	virtual int getColor(LevelSource *level, int x, int y, int z, int data);

	virtual void animateTick(Level *level, int x, int y, int z, Random *random);
	virtual void onRemove(Level *level, int x, int y, int z, int id, int data);
	virtual void tick(Level *level, int x, int y, int z, Random *random);

private:
	void die(Level *level, int x, int y, int z);
};
