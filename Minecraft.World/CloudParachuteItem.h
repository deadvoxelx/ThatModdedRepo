#pragma once

#include "Item.h"

class Player;
class Entity;
class Level;

class CloudParachuteItem : public Item
{
public:
	CloudParachuteItem(int id);

	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player);
	virtual void releaseUsing(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player, int durationLeft);
	virtual int getUseDuration(shared_ptr<ItemInstance> itemInstance);
	virtual shared_ptr<ItemInstance> useTimeDepleted(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player);
	virtual UseAnim getUseAnimation(shared_ptr<ItemInstance> itemInstance);
};