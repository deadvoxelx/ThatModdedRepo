#include "stdafx.h"
#include "AercloudTile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.h"
#include "..\Minecraft.Client\Minecraft.h"

const unsigned int AercloudTile::CLOUD_NAMES[CLOUD_NAMES_LENGTH] = {
	IDS_TILE_AERCLOUD,
	IDS_TILE_AERCLOUD_BLUE,
	IDS_TILE_AERCLOUD_GOLD,
};

const wstring AercloudTile::TEXTURE_NAMES[] = {
	L"aercloud", L"aercloud", L"aercloud"
};

AercloudTile::AercloudTile(int id, Material *material, bool allowSame) : HalfTransparentTile(id, L"aercloud", material, allowSame)
{
	icons = nullptr;
}

int AercloudTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

AABB *AercloudTile::getAABB(Level *level, int x, int y, int z)
{
	float r = 8 / 16.0f;
	return AABB::newTemp(x, y, z, x + 1, y + 1 - r, z + 1);
}

void AercloudTile::fallOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity, float fallDistance)
{
	entity->fallDistance = 0;
	if (entity->yd < 0)
	{
		entity->yd *= 0.1;
	}
}

void AercloudTile::entityInside(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
	entity->xd *= 0.4;
	entity->zd *= 0.4;

	if (entity->yd < 0)
	{
		entity->yd *= 0.5;
		entity->fallDistance = 0;
	}
}

bool AercloudTile::isSolidRender(bool isServerLevel)
{
	return false;
}

int AercloudTile::getRenderLayer()
{
	return 1;
}

bool AercloudTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	return HalfTransparentTile::shouldRenderFace(level, x, y, z, 1 - face);
}

int AercloudTile::getColor() const
{
	return 0xffffff;
}

int AercloudTile::getColor(int auxData)
{
	if (auxData == TYPE_BLUE)
	{
		return 0xa3d1ff; //Blue
	}
	if (auxData == TYPE_GOLD)
	{
		return 0xffef91; //Gold
	}
	return 0xffffff;
}

int AercloudTile::getColor(LevelSource *level, int x, int y, int z)
{
	return getColor( level, x, y, z, level->getData(x, y, z) );
}

int AercloudTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	int d = data;
	if (d == TYPE_BLUE) return 0xa3d1ff; //Blue
	if (d == TYPE_GOLD) return 0xffef91; //Gold
	return 0xffffff;
}

Icon* AercloudTile::getTexture(int face, int data)
{
	if (data < 0 || data >= CLOUD_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void AercloudTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [CLOUD_NAMES_LENGTH];

	for (int i = 0; i < CLOUD_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}