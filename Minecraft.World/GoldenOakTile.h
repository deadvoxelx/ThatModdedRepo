#pragma once
#include "Tile.h"

class ChunkRebuildData;
class GoldenOakTile : public Tile
{
	friend class ChunkRebuildData;
private:
	Icon *iconTop;

	// 4J Stu - I don't know why this is protected in Java
	//protected:
public:
	GoldenOakTile(int id);
public:
	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);
};