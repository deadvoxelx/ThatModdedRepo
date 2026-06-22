#pragma once
#include "Tile.h"

class NetherrackTile : public Tile
{
public:
	NetherrackTile(int id);

protected:
	virtual bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false);
};
