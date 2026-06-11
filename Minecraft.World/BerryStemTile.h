#pragma once
#include "Bush.h"

class BerryStemTile : public Bush
{
	friend class Tile;
public:
	static const int AGE_BIT = 8;

protected:
	BerryStemTile(int id);

public:
	virtual void tick(Level *level, int x, int y, int z, Random *random);

	void growTree(Level *level, int x, int y, int z, Random *random);
};