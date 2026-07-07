#include "EndOutpostFeature.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"

EndOutpostFeature::EndOutpostFeature(int blockId) : Feature(blockId)
{
}

bool EndOutpostFeature::place(Level *level, Random *random, int x, int y, int z)
{
	for (int groundx = -3; groundx <= 3; groundx++)
	{
		for (int groundz = -3; groundz <= 3; groundz++)
		{
			int belowTile = level->getTile(x + groundx, y - 1, z + groundz);
    		int thisTile = level->getTile(x + groundx, y, z + groundz);

			if ((belowTile == Tile::endStone_Id || belowTile == Tile::endSand_Id || belowTile == Tile::veloettGrass_Id) && (thisTile != Tile::endStone_Id && thisTile != Tile::endSand_Id && thisTile != Tile::veloettGrass_Id && thisTile != Tile::purulLog_Id && thisTile != Tile::purulVeloett_Id))
			{
				//Clear space up to roof height
				for (int dx = -3; dx <= 3; dx++)
				{
					for (int dy = 0; dy <= 12; dy++)
					{
						for (int dz = -3; dz <= 3; dz++)
						{
							placeBlock(level, x + dx, y + dy, z + dz, 0, 0);
						}
					}
				}

				//Outpost
				for (int pillary1 = 0; pillary1 <= 5; pillary1++)
				{
					placeBlock(level, x + 3, y + pillary1, z + 3, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x + 3, y + pillary1, z + 3, Tile::endStone_Id, 4);
					}
				}

				for (int pillary2 = 0; pillary2 <= 5; pillary2++)
				{
					placeBlock(level, x - 3, y + pillary2, z + 3, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x - 3, y + pillary2, z + 3, Tile::endStone_Id, 4);
					}
				}

				for (int pillary3 = 0; pillary3 <= 5; pillary3++)
				{
					placeBlock(level, x + 3, y + pillary3, z - 3, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x + 3, y + pillary3, z - 3, Tile::endStone_Id, 4);
					}
				}

				for (int pillary4 = 0; pillary4 <= 5; pillary4++)
				{
					placeBlock(level, x - 3, y + pillary4, z - 3, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x - 3, y + pillary4, z - 3, Tile::endStone_Id, 4);
					}
				}
			
				for (int floorx = -3; floorx <= 3; floorx++)
				{
					placeBlock(level, x + floorx, y + 6, z, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x + floorx, y + 6, z + 3, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x + floorx, y + 6, z - 3, Tile::stoneSlab2Half_Id, 5);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x + floorx, y + 6, z, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x + floorx, y + 6, z + 3, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x + floorx, y + 6, z - 3, Tile::stoneSlab2Half_Id, 6);
					}
				}
			
				for (int floorz = -3; floorz <= 3; floorz++)
				{
					placeBlock(level, x, y + 6, z + floorz, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x + 3, y + 6, z + floorz, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x - 3, y + 6, z + floorz, Tile::stoneSlab2Half_Id, 5);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x, y + 6, z + floorz, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x + 3, y + 6, z + floorz, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x - 3, y + 6, z + floorz, Tile::stoneSlab2Half_Id, 6);
					}
				}
			
				placeBlock(level, x, y + 6, z, Tile::endStone_Id, 3);
			
				placeBlock(level, x + 4, y + 6, z + 4, Tile::endStone_Id, 5);
				placeBlock(level, x - 4, y + 6, z + 4, Tile::endStone_Id, 5);
				placeBlock(level, x + 4, y + 6, z - 4, Tile::endStone_Id, 5);
				placeBlock(level, x - 4, y + 6, z - 4, Tile::endStone_Id, 5);
			
				placeBlock(level, x + 3, y + 6, z + 4, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 6, z + 4, Tile::endStone_Id, 3);
				placeBlock(level, x + 3, y + 6, z - 4, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 6, z - 4, Tile::endStone_Id, 3);
			
				placeBlock(level, x + 4, y + 6, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x - 4, y + 6, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x + 4, y + 6, z - 3, Tile::endStone_Id, 3);
				placeBlock(level, x - 4, y + 6, z - 3, Tile::endStone_Id, 3);
			
				placeBlock(level, x + 3, y + 6, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 6, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x + 3, y + 6, z - 3, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 6, z - 3, Tile::endStone_Id, 3);
			
				for (int spillary1 = 0; spillary1 <= 5; spillary1++)
				{
					placeBlock(level, x + 2, y + 6 + spillary1, z + 2, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x + 2, y + 6 + spillary1, z + 2, Tile::endStone_Id, 4);
					}
				}

				for (int spillary2 = 0; spillary2 <= 5; spillary2++)
				{
					placeBlock(level, x - 2, y + 6 + spillary2, z + 2, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x - 2, y + 6 + spillary2, z + 2, Tile::endStone_Id, 4);
					}
				}

				for (int spillary3 = 0; spillary3 <= 5; spillary3++)
				{
					placeBlock(level, x + 2, y + 6 + spillary3, z - 2, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x + 2, y + 6 + spillary3, z - 2, Tile::endStone_Id, 4);
					}
				}

				for (int spillary4 = 0; spillary4 <= 5; spillary4++)
				{
					placeBlock(level, x - 2, y + 6 + spillary4, z - 2, Tile::endStone_Id, 3);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x - 2, y + 6 + spillary4, z - 2, Tile::endStone_Id, 4);
					}
				}

				placeBlock(level, x + 2, y + 8, z + 2, Tile::endStone_Id, 5);
				placeBlock(level, x - 2, y + 8, z + 2, Tile::endStone_Id, 5);
				placeBlock(level, x + 2, y + 8, z - 2, Tile::endStone_Id, 5);
				placeBlock(level, x - 2, y + 8, z - 2, Tile::endStone_Id, 5);
			
				for (int sfloorx = -2; sfloorx <= 2; sfloorx++)
				{
					placeBlock(level, x + sfloorx, y + 11, z, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x + sfloorx, y + 11, z + 2, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x + sfloorx, y + 11, z - 2, Tile::stoneSlab2Half_Id, 5);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x + sfloorx, y + 11, z, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x + sfloorx, y + 11, z + 2, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x + sfloorx, y + 11, z - 2, Tile::stoneSlab2Half_Id, 6);
					}
				}
			
				for (int sfloorz = -2; sfloorz <= 2; sfloorz++)
				{
					placeBlock(level, x, y + 11, z + sfloorz, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x + 2, y + 11, z + sfloorz, Tile::stoneSlab2Half_Id, 5);
					placeBlock(level, x - 2, y + 11, z + sfloorz, Tile::stoneSlab2Half_Id, 5);
					if (random->nextInt(3) == 0)
					{
						placeBlock(level, x, y + 11, z + sfloorz, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x + 2, y + 11, z + sfloorz, Tile::stoneSlab2Half_Id, 6);
						placeBlock(level, x - 2, y + 11, z + sfloorz, Tile::stoneSlab2Half_Id, 6);
					}
				}
			
				placeBlock(level, x, y + 11, z, Tile::endStone_Id, 3);
			
				placeBlock(level, x + 3, y + 11, z + 3, Tile::endStone_Id, 5);
				placeBlock(level, x - 3, y + 11, z + 3, Tile::endStone_Id, 5);
				placeBlock(level, x + 3, y + 11, z - 3, Tile::endStone_Id, 5);
				placeBlock(level, x - 3, y + 11, z - 3, Tile::endStone_Id, 5);
			
				placeBlock(level, x + 2, y + 11, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x - 2, y + 11, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x + 2, y + 11, z - 3, Tile::endStone_Id, 3);
				placeBlock(level, x - 2, y + 11, z - 3, Tile::endStone_Id, 3);
			
				placeBlock(level, x + 3, y + 11, z + 2, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 11, z + 2, Tile::endStone_Id, 3);
				placeBlock(level, x + 3, y + 11, z - 2, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 11, z - 2, Tile::endStone_Id, 3);
			
				placeBlock(level, x + 3, y + 11, z, Tile::endStone_Id, 3);
				placeBlock(level, x - 3, y + 11, z, Tile::endStone_Id, 3);
				placeBlock(level, x, y + 11, z + 3, Tile::endStone_Id, 3);
				placeBlock(level, x , y + 11, z - 3, Tile::endStone_Id, 3);
			
				//Spawner
				level->setTileAndData(x, y + 7, z, Tile::mobSpawner_Id, 0, Tile::UPDATE_CLIENTS);
				shared_ptr<MobSpawnerTileEntity> entity = dynamic_pointer_cast<MobSpawnerTileEntity>( level->getTileEntity(x, y + 7, z) );
				if( entity != NULL )
				{
					entity->getSpawner()->setEntityId(L"EvupulDark");
				}
			
				//Chest
				level->setTileAndData(x, y + 12, z, Tile::chest_Id, 0, Tile::UPDATE_CLIENTS);
				WeighedTreasureArray wrapperArray(endOutpostTreasure, TREASURE_ITEMS_COUNT);
				WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
				shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity>( level->getTileEntity(x, y + 12, z) );
				if (chest != NULL)
				{
					WeighedTreasure::addChestItems(random, treasure, chest, 6 + random->nextInt(3));
				}
			}
		}
	}
    return true;
}

WeighedTreasure *EndOutpostFeature::endOutpostTreasure[EndOutpostFeature::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::ironIngot_Id, 0, 4, 9, 9),
	new WeighedTreasure(Item::goldIngot_Id, 0, 3, 7, 9),
	new WeighedTreasure(Item::diamond_Id, 0, 1, 4, 5),
	new WeighedTreasure(Item::nethanium_Id, 0, 1, 3, 3),
	new WeighedTreasure(Item::zaniteGemstone_Id, 0, 3, 9, 7),
	new WeighedTreasure(Tile::enchantedGravitite_Id, 0, 1, 3, 3),
	new WeighedTreasure(Item::aphalafTooth_Id, 0, 4, 7, 6),
	new WeighedTreasure(Item::string_Id, 0, 3, 12, 5),
	new WeighedTreasure(Item::dartGold_Id, 0, 5, 15, 5),
	new WeighedTreasure(Item::dartEnchanted_Id, 0, 5, 15, 5),
	new WeighedTreasure(Tile::endoriumGrate_Id, 0, 3, 7, 5),
	new WeighedTreasure(Item::endoriumHoe_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::nusaShard_Id, 0, 1, 1, 1),
};