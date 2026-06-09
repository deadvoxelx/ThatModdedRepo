#pragma once
#include "Tile.h"

class Random;
class Level;

class IcestoneTile : public Tile
{
public:
	IcestoneTile(int id);

	virtual void onPlace(Level *level, int x, int y, int z);
};