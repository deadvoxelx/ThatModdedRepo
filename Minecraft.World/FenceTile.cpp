#include "stdafx.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"
#include "FenceTile.h"

const unsigned int FenceTile::FENCE_NAMES[7] = { 
	IDS_TILE_OAK_FENCE,
	IDS_TILE_SPRUCE_FENCE,
	IDS_TILE_BIRCH_FENCE,
	IDS_TILE_JUNGLE_FENCE,
	IDS_TILE_NETHER_FENCE,
	IDS_TILE_PURUL_FENCE,
	IDS_TILE_SKYROOT_FENCE,
};

Icon *FenceTile::getTexture(int face, int data)
{
	if (data == TYPE_SPRUCE)
	{
		return Tile::wood->getTexture(face, TreeTile::DARK_TRUNK);
	}
	if (data == TYPE_BIRCH)
	{
		return Tile::wood->getTexture(face, TreeTile::BIRCH_TRUNK);
	}
	if (data == TYPE_JUNGLE)
	{
		return Tile::wood->getTexture(face, TreeTile::JUNGLE_TRUNK);
	}
	if (data == TYPE_NETHER)
	{
		return Tile::netherPlanks->getTexture(face, 0);
	}
	if (data == TYPE_PURUL)
	{
		return Tile::netherPlanks->getTexture(face, 1);
	}
	if (data == TYPE_SKYROOT)
	{
		return Tile::netherPlanks->getTexture(face, 2);
	}
	return Tile::wood->getTexture(face, 0);
}

FenceTile::FenceTile(int id, Tile *baseTile) : Tile(id, baseTile->material, isSolidRender())
{
	//this->texture = texture;
}

void FenceTile::addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source)
{
	bool n = connectsTo(level, x, y, z - 1);
	bool s = connectsTo(level, x, y, z + 1);
	bool w = connectsTo(level, x - 1, y, z);
	bool e = connectsTo(level, x + 1, y, z);

	float west = 6.0f / 16.0f;
	float east = 10.0f / 16.0f;
	float north = 6.0f / 16.0f;
	float south = 10.0f / 16.0f;

	if (n)
	{
		north = 0;
	}
	if (s)
	{
		south = 1;
	}
	if (n || s)
	{
		setShape(west, 0, north, east, 1.5f, south);
		Tile::addAABBs(level, x, y, z, box, boxes, source);
	}
	north = 6.0f / 16.0f;
	south = 10.0f / 16.0f;
	if (w)
	{
		west = 0;
	}
	if (e)
	{
		east = 1;
	}
	if (w || e || (!n && !s))
	{
		setShape(west, 0, north, east, 1.5f, south);
		Tile::addAABBs(level, x, y, z, box, boxes, source);
	}

	if (n)
	{
		north = 0;
	}
	if (s)
	{
		south = 1;
	}

	setShape(west, 0, north, east, 1.0f, south);
}

void FenceTile::updateShape(LevelSource *level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity) // 4J added forceData, forceEntity param
{
	bool n = connectsTo(level, x, y, z - 1);
	bool s = connectsTo(level, x, y, z + 1);
	bool w = connectsTo(level, x - 1, y, z);
	bool e = connectsTo(level, x + 1, y, z);

	float west = 6.0f / 16.0f;
	float east = 10.0f / 16.0f;
	float north = 6.0f / 16.0f;
	float south = 10.0f / 16.0f;

	if (n)
	{
		north = 0;
	}
	if (s)
	{
		south = 1;
	}
	if (w)
	{
		west = 0;
	}
	if (e)
	{
		east = 1;
	}

	setShape(west, 0, north, east, 1.0f, south);
}

bool FenceTile::isSolidRender(bool isServerLevel)
{
	return false;
}

bool FenceTile::isCubeShaped()
{
	return false;
}

bool FenceTile::isPathfindable(LevelSource *level, int x, int y, int z)
{
	return false;
}

int FenceTile::getRenderShape()
{
	return Tile::SHAPE_FENCE;
}

bool FenceTile::connectsTo(LevelSource *level, int x, int y, int z)
{
	int tile = level->getTile(x, y, z);
	Tile* tileInstance = Tile::tiles[tile];

	if (tileInstance == nullptr)
		return false;

	FenceTile* asFence = dynamic_cast<FenceTile*>(tileInstance);

	if (asFence && asFence->material == this->material)
		return true;

	if (dynamic_cast<FenceGateTile*>(tileInstance))
		return true;

	if (tileInstance->material->isSolidBlocking() && tileInstance->isCubeShaped())
		return tileInstance->material != Material::vegetable;

	return false;
}

bool FenceTile::isFence(int tile)
{
	return tile == Tile::fence_Id || tile == Tile::netherFence_Id;
}

void FenceTile::registerIcons(IconRegister *iconRegister)
{
	//icon = iconRegister->registerIcon(texture);
}

bool FenceTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	if (face == Facing::DOWN)
	{
		return Tile::shouldRenderFace(level, x, y, z, face);
	}
	return true;
}

bool FenceTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly)
{
	if (level->isClientSide) return true;
	if (LeashItem::bindPlayerMobs(player, level, x, y, z))
	{
		return true;
	}
	return false;
}

int FenceTile::getSpawnResourcesAuxValue(int data)
{
	return data;
}
