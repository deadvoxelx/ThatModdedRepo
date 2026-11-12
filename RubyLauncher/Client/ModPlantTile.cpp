#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "ModPlantTile.h"
#include "Material.h"
#include "Random.h"

ModPlantTile::ModPlantTile(int id, const std::wstring &iconName) : Bush(id, Material::replaceable_plant)
{
	this->iconName = iconName;
	setSoundType(Tile::SOUND_GRASS);

	updateDefaultShape();
}

void ModPlantTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(iconName);
}

void ModPlantTile::updateDefaultShape()
{
	float ss = 0.4f;
	setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 0.8f, 0.5f + ss);
}

int ModPlantTile::getResourceCount(Random *random)
{
	return dropNothing ? 0 : 1;
}

int ModPlantTile::getResource(int data, Random *random, int playerBonusLevel)
{
	if (dropNothing) return 0;
	if (dropId <= 0) return id;
	return dropId;
}

int ModPlantTile::getSpawnResourcesAuxValue(int data)
{
	return dropId > 0 ? dropAux : Tile::getSpawnResourcesAuxValue(data);
}

void ModPlantTile::setSoil(const std::vector<int> &blocks)
{
	soil = blocks;
}

void ModPlantTile::setDrop(int newDropId, int newDropAux)
{
	dropId = newDropId;
	dropAux = newDropAux;
}

void ModPlantTile::setDropNothing(bool value)
{
	dropNothing = value;
}

bool ModPlantTile::mayPlaceOn(int tile)
{
	if (soil.empty()) return Bush::mayPlaceOn(tile);

	for (int allowed : soil)
	{
		if (allowed == tile) return true;
	}
	return false;
}
