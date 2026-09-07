#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.entity.h"
#include "NusaCoreTile.h"
#include "Nusagar.h"
#include "..\Minecraft.Client\ServerLevel.h"
#include "..\Minecraft.Client\ServerPlayer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace
{
	void spawnSmokeRing(Level *level, int x, int y, int z, Random *random)
	{
		ServerLevel *serverLevel = dynamic_cast<ServerLevel *>(level);
		if (serverLevel == nullptr) return;

		const std::wstring particleName = std::to_wstring(static_cast<int>(eParticleType_reddust));
		for (int i = 0; i < 24; ++i)
		{
			double angle = (2.0 * M_PI * i) / 16.0;
			double radius = 1.25 + random->nextDouble() * 0.15;
			double px = x + 0.5 + cos(angle) * radius;
			double pz = z + 0.5 + sin(angle) * radius;
			serverLevel->sendParticles(particleName, px, y + 0.45, pz, 1, 0.06, 0.06, 0.06, 0.015);
		}
	}
}

const unsigned int NusaCoreTile::CORE_NAMES[CORE_NAMES_LENGTH] =
{
	IDS_TILE_NUSA_CORE,
	IDS_TILE_NUSA_CORE,
};

const wstring NusaCoreTile::TEXTURE_NAMES[] =
{
	L"nusaCore_side", L"nusaCoreActive_side"
};

NusaCoreTile::NusaCoreTile(int id, Material *material) : Tile(id, material)
{
	icons = nullptr;
	iconTop = nullptr;
}

Icon *NusaCoreTile::getTexture(int face, int data)
{
	if (face == Facing::UP || face == Facing::DOWN) return iconTop;
	data = data & 1;
	data = (data & 1) == TYPE_ACTIVE ? TYPE_ACTIVE : TYPE_DEFAULT;
	return icons[data];
}

void NusaCoreTile::registerIcons(IconRegister *iconRegister)
{
	iconTop = iconRegister->registerIcon(L"nusaCore_top");
	icons = new Icon * [CORE_NAMES_LENGTH];

	for (int i = 0; i < CORE_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}

void NusaCoreTile::tick(Level *level, int x, int y, int z, Random *random)
{
	if (level->dimension->id == 4 && (level->getData(x, y, z) & 1) == NusaCoreTile::TYPE_ACTIVE)
	{
		int data = level->getData(x, y, z);
		int pulses = data >> 1;

		if (pulses < CORE_ACTIVE_CYCLES - 1)
		{
			++pulses;
			level->setData(x, y, z, TYPE_ACTIVE | (pulses << 1), Tile::UPDATE_CLIENTS);
			level->playSound(x, y, z, eSoundType_RANDOM_NUSAGAR_SUMMONED, 6.0f, 1.0f);
			spawnSmokeRing(level, x, y, z, level->random);
			level->addToTickNextTick(x, y, z, id, CORE_CYCLE_DELAY);

			if (pulses == CORE_ACTIVE_CYCLES - 1)
			{
				for (auto& p : level->players)
				{
					if (p->dimension == level->dimension->id && p->instanceof(eTYPE_SERVERPLAYER))
					{
						dynamic_pointer_cast<ServerPlayer>(p)->sendMessage(app.GetString(IDS_GOOD_LUCK));
					}
				}
			}
		}
		else
		{
			this->setLightEmission(0.0f);
			level->setData(x, y, z, TYPE_DEFAULT, Tile::UPDATE_CLIENTS);
			
			level->removeTile(x, y, z);
			level->removeTile(x, y - 1, z);
			for (int dx = -6; dx <= 6; dx++)
			{
				for (int dz = -6; dz <= 6; dz++)
				{
					for (int dy = -1; dy <= 4; dy++)
					{
						level->removeTile(x + dx, y + dy, z + dz);
					}
					if (level->getTile(x + dx, y - 2, z + dz) == Tile::nustone_Id && level->getData(x + dx, y - 2, z + dz) == NustoneTile::TYPE_DARK) level->setTileAndData(x + dx, y - 2, z + dz, Tile::bossStone_Id, 0, Tile::UPDATE_CLIENTS);
					if (level->getTile(x + dx, y - 2, z + dz) == Tile::netherPlanks_Id && level->getData(x + dx, y - 2, z + dz) == NetherPlanksTile::TYPE_NUSA) level->setTileAndData(x + dx, y - 2, z + dz, Tile::bossStone_Id, 0, Tile::UPDATE_CLIENTS);
					if (level->getTile(x + dx, y - 2, z + dz) == Tile::nusaLog_Id) level->setTileAndData(x + dx, y - 2, z + dz, Tile::nustone_Id, NustoneTile::TYPE_DARK, Tile::UPDATE_CLIENTS);
					for (int fy = -26; fy <= 0; fy++)
					{
						if (level->getTile(x + dx, y + fy, z + dz) == Tile::nustone_Id && level->getData(x + dx, y + fy, z + dz) == NustoneTile::TYPE_DARK) level->setTileAndData(x + dx, y + fy, z + dz, Tile::bossStone_Id, 0, Tile::UPDATE_CLIENTS);
						if (level->getTile(x + dx, y + fy, z + dz) == Tile::netherPlanks_Id && level->getData(x + dx,y + fy, z + dz) == NetherPlanksTile::TYPE_NUSA) level->setTileAndData(x + dx, y + fy, z + dz, Tile::nustone_Id, NustoneTile::TYPE_DARK, Tile::UPDATE_CLIENTS);
					}
				}
			}
			level->playSound(x, y, z, eSoundType_RANDOM_EXPLODE, 1.0f, 1.0f);
			shared_ptr<Nusagar> nusagar = make_shared<Nusagar>(level);
			nusagar->moveTo(x, y + 2, z, 0, 0);
			level->addEntity(nusagar);
			
		}
	}
	else if ((level->getData(x, y, z) & 1) == TYPE_ACTIVE)
	{
		this->setLightEmission(0.0f);
		level->setData(x, y, z, TYPE_DEFAULT, Tile::UPDATE_CLIENTS);
	}
}

int NusaCoreTile::getTickDelay(Level *level)
{
	return level->dimension->id == 4 ? CORE_CYCLE_DELAY : 100;
}

bool NusaCoreTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly)
{
	if (level->isClientSide) return true;
	if ((level->getData(x, y, z) & 1) == NusaCoreTile::TYPE_ACTIVE) return false;

	if (level->dimension->id == 4)
	{
		this->setLightEmission(0.5f);
		level->setData(x, y, z, NusaCoreTile::TYPE_ACTIVE, Tile::UPDATE_CLIENTS);
		level->playSound(x, y, z, eSoundType_RANDOM_NUSAGAR_SUMMONED, 6.0f, 1.0f);
		spawnSmokeRing(level, x, y, z, level->random);
		level->addToTickNextTick(x, y, z, id, CORE_CYCLE_DELAY);
		return true;
	}
	else if (level->dimension->id == 2)
	{
		// Base
		int xTile = level->getTile(x + 1, y - 1, z);
		int xTile2 = level->getTile(x - 1, y - 1, z);
		int zTile = level->getTile(x, y - 1, z + 1);
		int zTile2 = level->getTile(x, y - 1, z - 1);
		int cTile = level->getTile(x + 1, y - 1, z + 1);
		int cTile2 = level->getTile(x - 1, y - 1, z - 1);
		int cTile3 = level->getTile(x - 1, y - 1, z + 1);
		int cTile4 = level->getTile(x + 1, y - 1, z - 1);

		// Other
		int cubeTile = level->getTile(x, y + 1, z);
		int tTile = level->getTile(x + 1, y, z + 1);
		int tTile2 = level->getTile(x - 1, y, z - 1);
		int tTile3 = level->getTile(x - 1, y, z + 1);
		int tTile4 = level->getTile(x + 1, y, z - 1);

		if (cubeTile == Tile::nusaCube_Id && tTile == Tile::endothaniumBlock_Id && tTile2 == Tile::endothaniumBlock_Id && tTile3 == Tile::endothaniumBlock_Id && tTile4 == Tile::endothaniumBlock_Id)
		{
			if (xTile == Tile::endoriumBlock_Id && xTile2 == Tile::endoriumBlock_Id && zTile == Tile::endoriumBlock_Id && zTile2 == Tile::endoriumBlock_Id)
			{
				if (cTile == Tile::nethaniumBlock_Id && cTile2 == Tile::nethaniumBlock_Id && cTile3 == Tile::nethaniumBlock_Id && cTile4 == Tile::nethaniumBlock_Id)
				{
					for (int dx = -1; dx <= 1; dx++)
					{
						for (int dy = -1; dy <= 2; dy++)
						{
							for (int dz = -1; dz <= 1; dz++)
							{
								level->setTileAndData(x + dx, y + dy, z + dz, 0, 0, Tile::UPDATE_CLIENTS);
								level->setTileAndData(x, y, z, Tile::nusaPortal_Id, 0, Tile::UPDATE_CLIENTS);
								level->setTileAndData(x, y - 1, z, Tile::nustone_Id, 1, Tile::UPDATE_CLIENTS);
							}
						}
					}
					level->playSound(x, y, z, eSoundType_AMBIENT_WEATHER_THUNDER, 1.0f, 1.0f);
					return true;
				}
				else
				{
					player->displayClientMessage(IDS_CANNOT_BE_DONE);
					return true;
				}
			}
			else
			{
				player->displayClientMessage(IDS_CANNOT_BE_DONE);
				return true;
			}
		}
		else
		{
			player->displayClientMessage(IDS_CANNOT_BE_DONE);
			return true;
		}
	}
	else
	{
		player->displayClientMessage(IDS_CANNOT_BE_DONE);
		return true;
	}
	return false;
}
