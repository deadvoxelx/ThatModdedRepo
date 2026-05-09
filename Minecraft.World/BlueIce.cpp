#include "stdafx.h"
#include "BlueIce.h"
#include "net.minecraft.world.level.h"

BlueIceTile::BlueIceTile(int id) : Tile(id, Material::ice)
{
	friction = 0.999f;
}

int BlueIceTile::getResourceCount(Random *random)
{
	return 0;
}

shared_ptr<ItemInstance> BlueIceTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::blueIce));
}