#include "stdafx.h"
#include "BerryStemTile.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.h"

BerryStemTile::BerryStemTile(int id) : Bush(id)
{
}

void BerryStemTile::tick(Level *level, int x, int y, int z, Random *random)
{
	if (level->isClientSide) return;

	Bush::tick(level, x, y, z, random);

	if (level->getRawBrightness(x, y + 1, z) >= Level::MAX_BRIGHTNESS - 6)
	{
		if (random->nextInt(7) == 0)
		{
			int data = level->getData(x, y, z);
			if ((data & AGE_BIT) == 0)
			{
				level->setData(x, y, z, data | AGE_BIT, false);
			}
			else
			{
				growTree(level, x, y, z, random);
			}
		}
	}
}

void BerryStemTile::growTree(Level *level, int x, int y, int z, Random *random)
{
	level->setTileAndData(x, y, z, Tile::netherLeaves_Id, 3, Tile::UPDATE_NONE);
}