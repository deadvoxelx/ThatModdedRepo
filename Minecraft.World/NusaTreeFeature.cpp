#include "NusaTreeFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.tile.h"

NusaTreeFeature::NusaTreeFeature(bool doUpdate) : Feature(doUpdate)
{
}

bool NusaTreeFeature::place(Level *level, Random *random, int x, int y, int z)
{
	int belowTile = level->getTile(x, y - 1, z);
	int trunkHeight = 6 + random->nextInt(7);

	if (belowTile != Tile::nugrass_Id && belowTile != Tile::nustone_Id) return false;

	for (int ix = -3; ix <= 3; ix++)
	{
		for (int iy = 0; iy <= trunkHeight + 1; iy++)
		{
			for (int iz = -3; iz <= 3; iz++)
			{
				int thisTile = level->getTile(x + ix, y + iy, z + iz);
				if (thisTile != 0 && thisTile != Tile::nusaShrub_Id && thisTile != Tile::netherLeaves_Id) return false;
			}
		}
	}

	placeBlock(level, x, y - 1, z, Tile::nustone_Id, 0);

	for (int trunkbase = 0; trunkbase <= trunkHeight; trunkbase++)
	{
		placeBlock(level, x, y + trunkbase, z, Tile::nusaLog_Id, 0);
		foliage(level, random, x, y + trunkHeight + 1, z);
	}
    return true;
}

bool NusaTreeFeature::foliage(Level* level, Random* random, int x, int y, int z)
{
	for (int leavesx = -2; leavesx <= 2; leavesx++)
	{
		for (int leavesy = 0; leavesy <= 1; leavesy++)
		{
			for (int leavesz = -2; leavesz <= 2; leavesz++)
			{
				if (random->nextInt(7) == 0) placeBlock(level, x + leavesx, y + leavesy, z + leavesz, Tile::netherLeaves_Id, 6);
			}
		}
	}
	
	for (int leavesx = -2; leavesx <= 2; leavesx++)
	{
		for (int leavesy = 0; leavesy <= 3; leavesy++)
		{
			if (random->nextInt(7) == 0) placeBlock(level, x + leavesx, y + leavesy - 4, z + 3, Tile::netherLeaves_Id, 6);
			if (random->nextInt(7) == 0) placeBlock(level, x + leavesx, y + leavesy - 4, z - 3, Tile::netherLeaves_Id, 6);
		}
	}
	
	for (int leavesz = -2; leavesz <= 2; leavesz++)
	{
		for (int leavesy = 0; leavesy <= 3; leavesy++)
		{
			if (random->nextInt(7) == 0) placeBlock(level, x + 3, y + leavesy - 4, z + leavesz, Tile::netherLeaves_Id, 6);
			if (random->nextInt(7) == 0) placeBlock(level, x - 3, y + leavesy - 4, z + leavesz, Tile::netherLeaves_Id, 6);
		}
	}
	return true;
}
