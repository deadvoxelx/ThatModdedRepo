#pragma once
#include "Tile.h"

class Random;

class DirtTile : public Tile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_AETHER = 1;

	static const int DIRT_NAMES_LENGTH = 2;

	static const unsigned int DIRT_NAMES[DIRT_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];
private:
	Icon** icons;
public:
	DirtTile(int id);
	virtual Icon* getTexture(int face, int data);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister* iconRegister);
};
