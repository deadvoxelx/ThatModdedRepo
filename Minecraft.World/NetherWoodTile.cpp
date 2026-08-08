#include "stdafx.h"
#include "NetherWoodTile.h"
#include "TreeTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "Facing.h"

NetherWoodTile::NetherWoodTile(int id) : Tile(id, Material::wood)
{
	iconTop = NULL;
}

Icon *NetherWoodTile::getTexture(int face, int data)
{
	if (face == Facing::UP || face == Facing::DOWN) return iconTop;
	return icon;
}

void NetherWoodTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(getIconName() + L"_side");
	iconTop = iconRegister->registerIcon(getIconName() + L"_top");
}

void NetherWoodTile::playerWillDestroy(Level *level, int x, int y, int z, int data, shared_ptr<Player> player)
{
	Tile::playerWillDestroy(level, x, y, z, data, player);

	TreeTile::chopTreeColumn(level, x, y, z, player);
}
