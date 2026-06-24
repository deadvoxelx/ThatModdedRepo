#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "FlowerTile.h"
#include "..\Minecraft.Client\Minecraft.h"

const unsigned int FlowerTile::FLOWER_NAMES[FLOWER_NAMES_LENGTH] = {
	IDS_TILE_FLOWER,
	IDS_TILE_FLOWER,
	IDS_TILE_FLOWER,
};

const wstring FlowerTile::TEXTURE_NAMES[] = {
	L"flower_dandelion", L"flower_white", L"flower_purple"
};

FlowerTile::FlowerTile(int id) : Bush(id,Material::replaceable_plant)
{
	icons = nullptr;
    updateDefaultShape();
}

void FlowerTile::updateDefaultShape()
{
    float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 0.8f, 0.5f + ss);
}

bool FlowerTile::canSurvive(Level *level, int x, int y, int z)
{
	int below = level->getTile(x, y - 1, z);
	return below == Tile::aetherGrass_Id || below == Tile::aetherDirt_Id || below == Tile::grass_Id || below == Tile::dirt_Id || below == Tile::netherSoil_Id;
}

bool FlowerTile::mayPlace(Level *level, int x, int y, int z)
{
	if (!Tile::mayPlace(level, x, y, z)) return false;

	return canSurvive(level, x, y, z);
}

int FlowerTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

Icon* FlowerTile::getTexture(int face, int data)
{
	if (data < 0 || data >= FLOWER_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void FlowerTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [FLOWER_NAMES_LENGTH];

	for (int i = 0; i < FLOWER_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
