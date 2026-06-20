#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.effect.h"
#include "HealingStoneItem.h"

HealingStoneItem::HealingStoneItem(int id) : Item( id )
{
	maxStackSize = 64;
}

bool HealingStoneItem::isFoil(shared_ptr<ItemInstance> itemInstance)
{
	return true;
}

shared_ptr<ItemInstance> HealingStoneItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	if (!player->abilities.instabuild)
	{
		instance->count--;
	}
	if (!level->isClientSide) player->addEffect(new MobEffectInstance(MobEffect::regeneration->id, 600, 0));

	return instance;
}