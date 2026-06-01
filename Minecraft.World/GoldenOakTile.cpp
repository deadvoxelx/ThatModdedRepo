#include "stdafx.h"
#include "GoldenOakTile.h"
#include "net.minecraft.world.h"
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