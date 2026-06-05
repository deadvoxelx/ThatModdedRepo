#include "stdafx.h"
#include "HolystoneTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int HolystoneTile::HOLYSTONE_NAMES[HOLYSTONE_NAMES_LENGTH] = {
	IDS_TILE_HOLYSTONE,
	IDS_TILE_HOLYSTONE_MOSSY,
	IDS_TILE_HOLYSTONE_BRICK,
};

const wstring HolystoneTile::TEXTURE_NAMES[] = {
	L"holystone", L"holystone_mossy", L"holystoneBricks"
};

HolystoneTile::HolystoneTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

int HolystoneTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

Icon* HolystoneTile::getTexture(int face, int data)
{
	if (data < 0 || data >= HOLYSTONE_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void HolystoneTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [HOLYSTONE_NAMES_LENGTH];

	for (int i = 0; i < HOLYSTONE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
