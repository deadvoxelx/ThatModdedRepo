#include "stdafx.h"
#include "DirtTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int DirtTile::DIRT_NAMES[DIRT_NAMES_LENGTH] = {
	IDS_TILE_DIRT,
	IDS_TILE_AETHER_DIRT,
};

const wstring DirtTile::TEXTURE_NAMES[] = {
	L"dirt", L"aetherDirt"
};

DirtTile::DirtTile(int id) : Tile(id, Material::dirt)
{
	icons = nullptr;
}

int DirtTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

Icon* DirtTile::getTexture(int face, int data)
{
	if (data < 0 || data >= DIRT_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void DirtTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [DIRT_NAMES_LENGTH];

	for (int i = 0; i < DIRT_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
