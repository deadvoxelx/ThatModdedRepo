#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"
#include "GoldDungeon.h"

GoldDungeon::GoldDungeon(int blockId) : Feature(blockId)
{
}

bool GoldDungeon::place(Level *level, Random *random, int x, int y, int z)
{
    return generate(level, random, x, y, z, 24);
}
  
bool GoldDungeon::generate(Level *level, Random *random, int x, int y, int z, int r)
{
    r = Mth::floor(r * 0.8);
	
    int wid = Mth::sqrt(r * r / 2.0);
	
    for (int j = 4; j > -5; j--)
	{
        int a = wid;
        if (j >= 3 || j <= -3) a--; 
        if (j == 4 || j == -4) a--; 
        for (int k = -a; k <= a; k++)
	    {
            for (int m = -a; m <= a; m++)
		    {
                if (j == 4)
		        {
                    placeBlock(level, x + k, y + j, z + m, Tile::dungeonStone_Id, 2);
                }
		        else if (j > -4)
		        {
                    if (k == a || -k == a || m == a || -m == a)
			        {
                        placeBlock(level, x + k, y + j, z + m, Tile::dungeonStone_Id, 2);
                    }
			        else
			        {
                       placeBlock(level, x + k, y + j, z + m, 0);
                        if (j == -2 && (k == a - 1 || -k == a - 1 || m == a - 1 || -m == a - 1) && (k % 3 == 0 || m % 3 == 0)) placeBlock(level, x + k, y + j + 2, z + m, 0, 0); 
                    } 
                }
		        else
		        {
                    placeBlock(level, x + k, y + j, z + m, Tile::dungeonStone_Id, 2);
                    if ((k == a - 2 || -k == a - 2) && (m == a - 2 || -m == a - 2))
			        {
                        placeBlock(level, x + k, y + j + 1, z + m, Tile::netherRack_Id, 0);
                        placeBlock(level, x + k, y + j + 2, z + m, Tile::fire_Id, 0);
                    } 
                } 
            } 
        } 
    } 
    int direction = random->nextInt(4);
    for (int i = wid; i < wid + 32; i++)
	{
        for (int k = -3; k < 2; k++)
	    {
            for (int m = -3; m < 4; m++)
		    {
                int a = 0;
                int b = 0;

                if (direction / 2 == 0)
		        {
                    a = i;
                    b = m;
                }
		        else
		        {
                    a = m;
                    b = i;
                } 
                if (direction % 2 == 0)
		        {
                    a = -a;
                    b = -b;
                } 
                if (!level->isEmptyTile(x + a, y + k, z + b))
		        {
                    if (k == -3)
			        {
                        placeBlock(level, x + a, y + k, z + b, Tile::holystone_Id, 0);
                    }
			        else if (k < 1)
			        {
                        if (i == wid)
			            {
                            if (m < 2 && m > -2 && k < 0)
				            {
                                placeBlock(level, x + a, y + k, z + b, 0, 0);
                            }
				            else
				            {
                                placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                            } 
                        }
			            else if (m == 3 || m == -3)
			            {
                            placeBlock(level, x + a, y + k, z + b, Tile::holystone_Id, 0);
                        }
			            else
			            {
              	            placeBlock(level, x + a, y + k, z + b, 0);
              	            if (k == -1 && (m == 2 || m == -2) && (i - wid - 2) % 3 == 0) placeBlock(level, x + a, y + k, z + b, 0, 0); 
                        }
                    }
			        else if (i == wid)
			        {
                        placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                    }
			        else
			        {
                        placeBlock(level, x + a, y + k, z + b, Tile::holystone_Id, 0);
                    }
                }
                a = -a;
                b = -b;
                if (i < wid + 6)
                if (k == -3)
			    {
                    placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                }
			    else if (k < 1)
			    {
                    if (i == wid)
			        {
                        if (m < 2 && m > -2 && k < 0)
				        {
                            placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                        }
				        else
				        {
                            placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                        } 
                    }
			        else if (i == wid + 5)
			        {
                        placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                    }
			        else if (i == wid + 4 && m == 0 && k == -2)
			        {
                        level->setTileAndData(x + a, y + k, z + b, Tile::chest_Id, 0, Tile::UPDATE_CLIENTS);
				        WeighedTreasureArray wrapperArray(goldDungeonTreasure, TREASURE_ITEMS_COUNT);
				        WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
				        shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity >( level->getTileEntity(x + a, y + k, z + b) );
				        if (chest != NULL )
				        {
					        WeighedTreasure::addChestItems(random, treasure, chest, 6);
				        } 
                    }
			        else if (m == 3 || m == -3)
			        {
                        placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                    }
			        else
			        {
                        placeBlock(level, x + a, y + k, z + b, 0);
                        if (k == -1 && (m == 2 || m == -2) && (i - wid - 2) % 3 == 0) placeBlock(level, x + a, y + k, z + b, 0, 0); 
                    }
                }
                else
			    {
            	    placeBlock(level, x + a, y + k, z + b, Tile::dungeonStone_Id, 2);
                }
            }
        }
    }
    //EntityFireMonster boss = new EntityFireMonster(world, x, y - 1, z, wid, direction);
    //world.spawnEntityInWorld(boss);
    return true;
}

WeighedTreasure *GoldDungeon::goldDungeonTreasure[GoldDungeon::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::lifeShard_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::regenerationStone_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gummySwetBlue_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gummySwetGold_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeHelmet_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeChestplate_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeLeggings_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeBoots_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeSword_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumHelmet_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumChestplate_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumLeggings_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumBoots_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumSword_Id, 0, 1, 1, 1),
};