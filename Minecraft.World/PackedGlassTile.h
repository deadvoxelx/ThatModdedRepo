#pragma once
#include "Tile.h"

class PackedGlassTile : public Tile
{
	friend class Tile;
protected:
	PackedGlassTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual int getResourceCount(Random *random);
};