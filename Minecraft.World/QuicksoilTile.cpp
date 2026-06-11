#include "stdafx.h"
#include "QuicksoilTile.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "Mth.h"

const unsigned int QuicksoilTile::QUICKSOIL_NAMES[QUICKSOIL_NAMES_LENGTH] = {
	IDS_TILE_QUICKSOIL,
	IDS_TILE_QUICKSOIL,
};

const wstring QuicksoilTile::TEXTURE_NAMES[] = {
	L"quicksoil", L"quicksoil"
};

QuicksoilTile::QuicksoilTile(int id) : Tile(id, Material::sand)
{
	friction = 0.989f;
	icons = nullptr;
}

void QuicksoilTile::stepOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
	entity->xd*=1.3;
    entity->zd*=1.3;
}

int QuicksoilTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return 0;
}

void QuicksoilTile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
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

Icon* QuicksoilTile::getTexture(int face, int data)
{
	if (data < 0 || data >= QUICKSOIL_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void QuicksoilTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [QUICKSOIL_NAMES_LENGTH];

	for (int i = 0; i < QUICKSOIL_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
