#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "ModSaplingTile.h"
#include "Sapling.h"
#include "Feature.h"
#include "Registry/WorldGen/FeatureRegistry.h"

ModSaplingTile::ModSaplingTile(int id, const std::wstring &iconName, Feature *treeFeature, const std::string &featureName) : Bush(id)
{
	this->iconName = iconName;
	this->treeFeature = treeFeature;
	this->featureName = featureName;
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

void ModSaplingTile::grow(Level *level, int x, int y, int z, Random *random)
{
	const int data = level->getData(x, y, z);

	level->setTileAndData(x, y, z, 0, 0, Tile::UPDATE_NONE);

	bool placed = false;
	if (!featureName.empty())
	{
		placed = FeatureRegistry::place(featureName, level, random, x, y, z);
	}
	else if (treeFeature != nullptr)
	{
		placed = treeFeature->place(level, random, x, y, z);
	}

	if (!placed)
	{
		level->setTileAndData(x, y, z, id, data, Tile::UPDATE_NONE);
	}
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
				grow(level, x, y, z, random);
			}
		}
	}
}

bool ModSaplingTile::canBonemeal(Level *level, int x, int y, int z)
{
	return true;
}

bool ModSaplingTile::bonemeal(Level *level, int x, int y, int z, Random *random)
{
	if (random->nextFloat() < 0.45f)
	{
		grow(level, x, y, z, random);
		return true;
	}
	return false;
}
