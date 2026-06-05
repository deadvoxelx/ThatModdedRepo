#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.food.h"
#include "net.minecraft.stats.h"
#include "AerogelTile.h"

AerogelTile::AerogelTile(int id) : HalfTransparentTile(id, L"aerogel", Material::ice, false)
{
}

int AerogelTile::getRenderLayer()
{
	return 1;
}

bool AerogelTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	return HalfTransparentTile::shouldRenderFace(level, x, y, z, 1 - face);
}