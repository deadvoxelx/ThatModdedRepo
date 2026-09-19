#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "BoneVineTile.h"

BoneVineTile::BoneVineTile(int id) : HangingPlant(id,Material::plant)
{
    updateDefaultShape();
}

void BoneVineTile::updateDefaultShape()
{
    float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 1.0f, 0.5f + ss);
}

bool BoneVineTile::mayPlace(Level *level, int x, int y, int z)
{
	if (!Tile::mayPlace(level, x, y, z)) return false;

	return canSurvive(level, x, y, z);
}

bool BoneVineTile::canSurvive(Level *level, int x, int y, int z)
{
	int above = level->getTile(x, y + 1, z);
	Material *m = level->getMaterial(x, y + 1, z);
	return above == Tile::boneVine_Id || above == Tile::netherLeaves_Id;
	return m == Material::stone || Material::endStone  || Material::netherrack || Material::grass || Material::veloettGrass || Material::dirt || Material::sand || Material::wood;
}

void BoneVineTile::neighborChanged(Level *level, int x, int y, int z, int type)
{
	if (!canSurvive(level, x, y, z))
	{
		level->destroyTile(x, y, z, true);
	}
}
