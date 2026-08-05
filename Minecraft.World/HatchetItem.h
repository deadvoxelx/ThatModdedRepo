#pragma once

#include "DiggerItem.h"

#define HATCHET_DIGGABLES 8
class HatchetItem : public DiggerItem
{
private:
	static TileArray *diggables;

public:
	static void staticCtor();
	HatchetItem(int id, const Tier *tier);
	virtual float getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile);
	virtual bool hurtEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> attacker);
};
