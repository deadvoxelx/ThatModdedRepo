#pragma once

#include "Item.h"

class Player;
class Level;

class HealingStoneItem : public Item
{
public:
	HealingStoneItem(int id);

	bool isFoil(shared_ptr<ItemInstance> itemInstance);
	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player);
};