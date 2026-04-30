#include "stdafx.h"
#include "TheOuterEndBiome.h"
#include "TheOuterEndBiomeDecorator.h"
#include "net.minecraft.world.entity.monster.h"

TheOuterEndBiome::TheOuterEndBiome(int id) : Biome(id)
{
	enemies.clear();
	friendlies.clear();
	friendlies_chicken.clear();
	friendlies_wolf.clear();
	waterFriendlies.clear();
	ambientFriendlies.clear();

	enemies.push_back(new MobSpawnerData(eTYPE_WATCHER, 7, 1, 1));
	enemies.push_back(new MobSpawnerData(eTYPE_ENDERMAN, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_WHITE_ENDERMAN, 4, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_EVUPUL, 7, 1, 3));
	enemies.push_back(new MobSpawnerData(eTYPE_EVUPULDARK, 10, 2, 4));

	decorator = new TheOuterEndBiomeDecorator(this);
}
