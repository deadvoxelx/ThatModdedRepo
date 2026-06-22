#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.effect.h"
#include "MilkBucketItem.h"

MilkBucketItem::MilkBucketItem(int id) : Item( id )
{
	setMaxStackSize(1);
}

shared_ptr<ItemInstance> MilkBucketItem::useTimeDepleted(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	if (!player->abilities.instabuild) instance->count--;

	if (!level->isClientSide)
	{
		if (id == Item::skyrootBucket_poison_Id) { player->addEffect(new MobEffectInstance(MobEffect::poison->id, 300, 0)); }
		else { player->removeAllEffects(); }
	}

	if (instance->count <= 0)
	{
		if (id == Item::skyrootBucket_milk_Id) return std::make_shared<ItemInstance>(Item::skyrootBucket_empty);
		if (id == Item::skyrootBucket_poison_Id) return std::make_shared<ItemInstance>(Item::skyrootBucket_empty);
		return std::make_shared<ItemInstance>(Item::bucket_empty);
	}
	return instance;
}

int MilkBucketItem::getUseDuration(shared_ptr<ItemInstance> itemInstance)
{
	return DRINK_DURATION;
}

UseAnim MilkBucketItem::getUseAnimation(shared_ptr<ItemInstance> itemInstance)
{
	return UseAnim_drink;
}

shared_ptr<ItemInstance> MilkBucketItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	player->startUsingItem(instance, getUseDuration(instance));
	return instance;
}
