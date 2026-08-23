#include "HerobrineFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"

HerobrineFeature::HerobrineFeature(int blockId) : Feature(blockId)
{
}

bool HerobrineFeature::place(Level *level, Random *random, int x, int y, int z)
{
    int belowTile = level->getTile(x, y - 1, z);
    int thisTile = level->getTile(x, y, z);
    if ((belowTile == Tile::grass_Id || belowTile == Tile::sand_Id) && (thisTile != Tile::grass_Id && thisTile != Tile::sand_Id && thisTile != Tile::ice_Id && thisTile != Tile::water_Id && thisTile != Tile::calmWater_Id && thisTile != Tile::leaves_Id))
    {
	    placeBlock(level, x, y, z, Tile::mossyCobblestone_Id, 0);
        placeBlock(level, x + 1, y, z, Tile::goldBlock_Id, 0);
        placeBlock(level, x - 1, y, z, Tile::goldBlock_Id, 0);
        placeBlock(level, x, y, z + 1, Tile::goldBlock_Id, 0);
        placeBlock(level, x, y, z - 1, Tile::goldBlock_Id, 0);
	
	    placeBlock(level, x + 1, y, z + 1, Tile::goldBlock_Id, 0);
	    placeBlock(level, x - 1, y, z - 1, Tile::goldBlock_Id, 0);
	    placeBlock(level, x + 1, y, z - 1, Tile::goldBlock_Id, 0);
	    placeBlock(level, x - 1, y, z + 1, Tile::goldBlock_Id, 0);
	
        placeBlock(level, x + 1, y + 1, z, Tile::redstoneTorch_on_Id, 0);
        placeBlock(level, x - 1, y + 1, z, Tile::redstoneTorch_on_Id, 0);
        placeBlock(level, x, y + 1, z + 1, Tile::redstoneTorch_on_Id, 0);
        placeBlock(level, x, y + 1, z - 1, Tile::redstoneTorch_on_Id, 0);
        placeBlock(level, x, y + 1, z, Tile::netherRack_Id, 0);
	    placeBlock(level, x, y + 2, z, Tile::fire_Id, 0);
    }
    return true;
}
