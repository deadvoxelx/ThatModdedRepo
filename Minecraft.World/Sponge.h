#pragma once

#include "Tile.h"

class Sponge : public Tile
{
	friend class Tile;
public:
	static const int RANGE = 2; // Voxel - this isnt used anymore, but leaving it anyway

	Sponge(int id);
	virtual void onPlace(Level *level, int x, int y, int z);
	virtual void tick(Level *level, int x, int y, int z, Random *random) override;
};
