#include "stdafx.h"
#include "VeloettGrassTile.h"
#include "net.minecraft.world.h"
#include "Facing.h"

VeloettGrassTile::VeloettGrassTile(int id) : Tile(id, Material::veloettGrass)
{
	iconTop = NULL;
	iconBottom = NULL;
}

Icon *VeloettGrassTile::getTexture(int face, int data)
{
	if (face == Facing::UP) return iconTop;
	if (face == Facing::DOWN) return iconBottom;
	return icon;
}

void VeloettGrassTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(getIconName() + L"_side");
	iconTop = iconRegister->registerIcon(getIconName() + L"_top");
	iconBottom = iconRegister->registerIcon(L"end_stone");
}
