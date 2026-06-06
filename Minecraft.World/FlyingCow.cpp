#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.item.h"
#include "..\Minecraft.Client\Textures.h"
#include "FlyingCow.h"
#include "MobCategory.h"
#include "AABB.h"

FlyingCow::FlyingCow(Level *level) : Cow(level)
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(0.9f, 1.3f);
}

int FlyingCow::getDeathLoot() 
{
	return Item::leather->id;
}

void FlyingCow::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = random->nextInt(3) + random->nextInt(1 + playerBonusLevel);
	for (int i = 0; i < count; i++)
	{
		spawnAtLocation(Item::leather_Id, 1);
	}
	count = random->nextInt(3) + 1 + random->nextInt(1 + playerBonusLevel);
	for (int i = 0; i < count; i++)
	{
		if (isOnFire())
		{
			spawnAtLocation(Item::beef_cooked_Id, 1);
		}
		else
		{
			spawnAtLocation(Item::beef_raw_Id, 1);
		}
	}
}

bool FlyingCow::mobInteract(shared_ptr<Player> player)
{
	return Cow::mobInteract(player);
}

bool FlyingCow::canSpawn()
{
	int xt = Mth::floor(x);
	int yt = Mth::floor(bb->y0);
	int zt = Mth::floor(z);
	return ( level->getTile(xt, yt - 1, zt) == Tile::grass_Id || level->getTile(xt, yt - 1, zt) == Tile::aetherGrass_Id ) && level->getDaytimeRawBrightness(xt, yt, zt) > 8 && PathfinderMob::canSpawn();
}

shared_ptr<AgableMob> FlyingCow::getBreedOffspring(shared_ptr<AgableMob> target)
{
	if( level->canCreateMore( GetType(), Level::eSpawnType_Breed) )
	{
		return std::make_shared<FlyingCow>(level);
	}
	else
	{
		return nullptr;
	}
}
