#include "stdafx.h"
#include "DungeonLightTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int DungeonLightTile::DUNGEONSTONE_NAMES[DUNGEONSTONE_NAMES_LENGTH] = {
	IDS_TILE_DUNGEON_STONE,
	IDS_TILE_ANGELIC_STONE,
	IDS_TILE_HELLFIRE_STONE,
};

const wstring DungeonLightTile::TEXTURE_NAMES[] = {
	L"carvedStone_light", L"angelicStone_light", L"hellfireStone_light"
};

DungeonLightTile::DungeonLightTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

int DungeonLightTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

Icon* DungeonLightTile::getTexture(int face, int data)
{
	if (data < 0 || data >= DUNGEONSTONE_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void DungeonLightTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [DUNGEONSTONE_NAMES_LENGTH];

	for (int i = 0; i < DUNGEONSTONE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}