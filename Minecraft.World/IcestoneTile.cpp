#include "stdafx.h"
#include "IcestoneTile.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.entity.h"

IcestoneTile::IcestoneTile(int id) : Tile(id, Material::stone)
{
}

void IcestoneTile::onPlace(Level* level, int x, int y, int z)
{
	Tile::onPlace(level, x, y, z);

	if (level->getTile(x, y + 1, z) == Tile::calmWater_Id)
	{
		if (!level->isClientSide)
		{
			level->setTileAndData(x, y + 1, z, Tile::ice_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	if (level->getTile(x, y - 1, z) == Tile::calmWater_Id)
	{
		if (!level->isClientSide)
		{
			level->setTileAndData(x, y - 1, z, Tile::ice_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	if (level->getTile(x + 1, y, z) == Tile::calmWater_Id)
	{
		if (!level->isClientSide)
		{
			level->setTileAndData(x + 1, y, z, Tile::ice_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	if (level->getTile(x - 1, y, z) == Tile::calmWater_Id)
	{
		if (!level->isClientSide)
		{
			level->setTileAndData(x - 1, y, z, Tile::ice_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	if (level->getTile(x, y, z + 1) == Tile::calmWater_Id)
	{
		if (!level->isClientSide)
		{
			level->setTileAndData(x, y, z + 1, Tile::ice_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	if (level->getTile(x, y, z - 1) == Tile::calmWater_Id)
	{
		if (!level->isClientSide)
		{
			level->setTileAndData(x, y, z - 1, Tile::ice_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
}