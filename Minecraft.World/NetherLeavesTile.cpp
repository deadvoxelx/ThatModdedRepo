#include "stdafx.h"
#include "NetherLeavesTile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int NetherLeavesTile::NETHER_LEAVES_NAMES[NETHER_LEAVES_NAMES_LENGTH] = {
	IDS_TILE_NETHERLEAVES,
	IDS_TILE_SKYROOT_LEAVES,
	IDS_TILE_GOLDENOAK_LEAVES,

};

const wstring NetherLeavesTile::TEXTURE_NAMES[] = {
	L"nether_leaves", L"skyrootLeaves", L"goldenOakLeaves"
};

NetherLeavesTile::NetherLeavesTile(int id) : TransparentTile(id, Material::leaves, true)
{
	icons = nullptr;
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
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::netherLeaves, 1, data));
}

void NetherLeavesTile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
{
	int d = data;
	if (d == TYPE_DEFAULT)
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
	else if (d == TYPE_SKYROOT)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::skyrootSapling_Id, 1, 0));
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
	else if (d == TYPE_GOLDENOAK)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::goldenOakSapling_Id, 1, 0));
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
}

Icon* NetherLeavesTile::getTexture(int face, int data)
{
	if (data < 0 || data >= NETHER_LEAVES_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void NetherLeavesTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [NETHER_LEAVES_NAMES_LENGTH];

	for (int i = 0; i < NETHER_LEAVES_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
