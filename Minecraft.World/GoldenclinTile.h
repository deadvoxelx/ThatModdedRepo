#pragma once
#include "Tile.h"

class ChunkRebuildData;
class GoldenclinTile : public Tile
{
	friend class ChunkRebuildData;
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_BRICK = 1;

	static const wstring TEXTURE_NAMES[];

	static const int GOLDENCLIN_NAMES_LENGTH = 2;

	static const unsigned int GOLDENCLIN_NAMES[GOLDENCLIN_NAMES_LENGTH];

private:
	Icon **icons;

public:

    GoldenclinTile(int id);

public:
	virtual Icon *getTexture(int face, int data);

	virtual unsigned int getDescriptionId(int iData = -1);
	virtual int getSpawnResourcesAuxValue(int data);
	void registerIcons(IconRegister *iconRegister);
};