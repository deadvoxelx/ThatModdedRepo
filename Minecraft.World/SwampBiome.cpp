#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.biome.h"
#include "SwampTreeFeature.h"
#include "SwampMushroomHugeFeature.h"

SwampBiome::SwampBiome(int id) : Biome(id)
{
	decorator->treeCount = 2;
	decorator->flowerCount = -999;
	decorator->deadBushCount = 1;
	decorator->mushroomCount = 8;
	decorator->reedsCount = 10;
	decorator->clayCount = 1;
	decorator->waterlilyCount = 4;

	decorator->hugeSwampMushrooms = 1;

	enemies.push_back(new MobSpawnerData(eTYPE_SLIME, 1, 1, 1));
}

Feature *SwampBiome::getTreeFeature(Random *random)
{
	return new SwampTreeFeature();
}