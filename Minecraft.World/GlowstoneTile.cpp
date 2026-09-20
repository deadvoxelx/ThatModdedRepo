#include "stdafx.h"
#include "GlowstoneTile.h"
#include "net.minecraft.world.item.h"

Glowstonetile::Glowstonetile(int id, Material *material) : Tile(id, material)
{
}

int Glowstonetile::getResourceCount(Random *random)
{
	return 4;
}

int Glowstonetile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Item::yellowDust->id;
}
