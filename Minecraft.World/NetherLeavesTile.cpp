#include "stdafx.h"
#include "NetherLeavesTile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"

NetherLeavesTile::NetherLeavesTile(int id) : TransparentTile(id, Material::leaves, true)
{
}

int NetherLeavesTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Item::stick->id;
}

int NetherLeavesTile::getResourceCountForLootBonus(int bonusLevel, Random *random)
{
	return getResourceCount(random) + random->nextInt(bonusLevel + 1);
}

int NetherLeavesTile::getResourceCount(Random *random)
{
	return 1 + random->nextInt(2);
}

shared_ptr<ItemInstance> NetherLeavesTile::getSilkTouchItemInstance(int data)
{
	if (id == Tile::skyrootLeaves_Id) return shared_ptr<ItemInstance>(new ItemInstance(Tile::skyrootLeaves));
	if (id == Tile::goldenOakLeaves_Id) return shared_ptr<ItemInstance>(new ItemInstance(Tile::goldenOakLeaves));
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::netherLeaves));
}

void NetherLeavesTile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
{
	if (id == Tile::netherLeaves_Id)
	{
		if (!level->isClientSide)
		{
			int chance = 5;

			chance = 5;
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::netherSapling_Id, 1, 0));
			}

			chance = 5;
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
	else if (id == Tile::skyrootLeaves_Id)
	{
		if (!level->isClientSide)
		{
			int chance = 5;

			chance = 5;
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::netherSapling_Id, 1, 0));
			}

			/*chance = 5;
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
			}*/
		}
	}
	else if (id == Tile::goldenOakLeaves_Id)
	{
		if (!level->isClientSide)
		{
			int chance = 5;

			chance = 5;
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::netherSapling_Id, 1, 0));
			}

			/*chance = 5;
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
			}*/
		}
	}
}
