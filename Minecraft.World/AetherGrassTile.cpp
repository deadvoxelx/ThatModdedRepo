#include "stdafx.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "AetherGrassTile.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "IntBuffer.h"

AetherGrassTile::AetherGrassTile(int id) : Tile(id, Material::grass)
{
	iconTop = nullptr;

	setTicking(true);
}

Icon *AetherGrassTile::getTexture(int face, int data)
{
	if (face == Facing::UP) return iconTop;
	if (face == Facing::DOWN) return Tile::dirt->getTexture(face, 1);
	return icon;
}

Icon *AetherGrassTile::getTexture(LevelSource *level, int x, int y, int z, int face)
{
	if (face == Facing::UP) return iconTop;
	if (face == Facing::DOWN) return Tile::dirt->getTexture(face, 1);
	return icon;
}

void AetherGrassTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(L"aetherGrass_side");
	iconTop = iconRegister->registerIcon(L"aetherGrass_top");
}

void AetherGrassTile::tick(Level *level, int x, int y, int z, Random *random)
{
	if (level->isClientSide) return;

	if (level->getRawBrightness(x, y + 1, z) < MIN_BRIGHTNESS && Tile::lightBlock[level->getTile(x, y + 1, z)] > 2)
	{
		level->setTileAndData(x, y, z, Tile::dirt_Id, 1, Tile::UPDATE_CLIENTS);
	}
	else
	{
		if (level->getRawBrightness(x, y + 1, z) >= Level::MAX_BRIGHTNESS - 6)
		{
			for (int i = 0; i < 4; i++)
			{
				int xt = x + random->nextInt(3) - 1;
				int yt = y + random->nextInt(5) - 3;
				int zt = z + random->nextInt(3) - 1;
				int above = level->getTile(xt, yt + 1, zt);
				if (level->getTile(xt, yt, zt) == Tile::dirt_Id && level->getRawBrightness(xt, yt + 1, zt) >= MIN_BRIGHTNESS && Tile::lightBlock[above] <= 2)
				{
					level->setTileAndData(x, y, z, Tile::aetherGrass_Id, 0, Tile::UPDATE_CLIENTS);
				}
			}
		}
	}
}

int AetherGrassTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Tile::dirt->getResource(1, random, playerBonusLevel);
}

bool AetherGrassTile::shouldTileTick(Level *level, int x,int y,int z)
{
	bool should = false;

	if( (level->getRawBrightness(x, y + 1, z) < MIN_BRIGHTNESS && Tile::lightBlock[level->getTile(x, y + 1, z)] > 2) ||
		(level->getRawBrightness(x, y + 1, z) >= Level::MAX_BRIGHTNESS - 6) )
	{
		should = true;
	}

	return should;
}
