#include "stdafx.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.h"
#include "ForestBiome.h"

ForestBiome::ForestBiome(int id) : Biome(id)
{
	friendlies_wolf.push_back(new MobSpawnerData(eTYPE_WOLF, 5, 4, 4));
    friendlies.push_back(new MobSpawnerData(eTYPE_MOOBLOOM, 5, 1, 2));

	decorator->treeCount = 10;
	decorator->grassCount = 2;
}

Feature *ForestBiome::getTreeFeature(Random *random)
{
    if (random->nextInt(10) == 0)
	{
        return new BirchFeature(false);
    }
    if (random->nextInt(10) == 0)
	{
        return new BasicTree(false);
    }
    return new TreeFeature(false);
}
