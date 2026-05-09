#pragma once
#include "Tile.h"

class BlueIceTile : public Tile
{
	friend class Tile;
protected:
	BlueIceTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual int getResourceCount(Random *random);
};