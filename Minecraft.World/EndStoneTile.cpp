#include "stdafx.h"
#include "EndStoneTile.h"
#include "net.minecraft.world.h"

const wstring EndStoneTile::TEXTURE_NAMES[] = {L"", L"smooth", L"mossy", L"bricks", L"bricksMossy", L"chiseled"};

const unsigned int EndStoneTile::END_STONE_NAMES[END_STONE_NAMES_LENGTH] = {
	IDS_TILE_WHITESTONE,
	IDS_TILE_ENDSTONESMOOTH,
	IDS_TILE_ENDSTONEMOSSY,
	IDS_TILE_ENDBRICKS,
	IDS_TILE_ENDBRICKSMOSSY,
	IDS_TILE_ENDBRICKSCHISELED,
};

EndStoneTile::EndStoneTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

Icon *EndStoneTile::getTexture(int face, int data)
{
	if (data < 0 || data >= END_STONE_NAMES_LENGTH) data = 0;
	return icons[data];
}

int EndStoneTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

unsigned int EndStoneTile::getDescriptionId(int iData /*= -1*/)
{
	if(iData < 0 ) iData = 0;
	return EndStoneTile::END_STONE_NAMES[iData];
}

void EndStoneTile::registerIcons(IconRegister *iconRegister)
{
	icons = new Icon*[END_STONE_NAMES_LENGTH];

	for (int i = 0; i < END_STONE_NAMES_LENGTH; i++)
	{
		wstring name = getIconName();
		if (!TEXTURE_NAMES[i].empty() ) name += L"_" + TEXTURE_NAMES[i];
		icons[i] = iconRegister->registerIcon(name);
	}
}