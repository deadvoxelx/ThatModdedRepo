#include "stdafx.h"
#include "EnchantedGravititeTile.h"

EnchantedGravititeTile::EnchantedGravititeTile(int id) : Tile(id, Material::metal)
{
}

int EnchantedGravititeTile::getColor() const
{
	return 0xe0a2e0;
}

int EnchantedGravititeTile::getColor(int auxData)
{
	return 0xe0a2e0;
}

int EnchantedGravititeTile::getColor(LevelSource *level, int x, int y, int z)
{
	return 0xe0a2e0;
}

int EnchantedGravititeTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	return 0xe0a2e0;
}