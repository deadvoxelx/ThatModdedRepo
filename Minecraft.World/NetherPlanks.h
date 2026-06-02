#pragma once
#include "Tile.h"

class ChunkRebuildData;
class NetherPlanksTile : public Tile
{
	friend class ChunkRebuildData;
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_PURUL = 1;
	static const int TYPE_SKYROOT = 2;

	static const wstring TEXTURE_NAMES[];

	static const int NETHER_PLANK_NAMES_LENGTH = 3;

	static const unsigned int NETHER_PLANK_NAMES[NETHER_PLANK_NAMES_LENGTH];

private:
	Icon **icons;

public:

    NetherPlanksTile(int id);

public:
	virtual Icon *getTexture(int face, int data);

	virtual unsigned int getDescriptionId(int iData = -1);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister *iconRegister);
};
