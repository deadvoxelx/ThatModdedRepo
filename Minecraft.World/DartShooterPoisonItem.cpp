#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.projectile.h"
#include "DartShooterPoisonItem.h"
#include "SoundTypes.h"

DartShooterPoisonItem::DartShooterPoisonItem(int id) : Item( id )
{
	maxStackSize = 1;
}

void DartShooterPoisonItem::releaseUsing(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player, int durationLeft)
{
	bool infiniteDarts = player->abilities.instabuild || EnchantmentHelper::getEnchantmentLevel(Enchantment::arrowInfinite->id, itemInstance) > 0;

	if (infiniteDarts || player->inventory->hasResource(Item::dartPoison_Id))
	{
		int timeHeld = getUseDuration(itemInstance) - durationLeft;
		float pow = timeHeld / static_cast<float>(MAX_DRAW_DURATION);
		pow = ((pow * pow) + pow * 2) / 3;
		if (pow < 0.1) return;
		if (pow > 1) pow = 1;

		shared_ptr<DartPoison> dart = std::make_shared<DartPoison>(level, player, pow * 2.0f);
		if (pow == 1) dart->setCritDartPoison(true);

		level->playEntitySound(player, eSoundType_RANDOM_BOW, 1.0f, 1 / (random->nextFloat() * 0.4f + 1.2f) + pow * 0.5f);

		if (infiniteDarts)
		{
			dart->pickup = Dart::PICKUP_CREATIVE_ONLY;
		}
		else
		{
			player->inventory->removeResource(Item::dartPoison_Id);
		}
		if (!level->isClientSide) level->addEntity(dart);
	}
}

shared_ptr<ItemInstance> DartShooterPoisonItem::useTimeDepleted(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	return instance;
}

int DartShooterPoisonItem::getUseDuration(shared_ptr<ItemInstance> itemInstance)
{
	return 20 * 60 * 60;
}

shared_ptr<ItemInstance> DartShooterPoisonItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	if (player->abilities.instabuild || player->inventory->hasResource(Item::dartPoison_Id))
	{
		player->startUsingItem(instance, getUseDuration(instance));
	}
	return instance;
}

int DartShooterPoisonItem::getEnchantmentValue()
{
	return 1;
}

bool DartShooterPoisonItem::isEnchantable(shared_ptr<ItemInstance> itemInstance)
{
	return getMaxStackSize() == 1;
}