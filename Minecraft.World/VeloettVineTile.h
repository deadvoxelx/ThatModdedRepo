#pragma once
#include "Bush.h"

class Random;
class Level;

class VeloettVineTile : public Bush
{
	friend class Tile;

protected:
	VeloettVineTile(int id);

public:
	virtual void updateDefaultShape();
	virtual bool canSurvive(Level *level, int x, int y, int z);
	virtual bool mayPlace(Level *level, int x, int y, int z);
	virtual void neighborChanged(Level *level, int x, int y, int z, int type);
};
