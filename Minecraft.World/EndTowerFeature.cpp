#include "EndTowerFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"

EndTowerFeature::EndTowerFeature(int blockId) : Feature(blockId)
{
}

bool EndTowerFeature::place(Level *level, Random *random, int x, int y, int z)
{
    int belowTile = level->getTile(x, y - 1, z);
    int thisTile = level->getTile(x, y, z);
	if ((belowTile == Tile::endStone_Id || belowTile == Tile::endSand_Id || belowTile == Tile::veloettGrass_Id) && (thisTile != Tile::endStone_Id && thisTile != Tile::endSand_Id && thisTile != Tile::veloettGrass_Id && thisTile != Tile::purulLog_Id && thisTile != Tile::purulVeloett_Id))
    {
		//UnderLayer
	    placeBlock(level, x, y - 1, z, Tile::endStone_Id, 0);
        placeBlock(level, x + 1, y - 1, z, Tile::endStone_Id, 0);
        placeBlock(level, x - 1, y - 1, z, Tile::endStone_Id, 0);
        placeBlock(level, x, y - 1, z + 1, Tile::endStone_Id, 0);
        placeBlock(level, x, y - 1, z - 1, Tile::endStone_Id, 0);
		
	    placeBlock(level, x + 1, y - 1, z + 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x + 1, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z + 1, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 2, y - 1, z + 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x + 2, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z + 2, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 1, y - 1, z + 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x + 1, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z + 2, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 2, y - 1, z + 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x + 2, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z + 1, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 2, y - 1, z, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z, Tile::endStone_Id, 0);
	    placeBlock(level, x, y - 1, z + 2, Tile::endStone_Id, 0);
	    placeBlock(level, x, y - 1, z - 2, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 3, y - 1, z + 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x + 3, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z + 2, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 2, y - 1, z + 3, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x + 2, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z + 3, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 3, y - 1, z + 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x + 3, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z + 1, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 1, y - 1, z + 3, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x + 1, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z + 3, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 3, y - 1, z, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z, Tile::endStone_Id, 0);
	    placeBlock(level, x, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x, y - 1, z + 3, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 3, y - 1, z + 3, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x + 3, y - 1, z - 3, Tile::endStone_Id, 0);
	    placeBlock(level, x - 3, y - 1, z + 3, Tile::endStone_Id, 0);

		placeBlock(level, x + 4, y - 1, z + 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 4, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x + 4, y - 1, z - 2, Tile::endStone_Id, 0);
	    placeBlock(level, x - 4, y - 1, z + 2, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 4, y - 1, z + 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 4, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x + 4, y - 1, z - 1, Tile::endStone_Id, 0);
	    placeBlock(level, x - 4, y - 1, z + 1, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 2, y - 1, z + 4, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z - 4, Tile::endStone_Id, 0);
	    placeBlock(level, x + 2, y - 1, z - 4, Tile::endStone_Id, 0);
	    placeBlock(level, x - 2, y - 1, z + 4, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 1, y - 1, z + 4, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z - 4, Tile::endStone_Id, 0);
	    placeBlock(level, x + 1, y - 1, z - 4, Tile::endStone_Id, 0);
	    placeBlock(level, x - 1, y - 1, z + 4, Tile::endStone_Id, 0);
		
		placeBlock(level, x + 4, y - 1, z, Tile::endStone_Id, 0);
	    placeBlock(level, x - 4, y - 1, z, Tile::endStone_Id, 0);
	    placeBlock(level, x, y - 1, z - 4, Tile::endStone_Id, 0);
	    placeBlock(level, x, y - 1, z + 4, Tile::endStone_Id, 0);
	
	
		//Floor 1
	    placeBlock(level, x, y, z, Tile::netherPlanks_Id, 1);
        placeBlock(level, x + 1, y, z, Tile::netherPlanks_Id, 1);
        placeBlock(level, x - 1, y, z, Tile::netherPlanks_Id, 1);
        placeBlock(level, x, y, z + 1, Tile::netherPlanks_Id, 1);
        placeBlock(level, x, y, z - 1, Tile::netherPlanks_Id, 1);
		
	    placeBlock(level, x + 1, y, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y, z + 4, Tile::endStone_Id, 3);
		
		//Floor 2
		placeBlock(level, x + 2, y + 4, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 4, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 4, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 4, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 4, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 4, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 4, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 4, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 4, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 4, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 4, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 4, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 4, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 4, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 4, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 4, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 4, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 4, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 4, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 4, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 4, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 4, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 4, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 4, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 4, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 4, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 4, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 4, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 4, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 4, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 4, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 4, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 4, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 4, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 4, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 4, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 4, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 4, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 4, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 4, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 4, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 4, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 4, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 4, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 4, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 4, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 4, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 4, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 4, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 4, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 4, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 4, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 4, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 4, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 5, y + 4, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 4, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 4, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 4, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 4, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 4, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 4, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 4, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 4, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 4, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 4, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 4, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 4, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 4, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 4, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 4, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 4, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 4, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 4, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 4, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 4, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 4, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 4, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 4, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 4, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 4, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 4, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 4, z + 4, Tile::obsidian_Id, 0);
		
		
		
		//Floor 3
		placeBlock(level, x + 2, y + 8, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 8, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 8, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 8, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 8, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 8, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 8, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 8, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 8, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 8, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 8, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 8, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 8, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 8, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 8, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 8, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 8, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 8, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 8, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 8, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 8, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 8, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 8, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 8, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 8, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 8, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 8, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 8, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 8, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 8, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 8, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 8, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 8, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 8, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 8, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 8, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 8, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 8, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 8, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 8, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 8, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 8, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 8, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 8, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 8, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 8, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 8, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 8, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 8, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 8, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 8, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 8, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 8, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 8, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 5, y + 8, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 8, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 8, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 8, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 8, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 8, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 8, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 8, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 8, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 8, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 8, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 8, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 8, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 8, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 8, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 8, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 8, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 8, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 8, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 8, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 8, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 8, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 8, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 8, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 8, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 8, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 8, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 8, z + 4, Tile::obsidian_Id, 0);
		
		
		
		//Floor 4
		placeBlock(level, x + 2, y + 12, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 12, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 12, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 12, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 12, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 12, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 12, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 12, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 12, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 12, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 12, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 12, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 12, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 12, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 12, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 12, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 12, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 12, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 12, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 12, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 12, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 12, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 12, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 12, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 12, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 12, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 12, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 12, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 12, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 12, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 12, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 12, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 12, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 12, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 12, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 12, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 12, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 12, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 12, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 12, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 12, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 12, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 12, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 12, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 12, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 12, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 12, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 12, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 12, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 12, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 12, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 12, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 12, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 12, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 5, y + 12, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 12, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 12, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 12, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 12, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 12, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 12, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 12, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 12, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 12, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 12, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 12, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 12, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 12, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 12, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 12, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 12, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 12, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 12, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 12, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 12, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 12, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 12, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 12, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 12, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 12, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 12, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 12, z + 4, Tile::obsidian_Id, 0);
		
		
		
		//Floor 5
		placeBlock(level, x + 2, y + 16, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 16, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 16, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 16, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 16, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 16, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 16, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 16, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 16, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 16, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 16, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 16, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 16, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 16, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 16, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 16, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 16, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 16, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 16, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 16, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 16, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 16, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 16, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 16, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 16, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 16, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 16, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 16, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 16, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 16, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 16, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 16, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 16, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 16, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 16, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 16, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 16, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 16, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 16, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 16, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 16, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 16, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 16, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 16, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 16, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 16, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 16, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 16, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 16, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 16, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 16, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 16, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 16, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 16, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 5, y + 16, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 16, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 16, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 16, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 16, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 16, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 16, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 16, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 16, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 16, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 16, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 16, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 16, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 16, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 16, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 16, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 16, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 16, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 16, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 16, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 16, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 16, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 16, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 16, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 16, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 16, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 16, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 16, z + 4, Tile::obsidian_Id, 0);
		
		
		
		//Floor 6
		placeBlock(level, x + 2, y + 20, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 20, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 20, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 20, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 20, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 20, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 20, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 20, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 20, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 20, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 20, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 20, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 20, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 20, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 20, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 20, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 20, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 20, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 20, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 20, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 20, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 20, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 20, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 20, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 20, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 20, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 20, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 20, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 20, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 20, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 20, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 20, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 20, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 20, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 20, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 20, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 20, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 20, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 20, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 20, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 20, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 20, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 20, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 20, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 20, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 20, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 20, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 20, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 20, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 20, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 20, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 20, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 20, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 20, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 5, y + 20, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 20, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 20, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 20, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 20, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 20, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 20, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 20, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 20, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 20, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 20, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 20, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 20, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 20, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 20, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 20, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 20, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 20, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 20, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 20, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 20, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 20, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 20, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 20, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 20, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 20, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 20, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 20, z + 4, Tile::obsidian_Id, 0);
		
		
		
		//Floor 7
		placeBlock(level, x + 2, y + 24, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 24, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 24, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 24, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 24, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 24, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 24, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 24, z - 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 24, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 24, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 24, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 24, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 24, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 24, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 3, y + 24, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 24, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 24, z + 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 24, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 24, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 24, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 24, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 3, y + 24, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 24, z - 3, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 24, z + 3, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 3, y + 24, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 24, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 24, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 24, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 24, z + 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 4, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 4, y + 24, z - 2, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 4, y + 24, z + 2, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 4, y + 24, z + 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 4, y + 24, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 4, y + 24, z - 1, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 4, y + 24, z + 1, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 2, y + 24, z + 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z - 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 2, y + 24, z - 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 2, y + 24, z + 4, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 1, y + 24, z + 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 24, z - 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x + 1, y + 24, z - 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 1, y + 24, z + 4, Tile::netherPlanks_Id, 1);
		
		placeBlock(level, x + 4, y + 24, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x - 4, y + 24, z, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 24, z - 4, Tile::netherPlanks_Id, 1);
	    placeBlock(level, x, y + 24, z + 4, Tile::netherPlanks_Id, 1);
		
		
		
		placeBlock(level, x + 5, y + 24, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 24, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 24, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 24, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 24, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 24, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 24, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 24, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 24, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 24, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 24, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 24, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 24, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 24, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 24, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 24, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 24, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 24, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 24, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 24, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 24, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 24, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 24, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 24, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 24, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 24, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 24, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 24, z + 4, Tile::obsidian_Id, 0);
		
		
		
		//Roof
	    placeBlock(level, x, y + 28, z, Tile::obsidian_Id, 0);
        placeBlock(level, x + 1, y + 28, z, Tile::obsidian_Id, 0);
        placeBlock(level, x - 1, y + 28, z, Tile::obsidian_Id, 0);
        placeBlock(level, x, y + 28, z + 1, Tile::obsidian_Id, 0);
        placeBlock(level, x, y + 28, z - 1, Tile::obsidian_Id, 0);
		
	    placeBlock(level, x + 1, y + 28, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 28, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 28, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 28, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z - 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 28, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 28, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 28, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 28, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 28, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 28, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z + 4, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 28, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z + 3, Tile::obsidian_Id, 0);

		placeBlock(level, x + 4, y + 28, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 28, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 4, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 28, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z + 4, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 28, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z + 4, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 28, z + 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z + 4, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 4, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 4, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z - 4, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z + 4, Tile::obsidian_Id, 0);
		
		
		
		placeBlock(level, x + 5, y + 28, z + 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 28, z - 3, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z + 3, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 28, z + 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 28, z - 2, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z + 2, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 28, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 5, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 3, y + 28, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 3, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 3, y + 28, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 2, y + 28, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 2, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 2, y + 28, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 28, z + 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 5, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 5, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z - 5, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z + 5, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 6, y + 28, z + 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 6, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 6, y + 28, z - 1, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 6, y + 28, z + 1, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 1, y + 28, z + 6, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z - 6, Tile::obsidian_Id, 0);
	    placeBlock(level, x + 1, y + 28, z - 6, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 1, y + 28, z + 6, Tile::obsidian_Id, 0);
		
		placeBlock(level, x + 6, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x - 6, y + 28, z, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z - 6, Tile::obsidian_Id, 0);
	    placeBlock(level, x, y + 28, z + 6, Tile::obsidian_Id, 0);
		
		
		
		//Walls
		placeBlock(level, x + 3, y + 1, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 1, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 1, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 1, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 1, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 1, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 1, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 1, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 1, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 1, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 1, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 1, z + 1, Tile::endStone_Id, 3);

		placeBlock(level, x + 2, y + 1, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 1, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 1, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 1, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 1, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 1, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 1, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 1, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 1, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 1, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 1, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 2, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 2, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 2, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 2, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 2, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 2, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 2, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 2, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 2, z + 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 2, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x + 4, y + 2, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 2, z + 1, Tile::endStone_Id, 5);

		placeBlock(level, x + 2, y + 2, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 2, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 2, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 2, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 2, z + 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 2, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x + 1, y + 2, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 2, z + 4, Tile::endStone_Id, 5);
		
		placeBlock(level, x + 4, y + 2, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x - 4, y + 2, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x, y + 2, z + 4, Tile::stained_glass_Id, 2);
		
		
		
		placeBlock(level, x + 3, y + 3, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 3, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 3, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 3, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 3, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 3, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 3, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 3, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 3, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 3, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 3, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 3, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 3, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 3, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 3, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 3, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 3, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 3, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 3, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 3, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 3, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 3, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 3, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 3, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 5, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 5, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 5, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 5, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 5, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 5, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 5, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 5, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 5, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 5, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 5, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 5, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 5, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 5, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 5, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 5, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 5, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 5, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 5, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 5, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 5, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 5, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 5, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 5, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 6, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 6, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 6, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 6, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 6, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 6, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 6, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 6, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 6, z + 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 6, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x + 4, y + 6, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 6, z + 1, Tile::endStone_Id, 5);

		placeBlock(level, x + 2, y + 6, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 6, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 6, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 6, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 6, z + 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 6, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x + 1, y + 6, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 6, z + 4, Tile::endStone_Id, 5);
		
		placeBlock(level, x + 4, y + 6, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x - 4, y + 6, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x, y + 6, z + 4, Tile::stained_glass_Id, 2);
		placeBlock(level, x, y + 6, z - 4, Tile::stained_glass_Id, 2);
		
		
		
		placeBlock(level, x + 3, y + 7, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 7, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 7, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 7, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 7, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 7, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 7, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 7, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 7, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 7, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 7, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 7, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 7, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 7, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 7, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 7, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 7, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 7, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 7, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 7, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 7, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 7, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 7, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 7, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 9, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 9, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 9, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 9, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 9, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 9, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 9, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 9, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 9, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 9, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 9, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 9, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 9, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 9, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 9, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 9, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 9, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 9, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 9, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 9, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 9, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 9, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 9, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 9, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 10, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 10, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 10, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 10, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 10, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 10, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 10, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 10, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 10, z + 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 10, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x + 4, y + 10, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 10, z + 1, Tile::endStone_Id, 5);

		placeBlock(level, x + 2, y + 10, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 10, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 10, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 10, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 10, z + 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 10, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x + 1, y + 10, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 10, z + 4, Tile::endStone_Id, 5);
		
		placeBlock(level, x + 4, y + 10, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x - 4, y + 10, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x, y + 10, z + 4, Tile::stained_glass_Id, 2);
		placeBlock(level, x, y + 10, z - 4, Tile::stained_glass_Id, 2);
		
		
		
		placeBlock(level, x + 3, y + 11, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 11, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 11, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 11, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 11, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 11, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 11, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 11, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 11, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 11, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 11, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 11, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 11, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 11, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 11, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 11, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 11, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 11, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 11, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 11, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 11, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 11, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 11, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 11, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 13, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 13, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 13, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 13, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 13, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 13, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 13, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 13, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 13, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 13, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 13, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 13, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 13, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 13, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 13, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 13, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 13, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 13, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 13, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 13, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 13, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 13, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 13, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 13, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 14, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 14, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 14, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 14, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 14, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 14, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 14, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 14, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 14, z + 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 14, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x + 4, y + 14, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 14, z + 1, Tile::endStone_Id, 5);

		placeBlock(level, x + 2, y + 14, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 14, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 14, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 14, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 14, z + 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 14, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x + 1, y + 14, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 14, z + 4, Tile::endStone_Id, 5);
		
		placeBlock(level, x + 4, y + 14, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x - 4, y + 14, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x, y + 14, z + 4, Tile::stained_glass_Id, 2);
		placeBlock(level, x, y + 14, z - 4, Tile::stained_glass_Id, 2);
		
		
		
		placeBlock(level, x + 3, y + 15, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 15, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 15, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 15, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 15, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 15, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 15, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 15, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 15, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 15, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 15, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 15, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 15, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 15, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 15, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 15, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 15, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 15, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 15, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 15, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 15, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 15, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 15, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 15, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 17, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 17, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 17, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 17, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 17, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 17, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 17, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 17, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 17, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 17, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 17, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 17, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 17, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 17, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 17, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 17, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 17, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 17, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 17, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 17, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 17, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 17, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 17, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 17, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 18, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 18, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 18, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 18, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 18, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 18, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 18, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 18, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 18, z + 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 18, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x + 4, y + 18, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 18, z + 1, Tile::endStone_Id, 5);

		placeBlock(level, x + 2, y + 18, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 18, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 18, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 18, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 18, z + 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 18, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x + 1, y + 18, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 18, z + 4, Tile::endStone_Id, 5);
		
		placeBlock(level, x + 4, y + 18, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x - 4, y + 18, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x, y + 18, z + 4, Tile::stained_glass_Id, 2);
		placeBlock(level, x, y + 18, z - 4, Tile::stained_glass_Id, 2);
		
		
		
		placeBlock(level, x + 3, y + 19, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 19, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 19, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 19, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 19, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 19, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 19, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 19, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 19, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 19, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 19, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 19, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 19, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 19, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 19, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 19, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 19, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 19, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 19, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 19, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 19, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 19, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 19, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 19, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 21, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 21, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 21, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 21, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 21, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 21, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 21, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 21, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 21, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 21, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 21, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 21, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 21, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 21, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 21, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 21, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 21, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 21, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 21, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 21, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 21, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 21, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 21, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 21, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 22, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 22, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 22, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 22, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 4, y + 22, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 22, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 22, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 22, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 22, z + 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 22, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x + 4, y + 22, z - 1, Tile::endStone_Id, 5);
	    placeBlock(level, x - 4, y + 22, z + 1, Tile::endStone_Id, 5);

		placeBlock(level, x + 2, y + 22, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 22, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 22, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 22, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 22, z + 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 22, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x + 1, y + 22, z - 4, Tile::endStone_Id, 5);
	    placeBlock(level, x - 1, y + 22, z + 4, Tile::endStone_Id, 5);
		
		placeBlock(level, x + 4, y + 22, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x - 4, y + 22, z, Tile::stained_glass_Id, 2);
	    placeBlock(level, x, y + 22, z + 4, Tile::stained_glass_Id, 2);
		placeBlock(level, x, y + 22, z - 4, Tile::stained_glass_Id, 2);
		
		
		
		placeBlock(level, x + 3, y + 23, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 23, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 23, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 23, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 4, y + 23, z + 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 23, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 23, z - 2, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 23, z + 2, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 23, z + 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 23, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x + 4, y + 23, z - 1, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 23, z + 1, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 2, y + 23, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 23, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 2, y + 23, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 2, y + 23, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 1, y + 23, z + 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 23, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x + 1, y + 23, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x - 1, y + 23, z + 4, Tile::endStone_Id, 3);
		
		placeBlock(level, x + 4, y + 23, z, Tile::endStone_Id, 3);
	    placeBlock(level, x - 4, y + 23, z, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 23, z - 4, Tile::endStone_Id, 3);
	    placeBlock(level, x, y + 23, z + 4, Tile::endStone_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 25, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 25, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 25, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 25, z + 3, Tile::purulLog_Id, 0);
		
		placeBlock(level, x + 3, y + 26, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 26, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 26, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 26, z + 3, Tile::purulLog_Id, 0);

		placeBlock(level, x + 3, y + 27, z + 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 27, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x + 3, y + 27, z - 3, Tile::purulLog_Id, 0);
	    placeBlock(level, x - 3, y + 27, z + 3, Tile::purulLog_Id, 0);
		
		
		
		//Ladder
		placeBlock(level, x, y + 1, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 1, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 2, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 2, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 3, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 3, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 4, z + 1, Tile::ladder_Id, 2);
		
		
		
		placeBlock(level, x, y + 5, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 5, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 6, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 6, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 7, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 7, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 8, z + 1, Tile::ladder_Id, 2);
		
		
		
		placeBlock(level, x, y + 9, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 9, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 10, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 10, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 11, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 11, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 12, z + 1, Tile::ladder_Id, 2);
		
		
		
		placeBlock(level, x, y + 13, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 13, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 14, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 14, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 15, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 15, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 16, z + 1, Tile::ladder_Id, 2);
		
		
		
		placeBlock(level, x, y + 17, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 17, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 18, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 18, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 19, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 19, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 20, z + 1, Tile::ladder_Id, 2);
		
		
		
		placeBlock(level, x, y + 21, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 21, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 22, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 22, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 23, z + 2, Tile::purulLog_Id, 0);
		placeBlock(level, x, y + 23, z + 1, Tile::ladder_Id, 2);
		
		placeBlock(level, x, y + 24, z + 1, Tile::ladder_Id, 2);

		level->setTileAndData(x, y + 25, z - 2, Tile::chest_Id, 0, Tile::UPDATE_CLIENTS);
			WeighedTreasureArray wrapperArray(endTowerTreasure, TREASURE_ITEMS_COUNT);
			WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
			shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity >( level->getTileEntity(x, y + 25, z - 2) );
			if (chest != NULL )
			{
				WeighedTreasure::addChestItems(random, treasure, chest, 8);
			}
    }
    return true;
}

WeighedTreasure *EndTowerFeature::endTowerTreasure[EndTowerFeature::TREASURE_ITEMS_COUNT] = 
{
		new WeighedTreasure(Item::saddle_Id, 0, 1, 1, 5),
		new WeighedTreasure(Item::ironIngot_Id, 0, 5, 11, 6),
		new WeighedTreasure(Item::goldIngot_Id, 0, 4, 12, 6),
		new WeighedTreasure(Item::diamond_Id, 0, 3, 7, 5),
		new WeighedTreasure(Tile::obsidian_Id, 0, 1, 4, 6),
		new WeighedTreasure(Item::gunpowder_Id, 0, 5, 12, 6),
		new WeighedTreasure(Item::string_Id, 0, 3, 12, 5),
		new WeighedTreasure(Item::nethanium_Id, 0, 1, 5, 3),
		new WeighedTreasure(Item::endorium_Id, 0, 1, 3, 2),
		new WeighedTreasure(Item::apple_gold_Id, 0, 1, 3, 3),
		new WeighedTreasure(Item::apple_gold_Id, 1, 1, 1, 1),
		new WeighedTreasure(Tile::endoriumGrate_Id, 0, 3, 7, 5),
};
