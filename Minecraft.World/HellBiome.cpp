#include "stdafx.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.level.biome.h"
#include "HellBiome.h"

HellBiome::HellBiome(int id) : Biome(id)
{
	enemies.clear();
	friendlies.clear();
	friendlies_chicken.clear();	// 4J added
	friendlies_wolf.clear(); 	// 4J added
	waterFriendlies.clear();
	ambientFriendlies.clear();

	enemies.push_back(new MobSpawnerData(eTYPE_GHAST, 40, 1, 1));
	enemies.push_back(new MobSpawnerData(eTYPE_PIGZOMBIE, 100, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_LAVASLIME, 5, 1, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_CAVESPIDER, 3, 1, 2));
	enemies.push_back(new MobSpawnerData(eTYPE_SKELETON, 3, 1, 3));
	enemies.push_back(new MobSpawnerData(eTYPE_ENDERMAN, 2, 4, 4));
}
