#pragma once
#include "HeavyTile.h"

class Random;

class SandTile : public HeavyTile
{
public:
	SandTile(int type);

	virtual int getColor() const;
	virtual int getColor(int auxData);
	virtual int getColor(LevelSource *level, int x, int y, int z);
	virtual int getColor(LevelSource *level, int x, int y, int z, int data);
};
