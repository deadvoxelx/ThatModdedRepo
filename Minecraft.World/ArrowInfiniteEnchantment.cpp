#include "stdafx.h"
#include "net.minecraft.world.item.h"
#include "DartShooterGoldItem.h"
#include "DartShooterPoisonItem.h"
#include "DartShooterEnchantedItem.h"
#include "DartShooterNethaniumItem.h"
#include "ArrowInfiniteEnchantment.h"

ArrowInfiniteEnchantment::ArrowInfiniteEnchantment(int id, int frequency) : Enchantment(id, frequency, EnchantmentCategory::bow)
{
	setDescriptionId(IDS_ENCHANTMENT_ARROW_INFINITE);
}

int ArrowInfiniteEnchantment::getMinCost(int level)
{
	return 5;
}

int ArrowInfiniteEnchantment::getMaxCost(int level)
{
	return 15;
}

int ArrowInfiniteEnchantment::getMaxLevel()
{
	return 1;
}

bool ArrowInfiniteEnchantment::canEnchant(shared_ptr<ItemInstance> item)
{
	if (dynamic_cast<DartShooterGoldItem *>(item->getItem()) != nullptr) return true;
	if (dynamic_cast<DartShooterEnchantedItem *>(item->getItem()) != nullptr) return true;
	if (dynamic_cast<DartShooterNethaniumItem *>(item->getItem()) != nullptr) return true;
	return Enchantment::canEnchant(item);
}
