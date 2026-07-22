#pragma once
#include "Tile.h"

class ChunkRebuildData;
class NustoneTile : public Tile
{
	friend class ChunkRebuildData;
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_BRICK = 1;
    static const int TYPE_DARK = 2;

	static const wstring TEXTURE_NAMES[];
	static const int NUSTONE_NAMES_LENGTH = 3;
	static const unsigned int NUSTONE_NAMES[NUSTONE_NAMES_LENGTH];

private:
	Icon **icons;

public:
    NustoneTile(int id);

public:
	virtual int getSpawnResourcesAuxValue(int data);
	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);
};