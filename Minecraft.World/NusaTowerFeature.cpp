#include "NusaTowerFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"

NusaTowerFeature::NusaTowerFeature(int blockId) : Feature(blockId)
{
}

bool NusaTowerFeature::place(Level *level, Random *random, int x, int y, int z)
{
    int belowTile = level->getTile(x, y - 1, z);
    int thisTile = level->getTile(x, y, z);
	{
		if ((belowTile == Tile::nustone_Id || belowTile == Tile::nugrass_Id) && (thisTile != Tile::nustone_Id && thisTile != Tile::nugrass_Id && thisTile != Tile::nusaLog_Id && thisTile != Tile::netherLeaves_Id))
		{
		// Clear space up to roof height
			for (int dx = -3; dx <= 3; dx++)
			{
				for (int dy = 0; dy <= 28; dy++)
				{
					for (int dz = -3; dz <= 3; dz++)
					{
						placeBlock(level, x + dx, y + dy, z + dz, 0, 0);
					}
				}
			}

		// UnderLayer
			for (int fx = -4; fx <= 4; fx++)
			{
				for (int fz = -2; fz <= 2; fz++)
				{
					placeBlock(level, x + fx, y - 1, z + fz, Tile::nustone_Id, 0);
				}
			}
			for (int gx = -2; gx <= 2; gx++)
			{
				for (int gz = -4; gz <= 4; gz++)
				{
					placeBlock(level, x + gx, y - 1, z + gz, Tile::nustone_Id, 0);
				}
			}

			placeBlock(level, x + 3, y - 1, z + 3, Tile::nustone_Id, 0);
			placeBlock(level, x - 3, y - 1, z - 3, Tile::nustone_Id, 0);
			placeBlock(level, x + 3, y - 1, z - 3, Tile::nustone_Id, 0);
			placeBlock(level, x - 3, y - 1, z + 3, Tile::nustone_Id, 0);

		// Walls
			for (int wallx = -2; wallx <= 2; wallx++)
			{
				for (int wally = 0; wally <= 23; wally++)
				{
					placeBlock(level, x + wallx, y + wally, z + 4, Tile::nustone_Id, 1);
					placeBlock(level, x + wallx, y + wally, z - 4, Tile::nustone_Id, 1);
				}
			}
			for (int wallz = -2; wallz <= 2; wallz++)
			{
				for (int wally = 0; wally <= 23; wally++)
				{
					placeBlock(level, x + 4, y + wally, z + wallz, Tile::nustone_Id, 1);
					placeBlock(level, x - 4, y + wally, z + wallz, Tile::nustone_Id, 1);
				}
			}
			for (int pillary = 0; pillary <= 27; pillary++)
			{
				placeBlock(level, x + 3, y + pillary, z + 3, Tile::nusaLog_Id, 0);
				placeBlock(level, x - 3, y + pillary, z + 3, Tile::nusaLog_Id, 0);
				placeBlock(level, x + 3, y + pillary, z - 3, Tile::nusaLog_Id, 0);
				placeBlock(level, x - 3, y + pillary, z - 3, Tile::nusaLog_Id, 0);
			}
			
		// Windows
			placeBlock(level, x + 4, y + 2, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x - 4, y + 2, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 2, z + 4, Tile::stained_glass_Id, 3);

			placeBlock(level, x + 4, y + 6, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x - 4, y + 6, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 6, z + 4, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 6, z - 4, Tile::stained_glass_Id, 3);

			placeBlock(level, x + 4, y + 10, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x - 4, y + 10, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 10, z + 4, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 10, z - 4, Tile::stained_glass_Id, 3);

			placeBlock(level, x + 4, y + 14, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x - 4, y + 14, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 14, z + 4, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 14, z - 4, Tile::stained_glass_Id, 3);

			placeBlock(level, x + 4, y + 18, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x - 4, y + 18, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 18, z + 4, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 18, z - 4, Tile::stained_glass_Id, 3);

			placeBlock(level, x + 4, y + 22, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x - 4, y + 22, z, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 22, z + 4, Tile::stained_glass_Id, 3);
			placeBlock(level, x, y + 22, z - 4, Tile::stained_glass_Id, 3);

		// Doorway
			placeBlock(level, x, y + 1, z + 4, 0, 0);
			placeBlock(level, x, y + 2, z + 4, 0, 0);
	
		// Floors
			for (int floorx = -3; floorx <= 3; floorx++)
			{
				for (int floorz = -2; floorz <= 2; floorz++)
				{
					placeBlock(level, x + floorx, y, z + floorz, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx, y + 4, z + floorz, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx, y + 8, z + floorz, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx, y + 12, z + floorz, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx, y + 16, z + floorz, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx, y + 20, z + floorz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int floorx2 = -2; floorx2 <= 2; floorx2++)
			{
				for (int floorz2 = -3; floorz2 <= 3; floorz2++)
				{
					placeBlock(level, x + floorx2, y, z + floorz2, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx2, y + 4, z + floorz2, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx2, y + 8, z + floorz2, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx2, y + 12, z + floorz2, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx2, y + 16, z + floorz2, Tile::netherPlanks_Id, 3);
					placeBlock(level, x + floorx2, y + 20, z + floorz2, Tile::netherPlanks_Id, 3);
				}
			}

			for (int floor7x = -4; floor7x <= 4; floor7x++)
			{
				for (int floor7z = -2; floor7z <= 2; floor7z++)
				{
					placeBlock(level, x + floor7x, y + 24, z + floor7z, Tile::netherPlanks_Id, 3);
				}
			}
			for (int floor7x2 = -2; floor7x2 <= 2; floor7x2++)
			{
				for (int floor7z2 = -4; floor7z2 <= 4; floor7z2++)
				{
					placeBlock(level, x + floor7x2, y + 24, z + floor7z2, Tile::netherPlanks_Id, 3);
				}
			}

			// 1st floor stairs
			placeBlock(level, x - 2, y + 1, z - 2, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 1, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 2, z - 1, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 2, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 3, z, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 3, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 4, z + 1, Tile::darkNustoneStairs_Id, 2);

			placeBlock(level, x - 2, y + 4, z - 2, 0, 0);
			placeBlock(level, x - 2, y + 4, z - 1, 0, 0);
			placeBlock(level, x - 2, y + 4, z, 0, 0);
			//
			
			// 2nd floor stairs
			placeBlock(level, x + 2, y + 5, z + 2, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 5, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 6, z + 1, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 6, z, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 7, z, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 7, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 8, z - 1, Tile::darkNustoneStairs_Id, 3);

			placeBlock(level, x + 2, y + 8, z + 2, 0, 0);
			placeBlock(level, x + 2, y + 8, z + 1, 0, 0);
			placeBlock(level, x + 2, y + 8, z, 0, 0);
			//

			// 3rd floor stairs
			placeBlock(level, x - 2, y + 9, z - 2, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 9, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 10, z - 1, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 10, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 11, z, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 11, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 12, z + 1, Tile::darkNustoneStairs_Id, 2);

			placeBlock(level, x - 2, y + 12, z - 2, 0, 0);
			placeBlock(level, x - 2, y + 12, z - 1, 0, 0);
			placeBlock(level, x - 2, y + 12, z, 0, 0);
			//
			
			// 4th floor stairs
			placeBlock(level, x + 2, y + 13, z + 2, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 13, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 14, z + 1, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 14, z, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 15, z, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 15, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 16, z - 1, Tile::darkNustoneStairs_Id, 3);

			placeBlock(level, x + 2, y + 16, z + 2, 0, 0);
			placeBlock(level, x + 2, y + 16, z + 1, 0, 0);
			placeBlock(level, x + 2, y + 16, z, 0, 0);
			//

			// 5th floor stairs
			placeBlock(level, x - 2, y + 17, z - 2, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 17, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 18, z - 1, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 18, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 19, z, Tile::darkNustoneStairs_Id, 2);
			placeBlock(level, x - 2, y + 19, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 20, z + 1, Tile::darkNustoneStairs_Id, 2);

			placeBlock(level, x - 2, y + 20, z - 2, 0, 0);
			placeBlock(level, x - 2, y + 20, z - 1, 0, 0);
			placeBlock(level, x - 2, y + 20, z, 0, 0);
			//
			
			// 6th floor stairs
			placeBlock(level, x + 2, y + 21, z + 2, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 21, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 22, z + 1, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 22, z, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 23, z, Tile::darkNustoneStairs_Id, 3);
			placeBlock(level, x + 2, y + 23, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 24, z - 1, Tile::darkNustoneStairs_Id, 3);

			placeBlock(level, x + 2, y + 24, z + 2, 0, 0);
			placeBlock(level, x + 2, y + 24, z + 1, 0, 0);
			placeBlock(level, x + 2, y + 24, z, 0, 0);
			//

			// 7th floor is the top floor, so no stairs

		// Chest/Core
			if (random->nextInt(2) == 0)
			{
				level->setTileAndData(x, y + 25, z, Tile::chest_Id, 0, Tile::UPDATE_CLIENTS);
				WeighedTreasureArray wrapperArray(nusaTowerTreasure, TREASURE_ITEMS_COUNT);
				WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
				shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity>( level->getTileEntity(x, y + 25, z) );
				if (chest != NULL)
				{
					WeighedTreasure::addChestItems(random, treasure, chest, 7 + random->nextInt(3));
				}
			}
			else
			{
				placeBlock(level, x, y + 25, z, Tile::nustone_Id, 1);
				placeBlock(level, x, y + 26, z, Tile::nusaCore_Id, 0);
			}

		// Roof
			for (int roofx = -4; roofx <= 4; roofx++)
			{
				for (int roofz = -4; roofz <= 4; roofz++)
				{
					placeBlock(level, x + roofx, y + 28, z + roofz, Tile::nustone_Id, 2);
				}
			}
			for (int roofx2 = -3; roofx2 <= 3; roofx2++)
			{
				for (int roofz2 = -5; roofz2 <= 5; roofz2++)
				{
					placeBlock(level, x + roofx2, y + 28, z + roofz2, Tile::nustone_Id, 2);
				}
			}
			for (int roofx3 = -5; roofx3 <= 5; roofx3++)
			{
				for (int roofz3 = -3; roofz3 <= 3; roofz3++)
				{
					placeBlock(level, x + roofx3, y + 28, z + roofz3, Tile::nustone_Id, 2);
				}
			}
		
			placeBlock(level, x + 6, y + 28, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 6, y + 28, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 6, y + 28, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 6, y + 28, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 28, z + 6, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 28, z - 6, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 28, z - 6, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 28, z + 6, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 6, y + 28, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 6, y + 28, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 28, z - 6, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 28, z + 6, Tile::nustone_Id, 2);

		// Rings
			// Floor 2
			placeBlock(level, x + 5, y + 4, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 4, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 4, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 4, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 4, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 4, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 4, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 4, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 4, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 4, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 4, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 4, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 4, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 4, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 4, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 4, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 4, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 4, z + 4, Tile::nustone_Id, 2);
		
			// Floor 3
			placeBlock(level, x + 5, y + 8, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 8, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 8, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 8, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 8, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 8, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 8, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 8, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 8, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 8, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 8, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 8, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 8, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 8, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 8, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 8, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 8, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 8, z + 4, Tile::nustone_Id, 2);
		
			// Floor 4
			placeBlock(level, x + 5, y + 12, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 12, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 12, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 12, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 12, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 12, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 12, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 12, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 12, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 12, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 12, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 12, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 12, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 12, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 12, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 12, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 12, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 12, z + 4, Tile::nustone_Id, 2);
		
			// Floor 5
			placeBlock(level, x + 5, y + 16, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 16, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 16, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 16, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 16, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 16, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 16, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 16, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 16, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 16, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 16, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 16, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 16, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 16, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 16, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 16, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 16, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 16, z + 4, Tile::nustone_Id, 2);
		
			// Floor 6
			placeBlock(level, x + 5, y + 20, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 20, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 20, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 20, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 20, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 20, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 20, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 20, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 20, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 20, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 20, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 20, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 20, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 20, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 20, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 20, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 20, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 20, z + 4, Tile::nustone_Id, 2);

			// Floor 7
			placeBlock(level, x + 5, y + 24, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 24, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 24, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 24, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 24, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 24, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 24, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 24, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 24, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 24, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 24, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 24, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 24, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 24, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 24, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 24, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 24, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 24, z + 4, Tile::nustone_Id, 2);
		}
	}
    return true;
}

WeighedTreasure *NusaTowerFeature::nusaTowerTreasure[NusaTowerFeature::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::coal_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::ironIngot_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::nethanium_Id, 0, 3, 7, 6),
	new WeighedTreasure(Tile::enchantedGravitite_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::endorium_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::endothaniumIngot_Id, 0, 1, 4, 3),
	new WeighedTreasure(Item::nethaniumBread_Id, 0, 3, 7, 5),
	new WeighedTreasure(Item::goldBread_Id, 0, 4, 9, 6),
	new WeighedTreasure(Item::apple_gold_Id, 1, 1, 1, 2),
	new WeighedTreasure(Item::endoriumHelmet_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumChestplate_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumLeggings_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumBoots_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::nusaShard_Id, 0, 1, 5, 5),
	new WeighedTreasure(Item::lifeShard_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::vampireBlade_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::valkyrieLance_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::valkyrieAxe_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::agilityCape_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::ironBubble_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::gravititePendant_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::gravititeRing_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumPendant_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumRing_Id, 0, 1, 1, 2),
};
