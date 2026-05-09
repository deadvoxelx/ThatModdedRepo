#include "stdafx.h"
#include "PackedGlassTile.h"
#include "net.minecraft.world.level.h"

PackedGlassTile::PackedGlassTile(int id) : Tile(id, Material::glass)
{
}

int PackedGlassTile::getResourceCount(Random *random)
{
	return 0;
}

shared_ptr<ItemInstance> PackedGlassTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::blueIce));
}