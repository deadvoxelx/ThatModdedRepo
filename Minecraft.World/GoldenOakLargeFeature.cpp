#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "GoldenOakLargeFeature.h"

GoldenOakLargeFeature::GoldenOakLargeFeature(bool doUpdate) : Feature(doUpdate)
{
}

bool GoldenOakLargeFeature::branch(Level *level, Random *random, int x, int y, int z, int slant)
{
    int directionX = random->nextInt(3) - 1;
    int directionY = slant;
    int directionZ = random->nextInt(3) - 1;
    int i = x;
    int k = z;
    for (int n = 0; n < random->nextInt(2); n++)
	{
    	x += directionX;
    	y += directionY;
    	z += directionZ;
    	i -= directionX;
    	k -= directionZ;
    	if (level->getTile(x, y, z) == Tile::netherLeaves_Id)
		{
        	placeBlock(level, x, y, z, Tile::goldenOakLog_Id, 0);
        	placeBlock(level, i, y, k, Tile::goldenOakLog_Id, 0);
    	} 
    } 
    return true;
}

bool GoldenOakLargeFeature::place(Level *level, Random *random, int x, int y, int z)
{
    int belowTile = level->getTile(x, y - 1, z);
	if (belowTile != Tile::aetherGrass_Id && belowTile != Tile::aetherDirt_Id && belowTile != Tile::grass_Id && belowTile != Tile::dirt_Id) return false;
	
	placeBlock(level, x, y - 1, z, Tile::aetherDirt_Id, 0);
 
    int height = 9;
    for (int x1 = x - 3; x1 < x + 4; x1++)
	{
    	for (int y1 = y + 5; y1 < y + 12; y1++)
		{
        	for (int z1 = z - 3; z1 < z + 4; z1++)
			{
        		if ((x1 - x) * (x1 - x) + (y1 - y - 8) * (y1 - y - 8) + (z1 - z) * (z1 - z) < 12 + random->nextInt(5) /*&& world.func_147437_c(x1, y1, z1)*/)
				{
					placeBlock(level, x1, y1, z1, Tile::netherLeaves_Id, 2);
				} 
        	} 
    	} 
    }
	int topOffset = random->nextInt(3);
	for (int n = 0; n < height - topOffset; n++)
	{
		branch(level, random, x, y + n, z, n / 4 - 1);
		int t = level->getTile(x, y + n, z);
		if (t == 0 || t == Tile::netherLeaves_Id || t == Tile::tallgrass_Id) placeBlock(level, x, y + n, z, Tile::goldenOakLog_Id, 0);
	}
    return true;
}
