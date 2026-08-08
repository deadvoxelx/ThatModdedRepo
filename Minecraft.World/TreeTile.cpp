#include "stdafx.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.piston.h"
#include "net.minecraft.h"
#include "net.minecraft.world.h"
#include "LeafTile.h"

#include "TreeTile.h"

const unsigned int TreeTile::TREE_NAMES[ TreeTile::TREE_NAMES_LENGTH] =
{
	IDS_TILE_LOG_OAK,
	IDS_TILE_LOG_SPRUCE,
	IDS_TILE_LOG_BIRCH,
	IDS_TILE_LOG_JUNGLE
};

const wstring  TreeTile::TREE_STRING_NAMES[ TreeTile::TREE_NAMES_LENGTH] = {L"oak", L"spruce", L"birch", L"jungle"};

const wstring TreeTile::TREE_TEXTURES[] = {L"tree_side", L"tree_spruce", L"tree_birch", L"tree_jungle"};

TreeTile::TreeTile(int id) : RotatedPillarTile(id, Material::wood)
{
}

int TreeTile::getResourceCount(Random *random)
{
	return 1;
}

int TreeTile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Tile::treeTrunk_Id;
}

void TreeTile::onRemove(Level *level, int x, int y, int z, int id, int data)
{
	int r = LeafTile::REQUIRED_WOOD_RANGE;
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
						//if ((currentData & LeafTile::UPDATE_LEAF_BIT) == 0)
						{
							level->setData(x + xo, y + yo, z + zo, currentData, Tile::UPDATE_NONE);
						}
					}
				}
	}
}


unsigned int TreeTile::getDescriptionId(int iData /*= -1*/)
{
	int type = iData & MASK_TYPE;
	if(type < 0 ) type = 0;
	return TreeTile::TREE_NAMES[type];
}

Icon *TreeTile::getTypeTexture(int type)
{
	return icons_side[type];
}

Icon *TreeTile::getTopTexture(int type)
{
	return icons_top[type];
}

int TreeTile::getWoodType(int data)
{
	return data & MASK_TYPE;
}

void TreeTile::playerWillDestroy(Level *level, int x, int y, int z, int data, shared_ptr<Player> player)
{
	Tile::playerWillDestroy(level, x, y, z, data, player);

	chopTreeColumn(level, x, y, z, player, getWoodType(data));
}

bool TreeTile::isHoldingAxe(shared_ptr<Player> player)
{
	if (player == nullptr) return false;

	shared_ptr<ItemInstance> held = player->getSelectedItem();
	if (held == nullptr || held->getItem() == nullptr) return false;
	return held->getItem()->getBaseItemType() == Item::eBaseItemType_hatchet;
}

void TreeTile::chopTreeColumn(Level *level, int x, int y, int z, shared_ptr<Player> player, int woodType)
{
	if (level->isClientSide || player == nullptr || player->abilities.instabuild) return;
	if (!isHoldingAxe(player)) return;

	Tile *logTile = Tile::tiles[level->getTile(x, y, z)];
	if (logTile == nullptr) return;

	shared_ptr<ItemInstance> held = player->getSelectedItem();

	for (int yy = y + 1; yy < Level::maxBuildHeight; yy++)
	{
		if (level->getTile(x, yy, z) != logTile->id) break;
		int upData = level->getData(x, yy, z);
		if (woodType >= 0 && (upData & MASK_TYPE) != woodType) break;

		level->levelEvent(player, LevelEvent::PARTICLES_DESTROY_BLOCK, x, yy, z, logTile->id + (upData << Tile::TILE_NUM_SHIFT));
		logTile->playerDestroy(level, player, x, yy, z, upData);
		level->removeTile(x, yy, z);

		if (held != nullptr)
		{
			held->mineBlock(level, logTile->id, x, yy, z, player);
			if (held->count == 0)
			{
				player->removeSelectedItem();
				break;
			}
		}
	}
}

shared_ptr<ItemInstance> TreeTile::getSilkTouchItemInstance(int data)
{
	// fix to avoid getting silktouched sideways logs
	return std::make_shared<ItemInstance>(id, 1, getWoodType(data));
}

void TreeTile::registerIcons(IconRegister *iconRegister)
{
	for (int i = 0; i < TREE_NAMES_LENGTH; i++)
	{
		icons_side[i] = iconRegister->registerIcon(getIconName() + L"_" + TREE_STRING_NAMES[i]);
		icons_top[i] = iconRegister->registerIcon(getIconName() + L"_" + TREE_STRING_NAMES[i] + L"_top");
	}
}
