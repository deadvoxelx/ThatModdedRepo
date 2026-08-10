#include "stdafx.h"
#include "net.minecraft.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "JavaMath.h"
#include "BlackrootTile.h"

const unsigned int BlackrootTile::BLACKROOT_NAMES[BLACKROOT_NAMES_LENGTH] =
{
	IDS_TILE_BLACKROOT,
	IDS_TILE_BLACKROOT,
};

const wstring BlackrootTile::TEXTURE_NAMES[] = { L"blackroot", L"blackrootGrown" };

BlackrootTile::BlackrootTile(int id) : HangingPlant(id,Material::plant)
{
	setTicking(true);
	icons = nullptr;
}

void BlackrootTile::tick(Level *level, int x, int y, int z, Random *random)
{
	HangingPlant::tick(level, x, y, z, random);

	int age = level->getData(x, y, z);
	if (age == 0)
	{
		if (random->nextInt(10) == 0)
		{
			age++;
			level->setData(x, y, z, age, Tile::UPDATE_CLIENTS);
		}
	}
}

int BlackrootTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}

void BlackrootTile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
{
	if (data == TYPE_DEFAULT)
	{
		if (!level->isClientSide) popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::blackroot_Id, 1, 0));
	}
	else if (data == TYPE_GROWN)
	{
		if (!level->isClientSide)
		{
			popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Tile::blackroot_Id, 1, 0)));

			int count = 2 + level->random->nextInt(3);
			for (int i = 0; i < count; i++)
			{
				popResource(level, x, y, z, shared_ptr<ItemInstance>(new ItemInstance(Item::blackrootBerry_Id, 1, 0)));
			}
		}
	}
}

Icon* BlackrootTile::getTexture(int face, int data)
{
	if (data < 0 || data >= BLACKROOT_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void BlackrootTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [BLACKROOT_NAMES_LENGTH];

	for (int i = 0; i < BLACKROOT_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}