#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "BoneVineFeature.h"

bool BoneVineFeature::place(Level *level, Random *random, int x, int y, int z)
{
	for (int i = 0; i < 10; i++)
	{
		int x2 = x + random->nextInt(8) - random->nextInt(8);
		int y2 = y - random->nextInt(4) - random->nextInt(4);
		int z2 = z + random->nextInt(8) - random->nextInt(8);

		int ceiling = -1;
		for (int yy = y2; yy < y2 + 11; yy++)
		{
			if (!level->isEmptyTile(x2, yy, z2))
			{
				ceiling = yy;
				break;
			}
		}
		if (ceiling < 0) continue;

		int h = 3 + random->nextInt(random->nextInt(7) + 1);
		for (int yy = 1; yy <= h; yy++)
		{
			int vy = ceiling - yy;
			if (vy < 0) break;
			if (!level->isEmptyTile(x2, vy, z2)) break;
			if (!Tile::boneVine->canSurvive(level, x2, vy, z2)) break;

			level->setTileAndData(x2, vy, z2, Tile::boneVine_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	return true;
}

