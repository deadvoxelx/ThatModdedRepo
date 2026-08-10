#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "GrassTile.h"
#include "HangingPlantTile.h"

void HangingPlant::_init()
{
	setTicking(true);
	updateDefaultShape();
}

HangingPlant::HangingPlant(int id, Material *material) : Tile(id, material, isSolidRender())
{
	_init();
}

HangingPlant::HangingPlant(int id) : Tile(id, Material::plant, isSolidRender())
{
	_init();
}

void HangingPlant::updateDefaultShape()
{
	float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 1.0f, 0.5f + ss);
}

bool HangingPlant::mayPlace(Level *level, int x, int y, int z)
{
	return Tile::mayPlace(level, x, y, z) && mayPlaceOn(level->getTile(x, y - 1, z));
}

bool HangingPlant::mayPlaceOn(int tile)
{
	return tile == Tile::netherSoil_Id || tile == Tile::netherRack_Id;
}

void HangingPlant::neighborChanged(Level *level, int x, int y, int z, int type)
{
	Tile::neighborChanged(level, x, y, z, type);
	checkAlive(level, x, y, z);
}

void HangingPlant::tick(Level *level, int x, int y, int z, Random *random)
{
	checkAlive(level, x, y, z);
}

void HangingPlant::checkAlive(Level *level, int x, int y, int z)
{
	if (!canSurvive(level, x, y, z))
	{
		this->spawnResources(level, x, y, z, level->getData(x, y, z), 0);
		level->setTileAndData(x, y, z, 0, 0, UPDATE_CLIENTS);
	}
}

bool HangingPlant::canSurvive(Level *level, int x, int y, int z)
{
	return (mayPlaceOn(level->getTile(x, y + 1, z)));
}

AABB *HangingPlant::getAABB(Level *level, int x, int y, int z)
{
	return NULL;
}

bool HangingPlant::blocksLight()
{
	return false;
}

bool HangingPlant::isSolidRender(bool isServerLevel)
{
	return false;
}

bool HangingPlant::isCubeShaped()
{
	return false;
}

int HangingPlant::getRenderShape()
{
	return Tile::SHAPE_CROSS_TEXTURE;
}
