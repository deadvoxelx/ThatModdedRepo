#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.dimension.h"
#include "NusaCoreTile.h"

const unsigned int NusaCoreTile::CORE_NAMES[CORE_NAMES_LENGTH] = {
	IDS_TILE_NUSA_CORE,
	IDS_TILE_NUSA_CORE,
};

const wstring NusaCoreTile::TEXTURE_NAMES[] = {
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
	if (data < 0 || data >= CORE_NAMES_LENGTH)
	{
		data = 0;
	}
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
	if (level->getData(x, y, z) == NusaCoreTile::TYPE_ACTIVE)
	{
		this->setLightEmission(0.0f);
		setTicking(false);
		level->setData(x, y, z, NusaCoreTile::TYPE_DEFAULT, Tile::UPDATE_CLIENTS);
	}
	level->addToTickNextTick(x, y, z, id, getTickDelay(level));
}

int NusaCoreTile::getTickDelay(Level *level)
{
	return 100;
}

bool NusaCoreTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly)
{
	if (level->dimension->id == 4)
	{
		this->setLightEmission(0.5f);
		setTicking(true);
		level->setData(x, y, z, NusaCoreTile::TYPE_ACTIVE, Tile::UPDATE_CLIENTS);
		//shared_ptr<LightningBolt> l = std::make_shared<LightningBolt>(level, x, y, z);
		//l->moveTo(x, y, z, 0, 0);
		//level->addEntity(l);
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
