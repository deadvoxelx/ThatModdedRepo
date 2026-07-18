#include "stdafx.h"
#include "NustoneTile.h"
#include "net.minecraft.world.h"

const wstring NustoneTile::TEXTURE_NAMES[] = {L"nustone", L"nubricks", L"darkNustone"};

const unsigned int NustoneTile::NUSTONE_NAMES[NUSTONE_NAMES_LENGTH] = {
	IDS_TILE_NUSTONE,
	IDS_TILE_NUBRICKS,
	IDS_TILE_DARK_NUSTONE,
};

NustoneTile::NustoneTile(int id) : Tile(id, Material::endStone)
{
	icons = nullptr;
}

int NustoneTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

Icon *NustoneTile::getTexture(int face, int data)
{
	if (data < 0 || data >= NUSTONE_NAMES_LENGTH) data = 0;
	return icons[data];
}

void NustoneTile::registerIcons(IconRegister *iconRegister)
{
	icons = new Icon * [NUSTONE_NAMES_LENGTH];

	for (int i = 0; i < NUSTONE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}