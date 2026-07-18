#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "NusaShrubFeature.h"
#include "net.minecraft.world.level.tile.h"

bool NusaShrubFeature::place(Level *level, Random *random, int x, int y, int z)
{
	for (int i = 0; i < 48; i++)
	{
		int x2 = x + random->nextInt(8) - random->nextInt(8);
		int y2 = y + random->nextInt(4) - random->nextInt(4);
		int z2 = z + random->nextInt(8) - random->nextInt(8);
		if (!level->isEmptyTile(x2, y2, z2)) continue;
		if (level->getTile(x2, y2 - 1, z2) != Tile::nugrass_Id) continue;
		level->setTileAndData(x2, y2, z2, Tile::nusaShrub_Id, 0, Tile::UPDATE_CLIENTS);
	}
	return true;
}