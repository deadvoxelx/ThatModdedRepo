#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.item.h"
#include "..\Minecraft.Client\Textures.h"
#include "Moobloom.h"
#include "MobCategory.h"
#include "AABB.h"

Moobloom::Moobloom(Level *level) : Cow(level)
{
	this->defineSynchedData();
	setHealth(getMaxHealth());
	this->setSize(0.9f, 1.3f);
}

bool Moobloom::mobInteract(shared_ptr<Player> player)
{
	shared_ptr<ItemInstance> item = player->inventory->getSelected();
	/*if (item != nullptr && item->id == Item::bowl_Id && getAge() >= 0)
	{
		if (item->count == 1) 
		{
			player->inventory->setItem(player->inventory->selected, std::make_shared<ItemInstance>(Item::mushroomStew));
			return true;
		}

		if (player->inventory->add(std::make_shared<ItemInstance>(Item::mushroomStew)) && !player->abilities.instabuild) 
		{
			player->inventory->removeItem(player->inventory->selected, 1);
			return true;
		}
	}*/
		
	if (item != nullptr && item->id == Item::shears_Id && getAge() >= 0 && level->canCreateMore(eTYPE_COW, Level::eSpawnType_Breed))
	{
		remove();
		level->addParticle(eParticleType_largeexplode, x, y + bbHeight / 2, z, 0, 0, 0);
		if(!level->isClientSide)
		{
			remove();
			shared_ptr<Cow> cow = std::make_shared<Cow>(level);
			cow->moveTo(x, y, z, yRot, xRot);
			cow->setHealth(getHealth());
			cow->yBodyRot = yBodyRot;
			level->addEntity(cow);
			for (int i = 0; i < 5; i++)
			{
				level->addEntity(std::make_shared<ItemEntity>(level, x, y + bbHeight, z, shared_ptr<ItemInstance>(new ItemInstance(Tile::flower))));
			}
			return true;
		}
		return true;
	}
	return Cow::mobInteract(player);
}

bool Moobloom::canSpawn()
{
	int xt = Mth::floor(x);
	int yt = Mth::floor(bb->y0);
	int zt = Mth::floor(z);
	return ( level->getTile(xt, yt - 1, zt) == Tile::grass_Id ) && level->getDaytimeRawBrightness(xt, yt, zt) > 8 && PathfinderMob::canSpawn();
}

shared_ptr<AgableMob> Moobloom::getBreedOffspring(shared_ptr<AgableMob> target)
{
	if( level->canCreateMore( GetType(), Level::eSpawnType_Breed) )
	{
		return std::make_shared<Moobloom>(level);
	}
	else
	{
		return nullptr;
	}
}