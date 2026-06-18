#include "stdafx.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.h"
#include "AetherPortalTile.h"
#include "LiquidTile.h"

AetherPortalTile::AetherPortalTile(int id) : HalfTransparentTile(id, L"water", Material::portal, false)
{
	setTicking(true);
}

void AetherPortalTile::tick(Level *level, int x, int y, int z, Random *random)
{
	HalfTransparentTile::tick(level, x, y, z, random);
}

AABB *AetherPortalTile::getAABB(Level *level, int x, int y, int z)
{
	return nullptr;
}

void AetherPortalTile::updateShape(LevelSource *level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity)
{
	if (level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id)
	{
		float xr = 8 / 16.0f;
		float yr = 2 / 16.0f;
		setShape(0.5f - xr, 0, 0.5f - yr, 0.5f + xr, 1, 0.5f + yr);
	}
	else
	{
		float xr = 2 / 16.0f;
		float yr = 8 / 16.0f;
		setShape(0.5f - xr, 0, 0.5f - yr, 0.5f + xr, 1, 0.5f + yr);
	}
}

bool AetherPortalTile::isSolidRender(bool isServerLevel)
{
	return false;
}

bool AetherPortalTile::isCubeShaped()
{
	return false;
}

bool AetherPortalTile::validPortalFrame(Level* level, int x, int y, int z, int xd, int zd, bool actuallySpawn)
{
	for (int xx = -1; xx <= 2; xx++)
	{
		for (int yy = -1; yy <= 3; yy++)
		{
			bool edge = (xx == -1) || (xx == 2) || (yy == -1) || (yy == 3);
			if ((xx == -1 || xx == 2) && (yy == -1 || yy == 3)) continue;

			int t = level->getTile(x + xd * xx, y + yy, z + zd * xx);

			if (edge)
			{
				if (t != Tile::glowstone_Id) return false;
			}
			else
			{
				if (t != 0 && t != Tile::water_Id && t != Tile::calmWater_Id) return false;
			}
		}
	}
	if (!actuallySpawn) return true;

	for (int xx = 0; xx < 2; xx++)
	{
		for (int yy = 0; yy < 3; yy++)
		{
			level->setTileAndData(x + xd * xx, y + yy, z + zd * xx, Tile::aetherPortal_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
	return true;
}

bool AetherPortalTile::trySpawnPortal(Level *level, int x, int y, int z, bool actuallySpawn)
{
	int xd = 0;
	int zd = 0;
	if (level->getTile(x - 1, y, z) == Tile::glowstone_Id || level->getTile(x + 1, y, z) == Tile::glowstone_Id) xd = 1;
	if (level->getTile(x, y, z - 1) == Tile::glowstone_Id || level->getTile(x, y, z + 1) == Tile::glowstone_Id) zd = 1;

	bool twoPosible = false;
	if (xd == zd)
	{
		if (xd == 1) twoPosible = true;
		else return false;
	}

	bool changedx = false;
	if (level->getTile(x - xd, y, z) == 0)
	{
		changedx = true;
		x--;
	}
	else if (level->getTile(x, y, z - zd) == 0 && !twoPosible)
	{
		z--;
	}

	if (!twoPosible)
	{
		if (!AetherPortalTile::validPortalFrame(level, x, y, z, xd, zd, actuallySpawn)) return false;
	}
	else
	{
		if (!AetherPortalTile::validPortalFrame(level, x, y, z, xd, 0, actuallySpawn))
		{
			if (changedx) x++;

			if (level->getTile(x, y, z - zd) == 0) z--;

			if (!AetherPortalTile::validPortalFrame(level, x, y, z, 0, zd, actuallySpawn))
				return false;
		}
	}
	return true;
}

void AetherPortalTile::neighborChanged(Level *level, int x, int y, int z, int type)
{
	int xd = 0;
	int zd = 1;
	if (level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id)
	{
		xd = 1;
		zd = 0;
	}

	int yBottom = y;
	while (level->getTile(x, yBottom - 1, z) == id)
		yBottom--;

	if (level->getTile(x, yBottom - 1, z) != Tile::glowstone_Id)
	{
		level->removeTile(x, y, z);
		return;
	}

	int height = 1;
	while (height < 4 && level->getTile(x, yBottom + height, z) == id)
		height++;

	if (height != 3 || level->getTile(x, yBottom + height, z) != Tile::glowstone_Id)
	{
		level->removeTile(x, y, z);
		return;
	}

	bool we = level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id;
	bool ns = level->getTile(x, y, z - 1) == id || level->getTile(x, y, z + 1) == id;
	if (we && ns)
	{
		level->removeTile(x, y, z);
		return;
	}

	if (!(//
		(level->getTile(x + xd, y, z + zd) == Tile::glowstone_Id && level->getTile(x - xd, y, z - zd) == id) || //
		(level->getTile(x - xd, y, z - zd) == Tile::glowstone_Id && level->getTile(x + xd, y, z + zd) == id)//
		))
	{
		level->removeTile(x, y, z);
		return;
	}
}

bool AetherPortalTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	if (level->getTile(x, y, z) == id) return false;

	bool w = level->getTile(x - 1, y, z) == id && level->getTile(x - 2, y, z) != id;
	bool e = level->getTile(x + 1, y, z) == id && level->getTile(x + 2, y, z) != id;

	bool n = level->getTile(x, y, z - 1) == id && level->getTile(x, y, z - 2) != id;
	bool s = level->getTile(x, y, z + 1) == id && level->getTile(x, y, z + 2) != id;

	bool we = w || e;
	bool ns = n || s;

	if (we && face == 4) return true;
	if (we && face == 5) return true;
	if (ns && face == 2) return true;
	if (ns && face == 3) return true;

	return false;
}

int AetherPortalTile::getResourceCount(Random *random)
{
	return 0;
}

int AetherPortalTile::getRenderLayer()
{
	return 1;
}

void AetherPortalTile::entityInside(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
	if (entity->GetType() == eTYPE_EXPERIENCEORB ) return;		// 4J added

	if (entity->riding == nullptr && entity->rider.lock() == nullptr) entity->handleInsideAetherPortal();
}

void AetherPortalTile::animateTick(Level *level, int xt, int yt, int zt, Random *random)
{
	// No particles for the Aether portal
}

int AetherPortalTile::cloneTileId(Level *level, int x, int y, int z)
{
	return 0;
}

bool AetherPortalTile::mayPick()
{
	return false;
}

int AetherPortalTile::getColor() const
{
	return 0x4040ff;
}

int AetherPortalTile::getColor(int auxData)
{
	return 0x4040ff;
}

int AetherPortalTile::getColor(LevelSource *level, int x, int y, int z)
{
	return 0x4040ff;
}

int AetherPortalTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	return 0x4040ff;
}
