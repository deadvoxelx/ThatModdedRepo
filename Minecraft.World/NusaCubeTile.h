#pragma once

#include "Tile.h"

class NusaCubeTile : public Tile
{
private:
	Icon *icon;
	Icon *iconTop;

public:
	NusaCubeTile(int id, Material *material);

	int getRenderShape();
	virtual void updateDefaultShape();
	bool isCubeShaped();
	bool isSolidRender(bool isServerLevel = false);
	AABB *getAABB(Level *level, int x, int y, int z);
	bool shouldRenderFace(LevelSource *level, int x, int y, int z, int face);
	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);
};