#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.projectile.h"
#include "DartShooterGoldItem.h"
#include "SoundTypes.h"

DartShooterGoldItem::DartShooterGoldItem(int id) : Item( id )
{
	maxStackSize = 1;
}

void DartShooterGoldItem::releaseUsing(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player, int durationLeft)
{
	bool infiniteArrows = player->abilities.instabuild || EnchantmentHelper::getEnchantmentLevel(Enchantment::arrowInfinite->id, itemInstance) > 0;

	if (infiniteArrows || player->inventory->hasResource(Item::dartGold_Id))
	{
		int timeHeld = getUseDuration(itemInstance) - durationLeft;
		float pow = timeHeld / static_cast<float>(MAX_DRAW_DURATION);
		pow = ((pow * pow) + pow * 2) / 3;
		if (pow < 0.1) return;
		if (pow > 1) pow = 1;

		shared_ptr<Arrow> arrow = std::make_shared<Arrow>(level, player, pow * 2.0f);
		if (pow == 1) arrow->setCritArrow(true);

		level->playEntitySound(player, eSoundType_RANDOM_BOW, 1.0f, 1 / (random->nextFloat() * 0.4f + 1.2f) + pow * 0.5f);

		if (infiniteArrows)
		{
			arrow->pickup = Arrow::PICKUP_CREATIVE_ONLY;
		}
		else
		{
			player->inventory->removeResource(Item::dartGold_Id);
		}
		if (!level->isClientSide) level->addEntity(arrow);
	}
}

shared_ptr<ItemInstance> DartShooterGoldItem::useTimeDepleted(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	return instance;
}

int DartShooterGoldItem::getUseDuration(shared_ptr<ItemInstance> itemInstance)
{
	return 20 * 60 * 60;
}

shared_ptr<ItemInstance> DartShooterGoldItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	if (player->abilities.instabuild || player->inventory->hasResource(Item::dartGold_Id))
	{
		player->startUsingItem(instance, getUseDuration(instance));
	}
	return instance;
}

int DartShooterGoldItem::getEnchantmentValue()
{
	return 1;
}