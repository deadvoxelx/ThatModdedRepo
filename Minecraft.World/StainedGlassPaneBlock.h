#pragma once

#include "ThinFenceTile.h"

class StainedGlassPaneBlock : public ThinFenceTile
{
	friend class ChunkRebuildData;
public:
	static const int BLACK = 0;
	static const int RED = 1;
	static const int GREEN = 2;
	static const int BROWN = 3;
	static const int BLUE = 4;
	static const int PURPLE = 5;
	static const int CYAN = 6;
	static const int LIGHTGRAY = 7;
	static const int GRAY = 8;
	static const int PINK = 9;
	static const int LIME = 10;
	static const int YELLOW = 11;
	static const int LIGHTBLUE = 12;
	static const int MAGENTA = 13;
	static const int ORANGE = 14;
	static const int WHITE = 15;

	static const int STAINED_GLASS_NAMES_LENGTH = 16;

	static const unsigned int STAINED_GLASS_NAMES[STAINED_GLASS_NAMES_LENGTH];

private:
	Icon *iconSide;

public:
	StainedGlassPaneBlock(int id);

	int getSpawnResourcesAuxValue(int data);
	static int getItemAuxValueForBlockData(int data);
	int getRenderLayer();
	void registerIcons(IconRegister *iconRegister);
	Icon *getEdgeTexture(int data);

	virtual int getColor() const;
	virtual int getColor(int auxData);
	virtual int getColor(LevelSource *level, int x, int y, int z);
	virtual int getColor(LevelSource *level, int x, int y, int z, int data);
};
