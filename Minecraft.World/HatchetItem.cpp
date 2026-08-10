#include "stdafx.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.level.tile.h"
#include "HatchetItem.h"

TileArray *HatchetItem::diggables = NULL;

void HatchetItem::staticCtor()
{
	HatchetItem::diggables = new TileArray( HATCHET_DIGGABLES);
	diggables->data[0] = Tile::wood;
	diggables->data[1] = Tile::bookshelf;
	diggables->data[2] = Tile::treeTrunk;
	diggables->data[3] = Tile::chest;
	diggables->data[4] = Tile::stoneSlab;
	diggables->data[5] = Tile::stoneSlabHalf;
	diggables->data[6] = Tile::pumpkin;
	diggables->data[7] = Tile::litPumpkin;
}

HatchetItem::HatchetItem(int id, const Tier *tier) : DiggerItem (id, 6, tier, diggables)
{
}

float HatchetItem::getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile)
{
    if (tile != NULL && (tile->material == Material::wood  || tile->material == Material::plant || tile->material == Material::replaceable_plant || tile->material == Material::vegetable))
	{
        return speed;
    }
    return DiggerItem::getDestroySpeed(itemInstance, tile);
}

bool HatchetItem::hurtEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> attacker) 
{
	itemInstance->hurtAndBreak(2, attacker);
	if (id == Item::nethaniumAxe_Id)
	{
		mob->setOnFire(3);
	}
	if (id == Item::gravititeAxe_Id)
	{
		mob->yd = 1.0f;
	}
	return true;
}
