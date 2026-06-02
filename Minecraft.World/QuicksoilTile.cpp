#include "stdafx.h"
#include "QuicksoilTile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.level.h"
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