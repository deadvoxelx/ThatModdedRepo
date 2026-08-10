#include "stdafx.h"
#include "FossilTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.h"

const unsigned int FossilTile::FOSSIL_NAMES[FOSSIL_NAMES_LENGTH] = {
	IDS_TILE_FOSSIL_ORE,
	IDS_TILE_FOSSIL_ORE,
	IDS_TILE_FOSSIL_ORE,
	IDS_TILE_FOSSIL_ORE,
};

const wstring FossilTile::TEXTURE_NAMES[] = {
	L"fossilStone", L"fossilNetherrack", L"fossilHolystone", L"fossilEndstone"
};

FossilTile::FossilTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

int FossilTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

void FossilTile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
{
	int d = data;
	if (d == TYPE_DEFAULT)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::shellFossil_Id, 1, 0));
		}
	}
	else if (d == TYPE_NETHER)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::fireFossil_Id, 1, 0));
		}
	}
	else if (d == TYPE_AETHER)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::airFossil_Id, 1, 0));
		}
	}
	else if (d == TYPE_END)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::ribFossil_Id, 1, 0));
		}
	}
}

Icon* FossilTile::getTexture(int face, int data)
{
	if (data < 0 || data >= FOSSIL_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void FossilTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [FOSSIL_NAMES_LENGTH];

	for (int i = 0; i < FOSSIL_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}