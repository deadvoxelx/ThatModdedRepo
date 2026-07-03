#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "BirchFeature.h"
#include "net.minecraft.world.level.tile.h"

BirchFeature::BirchFeature(bool doUpdate) : Feature(doUpdate)
{
}

bool BirchFeature::branch(Level *level, Random *random, int x, int y, int z, int slant)
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
    	if (level->getTile(x, y, z) == Tile::leaves_Id)
		{
        	placeBlock(level, x, y, z, Tile::treeTrunk_Id, TreeTile::BIRCH_TRUNK);
        	placeBlock(level, i, y, k, Tile::treeTrunk_Id, TreeTile::BIRCH_TRUNK);
    	} 
    } 
    return true;
}

bool BirchFeature::place(Level *level, Random *random, int x, int y, int z)
{
    int belowTile = level->getTile(x, y - 1, z);
	if (belowTile != Tile::aetherGrass_Id && belowTile != Tile::aetherDirt_Id && belowTile != Tile::grass_Id && belowTile != Tile::dirt_Id) return false;
	
	placeBlock(level, x, y - 1, z, Tile::dirt_Id, 0);
 
    int height = 9;
    for (int x1 = x - 2; x1 < x + 3; x1++)
	{
    	for (int y1 = y + 5; y1 < y + 12; y1++)
		{
        	for (int z1 = z - 2; z1 < z + 3; z1++)
			{
        		if ((x1 - x) * (x1 - x) + (y1 - y - 8) * (y1 - y - 8) + (z1 - z) * (z1 - z) < 12 + random->nextInt(7))
				{
					placeBlock(level, x1, y1, z1, Tile::leaves_Id, LeafTile::BIRCH_LEAF);
				} 
        	} 
    	} 
    }
	int topOffset = random->nextInt(3);
	for (int n = 0; n < height - topOffset; n++)
	{
		branch(level, random, x, y + n, z, n / 4 - 1);
		int t = level->getTile(x, y + n, z);
		if (t == 0 || t == Tile::leaves_Id || t == Tile::tallgrass_Id) placeBlock(level, x, y + n, z, Tile::treeTrunk_Id, TreeTile::BIRCH_TRUNK);
	}
    return true;
}
