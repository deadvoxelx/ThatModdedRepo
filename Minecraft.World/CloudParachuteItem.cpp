#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.effect.h"
#include "CloudParachuteItem.h"
#include "Entity.h"

CloudParachuteItem::CloudParachuteItem(int id) : Item( id )
{
	maxStackSize = 1;
}

shared_ptr<ItemInstance> CloudParachuteItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	// the main function is handled in Player.cpp under "void Player::updateFrameTick()"
	player->startUsingItem(instance, getUseDuration(instance));

	return instance;
}

void CloudParachuteItem::releaseUsing(shared_ptr<ItemInstance> itemInstance, Level* level, shared_ptr<Player> player, int durationLeft)
{
	itemInstance->count--;
}

int CloudParachuteItem::getUseDuration(shared_ptr<ItemInstance> itemInstance)
{
	return 20 * 60 * 60;
}

shared_ptr<ItemInstance> CloudParachuteItem::useTimeDepleted(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	return instance;
}

UseAnim CloudParachuteItem::getUseAnimation(shared_ptr<ItemInstance> itemInstance)
{
	return UseAnim_block;
} 