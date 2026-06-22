#pragma once

#include "Item.h"

class Player;
class Level;

class DartShooterEnchantedItem : public Item
{
public:
	static const int MAX_DRAW_DURATION = 0;

public:
	DartShooterEnchantedItem(int id);

	virtual void releaseUsing(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player, int durationLeft);
	virtual shared_ptr<ItemInstance> useTimeDepleted(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player);
	virtual int getUseDuration(shared_ptr<ItemInstance> itemInstance);
	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player);
	virtual int getEnchantmentValue();
};