#include "stdafx.h"
#include "LavenderForestBiome.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.tile.h"

LavenderBiome::LavenderBiome(int id) : Biome(id)
{
	friendlies_wolf.push_back(new MobSpawnerData(eTYPE_WOLF, 8, 4, 4));

    decorator->treeCount = 10;
    decorator->grassCount = 7;
	decorator->flowerCount = 2;
}

Feature *LavenderBiome::getTreeFeature(Random *random)
{
	return new LavenderTreeFeature(false);
}

Feature *LavenderBiome::getGrassFeature(Random *random)
{
	if (random->nextInt(4) == 0)
	{
		return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::FERN);
	}
	return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::TALL_GRASS);
}