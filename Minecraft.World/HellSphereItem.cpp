#include "stdafx.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.projectile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "HellSphereItem.h"
#include "SoundTypes.h"

HellSphereItem::HellSphereItem(int id) : Item(id)
{
	this->maxStackSize = 16;
}

shared_ptr<ItemInstance> HellSphereItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	if (!player->abilities.instabuild)
	{
		instance->count--;
	}
	level->playEntitySound((shared_ptr<Entity> ) player, eSoundType_RANDOM_BOW, 0.5f, 0.4f / (random->nextFloat() * 0.4f + 0.8f));
	player->swing();
	if (!level->isClientSide) level->addEntity( shared_ptr<HellSphere>( new HellSphere(level, player) ) );
	return instance;
}
