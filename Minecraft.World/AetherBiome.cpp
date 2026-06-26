#include "stdafx.h"
#include "AetherBiome.h"
#include "AetherBiomeDecorator.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"

AetherBiome::AetherBiome(int id) : Biome(id)
{
	enemies.clear();
	friendlies.clear();
	friendlies_chicken.clear();
	friendlies_wolf.clear();
	waterFriendlies.clear();
	ambientFriendlies.clear();

	friendlies.push_back(new MobSpawnerData(eTYPE_SHEEPUFF, 10, 4, 4));
	friendlies.push_back(new MobSpawnerData(eTYPE_FLYING_COW, 8, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_ZEPHYR, 5, 1, 2));
	enemies.push_back(new MobSpawnerData(eTYPE_AERWHALE, 2, 1, 1));
	enemies.push_back(new MobSpawnerData(eTYPE_COCKATRICE, 10, 4, 4));
	enemies.push_back(new MobSpawnerData(eTYPE_SWET, 1, 2, 3));
	//ambientFriendlies.push_back(new MobSpawnerData(eTYPE_COCKATRICESPAWNER, 8, 1, 2));

	delete decorator;
	decorator = new AetherBiomeDecorator(this);
}

Feature *AetherBiome::getTreeFeature(Random *random)
{
	if (random->nextInt(10) == 0)
	{
		if (random->nextInt(3) == 0)
		{
			return new GoldenOakTreeFeature(false);
		}
		return new GoldenOakLargeFeature(false);
	}
	return new SkyrootTreeFeature(false);
}

Feature *AetherBiome::getGrassFeature(Random *random)
{
	return new TallGrassFeature(Tile::tallgrass_Id, 1);
}

int AetherBiome::getGrassColor()
{
	return 0x8ab69a;
}

int AetherBiome::getFolageColor()
{
	return 0x8ab69a;
}

int AetherBiome::getSkyColor(float temp)
{
	return 0x9ecbff;
}
