#pragma once
#include "Tile.h"

class ChunkRebuildData;
class EndStoneTile : public Tile
{
	friend class ChunkRebuildData;
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_SMOOTH = 1;
    static const int TYPE_MOSSY = 2;
    static const int TYPE_BRICK = 3;
	static const int TYPE_MOSSY_BRICK = 4;
	static const int TYPE_CHISELED = 5;

	static const wstring TEXTURE_NAMES[];

	static const int END_STONE_NAMES_LENGTH = 6;

	static const unsigned int END_STONE_NAMES[END_STONE_NAMES_LENGTH];

private:
	Icon **icons;

public:

    EndStoneTile(int id);

public:
	virtual Icon *getTexture(int face, int data);

	virtual unsigned int getDescriptionId(int iData = -1);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister *iconRegister);
};