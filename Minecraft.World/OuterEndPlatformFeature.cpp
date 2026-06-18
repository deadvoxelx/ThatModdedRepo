#include "OuterEndPlatformFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"

OuterEndPlatformFeature::OuterEndPlatformFeature(int blockId)
    : Feature(blockId)
{
}

bool OuterEndPlatformFeature::place(Level *level, Random *random, int x, int y, int z)
{
    for (int dx = -2; dx <= 2; dx++)
    {
        for (int dz = -2; dz <= 2; dz++)
        {
            placeBlock(level, x + dx, y, z + dz, Tile::obsidian_Id, 0);
        }
    }
    return true;
}