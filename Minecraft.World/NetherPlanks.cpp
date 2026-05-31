#include "stdafx.h"
#include "NetherPlanks.h"
#include "net.minecraft.world.h"

const wstring NetherPlanksTile::TEXTURE_NAMES[] = {L"", L"purul"};

const unsigned int NetherPlanksTile::NETHER_PLANK_NAMES[NETHER_PLANK_NAMES_LENGTH] = {
	IDS_TILE_NETHER_PLANKS,
	IDS_TILE_PURULPLANKS,
};

NetherPlanksTile::NetherPlanksTile(int id) : Tile(id, Material::wood)
{
	icons = nullptr;
}

Icon *NetherPlanksTile::getTexture(int face, int data)
{
	if (data < 0 || data >= NETHER_PLANK_NAMES_LENGTH) data = 0;
	return icons[data];
}

int NetherPlanksTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

unsigned int NetherPlanksTile::getDescriptionId(int iData /*= -1*/)
{
	if(iData < 0 ) iData = 0;
	return NetherPlanksTile::NETHER_PLANK_NAMES[iData];
}

void NetherPlanksTile::registerIcons(IconRegister *iconRegister)
{
	icons = new Icon*[NETHER_PLANK_NAMES_LENGTH];

	for (int i = 0; i < NETHER_PLANK_NAMES_LENGTH; i++)
	{
		wstring name = getIconName();
		if (!TEXTURE_NAMES[i].empty() ) name += L"_" + TEXTURE_NAMES[i];
		icons[i] = iconRegister->registerIcon(name);
	}
}
