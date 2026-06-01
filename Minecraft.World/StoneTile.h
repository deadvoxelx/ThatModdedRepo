#pragma once
#include "Tile.h"

class Random;

class StoneTile : public Tile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_HOLYSTONE = 1;
	static const int TYPE_HOLYSTONE_MOSSY = 2;

	static const int STONE_NAMES_LENGTH = 3;

	static const unsigned int STONE_NAMES[STONE_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];
private:
	Icon** icons;
public:
	StoneTile(int id);
	virtual Icon* getTexture(int face, int data);
	virtual int getResource(int data, Random* random, int playerBonusLevel);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister* iconRegister);
};
