#include "stdafx.h"
#include "PackedIce.h"
#include "net.minecraft.world.level.h"

PackedIceTile::PackedIceTile(int id) : Tile(id, Material::ice)
{
	friction = 0.98f;
}

int PackedIceTile::getResourceCount(Random *random)
{
	return 0;
}

shared_ptr<ItemInstance> PackedIceTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::packedIce));
}