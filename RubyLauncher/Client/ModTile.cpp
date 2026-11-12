#include "ModTile.h"
#include "Level.h"
#include "LevelSource.h"
#include "Random.h"
#include "net.minecraft.world.level.h"

int ModTile::getRenderLayer()
{
	return portalBlock ? 1 : Tile::getRenderLayer();
}

bool ModTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	if (!portalBlock) return Tile::shouldRenderFace(level, x, y, z, face);

	if (level->getTile(x, y, z) == id) return false;

	const bool w = level->getTile(x - 1, y, z) == id && level->getTile(x - 2, y, z) != id;
	const bool e = level->getTile(x + 1, y, z) == id && level->getTile(x + 2, y, z) != id;
	const bool n = level->getTile(x, y, z - 1) == id && level->getTile(x, y, z - 2) != id;
	const bool s = level->getTile(x, y, z + 1) == id && level->getTile(x, y, z + 2) != id;

	const bool we = w || e;
	const bool ns = n || s;

	if (we && face == 4) return true; // -x
	if (we && face == 5) return true; // +x
	if (ns && face == 2) return true; // -z
	if (ns && face == 3) return true; // +z

	return false;
}

void ModTile::updateShape(LevelSource *level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity)
{
	if (!portalBlock)
	{
		Tile::updateShape(level, x, y, z, forceData, forceEntity);
		return;
	}

	if (level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id)
	{
		const float xr = 8 / 16.0f;
		const float yr = 2 / 16.0f;
		setShape(0.5f - xr, 0, 0.5f - yr, 0.5f + xr, 1, 0.5f + yr);
	}
	else
	{
		const float xr = 2 / 16.0f;
		const float yr = 8 / 16.0f;
		setShape(0.5f - xr, 0, 0.5f - yr, 0.5f + xr, 1, 0.5f + yr);
	}
}

void ModTile::animateTick(Level *level, int x, int y, int z, Random *random)
{
	if (!portalBlock) return;

	if (random->nextInt(100) == 0)
	{
		level->playLocalSound(x + 0.5, y + 0.5, z + 0.5, eSoundType_PORTAL_PORTAL, 0.5f, random->nextFloat() * 0.4f + 0.8f, false);
	}

	for (int i = 0; i < 4; i++)
	{
		double px = x + random->nextFloat();
		double py = y + random->nextFloat();
		double pz = z + random->nextFloat();

		double xa = (random->nextFloat() - 0.5) * 0.5;
		double ya = (random->nextFloat() - 0.5) * 0.5;
		double za = (random->nextFloat() - 0.5) * 0.5;

		const int flip = random->nextInt(2) * 2 - 1;
		if (level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id)
		{
			pz = z + 0.5 + 0.25 * flip;
			za = (random->nextFloat() * 2) * flip;
		}
		else
		{
			px = x + 0.5 + 0.25 * flip;
			xa = (random->nextFloat() * 2) * flip;
		}

		level->addParticle(eParticleType_netherportal, px, py, pz, xa, ya, za);
	}
}

void ModTile::neighborChanged(Level *level, int x, int y, int z, int type)
{
	if (!portalBlock || portalFrameId <= 0) return;

	int xd = 0;
	int zd = 1;
	if (level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id)
	{
		xd = 1;
		zd = 0;
	}

	int yBottom = y;
	while (level->getTile(x, yBottom - 1, z) == id) yBottom--;

	if (level->getTile(x, yBottom - 1, z) != portalFrameId)
	{
		level->removeTile(x, y, z);
		return;
	}

	int height = 1;
	while (height < 4 && level->getTile(x, yBottom + height, z) == id) height++;

	if (height != 3 || level->getTile(x, yBottom + height, z) != portalFrameId)
	{
		level->removeTile(x, y, z);
		return;
	}

	const bool we = level->getTile(x - 1, y, z) == id || level->getTile(x + 1, y, z) == id;
	const bool ns = level->getTile(x, y, z - 1) == id || level->getTile(x, y, z + 1) == id;
	if (we && ns)
	{
		level->removeTile(x, y, z);
		return;
	}

	const bool inner = level->getTile(x - xd, y, z - zd) == id;
	const bool outer = level->getTile(x + xd, y, z + zd) == portalFrameId;
	const bool innerFlipped = level->getTile(x + xd, y, z + zd) == id;
	const bool outerFlipped = level->getTile(x - xd, y, z - zd) == portalFrameId;

	if (!((outer && inner) || (innerFlipped && outerFlipped)))
	{
		level->removeTile(x, y, z);
	}
}
