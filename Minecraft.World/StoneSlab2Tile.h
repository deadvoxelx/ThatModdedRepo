#pragma once
using namespace std;

#include "HalfSlabTile.h"

class ChunkRebuildData;

class StoneSlab2Tile : public HalfSlabTile
{
	friend ChunkRebuildData;
public:
	static const int GOLDENCLIN_SLAB = 0;
	static const int GOLDENCLINBRICK_SLAB = 1;
	static const int NETHERBRICK_MOSSY_SLAB = 2;
	static const int ENDSTONE_SLAB = 3;
	static const int ENDSTONE_MOSSY_SLAB = 4;
	static const int ENDBRICK_SLAB = 5;
	static const int ENDBRICK_MOSSY_SLAB = 6;
	//static const int QUARTZ_SLAB = 7;

	static const int SLAB2_NAMES_LENGTH = 7;

	static const unsigned int SLAB2_NAMES[SLAB2_NAMES_LENGTH];

private:
	Icon *iconSide;

public:
	StoneSlab2Tile(int id, bool fullSize);

	virtual Icon *getTexture(int face, int data);

	void registerIcons(IconRegister *iconRegister);
	virtual int getResource(int data, Random *random, int playerBonusLevel);
	virtual unsigned int getDescriptionId(int iData = -1);
	virtual int getAuxName(int auxValue);
protected:
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);
};