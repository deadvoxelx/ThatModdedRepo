#include "stdafx.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.item.h"
#include "PortalTile.h"
#include "FireTile.h"

PortalTile::PortalTile(int id) : HalfTransparentTile(id, L"portal", Material::portal, false)
{
	setTicking(true);
}

void PortalTile::tick(Level *level, int x, int y, int z, Random *random)
{
	HalfTransparentTile::tick(level, x, y, z, random);

	if (level->dimension->isNaturalDimension() && random->nextInt(2000) < level->difficulty)
	{
		// locate floor
		int y0 = y;
		while (!level->isTopSolidBlocking(x, y0, z) && y0 > 0)
		{
			y0--;
		}
		if (y0 > 0 && !level->isSolidBlockingTile(x, y0 + 1, z))
		{
			// spawn a pig man here
			int iResult = 0;
			shared_ptr<Entity> entity = SpawnEggItem::spawnMobAt(level, 57, x + .5, y0 + 1.1, z + .5, &iResult);
			if (entity != nullptr)
			{
				entity->changingDimensionDelay = entity->getDimensionChangingDelay();
			}
		}
	}
}

AABB *PortalTile::getAABB(Level *level, int x, int y, int z)
{
	return nullptr;
}

void PortalTile::updateShape(LevelSource *level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity) // 4J added forceData, forceEntity param
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

bool PortalTile::isSolidRender(bool isServerLevel)
{
	return false;
}

bool PortalTile::isCubeShaped()
{
	return false;
}

bool PortalTile::validPortalFrame(Level* level, int x, int y, int z, int xd, int zd, bool actuallySpawn)
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
				if (t != Tile::obsidian_Id) return false;
			}
			else
			{
				if (t != 0 && t != Tile::fire_Id) return false;
			}
		}
	}
	if (!actuallySpawn) return true;

	for (int xx = 0; xx < 2; xx++)
	{
		for (int yy = 0; yy < 3; yy++)
		{
			level->setTileAndData(x + xd * xx, y + yy, z + zd * xx, Tile::portalTile_Id, 0, Tile::UPDATE_CLIENTS);
		}
	}
		
	return true;
}

bool PortalTile::detectAndSpawnPortal(Level* level, int x, int y, int z, int axis, bool actuallySpawn) {
	int baseX = x;
	int baseZ = z;
	int hCoord;
	if (axis == 1) {
		hCoord = x;
	}
	else {
		hCoord = z;
	}

	int left = hCoord;
	while (true) {
		int nextX = (axis == 1) ? left - 1 : baseX;
		int nextZ = (axis == 0) ? left - 1 : baseZ;
		int tile = level->getTile(nextX, y, nextZ);
		if (tile == Tile::obsidian_Id) break;
		if (tile != 0 && tile != Tile::fire_Id) return false;
		left--;
		if (left < -30000000) return false;
	}

	int right = hCoord;
	while (true) {
		int nextX = (axis == 1) ? right + 1 : baseX;
		int nextZ = (axis == 0) ? right + 1 : baseZ;
		int tile = level->getTile(nextX, y, nextZ);
		if (tile == Tile::obsidian_Id) break;
		if (tile != 0 && tile != Tile::fire_Id) return false;
		right++;
		if (right > 30000000) return false;
	}
	int interiorWidth = right - left + 1;

	int bottom = y;
	while (true) {
		int tileBelow = level->getTile((axis == 1) ? hCoord : baseX, bottom - 1, (axis == 0) ? hCoord : baseZ);
		if (tileBelow == Tile::obsidian_Id) break;
		if (tileBelow != 0 && tileBelow != Tile::fire_Id) return false;
		bottom--;
		if (bottom < -64) return false;
	}

	int top = y;
	while (true) {
		int tileAbove = level->getTile((axis == 1) ? hCoord : baseX, top + 1, (axis == 0) ? hCoord : baseZ);
		if (tileAbove == Tile::obsidian_Id) break;
		if (tileAbove != 0 && tileAbove != Tile::fire_Id) return false;
		top++;
		if (top > 320) return false;
	}
	int interiorHeight = top - bottom + 1;

	if (interiorWidth < 2 || interiorWidth > 21) return false;
	if (interiorHeight < 3 || interiorHeight > 21) return false;

	for (int cy = bottom; cy <= top; cy++) {
		int obsX = (axis == 1) ? left - 1 : baseX;
		int obsZ = (axis == 0) ? left - 1 : baseZ;
		if (level->getTile(obsX, cy, obsZ) != Tile::obsidian_Id) return false;
	}

	for (int cy = bottom; cy <= top; cy++) {
		int obsX = (axis == 1) ? right + 1 : baseX;
		int obsZ = (axis == 0) ? right + 1 : baseZ;
		if (level->getTile(obsX, cy, obsZ) != Tile::obsidian_Id) return false;
	}

	for (int cx = left; cx <= right; cx++) {
		int obsX = (axis == 1) ? cx : baseX;
		int obsZ = (axis == 0) ? cx : baseZ;
		if (level->getTile(obsX, bottom - 1, obsZ) != Tile::obsidian_Id) return false;
	}

	for (int cx = left; cx <= right; cx++) {
		int obsX = (axis == 1) ? cx : baseX;
		int obsZ = (axis == 0) ? cx : baseZ;
		if (level->getTile(obsX, top + 1, obsZ) != Tile::obsidian_Id) return false;
	}

	for (int cx = left; cx <= right; cx++) {
		for (int cy = bottom; cy <= top; cy++) {
			int tile = level->getTile(
				(axis == 1) ? cx : baseX,
				cy,
				(axis == 0) ? cx : baseZ
			);
			if (tile != 0 && tile != Tile::fire_Id) return false;
		}
	}

	if (actuallySpawn) {
		for (int cx = left; cx <= right; cx++) {
			for (int cy = bottom; cy <= top; cy++) {
				int px = (axis == 1) ? cx : baseX;
				int pz = (axis == 0) ? cx : baseZ;
				level->setTileAndData(px, cy, pz, Tile::portalTile_Id, 0, Tile::UPDATE_CLIENTS);
			}
		}
	}
	return true;
}

bool PortalTile::trySpawnPortal(Level* level, int x, int y, int z, bool actuallySpawn) {
	int centerTile = level->getTile(x, y, z);
	if (centerTile != 0 && centerTile != Tile::fire_Id) return false;

	if (detectAndSpawnPortal(level, x, y, z, 1, actuallySpawn)) return true; // X
	if (detectAndSpawnPortal(level, x, y, z, 0, actuallySpawn)) return true; // Z

	return false;
}

void PortalTile::neighborChanged(Level* level, int x, int y, int z, int type) {
	int axis = -1; // 0:Z, 1:X
	if (level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id) {
		axis = 1;
	}
	else if (level->getTile(x, y, z - 1) == id || level->getTile(x, y, z + 1) == id) {
		axis = 0;
	}
	else {
		level->removeTile(x, y, z);
		return;
	}

	int bottomY = y;
	while (level->getTile(x, bottomY - 1, z) == id) bottomY--;
	if (level->getTile(x, bottomY - 1, z) != Tile::obsidian_Id) {
		level->removeTile(x, y, z);
		return;
	}

	int topY = y;
	while (level->getTile(x, topY + 1, z) == id) topY++;
	if (level->getTile(x, topY + 1, z) != Tile::obsidian_Id) {
		level->removeTile(x, y, z);
		return;
	}
	int height = topY - bottomY + 1;
	if (height < 3 || height > 21) {
		level->removeTile(x, y, z);
		return;
	}

	int baseX = x, baseZ = z;
	int hCoord = (axis == 1) ? x : z;
	int left = hCoord, right = hCoord;

	while (true) {
		int nx = (axis == 1) ? left - 1 : baseX;
		int nz = (axis == 0) ? left - 1 : baseZ;
		int tile = level->getTile(nx, y, nz);
		if (tile == Tile::obsidian_Id) break;
		if (tile != id) {
			level->removeTile(x, y, z);
			return;
		}
		left--;
	}
	while (true) {
		int nx = (axis == 1) ? right + 1 : baseX;
		int nz = (axis == 0) ? right + 1 : baseZ;
		int tile = level->getTile(nx, y, nz);
		if (tile == Tile::obsidian_Id) break;
		if (tile != id) {
			level->removeTile(x, y, z);
			return;
		}
		right++;
	}
	int width = right - left + 1;
	if (width < 2 || width > 21) {
		level->removeTile(x, y, z);
		return;
	}

	for (int cy = bottomY; cy <= topY; cy++) {
		if (level->getTile((axis == 1) ? left - 1 : baseX, cy, (axis == 0) ? left - 1 : baseZ) != Tile::obsidian_Id) { level->removeTile(x, y, z); return; }
		if (level->getTile((axis == 1) ? right + 1 : baseX, cy, (axis == 0) ? right + 1 : baseZ) != Tile::obsidian_Id) { level->removeTile(x, y, z); return; }
	}
	for (int cx = left; cx <= right; cx++) {
		if (level->getTile((axis == 1) ? cx : baseX, bottomY - 1, (axis == 0) ? cx : baseZ) != Tile::obsidian_Id) { level->removeTile(x, y, z); return; }
		if (level->getTile((axis == 1) ? cx : baseX, topY + 1, (axis == 0) ? cx : baseZ) != Tile::obsidian_Id) { level->removeTile(x, y, z); return; }
	}

	for (int cx = left; cx <= right; cx++) {
		for (int cy = bottomY; cy <= topY; cy++) {
			int tile = level->getTile((axis == 1) ? cx : baseX, cy, (axis == 0) ? cx : baseZ);
			if (tile != id && tile != 0 && tile != Tile::fire_Id) {
				level->removeTile(x, y, z);
				return;
			}
		}
	}
}

bool PortalTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
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

int PortalTile::getResourceCount(Random *random)
{
	return 0;
}

int PortalTile::getRenderLayer()
{
	return 2;
}

void PortalTile::entityInside(Level *level, int x, int y, int z, shared_ptr<Entity> entity)
{
	if (entity->GetType() == eTYPE_EXPERIENCEORB ) return;		// 4J added

	if (entity->riding == nullptr && entity->rider.lock() == nullptr) entity->handleInsidePortal();
}

void PortalTile::animateTick(Level *level, int xt, int yt, int zt, Random *random)
{
	if (random->nextInt(100) == 0)
	{
		level->playLocalSound(xt + 0.5, yt + 0.5, zt + 0.5, eSoundType_PORTAL_PORTAL, 0.5f, random->nextFloat() * 0.4f + 0.8f, false);
	}
	for (int i = 0; i < 4; i++)
	{
		double x = xt + random->nextFloat();
		double y = yt + random->nextFloat();
		double z = zt + random->nextFloat();
		double xa = 0;
		double ya = 0;
		double za = 0;
		int flip = random->nextInt(2) * 2 - 1;
		xa = (random->nextFloat() - 0.5) * 0.5;
		ya = (random->nextFloat() - 0.5) * 0.5;
		za = (random->nextFloat() - 0.5) * 0.5;
		if (level->getTile(xt - 1, yt, zt) == id || level->getTile(xt + 1, yt, zt) == id)
		{
			z = zt + 0.5 + (0.25) * flip;
			za = (random->nextFloat() * 2) * flip;
		}
		else
		{
			x = xt + 0.5 + (0.25) * flip;
			xa = (random->nextFloat() * 2) * flip;
		}

		level->addParticle(eParticleType_netherportal, x, y, z, xa, ya, za);
	}
}

int PortalTile::cloneTileId(Level *level, int x, int y, int z)
{
	return 0;
}

// 4J Added - We cannot collect the portal tile, so don't consider it as a hit result
// Bug #754 - Riding a minecart into a portal will trap the player
bool PortalTile::mayPick()
{
	return false;
}