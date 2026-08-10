#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "NetherrackSpikeFeature.h"

NetherrackSpikeFeature::NetherrackSpikeFeature(int blockId) : Feature(blockId)
{
}

bool NetherrackSpikeFeature::place(Level *level, Random *random, int x, int y, int z)
{
	int width = 2 + random->nextInt(5);
	int height = 24 + random->nextInt(32);
	int variation = 2 + random->nextInt(7);

	while (y > 0 && !level->getMaterial(x, y - 1, z)->blocksMotion()) y--;

	for (int groundx = -3; groundx <= 3; groundx++)
	{
		for (int groundz = -3; groundz <= 3; groundz++)
		{
			int belowTile = level->getTile(x + groundx, y - 1, z + groundz);

			if (belowTile == Tile::netherRack_Id || belowTile == Tile::netherSoil_Id || belowTile == Tile::soulsand_Id || belowTile == Tile::lavaRock_Id)
			{
				for (int dx = -width; dx <= width; dx++)
				{
					for (int dz = -width; dz <= width; dz++)
					{
						if (dx * dx + dz * dz > width * width) continue;

						double dist = Mth::sqrt((double)(dx * dx + dz * dz));
						double spike = height * (1.0 - dist / (double)width);
						if (spike < 0) spike = 0;

						int column = Mth::floor(spike) + random->nextInt(variation * 2 + 1) - variation;
						if (column < 0) column = 0;

						for (int dy = 0; dy <= column; dy++)
						{
							int bx = x + dx;
							int by = y + dy;
							int bz = z + dz;
							if (level->getTile(bx, by, bz) == 0 || level->getTile(bx, by, bz) == Tile::netherLeaves_Id || level->getTile(bx, by, bz) == Tile::lava_Id || level->getTile(bx, by, bz) == Tile::calmLava_Id || level->getTile(bx, by, bz) == Tile::netherBrick_Id)
							{
								placeBlock(level, bx, by, bz, Tile::netherRack_Id, 0);
							}
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}