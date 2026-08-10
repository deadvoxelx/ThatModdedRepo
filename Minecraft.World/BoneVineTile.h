#pragma once
#include "HangingPlantTile.h"

class Random;
class Level;

class BoneVineTile : public HangingPlant
{
	friend class Tile;

protected:
	BoneVineTile(int id);

public:
	virtual bool canSurvive(Level *level, int x, int y, int z);
	virtual bool mayPlace(Level *level, int x, int y, int z);
    virtual void updateDefaultShape();
};
