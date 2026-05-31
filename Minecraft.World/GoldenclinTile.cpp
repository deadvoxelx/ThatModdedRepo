#include "stdafx.h"
#include "GoldenclinTile.h"
#include "net.minecraft.world.h"

const wstring GoldenclinTile::TEXTURE_NAMES[] = {L"", L"bricks"};

const unsigned int GoldenclinTile::GOLDENCLIN_NAMES[GOLDENCLIN_NAMES_LENGTH] = {
	IDS_TILE_GOLDENCLIN,
	IDS_TILE_GOLDENCLIN_BRICKS,
};

GoldenclinTile::GoldenclinTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

Icon *GoldenclinTile::getTexture(int face, int data)
{
	if (data < 0 || data >= GOLDENCLIN_NAMES_LENGTH) data = 0;
	return icons[data];
}

int GoldenclinTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

unsigned int GoldenclinTile::getDescriptionId(int iData /*= -1*/)
{
	if(iData < 0 ) iData = 0;
	return GoldenclinTile::GOLDENCLIN_NAMES[iData];
}

void GoldenclinTile::registerIcons(IconRegister *iconRegister)
{
	icons = new Icon*[GOLDENCLIN_NAMES_LENGTH];

	for (int i = 0; i < GOLDENCLIN_NAMES_LENGTH; i++)
	{
		wstring name = getIconName();
		if (!TEXTURE_NAMES[i].empty() ) name += L"_" + TEXTURE_NAMES[i];
		icons[i] = iconRegister->registerIcon(name);
	}
}