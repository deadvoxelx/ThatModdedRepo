#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "DeadBushTile.h"

DeadBushTile::DeadBushTile(int id) : Bush(id,Material::replaceable_plant)
{
    updateDefaultShape();
}

void DeadBushTile::updateDefaultShape()
{
    float ss = 0.4f;
    this->setShape(0.5f - ss, 0, 0.5f - ss, 0.5f + ss, 0.8f, 0.5f + ss);
}

bool DeadBushTile::mayPlace(Level *level, int x, int y, int z)
{
	if (!Tile::mayPlace(level, x, y, z)) return false;

	return canSurvive(level, x, y, z);
}

bool DeadBushTile::canSurvive(Level *level, int x, int y, int z)
{
	int below = level->getTile(x, y - 1, z);
	return below == Tile::sand_Id || below == Tile::stainedSand_Id || below == Tile::endSand_Id || below == Tile::grass_Id || below == Tile::dirt_Id || below == Tile::clay_Id || below == Tile::netherSoil_Id;
}

int DeadBushTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Item::stick->id;
}

int DeadBushTile::getResourceCount(Random *random)
{
	return 1 + random->nextInt(2);
}

int DeadBushTile::getResourceCountForLootBonus(int bonusLevel, Random *random)
{
	return getResourceCount(random) + random->nextInt(bonusLevel + 1);
}

shared_ptr<ItemInstance> DeadBushTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::deadBush));
}

void DeadBushTile::spawnResources(Level *level, int x, int y, int z, int data, float odds,  int playerBonusLevel)
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
			popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::stick_Id, 1, 0));
		}
	}
}
