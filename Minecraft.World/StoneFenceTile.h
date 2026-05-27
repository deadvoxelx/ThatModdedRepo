#pragma once
#include "Tile.h"
#include "Definitions.h"

class StoneFenceTile : public Tile
{
private:
	wstring texture;

public:
	/*static const int TYPE_NORMAL = 0;
	static const int TYPE_SPRUCE = 1;
	static const int TYPE_BIRCH = 2;
	static const int TYPE_JUNGLE = 3;
	static const int TYPE_NETHER = 4;
	static const int TYPE_PURUL = 5;

	static const unsigned int FENCE_NAMES[6];*/

	StoneFenceTile(int id, Tile *baseTile);
	Icon *getTexture(int face, int data);
	virtual void addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source);
	virtual void updateShape(LevelSource *level, int x, int y, int z, int forceData = -1, shared_ptr<TileEntity> forceEntity = shared_ptr<TileEntity>()); // 4J added forceData, forceEntity param
	virtual bool isSolidRender(bool isServerLevel = false);
	virtual bool isCubeShaped();
	virtual bool isPathfindable(LevelSource *level, int x, int y, int z);
	virtual bool shouldRenderFace(LevelSource *level, int x, int y, int z, int face);
	virtual int getRenderShape();
	virtual bool connectsTo(LevelSource *level, int x, int y, int z);
	static bool isFence(int tile);
	virtual void registerIcons(IconRegister *iconRegister);
	virtual bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false);
	//int getSpawnResourcesAuxValue(int data);
};