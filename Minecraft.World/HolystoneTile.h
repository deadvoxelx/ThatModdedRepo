#pragma once
#include "Tile.h"

class Random;

class HolystoneTile : public Tile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_MOSSY = 1;

	static const int HOLYSTONE_NAMES_LENGTH = 2;

	static const unsigned int HOLYSTONE_NAMES[HOLYSTONE_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];
private:
	Icon** icons;
public:
	HolystoneTile(int id);
	virtual Icon* getTexture(int face, int data);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister* iconRegister);
};