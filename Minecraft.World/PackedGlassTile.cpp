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

int PackedGlassTile::getColor(LevelSource *level, int x, int y, int z)
{
	return (x * x * 3187961 + x * 987243 + y * y * 43297126 + y * 987121 + z * z * 927469861 + z * 1861) & 0xffffff;
}

int PackedGlassTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	return (x * x * 3187961 + x * 987243 + y * y * 43297126 + y * 987121 + z * z * 927469861 + z * 1861) & 0xffffff;
}
