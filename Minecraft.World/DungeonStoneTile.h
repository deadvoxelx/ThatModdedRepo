#pragma once
#include "Tile.h"

class Random;

class DungeonStoneTile : public Tile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_ANGELIC = 1;
	static const int TYPE_HELLFIRE = 2;

	static const int DUNGEONSTONE_NAMES_LENGTH = 3;

	static const unsigned int DUNGEONSTONE_NAMES[DUNGEONSTONE_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	Icon** icons;

public:
	DungeonStoneTile(int id);
	virtual Icon* getTexture(int face, int data);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister* iconRegister);
};