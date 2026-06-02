#pragma once
#include "TransparentTile.h"

class NetherLeavesTile : public TransparentTile
{
	friend class Tile;
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_SKYROOT = 1;
	static const int TYPE_GOLDENOAK = 2;

	static const int NETHER_LEAVES_NAMES_LENGTH = 3;

	static const unsigned int NETHER_LEAVES_NAMES[NETHER_LEAVES_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	Icon** icons;

protected:
	NetherLeavesTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual int getResource(int data, Random *random, int playerBonusLevel);
	virtual int getResourceCountForLootBonus(int bonusLevel, Random *random);
	virtual int getResourceCount(Random *random);
	virtual void spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel);

	virtual Icon* getTexture(int face, int data);
	void registerIcons(IconRegister* iconRegister);
};
