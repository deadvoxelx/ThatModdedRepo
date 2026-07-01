#include "stdafx.h"
#include "DungeonStoneTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int DungeonStoneTile::DUNGEONSTONE_NAMES[DUNGEONSTONE_NAMES_LENGTH] = {
	IDS_TILE_DUNGEON_STONE,
	IDS_TILE_ANGELIC_STONE,
	IDS_TILE_HELLFIRE_STONE,
};

const wstring DungeonStoneTile::TEXTURE_NAMES[] = {
	L"carvedStone", L"angelicStone", L"hellfireStone"
};

DungeonStoneTile::DungeonStoneTile(int id) : Tile(id, Material::stone)
{
	icons = nullptr;
}

int DungeonStoneTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

Icon* DungeonStoneTile::getTexture(int face, int data)
{
	if (data < 0 || data >= DUNGEONSTONE_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void DungeonStoneTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [DUNGEONSTONE_NAMES_LENGTH];

	for (int i = 0; i < DUNGEONSTONE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}