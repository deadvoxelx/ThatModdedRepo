#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "DungeonStoneFeature.h"

DungeonStoneFeature::DungeonStoneFeature(int tile)
{
	this->tile = tile;
}

bool DungeonStoneFeature::place(Level *level, Random *random, int x, int y, int z)
{
	if (level->getTile(x, y, z) != Tile::dungeonStone_Id) return false;

	level->setTileAndData(x, y, z, tile, 2, Tile::UPDATE_CLIENTS);
	level->setInstaTick(true);
	Tile::tiles[tile]->tick(level, x, y, z, random);
	level->setInstaTick(false);

	return true;
}