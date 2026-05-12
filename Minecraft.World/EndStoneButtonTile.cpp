#include "stdafx.h"
#include "net.minecraft.h"
#include "EndStoneButtonTile.h"

EndStoneButtonTile::EndStoneButtonTile(int id) : ButtonTile(id, false)
{
}

Icon *EndStoneButtonTile::getTexture(int face, int data)
{
	return Tile::endStone->getTexture(Facing::UP);
}