#include "stdafx.h"
#include "GrassTile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.phys.h"
#include "LavaReedTile.h"

LavaReedTile::LavaReedTile(int id) : Tile( id, Material::plant,isSolidRender() )
{
	this->updateDefaultShape();
	this->setTicking(true);
}

void LavaReedTile::updateDefaultShape()
{
	float ss = 6 / 16.0f;
	this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 1, 0.5f + ss);
}

void LavaReedTile::tick(Level *level, int x, int y, int z, Random* random)
{
	if (level->isEmptyTile(x, y + 1, z))
	{
		int height = 1;
		while (level->getTile(x, y - height, z) == id)
		{
			height++;
		}
		if (height < 5)
		{
			int age = level->getData(x, y, z);
			if (age == 15) 
			{
				level->setTileAndUpdate(x, y + 1, z, id);
				level->setData(x, y, z, 0, Tile::UPDATE_NONE);
			} 
			else 
			{
				level->setData(x, y, z, age + 1, Tile::UPDATE_NONE);
			}
		}
	}
}

bool LavaReedTile::mayPlace(Level *level, int x, int y, int z) 
{
	int below = level->getTile(x, y - 1, z);
	if (below == id) return true;
	if (below != Tile::lavaRock_Id) return false;
	if (level->getMaterial(x - 1, y - 1, z) == Material::lava) return true;
	if (level->getMaterial(x + 1, y - 1, z) == Material::lava) return true;
	if (level->getMaterial(x, y - 1, z - 1) == Material::lava) return true;
	if (level->getMaterial(x, y - 1, z + 1) == Material::lava) return true;
	return false;
}

void LavaReedTile::neighborChanged(Level *level, int x, int y, int z, int type)
{
	checkAlive(level, x, y, z);
}

const void LavaReedTile::checkAlive(Level *level, int x, int y, int z)
{
	if (!canSurvive(level, x, y, z)) 
	{
		spawnResources(level, x, y, z, level->getData(x, y, z), 0);
		level->removeTile(x, y, z);
	}
}

bool LavaReedTile::canSurvive(Level *level, int x, int y, int z)
{
	return mayPlace(level, x, y, z);
}

AABB *LavaReedTile::getAABB(Level *level, int x, int y, int z)
{
	return nullptr;
}

int LavaReedTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Item::lavaReeds->id;
}

bool LavaReedTile::blocksLight()
{
	return false;
}

bool LavaReedTile::isSolidRender(bool isServerLevel)
{
	return false;
}

bool LavaReedTile::isCubeShaped() 
{
	return false;
}

int LavaReedTile::getRenderShape()
{
	return Tile::SHAPE_CROSS_TEXTURE;
}

int LavaReedTile::cloneTileId(Level *level, int x, int y, int z)
{
	return Item::lavaReeds_Id;
}

bool LavaReedTile::shouldTileTick(Level *level, int x,int y,int z)
{
	return level->isEmptyTile(x, y + 1, z);
}
