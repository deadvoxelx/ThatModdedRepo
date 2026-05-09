#pragma once
#include "Tile.h"

class PackedIceTile : public Tile
{
	friend class Tile;
protected:
	PackedIceTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual int getResourceCount(Random *random);
};