#include "stdafx.h"
#include "GoldenOakTile.h"
#include "TreeTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "Facing.h"

GoldenOakTile::GoldenOakTile(int id) : Tile(id, Material::wood)
{
	iconTop = NULL;
}

Icon *GoldenOakTile::getTexture(int face, int data)
{
	if (face == Facing::UP || face == Facing::DOWN) return iconTop;
	return icon;
}

void GoldenOakTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(getIconName() + L"_side");
	iconTop = iconRegister->registerIcon(L"skyrootLog_top");
}

void GoldenOakTile::playerWillDestroy(Level *level, int x, int y, int z, int data, shared_ptr<Player> player)
{
	Tile::playerWillDestroy(level, x, y, z, data, player);

	TreeTile::chopTreeColumn(level, x, y, z, player);
}

shared_ptr<ItemInstance> GoldenOakTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::goldenOakLog));
}

void GoldenOakTile::playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data)
{
	if (!level->isClientSide)
	{
		popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Tile::skyrootLog_Id, 1, 0)));

		int count = 1 + Mth::nextInt(level->random, 0, 1);
		for (int i = 0; i < count; i++)
		{
			popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Item::goldenAmber_Id, 1, 0)));
		}
	}
}
