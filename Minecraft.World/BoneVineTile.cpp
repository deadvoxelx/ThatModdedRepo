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
	int below = level->getTile(x, y + 1, z);
	return below == Tile::boneVine_Id || below == Tile::netherRack_Id || below == Tile::netherSoil_Id || below == Tile::netherLeaves_Id;
}
