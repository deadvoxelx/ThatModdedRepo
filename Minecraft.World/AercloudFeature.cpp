#include "stdafx.h"
#include "AerCloudFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"

AerCloudFeature::AerCloudFeature(int tile, int data, int cloudAmount) : Feature(false)
{
	this->tile = tile;
	this->data = data;
	this->cloudAmount = cloudAmount;
}
  
bool AerCloudFeature::place(Level *level, Random *random, int xIn, int yIn, int zIn)
{
    yIn += random->nextInt(64);
    for (int amount = 0; amount < cloudAmount; amount++)
    {
        boolean offsetY = random->nextBoolean();
        int xOffset = random->nextInt(2);
        int yOffset = offsetY ? (random->nextInt(3) - 1) : 0;
        int zOffset = random->nextInt(2);
        xIn += xOffset;
        yIn += yOffset;
        zIn += zOffset;
        for (int x = xIn; x < xIn + random->nextInt(2) + 3; x++)
        {
            for (int y = yIn; y < yIn + random->nextInt(1) + 2; y++)
            {
                for (int z = zIn; z < zIn + random->nextInt(2) + 3; z++)
                {
                    if (level->isEmptyTile(x, y, z) && ( (Mth::abs(x - xIn)) + (Mth::abs(y - yIn)) + (Mth::abs(z - zIn)) < 4 + random->nextInt(2) )) level->setTileAndData(x, y, z, tile, data, Tile::UPDATE_NONE);
                } 
            } 
        } 
    } 
    return true;
}
