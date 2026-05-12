#pragma once

#include "Tile.h"

class WallTile : public Tile
{
public:
	static const float WALL_WIDTH;
	static const float WALL_HEIGHT;
	static const float POST_WIDTH;
	static const float POST_HEIGHT;

	static const int TYPE_NORMAL = 0;
	static const int TYPE_MOSSY = 1;
	static const int TYPE_STONEBRICK = 2;
	static const int TYPE_GOLDENCLIN = 3;
	static const int TYPE_GOLDENCLINBRICK = 4;
	static const int TYPE_QUARTZ = 5;
	static const int TYPE_BRICK = 6;
	static const int TYPE_NETHERBRICK = 7;
	static const int TYPE_NETHERBRICK_MOSSY = 8;
	static const int TYPE_ENDSTONE = 9;
	static const int TYPE_ENDSTONE_MOSSY = 10;
	static const int TYPE_ENDBRICK = 11;
	static const int TYPE_ENDBRICK_MOSSY = 12;

	static const unsigned int COBBLE_NAMES[13];

	WallTile(int id, Tile *baseTile);

	Icon *getTexture(int face, int data);
	int getRenderShape();
	bool isCubeShaped();
	bool isPathfindable(LevelSource *level, int x, int y, int z);
	bool isSolidRender(bool isServerLevel = false);
	void updateShape(LevelSource *level, int x, int y, int z, int forceData = -1, shared_ptr<TileEntity> forceEntity = shared_ptr<TileEntity>());
	AABB *getAABB(Level *level, int x, int y, int z);
	bool connectsTo(LevelSource *level, int x, int y, int z);
	int getSpawnResourcesAuxValue(int data);
	bool shouldRenderFace(LevelSource *level, int x, int y, int z, int face);
	void registerIcons(IconRegister *iconRegister);
};
