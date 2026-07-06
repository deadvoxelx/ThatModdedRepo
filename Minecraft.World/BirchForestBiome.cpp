#include "stdafx.h"
#include "BirchForestBiome.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.tile.h"

BirchForestBiome::BirchForestBiome(int id) : Biome(id)
{
	friendlies_wolf.push_back(new MobSpawnerData(eTYPE_WOLF, 5, 4, 4));

	decorator->treeCount = 10;
	decorator->grassCount = 8;
	decorator->flowerCount = 4;
}

Feature *BirchForestBiome::getTreeFeature(Random *random)
{
    return new BirchFeature(false);
}

Feature *BirchForestBiome::getGrassFeature(Random *random)
{
	if (random->nextInt(4) == 0)
	{
		return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::FERN);
	}
	return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::TALL_GRASS);
}