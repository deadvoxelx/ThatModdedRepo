#include "stdafx.h"
#include "NetherrackTile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

NetherrackTile::NetherrackTile(int id) : Tile(id, Material::netherrack)
{
}

bool NetherrackTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly)
{
	if (level->isClientSide) return true;

	// Base
	int bTile = level->getTile(x, y - 1, z);
	int xTile = level->getTile(x + 1, y - 1, z);
	int xTile2 = level->getTile(x - 1, y - 1, z);
	int zTile = level->getTile(x, y - 1, z + 1);
	int zTile2 = level->getTile(x, y - 1, z - 1);
	int cTile = level->getTile(x + 1, y - 1, z + 1);
	int cTile2 = level->getTile(x - 1, y - 1, z - 1);
	int cTile3 = level->getTile(x - 1, y - 1, z + 1);
	int cTile4 = level->getTile(x + 1, y - 1, z - 1);

	// Other
	int fTile = level->getTile(x, y + 1, z);
	int tTile = level->getTile(x + 1, y, z);
	int tTile2 = level->getTile(x - 1, y, z);
	int tTile3 = level->getTile(x, y, z + 1);
	int tTile4 = level->getTile(x, y, z - 1);

	if (bTile == Tile::mossyCobblestone_Id && fTile == Tile::fire_Id && tTile == Tile::redstoneTorch_on_Id && tTile2 == Tile::redstoneTorch_on_Id && tTile3 == Tile::redstoneTorch_on_Id && tTile4 == Tile::redstoneTorch_on_Id)
	{
		if (xTile == Tile::goldBlock_Id && xTile2 == Tile::goldBlock_Id && zTile == Tile::goldBlock_Id && zTile2 == Tile::goldBlock_Id)
		{
			if (cTile == Tile::goldBlock_Id && cTile2 == Tile::goldBlock_Id && cTile3 == Tile::goldBlock_Id && cTile4 == Tile::goldBlock_Id)
			{
				for (int dx = -1; dx <= 1; dx++)
				{
					for (int dy = -1; dy <= 2; dy++)
					{
						for (int dz = -1; dz <= 1; dz++)
						{
							level->setTileAndData(x + dx, y + dy, z + dz, 0, 0, Tile::UPDATE_CLIENTS);
						}
					}
				}

				shared_ptr<LightningBolt> l = std::make_shared<LightningBolt>(level, x, y, z);
				l->moveTo(x, y, z, 0, 0);
				level->addEntity(l);

				player->displayClientMessage(IDS_HEROBRINE_ARRIVED);

				return true;
			}
		}
	}
	return false;
}
