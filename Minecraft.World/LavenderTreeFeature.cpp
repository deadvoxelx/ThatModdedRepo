#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "LavenderTreeFeature.h"

LavenderTreeFeature::LavenderTreeFeature(bool doUpdate) : Feature(doUpdate)
{
}

bool LavenderTreeFeature::place(Level *level, Random *random, int x, int y, int z)
{
	int treeHeight = random->nextInt(6) + 8;
	int trunkHeight = 3 + random->nextInt(3);
	int topHeight = treeHeight - trunkHeight;
	int leafRadius = 2 + random->nextInt(2);

	bool free = true;
	if (y < 1 || y + treeHeight + 1 > Level::maxBuildHeight) return false;

	if (app.getLevelGenerationOptions() != NULL)
	{
		LevelGenerationOptions *levelGenOptions = app.getLevelGenerationOptions();
		bool intersects = levelGenOptions->checkIntersects(x - leafRadius, y - 1, z - leafRadius, x + leafRadius, y + treeHeight, z + leafRadius);
		if (intersects)
		{
			return false;
		}
	}

	for (int yy = y; yy <= y + 1 + treeHeight && free; yy++)
	{
		int r = 1;
		if ((yy - y) < trunkHeight)
		{
			r = 0;
		}
		else
		{
			r = leafRadius;
		}
		for (int xx = x - r; xx <= x + r && free; xx++)
		{
			for (int zz = z - r; zz <= z + r && free; zz++)
			{
				if (yy >= 0 && yy < Level::maxBuildHeight)
				{
					int tt = level->getTile(xx, yy, zz);
					if (tt != 0 && tt != Tile::leaves_Id) free = false;
				}
				else
				{
					free = false;
				}
			}
		}
	}

	if (!free) return false;

	int belowTile = level->getTile(x, y - 1, z);
	if ((belowTile != Tile::grass_Id && belowTile != Tile::dirt_Id) || y >= Level::maxBuildHeight - treeHeight - 1) return false;

	placeBlock(level, x, y - 1, z, Tile::dirt_Id, 1);

	int currentRadius = random->nextInt(2);
	int maxRadius = 1;
	int minRadius = 0;
	for (int heightPos = 0; heightPos <= topHeight; heightPos++)
	{
		const int yy = y + treeHeight - heightPos;

		for (int xx = x - currentRadius; xx <= x + currentRadius; xx++)
		{
			int xo = xx - (x);
			for (int zz = z - currentRadius; zz <= z + currentRadius; zz++)
			{
				int zo = zz - (z);
				if (abs(xo) == currentRadius && abs(zo) == currentRadius && currentRadius > 0) continue;
				if (!Tile::solid[level->getTile(xx, yy, zz)]) placeBlock(level, xx, yy, zz, Tile::leaves_Id, LeafTile::PURPLE_LEAF);
			}
		}

		if (currentRadius >= maxRadius)
		{
			currentRadius = minRadius;
			minRadius = 1;
			maxRadius += 1;
			if (maxRadius > leafRadius)
			{
				maxRadius = leafRadius;
			}
		}
		else
		{
			currentRadius = currentRadius + 1;
		}
	}

	int topOffset = random->nextInt(3);
	for (int hh = 0; hh < treeHeight - topOffset; hh++)
	{
		int t = level->getTile(x, y + hh, z);
		if (t == 0 || t == Tile::leaves_Id) placeBlock(level, x, y + hh, z, Tile::treeTrunk_Id, TreeTile::DARK_TRUNK);
	}
	return true;
}