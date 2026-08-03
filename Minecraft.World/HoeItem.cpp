#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.entity.player.h"
#include "ItemInstance.h"
#include "HoeItem.h"

HoeItem::HoeItem(int id, const Tier *tier) : Item(id)
{
	this->tier = tier;
	maxStackSize = 1;
	setMaxDamage(tier->getUses());
}

float HoeItem::getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile)
{
	if (tile->id == Tile::leaves_Id)
	{
		return 8;
	}

	if (tile->id == Tile::netherLeaves_Id)
	{
		return 8;
	}

	if (tile->id == Tile::hayBlock_Id)
	{
		return 5;
	}
	
	if (tile->id == Tile::sponge_Id)
	{
		return 5;
	}

	if (tile->id == Tile::vine_Id)
	{
		return 10;
	}

	if (tile->id == Tile::netherVine_Id)
	{
		return 10;
	}

	return 1.0f;
}

bool HoeItem::mineBlock(shared_ptr<ItemInstance> itemInstance, Level *level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner)
{
	if (Tile::tiles[tile] != nullptr && Tile::tiles[tile]->getDestroySpeed(level, x, y, z) != 0.0) itemInstance->hurtAndBreak(1, owner);
	if (id == Item::holystoneHoe_Id)
	{
		if (!level->isClientSide)
		{
			if (random->nextInt(10) == 0)
			{
				owner->spawnAtLocation(Item::ambrosiumShard->id, 1);
			}
		}
	}
	return true;
}

bool HoeItem::useOn(shared_ptr<ItemInstance> instance, shared_ptr<Player> player, Level *level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, bool bTestUseOnOnly)
{
	if (!player->mayUseItemAt(x, y, z, face, instance)) return false;

	int targetType = level->getTile(x, y, z);
	int above = level->getTile(x, y + 1, z);

	if (face != 0 && above == 0 && (targetType == Tile::grass_Id || targetType == Tile::dirt_Id)) 
	{
		if(!bTestUseOnOnly)
		{
			Tile *tile = Tile::farmland;
			level->playSound(x + 0.5f, y + 0.5f, z + 0.5f, tile->soundType->getStepSound(), (tile->soundType->getVolume() + 1) / 2, tile->soundType->getPitch() * 0.8f);

			if (level->isClientSide) return true;
			level->setTileAndUpdate(x, y, z, tile->id);
			instance->hurtAndBreak(1, player);
		}
		return true;
	}
	return false;
}

bool HoeItem::isHandEquipped() 
{
	return true;
}

const Item::Tier *HoeItem::getTier()
{
	return tier;
}
