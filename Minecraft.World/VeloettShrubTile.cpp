#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "VeloettShrubTile.h"

VeloettShrubTile::VeloettShrubTile(int id) : Bush(id,Material::replaceable_plant)
{
    updateDefaultShape();
}

void VeloettShrubTile::updateDefaultShape()
{
    float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 0.8f, 0.5f + ss);
}

bool VeloettShrubTile::mayPlace(Level *level, int x, int y, int z)
{
	if (!Tile::mayPlace(level, x, y, z)) return false;

	return canSurvive(level, x, y, z);
}

bool VeloettShrubTile::canSurvive(Level *level, int x, int y, int z)
{
	int below = level->getTile(x, y - 1, z);
	return below == Tile::veloettGrass_Id || below == Tile::endStone_Id || below == Tile::nugrass_Id || below == Tile::nustone_Id;
}

int VeloettShrubTile::getResource(int data, Random *random, int playerBonusLevel)
{
	if (id == Tile::veloettShrub_Id) return Item::veloettBerry->id;
	return 0;
}

int VeloettShrubTile::getResourceCount(Random *random)
{
	if (id == Tile::veloettShrub_Id) return 1 + random->nextInt(2);
	return 0;
}

int VeloettShrubTile::getResourceCountForLootBonus(int bonusLevel, Random *random)
{
	return getResourceCount(random) + random->nextInt(bonusLevel + 1);
}

shared_ptr<ItemInstance> VeloettShrubTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::veloettShrub));
}

void VeloettShrubTile::spawnResources(Level *level, int x, int y, int z, int data, float odds,  int playerBonusLevel)
{
	if (id == Tile::veloettShrub_Id)
	{
		if (!level->isClientSide)
		{
			int chance = 2;

			chance = 2;
			if (playerBonusLevel > 0)
			{
				chance -= 1 << playerBonusLevel;
				if (chance < 2)
				{
					chance = 2;
				}
			}
			if (level->random->nextInt(chance) == 0)
			{
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::veloettBerry_Id, 1, 0));
			}
		}
	}
}
