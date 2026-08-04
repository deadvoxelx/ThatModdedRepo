#include "stdafx.h"
#include "SandTile.h"
#include "net.minecraft.world.item.h"

SandTile::SandTile(int type) : HeavyTile(type)
{
}

int SandTile::getColor() const
{
	return 0xcf7f4c;
}

int SandTile::getColor(int auxData)
{
	return 0xcf7f4c;
}

int SandTile::getColor(LevelSource *level, int x, int y, int z)
{
	return 0xcf7f4c;
}

int SandTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	return 0xcf7f4c;
}