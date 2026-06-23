#pragma once

#include "DiggerItem.h"

class Player;
class Level;

#define PICKAXE_DIGGABLES 35

class RelicMalletItem : public DiggerItem
{
private:
	static TileArray diggables;

public: // 
	static void staticCtor();

	RelicMalletItem(int id, const Tier *tier);

public:
	virtual bool canDestroySpecial(Tile *tile);
	virtual float getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile);	// 4J - brought forward from 1.2.3
	virtual bool mineBlock(shared_ptr<ItemInstance> itemInstance, Level *level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner);

	virtual UseAnim getUseAnimation(shared_ptr<ItemInstance> itemInstance);
	virtual int getUseDuration(shared_ptr<ItemInstance> itemInstance);
	virtual shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player);
};
