#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "ModSaplingTile.h"
#include "Sapling.h"
#include "Feature.h"

ModSaplingTile::ModSaplingTile(int id, const std::wstring &iconName, Feature *treeFeature) : Bush(id)
{
	this->iconName = iconName;
	this->treeFeature = treeFeature;
	setSoundType(Tile::SOUND_GRASS);
}

ModSaplingTile::~ModSaplingTile()
{
	delete treeFeature;
}

void ModSaplingTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(iconName);
}

void ModSaplingTile::tick(Level *level, int x, int y, int z, Random *random)
{
	if (level->isClientSide) return;

	Bush::tick(level, x, y, z, random);

	if (level->getRawBrightness(x, y + 1, z) >= Level::MAX_BRIGHTNESS - 6)
	{
		if (random->nextInt(7) == 0)
		{
			const int data = level->getData(x, y, z);
			const int AGE_BIT = 8;

			if ((data & AGE_BIT) == 0)
			{
				level->setData(x, y, z, data | AGE_BIT, Tile::UPDATE_NONE);
			}
			else
			{
				level->setTileAndData(x, y, z, 0, 0, Tile::UPDATE_NONE);
				if (!treeFeature->place(level, random, x, y, z))
				{
					level->setTileAndData(x, y, z, id, data, Tile::UPDATE_NONE);
				}
			}
		}
	}
}
