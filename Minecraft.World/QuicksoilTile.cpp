#include "stdafx.h"
#include "QuicksoilTile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "Mth.h"

QuicksoilTile::QuicksoilTile(int id) : Tile(id, Material::sand)
{
	friction = 0.989f;
}

void QuicksoilTile::stepOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
	entity->xd*=1.6;
    entity->zd*=1.6;
}

void QuicksoilTile::playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data)
{
	int d = data;
	if (d == TYPE_NATURAL)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Tile::quicksoil_Id, 2, 1)));
		}
	}
	else if (d == TYPE_UNNATURAL)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Tile::quicksoil_Id, 1, 1)));
		}
	}
}

shared_ptr<ItemInstance> QuicksoilTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::quicksoil, 1, 1));
}
