#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "BirchFeature.h"
#include "net.minecraft.world.level.tile.h"

BirchFeature::BirchFeature(bool doUpdate) : Feature(doUpdate), baseHeight(9), trunkType(TreeTile::BIRCH_TRUNK), leafType(LeafTile::BIRCH_LEAF), trunkTile(Tile::treeTrunk_Id), leafTile(Tile::leaves_Id)
{
}

BirchFeature::BirchFeature(bool doUpdate, int baseHeight, int trunkType, int leafType, int trunkTile, int leafTile) : Feature(doUpdate), baseHeight(baseHeight), trunkType(trunkType), leafType(leafType), trunkTile(trunkTile), leafTile(leafTile)
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
    	if (level->getTile(x, y, z) == leafTile)
		{
        	placeBlock(level, x, y, z, trunkTile, trunkType);
        	placeBlock(level, i, y, k, trunkTile, trunkType);
    	} 
    } 
    return true;
}

bool BirchFeature::place(Level *level, Random *random, int x, int y, int z)
{
    Material *belowMaterial = level->getMaterial(x, y - 1, z);
	if (belowMaterial != Material::grass && belowMaterial != Material::dirt) return false;
	
	placeBlock(level, x, y - 1, z, Tile::dirt_Id, 0);
 
    int height = baseHeight;
    for (int x1 = x - 2; x1 < x + 3; x1++)
	{
    	for (int y1 = y + 5; y1 < y + 12; y1++)
		{
        	for (int z1 = z - 2; z1 < z + 3; z1++)
			{
        		if ((x1 - x) * (x1 - x) + (y1 - y - 8) * (y1 - y - 8) + (z1 - z) * (z1 - z) < 12 + random->nextInt(7))
				{
					placeBlock(level, x1, y1, z1, leafTile, leafType);
				} 
        	} 
    	} 
    }
	int topOffset = random->nextInt(3);
	for (int n = 0; n < height - topOffset; n++)
	{
		branch(level, random, x, y + n, z, n / 4 - 1);
		int t = level->getTile(x, y + n, z);
        if (t == 0 || t == leafTile || t == Tile::tallgrass_Id) placeBlock(level, x, y + n, z, trunkTile, trunkType);
	}
    return true;
}