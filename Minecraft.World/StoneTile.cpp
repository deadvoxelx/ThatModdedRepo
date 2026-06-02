#include "stdafx.h"
#include "StoneTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int StoneTile::STONE_NAMES[STONE_NAMES_LENGTH] = {
	IDS_TILE_STONE,
	IDS_TILE_HOLYSTONE,
	IDS_TILE_HOLYSTONE_MOSSY,
};

const wstring StoneTile::TEXTURE_NAMES[] = {
	L"stone", L"holystone", L"holystone_mossy"
};

StoneTile::StoneTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

int StoneTile::getResource(int data, Random* random, int playerBonusLevel)
{

	if (data < 0 || data >= STONE_NAMES_LENGTH) data = 0;

	if (data == 0) {
		return Tile::cobblestone_Id;
	}

	return Tile::stone_Id;
}

int StoneTile::getSpawnResourcesAuxValue(int data)
{
	if (data < 0 || data >= STONE_NAMES_LENGTH) data = 0;

	return data;
}

Icon* StoneTile::getTexture(int face, int data)
{
	if (data < 0 || data >= STONE_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void StoneTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [STONE_NAMES_LENGTH];

	for (int i = 0; i < STONE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
