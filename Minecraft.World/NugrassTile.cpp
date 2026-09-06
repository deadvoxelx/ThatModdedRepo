#include "stdafx.h"
#include "NugrassTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "Facing.h"

NugrassTile::NugrassTile(int id) : Tile(id, Material::netherrack)
{
	iconTop = NULL;
	iconBottom = NULL;
}

Icon *NugrassTile::getTexture(int face, int data)
{
	if (face == Facing::UP) return iconTop;
	if (face == Facing::DOWN) return iconBottom;
	return icon;
}

void NugrassTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(getIconName() + L"_side");
	iconTop = iconRegister->registerIcon(getIconName() + L"_top");
	iconBottom = iconRegister->registerIcon(L"nustone");
}

shared_ptr<ItemInstance> NugrassTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::nugrass));
}

void NugrassTile::playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data)
{
	if (!level->isClientSide)
	{
		popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Tile::nustone_Id, 1, 0)));
	}
}