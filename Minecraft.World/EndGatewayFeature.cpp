#include "EndGatewayFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"

EndGatewayFeature::EndGatewayFeature(int blockId)
    : Feature(blockId)
{
}

bool EndGatewayFeature::place(Level *level, Random *random, int x, int y, int z)
{
    // Clear space
    for (int dx = -2; dx <= 2; dx++)
    {
        for (int dy = -2; dy <= 3; dy++)
        {
            for (int dz = -2; dz <= 2; dz++)
            {
                placeBlock(level, x + dx, y + dy, z + dz, 0, 0);
            }
        }
    }

    // Center column
    placeBlock(level, x, y - 2, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y - 1, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y, z, Tile::endGateway_Id, 0);
    //placeBlock(level, x, y + 1, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y + 2, z, Tile::unbreakable_Id, 0);

    // Upper cross
    placeBlock(level, x + 1, y + 1, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x - 1, y + 1, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y + 1, z + 1, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y + 1, z - 1, Tile::unbreakable_Id, 0);

    // Lower cross
    placeBlock(level, x + 1, y - 1, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x - 1, y - 1, z, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y - 1, z + 1, Tile::unbreakable_Id, 0);
    placeBlock(level, x, y - 1, z - 1, Tile::unbreakable_Id, 0);

    return true;
}
