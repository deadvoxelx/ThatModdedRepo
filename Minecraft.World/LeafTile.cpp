#include "stdafx.h"
#include "LeafTile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.h"
#include "..\Minecraft.Client\Minecraft.h"

const unsigned int LeafTile::LEAF_NAMES[LEAF_NAMES_LENGTH] = {
	IDS_TILE_LEAVES_OAK,
	IDS_TILE_LEAVES_SPRUCE,
	IDS_TILE_LEAVES_BIRCH,
	IDS_TILE_LEAVES_JUNGLE,
	IDS_TILE_LEAVES_PURPLE,
};

const wstring LeafTile::TEXTURE_NAMES[] = {
	L"leaves", L"leaves_spruce", L"leaves", L"leaves_jungle", L"leaves_spruce"
};

LeafTile::LeafTile(int id) : TransparentTile(id, Material::leaves, true)
{
	icons = nullptr;
	checkBuffer = nullptr;
	setTicking(true);
}

LeafTile::~LeafTile()
{
	delete [] checkBuffer;
}

int LeafTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Item::stick->id;
}

int LeafTile::getResourceCountForLootBonus(int bonusLevel, Random *random)
{
	return getResourceCount(random) + random->nextInt(bonusLevel + 1);
}

int LeafTile::getResourceCount(Random *random)
{
	return 1 + random->nextInt(2);
}

shared_ptr<ItemInstance> LeafTile::getSilkTouchItemInstance(int data)
{
	return shared_ptr<ItemInstance>(new ItemInstance(Tile::leaves, 1, data));
}

void LeafTile::spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel)
{
	int d = data;
	if (d == NORMAL_LEAF)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::sapling_Id, 1, 0));
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

			chance = 200;
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::apple_Id, 1, 0));
			}
		}
	}
	else if (d == EVERGREEN_LEAF)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::sapling_Id, 1, 1));
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
	else if (d == BIRCH_LEAF)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::sapling_Id, 1, 2));
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
	else if (d == JUNGLE_LEAF)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Tile::sapling_Id, 1, 3));
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
	else if (d == PURPLE_LEAF)
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
				popResource(level, x, y, z, std::make_shared<ItemInstance>(Item::stick_Id, 1, 0));
			}
		}
	}
}

Icon* LeafTile::getTexture(int face, int data)
{
	if (data < 0 || data >= LEAF_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void LeafTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [LEAF_NAMES_LENGTH];

	for (int i = 0; i < LEAF_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}

bool LeafTile::isSolidRender(bool isServerLevel)
{
	if(isServerLevel) return true;
	return !allowSame;
}

int LeafTile::getColor() const
{
	return Minecraft::GetInstance()->getColourTable()->getColor( eMinecraftColour_Foliage_Common );
}

int LeafTile::getColor(int data)
{
	if ((data) == EVERGREEN_LEAF)
	{
		return FoliageColor::getEvergreenColor();
	}
	if ((data) == BIRCH_LEAF)
	{
		return FoliageColor::getBirchColor();
	}
	if ((data) == JUNGLE_LEAF)
	{
		return FoliageColor::getDefaultColor();
	}
	if ((data) == PURPLE_LEAF)
	{
		return FoliageColor::getPurpleSpruceColor();
	}

	return FoliageColor::getDefaultColor();
}

int LeafTile::getColor(LevelSource *level, int x, int y, int z)
{
	return getColor(level, x, y, z, level->getData(x, y, z) );
}

int LeafTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	if ((data) == EVERGREEN_LEAF)
	{
		return FoliageColor::getEvergreenColor();
	}
	if ((data) == BIRCH_LEAF)
	{
		return FoliageColor::getBirchColor();
	}
	if ((data) == JUNGLE_LEAF)
	{
		return FoliageColor::getDefaultColor();
	}
	if ((data) == PURPLE_LEAF)
	{
		return FoliageColor::getPurpleSpruceColor();
	}

	int totalRed = 0;
	int totalGreen = 0;
	int totalBlue = 0;

	for (int oz = -1; oz <= 1; oz++)
	{
		for (int ox = -1; ox <= 1; ox++)
		{
			int foliageColor = level->getBiome(x + ox, z + oz)->getFolageColor();

			totalRed += (foliageColor & 0xff0000) >> 16;
			totalGreen += (foliageColor & 0xff00) >> 8;
			totalBlue += (foliageColor & 0xff);
		}
	}
	return (((totalRed / 9) & 0xFF) << 16) | (((totalGreen / 9) & 0xFF) << 8) | (((totalBlue / 9) & 0xFF));
}

void LeafTile::animateTick(Level *level, int x, int y, int z, Random *random)
{
	if (level->isRainingAt(x, y + 1, z) && !level->isTopSolidBlocking(x, y - 1, z) && random->nextInt(15) == 1)
	{
		double xx = x + random->nextFloat();
		double yy = y - 0.05;
		double zz = z + random->nextFloat();

		level->addParticle(eParticleType_dripWater, xx, yy, zz, 0, 0, 0);
	}
}

void LeafTile::onRemove(Level *level, int x, int y, int z, int id, int data)
{
	int r = 1;
	int r2 = r + 1;

	if (level->hasChunksAt(x - r2, y - r2, z - r2, x + r2, y + r2, z + r2))
	{
		for (int xo = -r; xo <= r; xo++)
			for (int yo = -r; yo <= r; yo++)
				for (int zo = -r; zo <= r; zo++)
				{
					int t = level->getTile(x + xo, y + yo, z + zo);
					if (t == Tile::leaves_Id)
					{
						int currentData = level->getData(x + xo, y + yo, z + zo);
						level->setData(x + xo, y + yo, z + zo, currentData, Tile::UPDATE_NONE);
					}
				}
	}

}

void LeafTile::tick(Level *level, int x, int y, int z, Random *random)
{
	if (level->isClientSide) return;

	int currentData = level->getData(x, y, z);
	//if ((currentData & UPDATE_LEAF_BIT) != 0 && (currentData & PERSISTENT_LEAF_BIT) == 0)
	{
		int r = REQUIRED_WOOD_RANGE;
		int r2 = r + 1;

		int W = 32;
		int WW = W * W;
		int WO = W / 2;
		if (checkBuffer == nullptr)
		{
			checkBuffer = new int[W * W * W];
		}

		if (level->hasChunksAt(x - r2, y - r2, z - r2, x + r2, y + r2, z + r2))
		{
			// 4J Stu - Assuming we remain in the same chunk, getTile accesses an array that varies least by y
			// Changing the ordering here to loop by y last
			for (int xo = -r; xo <= r; xo++)
				for (int zo = -r; zo <= r; zo++)
					for (int yo = -r; yo <= r; yo++)
					{
						int t = level->getTile(x + xo, y + yo, z + zo);
						if (t == Tile::treeTrunk_Id || t == Tile::grass_Id || t == Tile::dirt_Id)
						{
							checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] = 0;
						}
						else if (t == Tile::leaves_Id)
						{
							checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] = -2;
						}
						else
						{
							checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] = -1;
						}
					}
					for (int i = 1; i <= REQUIRED_WOOD_RANGE; i++)
					{
						for (int xo = -r; xo <= r; xo++)
							for (int yo = -r; yo <= r; yo++)
								for (int zo = -r; zo <= r; zo++)
								{
									if (checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO)] == i - 1)
									{
										if (checkBuffer[(xo + WO - 1) * WW + (yo + WO) * W + (zo + WO)] == -2)
										{
											checkBuffer[(xo + WO - 1) * WW + (yo + WO) * W + (zo + WO)] = i;
										}
										if (checkBuffer[(xo + WO + 1) * WW + (yo + WO) * W + (zo + WO)] == -2)
										{
											checkBuffer[(xo + WO + 1) * WW + (yo + WO) * W + (zo + WO)] = i;
										}
										if (checkBuffer[(xo + WO) * WW + (yo + WO - 1) * W + (zo + WO)] == -2)
										{
											checkBuffer[(xo + WO) * WW + (yo + WO - 1) * W + (zo + WO)] = i;
										}
										if (checkBuffer[(xo + WO) * WW + (yo + WO + 1) * W + (zo + WO)] == -2)
										{
											checkBuffer[(xo + WO) * WW + (yo + WO + 1) * W + (zo + WO)] = i;
										}
										if (checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO - 1)] == -2)
										{
											checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO - 1)] = i;
										}
										if (checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO + 1)] == -2)
										{
											checkBuffer[(xo + WO) * WW + (yo + WO) * W + (zo + WO + 1)] = i;
										}
									}
								}
					}
		}

		int mid = checkBuffer[(WO) * WW + (WO) * W + (WO)];
		if (mid >= 0)
		{
			level->setData(x, y, z, currentData, Tile::UPDATE_NONE);
		}
		else
		{
			die(level, x, y, z);
		}
	}
}

void LeafTile::die(Level *level, int x, int y, int z)
{
	Tile::spawnResources(level, x, y, z, level->getData(x, y, z), 0);
	level->removeTile(x, y, z);
}
