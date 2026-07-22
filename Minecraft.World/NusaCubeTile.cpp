#include "stdafx.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "NusaCubeTile.h"

NusaCubeTile::NusaCubeTile(int id, Material *material) : Tile(id, material, isSolidRender())
{
	icon = nullptr;
	iconTop = nullptr;
}

int NusaCubeTile::getRenderShape()
{
	return SHAPE_CUBE;
}

void NusaCubeTile::updateDefaultShape()
{
	const float cubeStart = 4.0f / 16.0f;
	const float cubeFinish = 12.0f / 16.0f;
	const float cubeYStart = 0.0f / 16.0f;
	const float cubeYFinish = 8.0f / 16.0f;
	this->setShape(cubeStart, cubeYStart, cubeStart, cubeFinish, cubeYFinish, cubeFinish);
}

bool NusaCubeTile::isCubeShaped()
{
	return false;
}

bool NusaCubeTile::isSolidRender(bool isServerLevel)
{
	return false;
}

AABB *NusaCubeTile::getAABB(Level *level, int x, int y, int z)
{
	float r = 8 / 16.0f;
	return AABB::newTemp(x + r, y + r, z + r, x + 1 - r, y + 1 - r, z + 1 - r);
}

bool NusaCubeTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	return true;
}

Icon *NusaCubeTile::getTexture(int face, int data)
{
	if (face == Facing::UP || face == Facing::DOWN) return iconTop;
	return icon;
}

void NusaCubeTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(getIconName() + L"_side");
	iconTop = iconRegister->registerIcon(getIconName() + L"_top");
}
