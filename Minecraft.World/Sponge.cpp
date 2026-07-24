#include "stdafx.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "Sponge.h"

const int RANGE = 2; // Voxel - this isnt used anymore, but leaving it anyway

Sponge::Sponge(int id) : Tile(id, Material::sponge)
{
	setTicking(true);
}

void Sponge::onPlace(Level* level, int x, int y, int z)
{
	Tile::onPlace(level, x, y, z);

	for (int waterx = -2; waterx <= 2; waterx++)
	{
		for (int waterz = -2; waterz <= 2; waterz++)
		{
			for (int watery = -2; watery <= 2; watery++)
			{
				int wt = level->getTile(x + waterx, y + watery, z + waterz);

				if (wt == Tile::calmWater_Id || wt == Tile::water_Id)
				{
					if (!level->isClientSide) level->setTileAndData(x + waterx, y + watery, z + waterz, 0, 0, Tile::UPDATE_CLIENTS);
				}
			}
		}
	}
}

void Sponge::tick(Level* level, int x, int y, int z, Random* random)
{
	for (int waterx = -2; waterx <= 2; waterx++)
	{
		for (int waterz = -2; waterz <= 2; waterz++)
		{
			for (int watery = -2; watery <= 2; watery++)
			{
				int wt = level->getTile(x + waterx, y + watery, z + waterz);

				if (wt == Tile::calmWater_Id || wt == Tile::water_Id)
				{
					if (!level->isClientSide) level->setTileAndData(x + waterx, y + watery, z + waterz, 0, 0, Tile::UPDATE_CLIENTS);
				}
			}
		}
	}
}
