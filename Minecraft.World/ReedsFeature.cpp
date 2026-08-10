#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.dimension.h"
#include "ReedsFeature.h"

bool ReedsFeature::place(Level *level, Random *random, int x, int y, int z)
{
	for (int i = 0; i < 20; i++)
	{
		int x2 = x + random->nextInt(4) - random->nextInt(4);
		int y2 = y;
		int z2 = z + random->nextInt(4) - random->nextInt(4);

		if(app.getLevelGenerationOptions() != nullptr)
		{
			LevelGenerationOptions *levelGenOptions = app.getLevelGenerationOptions();
			bool intersects = levelGenOptions->checkIntersects(x2, y2, z2, x2, y2, z2);
			if(intersects)
			{
				continue;
			}
		}
		if (level->dimension->id == -1)
		{
			if (level->isEmptyTile(x2, y2, z2)) 
			{
				if (level->getMaterial(x2-1, y2-1, z2) == Material::lava || 
					level->getMaterial(x2+1, y2-1, z2) == Material::lava || 
					level->getMaterial(x2, y2-1, z2-1) == Material::lava || 
					level->getMaterial(x2, y2-1, z2+1) == Material::lava)
				{

					int h = 2 + random->nextInt(4);
					for (int yy = 0; yy < h; yy++)
					{
						if ( Tile::lavaReed->canSurvive(level, x2, y2 + yy, z2) )
						{
							level->setTileAndData(x2, y2 + yy, z2, Tile::lavaReed_Id, 0, Tile::UPDATE_CLIENTS);
						}
					}
				}
			}
		}
		else
		{
			if (level->isEmptyTile(x2, y2, z2)) 
			{
				if (level->getMaterial(x2-1, y2-1, z2) == Material::water || 
					level->getMaterial(x2+1, y2-1, z2) == Material::water || 
					level->getMaterial(x2, y2-1, z2-1) == Material::water || 
					level->getMaterial(x2, y2-1, z2+1) == Material::water)
				{

					int h = 2 + random->nextInt(random->nextInt(3) + 1);
					for (int yy = 0; yy < h; yy++)
					{
						if ( Tile::reeds->canSurvive(level, x2, y2 + yy, z2) )
						{
							level->setTileAndData(x2, y2 + yy, z2, Tile::reeds_Id, 0, Tile::UPDATE_CLIENTS);
						}
					}
				}
			}
		}
	}
	return true;
}
