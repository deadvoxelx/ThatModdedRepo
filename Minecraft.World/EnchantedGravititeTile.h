#pragma once
#include "Tile.h"

class EnchantedGravititeTile : public Tile
{
public:
	EnchantedGravititeTile(int id);

	virtual int getColor() const;
	virtual int getColor(int auxData);
	virtual int getColor(LevelSource *level, int x, int y, int z);
	virtual int getColor(LevelSource *level, int x, int y, int z, int data);
};