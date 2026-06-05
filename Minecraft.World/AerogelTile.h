#pragma once
#include "HalfTransparentTile.h"

class Random;

class AerogelTile : public HalfTransparentTile
{
public:
	AerogelTile(int id);
	virtual int getRenderLayer();
	virtual bool shouldRenderFace(LevelSource *level, int x, int y, int z, int face);
};
