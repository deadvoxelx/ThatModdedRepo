#include "stdafx.h"
#include "NurealmBiome.h"
#include "NurealmBiomeDecorator.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"

NurealmBiome::NurealmBiome(int id) : Biome(id)
{
	enemies.clear();
	friendlies.clear();
	friendlies_chicken.clear();
	friendlies_wolf.clear();
	waterFriendlies.clear();
	ambientFriendlies.clear();

	enemies.push_back(new MobSpawnerData(eTYPE_NUSKULL, 25, 1, 3));
	enemies.push_back(new MobSpawnerData(eTYPE_NUCLEAR_NUSKULL, 1, 1, 2));
	enemies.push_back(new MobSpawnerData(eTYPE_NUSA_DEMON, 6, 1, 1));

	decorator = new NurealmBiomeDecorator(this);
}

Feature *NurealmBiome::getGrassFeature(Random *random)
{
	return new TallGrassFeature(Tile::nusaShrub_Id, 0);
}
