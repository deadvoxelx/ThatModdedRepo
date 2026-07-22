#include "NusaTempleFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.dimension.h"

NusaTempleFeature::NusaTempleFeature(int blockId) : Feature(blockId)
{
}

bool NusaTempleFeature::place(Level *level, Random *random, int x, int y, int z)
{
  while (y > 0 && !level->getMaterial(x, y - 1, z)->blocksMotion()) y--;

  int r = 16 * 3;

  int xc = Mth::floor(x * 1.0);
  int yc = Mth::floor(y * 1.0);
  int zc = Mth::floor(z * 1.0);

  int XZSIZE = level->dimension->getXZSize() * 16;
  int XZOFFSET = (XZSIZE / 2) - 32;

  if( (xc - r) < -XZOFFSET )
  {
	return false;
  }
  else if ( (xc + r) >= XZOFFSET )
  {
	return false;
  }
  if( (zc - r) < -XZOFFSET )
  {
	return false;
  }
  else if ( (zc + r) >= XZOFFSET )
  {
	return false;
  }

  if (random->nextInt(42) == 0)
  {
    for (int groundx = -6; groundx <= 6; groundx++)
	{
		for (int groundz = -6; groundz <= 6; groundz++)
		{
			for (int groundy = 0; groundy <= 5; groundy++)
			{
				Material *belowTile = level->getMaterial(x + groundx, y - 1, z + groundz);
    			Material *thisTile = level->getMaterial(x + groundx, y + 3 + groundy, z + groundz);

				if (belowTile->blocksMotion() && !thisTile->blocksMotion())
				{
					//Clear space up to roof height
					for (int dx = -6; dx <= 6; dx++)
					{
						for (int dy = 0; dy <= 9; dy++)
						{
							for (int dz = -6; dz <= 6; dz++)
							{
								placeBlock(level, x + dx, y + dy, z + dz, 0, 0);
							}
						}
					}
				
					//Temple
					for (int pyramidx = -6; pyramidx <= 6; pyramidx++)
					{
						for (int pyramidz = -6; pyramidz <= 6; pyramidz++)
						{
							//Under layer
							placeBlock(level, x + pyramidx, y - 1, z + pyramidz, Tile::endStone_Id, 0);
							placeBlock(level, x + pyramidx, y - 2, z + pyramidz, Tile::endStone_Id, 0);
							placeBlock(level, x + pyramidx, y - 3, z + pyramidz, Tile::endStone_Id, 0);
						
							//Base
							placeBlock(level, x + pyramidx, y, z + pyramidz, Tile::endStone_Id, 3);
							if (random->nextInt(3) == 0)
							{
								placeBlock(level, x + pyramidx, y, z + pyramidz, Tile::endStone_Id, 4);
							}
						
							placeBlock(level, x + 6, y, z + 6, Tile::endStone_Id, 5);
							placeBlock(level, x - 6, y, z - 6, Tile::endStone_Id, 5);
							placeBlock(level, x - 6, y, z + 6, Tile::endStone_Id, 5);
							placeBlock(level, x + 6, y, z - 6, Tile::endStone_Id, 5);
						}
					}
				
					for (int pyramidx = -5; pyramidx <= 5; pyramidx++)
					{
						for (int pyramidz = -5; pyramidz <= 5; pyramidz++)
						{
							placeBlock(level, x + pyramidx, y + 1, z + pyramidz, Tile::endStone_Id, 3);
							if (random->nextInt(3) == 0)
							{
								placeBlock(level, x + pyramidx, y + 1, z + pyramidz, Tile::endStone_Id, 4);
							}
						
							placeBlock(level, x + 5, y + 1, z + 5, Tile::endStone_Id, 5);
							placeBlock(level, x - 5, y + 1, z - 5, Tile::endStone_Id, 5);
							placeBlock(level, x - 5, y + 1, z + 5, Tile::endStone_Id, 5);
							placeBlock(level, x + 5, y + 1, z - 5, Tile::endStone_Id, 5);
						}
					}
				
					for (int pyramidx = -4; pyramidx <= 4; pyramidx++)
					{
						for (int pyramidz = -4; pyramidz <= 4; pyramidz++)
						{
							placeBlock(level, x + pyramidx, y + 2, z + pyramidz, Tile::endStone_Id, 3);
							if (random->nextInt(3) == 0)
							{
								placeBlock(level, x + pyramidx, y + 2, z + pyramidz, Tile::endStone_Id, 4);
							}
						
							placeBlock(level, x + 4, y + 2, z + 4, Tile::endStone_Id, 5);
							placeBlock(level, x - 4, y + 2, z - 4, Tile::endStone_Id, 5);
							placeBlock(level, x - 4, y + 2, z + 4, Tile::endStone_Id, 5);
							placeBlock(level, x + 4, y + 2, z - 4, Tile::endStone_Id, 5);
						}
					}
				
					for (int pyramidx = -3; pyramidx <= 3; pyramidx++)
					{
						for (int pyramidz = -3; pyramidz <= 3; pyramidz++)
						{
							placeBlock(level, x + pyramidx, y + 3, z + pyramidz, Tile::endStone_Id, 3);
							if (random->nextInt(3) == 0)
							{
								placeBlock(level, x + pyramidx, y + 3, z + pyramidz, Tile::endStone_Id, 4);
							}
						
							placeBlock(level, x + 3, y + 3, z + 3, Tile::endStone_Id, 5);
							placeBlock(level, x - 3, y + 3, z - 3, Tile::endStone_Id, 5);
							placeBlock(level, x - 3, y + 3, z + 3, Tile::endStone_Id, 5);
							placeBlock(level, x + 3, y + 3, z - 3, Tile::endStone_Id, 5);
						}
					}
				
					for (int pyramidx = -2; pyramidx <= 2; pyramidx++)
					{
						for (int pyramidz = -2; pyramidz <= 2; pyramidz++)
						{
							placeBlock(level, x + pyramidx, y + 3, z + pyramidz, Tile::nustone_Id, 1);
						}
					}
				
					placeBlock(level, x, y + 4, z, Tile::nustone_Id, 1);
					placeBlock(level, x, y + 5, z, Tile::nusaCore_Id, 0);

					for (int pillary = 0; pillary <= 5; pillary++)
					{
						placeBlock(level, x + 2, y + 3 + pillary, z + 2, Tile::purulLog_Id, 0);
						placeBlock(level, x - 2, y + 3 + pillary, z - 2, Tile::purulLog_Id, 0);
						placeBlock(level, x - 2, y + 3 + pillary, z + 2, Tile::purulLog_Id, 0);
						placeBlock(level, x + 2, y + 3 + pillary, z - 2, Tile::purulLog_Id, 0);
					
						placeBlock(level, x + 6, y + 1 + pillary, z + 6, Tile::cobbleWall_Id, 11);
						placeBlock(level, x - 6, y + 1 + pillary, z - 6, Tile::cobbleWall_Id, 11);
						placeBlock(level, x - 6, y + 1 + pillary, z + 6, Tile::cobbleWall_Id, 11);
						placeBlock(level, x + 6, y + 1 + pillary, z - 6, Tile::cobbleWall_Id, 11);
					
						placeBlock(level, x + 6, y + 7, z + 6, Tile::endoriumLamp_Id, 0);
						placeBlock(level, x - 6, y + 7, z - 6, Tile::endoriumLamp_Id, 0);
						placeBlock(level, x - 6, y + 7, z + 6, Tile::endoriumLamp_Id, 0);
						placeBlock(level, x + 6, y + 7, z - 6, Tile::endoriumLamp_Id, 0);
					}
			
					for (int floorx = -3; floorx <= 3; floorx++)
					{
						for (int floorz = -3; floorz <= 3; floorz++)
						{
							placeBlock(level, x + floorx, y + 9, z + floorz, Tile::stoneSlab2Half_Id, 5);
							placeBlock(level, x + floorx, y + 9, z + floorz, Tile::stoneSlab2Half_Id, 5);
							placeBlock(level, x + floorx, y + 9, z + floorz, Tile::stoneSlab2Half_Id, 5);
							if (random->nextInt(3) == 0)
							{
								placeBlock(level, x + floorx, y + 9, z + floorz, Tile::stoneSlab2Half_Id, 6);
								placeBlock(level, x + floorx, y + 9, z + floorz, Tile::stoneSlab2Half_Id, 6);
								placeBlock(level, x + floorx, y + 9, z + floorz, Tile::stoneSlab2Half_Id, 6);
							}
							placeBlock(level, x + 3, y + 9, z + 3, Tile::endStone_Id, 5);
							placeBlock(level, x - 3, y + 9, z - 3, Tile::endStone_Id, 5);
							placeBlock(level, x - 3, y + 9, z + 3, Tile::endStone_Id, 5);
							placeBlock(level, x + 3, y + 9, z - 3, Tile::endStone_Id, 5);
						
							placeBlock(level, x + 3, y + 9, z, Tile::endStone_Id, 3);
							placeBlock(level, x - 3, y + 9, z, Tile::endStone_Id, 3);
							placeBlock(level, x, y + 9, z + 3, Tile::endStone_Id, 3);
							placeBlock(level, x, y + 9, z - 3, Tile::endStone_Id, 3);
						}
					}
				
					placeBlock(level, x + 3, y + 3, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x + 3, y + 3, z - 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 3, y + 3, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 3, y + 3, z - 2, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 2, y + 3, z + 3, Tile::obsidian_Id, 0);
					placeBlock(level, x + 2, y + 3, z - 3, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y + 3, z + 3, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y + 3, z - 3, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 4, y + 2, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x + 4, y + 2, z - 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 4, y + 2, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 4, y + 2, z - 2, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 2, y + 2, z + 4, Tile::obsidian_Id, 0);
					placeBlock(level, x + 2, y + 2, z - 4, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y + 2, z + 4, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y + 2, z - 4, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 5, y + 1, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x + 5, y + 1, z - 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 5, y + 1, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 5, y + 1, z - 2, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 2, y + 1, z + 5, Tile::obsidian_Id, 0);
					placeBlock(level, x + 2, y + 1, z - 5, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y + 1, z + 5, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y + 1, z - 5, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 6, y, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x + 6, y, z - 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 6, y, z + 2, Tile::obsidian_Id, 0);
					placeBlock(level, x - 6, y, z - 2, Tile::obsidian_Id, 0);
				
					placeBlock(level, x + 2, y, z + 6, Tile::obsidian_Id, 0);
					placeBlock(level, x + 2, y, z - 6, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y, z + 6, Tile::obsidian_Id, 0);
					placeBlock(level, x - 2, y, z - 6, Tile::obsidian_Id, 0);
				}
				return true;
			}
		}
	}
  }
  return false;
}
