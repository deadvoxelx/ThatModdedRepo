#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "VeloettVineTile.h"

VeloettVineTile::VeloettVineTile(int id) : Bush(id,Material::plant)
{
    updateDefaultShape();
}

void VeloettVineTile::updateDefaultShape()
{
    float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 1.0f, 0.5f + ss);
}

bool VeloettVineTile::mayPlace(Level *level, int x, int y, int z)
{
	if (!Tile::mayPlace(level, x, y, z)) return false;

	return canSurvive(level, x, y, z);
}

bool VeloettVineTile::canSurvive(Level *level, int x, int y, int z)
{
	int below = level->getTile(x, y - 1, z);
	Material *m = level->getMaterial(x, y - 1, z);
	return below == Tile::veloettVine_Id;
	return m == Material::stone || Material::endStone  || Material::netherrack || Material::grass || Material::veloettGrass || Material::dirt || Material::wood;
}

void VeloettVineTile::neighborChanged(Level *level, int x, int y, int z, int type)
{
	if (!canSurvive(level, x, y, z))
	{
		level->destroyTile(x, y, z, true);
	}
}
