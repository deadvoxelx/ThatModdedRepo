#pragma once
#include "Bush.h"

class Random;
class Level;

class BlackrootTile : public HangingPlant
{
	friend class Tile;

protected:
	BlackrootTile(int id);

private:
	Icon** icons;

public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_GROWN = 1;

	static const int BLACKROOT_NAMES_LENGTH = 2;

	static const unsigned int BLACKROOT_NAMES[BLACKROOT_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

	virtual void tick(Level *level, int x, int y, int z, Random *random);
	virtual Icon* getTexture(int face, int data);
	virtual int getSpawnResourcesAuxValue(int data);
	virtual void spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel);
	void registerIcons(IconRegister* iconRegister);
};
