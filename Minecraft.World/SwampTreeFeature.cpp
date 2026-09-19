#include "stdafx.h"
#include "SwampTreeFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "JavaMath.h"

SwampTreeFeature::SwampTreeFeature(bool doUpdate) : Feature(doUpdate), baseHeight(5), trunkType(0), leafType(0), trunkTile(Tile::treeTrunk_Id), leafTile(Tile::leaves_Id)
{
}

SwampTreeFeature::SwampTreeFeature(bool doUpdate, int baseHeight, int trunkType, int leafType, int trunkTile, int leafTile) : Feature(doUpdate), baseHeight(baseHeight), trunkType(trunkType), leafType(leafType), trunkTile(trunkTile), leafTile(leafTile)
{
}

bool SwampTreeFeature::place(Level *level, Random *random, int x, int y, int z)
{
	int treeHeight = random->nextInt(4) + baseHeight;
	while (level->getMaterial(x, y - 1, z) == Material::water)
		y--;

	bool free = true;
	if (y < 1 || y + treeHeight + 1 > Level::genDepth) return false;

	// 4J Stu Added to stop tree features generating areas previously place by game rule generation
	if(app.getLevelGenerationOptions() != nullptr)
	{
		LevelGenerationOptions *levelGenOptions = app.getLevelGenerationOptions();
		bool intersects = levelGenOptions->checkIntersects(x - 3, y - 1, z - 3, x + 3, y + treeHeight, z + 3);
		if(intersects)
		{
			//app.DebugPrintf("Skipping reeds feature generation as it overlaps a game rule structure\n");
			return false;
		}
	}

	for (int yy = y; yy <= y + 1 + treeHeight; yy++) 
	{
		int r = 1;
		if (yy == y) r = 0;
		if (yy >= y + 1 + treeHeight - 2) r = 3;
		for (int xx = x - r; xx <= x + r && free; xx++)
		{
			for (int zz = z - r; zz <= z + r && free; zz++)
			{
				if (yy >= 0 && yy < Level::genDepth)
				{
					int tt = level->getTile(xx, yy, zz);
					if (tt != 0 && tt != leafTile)
					{
						if (tt == Tile::calmWater_Id || tt == Tile::water_Id)
						{
							if (yy > y) free = false;
						}
						else
						{
							free = false;
						}
					}
				}
				else
				{
					free = false;
				}
			}
		}
	}

	if (!free) return false;

	Material *belowMaterial = level->getMaterial(x, y - 1, z);
	if ((belowMaterial != Material::grass && belowMaterial != Material::dirt) || y >= Level::maxBuildHeight - treeHeight - 1 ) return false;

	placeBlock(level, x, y - 1, z, Tile::dirt_Id);

	for (int yy = y - 3 + treeHeight; yy <= y + treeHeight; yy++)
	{
		int yo = yy - (y + treeHeight);
		int offs = 2 - yo / 2;
		for (int xx = x - offs; xx <= x + offs; xx++)
		{
			int xo = xx - (x);
			for (int zz = z - offs; zz <= z + offs; zz++)
			{
				int zo = zz - (z);
				if (abs(xo) == offs && abs(zo) == offs && (random->nextInt(2) == 0 || yo == 0)) continue;
				if (!Tile::solid[level->getTile(xx, yy, zz)]) placeBlock(level, xx, yy, zz, leafTile, leafType);
			}
		}
	}

	for (int hh = 0; hh < treeHeight; hh++)
	{
		int t = level->getTile(x, y + hh, z);
		if (t == 0 || t == leafTile || t == Tile::water_Id || t == Tile::calmWater_Id) placeBlock(level, x, y + hh, z, trunkTile, trunkType);
	}

	for (int yy = y - 3 + treeHeight; yy <= y + treeHeight; yy++)
	{
		int yo = yy - (y + treeHeight);
		int offs = 2 - yo / 2;
		for (int xx = x - offs; xx <= x + offs; xx++)
		{
			for (int zz = z - offs; zz <= z + offs; zz++)
			{
				if (level->getTile(xx, yy, zz) == leafTile)
				{
					if (random->nextInt(4) == 0 && level->getTile(xx - 1, yy, zz) == 0)
					{
						addVine(level, xx - 1, yy, zz, VineTile::VINE_EAST);
					}
					if (random->nextInt(4) == 0 && level->getTile(xx + 1, yy, zz) == 0)
					{
						addVine(level, xx + 1, yy, zz, VineTile::VINE_WEST);
					}
					if (random->nextInt(4) == 0 && level->getTile(xx, yy, zz - 1) == 0)
					{
						addVine(level, xx, yy, zz - 1, VineTile::VINE_SOUTH);
					}
					if (random->nextInt(4) == 0 && level->getTile(xx, yy, zz + 1) == 0)
					{
						addVine(level, xx, yy, zz + 1, VineTile::VINE_NORTH);
					}
				}
			}
		}
	}
	return true;

}

void SwampTreeFeature::addVine(Level *level, int xx, int yy, int zz, int dir)
{
    placeBlock(level, xx, yy, zz, Tile::vine_Id, dir);
    int maxDir = 4;
    while (level->getTile(xx, --yy, zz) == 0 && maxDir > 0)
	{
        placeBlock(level, xx, yy, zz, Tile::vine_Id, dir);
        maxDir--;
    }
}