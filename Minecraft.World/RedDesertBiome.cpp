#include "stdafx.h"
#include "RedDesertBiome.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.entity.monster.h"

RedDesertBiome::RedDesertBiome(int id) : Biome(id)
{
	friendlies.clear();
	friendlies_chicken.clear();
	friendlies_wolf.clear();
	topMaterial = static_cast<BYTE>(Tile::stainedSand_Id);
	material = static_cast<BYTE>(Tile::stainedSand_Id);

	decorator->treeCount = -999;
	decorator->deadBushCount = 15;
	decorator->reedsCount = 50;
	decorator->cactusCount = 13;

	enemies.push_back(new MobSpawnerData(eTYPE_HUSK, 10, 4, 4));
}

void RedDesertBiome::decorate(Level *level, Random *random, int xo, int zo)
{
	Biome::decorate(level, random, xo, zo);

	//if (random->nextInt(250) == 0)
	//{
	//	int x = xo + random->nextInt(16) + 8;
	//	int z = zo + random->nextInt(16) + 8;
	//	Feature *well = new DesertWellFeature();
	//	well->place(level, random, x, level->getHeightmap(x, z) + 1, z);
	//}

	OreFeature redDesertGoldFeature(Tile::goldOre_Id, 0, 5, Tile::stone_Id);
	for (int i = 0; i < 12; i++)
	{
		int x = xo + random->nextInt(16);
		int y = 32 + random->nextInt(40);
		int z = zo + random->nextInt(16);
		redDesertGoldFeature.place(level, random, x, y, z);
	}
}
