#pragma once
#include "Tile.h"

class Random;

class FossilTile : public Tile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_NETHER = 1;
	static const int TYPE_AETHER = 2;
	static const int TYPE_END = 3;

	static const int FOSSIL_NAMES_LENGTH = 4;

	static const unsigned int FOSSIL_NAMES[FOSSIL_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];
private:
	Icon** icons;
public:
	FossilTile(int id);
	virtual Icon* getTexture(int face, int data);
	virtual int getSpawnResourcesAuxValue(int data);
	virtual void spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel);
	void registerIcons(IconRegister* iconRegister);
};