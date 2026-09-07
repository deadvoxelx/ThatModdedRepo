#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"
#include "DungeonStoneFeature.h"
#include "GoldDungeon.h"

static bool inIslandShape(int dx, int dy, int dz, int l)
{
	int k3 = Mth::floor(dx * (1.0 + (dy / l) * 10.0) / 0.8f);
	int i4 = dy;
	if (dy > l * 0.625)
	{
		i4 = Mth::floor(i4 * 1.375);
		i4 -= Mth::floor(l * 0.25);
	}
	else if (dy < l * -0.625)
	{
		i4 = Mth::floor(i4 * 1.350000023841858);
		i4 += Mth::floor(l * 0.25);
	}
	int k4 = Mth::floor(dz * (1.0 + (dy / l) * 10.0) / 0.8f);
	return Mth::sqrt(k3 * k3 + i4 * i4 + k4 * k4 + 0.0) <= l;
}

GoldDungeon::GoldDungeon(int blockId) : Feature(blockId)
{
}

bool GoldDungeon::place(Level *level, Random *random, int x, int y, int z)
{
    return generate(level, random, x, y, z, 24, true);
}
  
bool GoldDungeon::generate(Level *level, Random *random, int x, int y, int z, int r, bool spawnBoss)
{
    int islandL = r;
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
                int rnd = random->nextInt(48);

                if (j == 4)
		        {
                    placeBlock(level, x + k, y + j, z + m, Tile::dungeonStone_Id, 2);
                    if (rnd < 8)
				    {
                        (new DungeonStoneFeature(Tile::dungeonLight_Id))->place(level, random, x + k, y + j, z + m);
                    }
                }
		        else if (j > -4)
		        {
                    if (k == a || -k == a || m == a || -m == a)
			        {
                        placeBlock(level, x + k, y + j, z + m, Tile::dungeonStone_Id, 2);
                        if (rnd < 8)
				        {
                            (new DungeonStoneFeature(Tile::dungeonLight_Id))->place(level, random, x + k, y + j, z + m);
                        }
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
                    if (rnd < 8)
				    {
                        (new DungeonStoneFeature(Tile::dungeonLight_Id))->place(level, random, x + k, y + j, z + m);
                    }
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
                    }				        else if (i == wid + 4 && m == 0 && k == -2)
				        {
                        if (level->getTile(x + a, y + k, z + b) != Tile::treasureChest_Id)
                        {
                        level->setTileAndData(x + a, y + k, z + b, Tile::treasureChest_Id, 0, Tile::UPDATE_CLIENTS);
				        shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity >( level->getTileEntity(x + a, y + k, z + b) );
				        if (chest != NULL )
				        {
					        bool hasLoot = false;
					        for (unsigned int slot = 0; slot < chest->getContainerSize(); slot++)
					        {
						        if (chest->getItem(slot) != NULL)
						        {
							        hasLoot = true;
							        break;
						        }
					        }
					        if (!hasLoot)
					        {
						        WeighedTreasureArray wrapperArray(goldDungeonTreasure, TREASURE_ITEMS_COUNT);
						        WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
						        WeighedTreasure::addChestItems(random, treasure, chest, 9);
					        }
				        }
                        }
                        if (spawnBoss)
                        {
				            shared_ptr<SunSpirit> sunSpirit = shared_ptr<SunSpirit>(new SunSpirit(level));
				            sunSpirit->moveTo(x, y - 1, z, wid, direction);
				            level->addEntity(sunSpirit);
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
    return true;
}

WeighedTreasure *GoldDungeon::goldDungeonTreasure[GoldDungeon::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::lifeShard_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::regenerationStone_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::ironBubble_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::zanitePendant_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::zaniteRing_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::gravititePendant_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::gravititeRing_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::nethaniumPendant_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::nethaniumRing_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::gummySwetBlue_Id, 0, 1, 3, 3),
	new WeighedTreasure(Item::gummySwetGold_Id, 0, 1, 3, 3),
    new WeighedTreasure(Item::vampireBlade_Id, 0, 1, 1, 3),
    new WeighedTreasure(Item::valkyrieLance_Id, 0, 1, 1, 3),
    new WeighedTreasure(Item::valkyrieAxe_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::valkyriePickaxe_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::valkyrieShovel_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::valkyrieHoe_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::agilityCape_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::invisibilityCape_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::swetCape_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::valkyrieCape_Id, 0, 1, 1, 2),
    new WeighedTreasure(Item::gravititeSword_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeHelmet_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeChestplate_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeLeggings_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::gravititeBoots_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumHelmet_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumChestplate_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumLeggings_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::nethaniumBoots_Id, 0, 1, 1, 1),
};
