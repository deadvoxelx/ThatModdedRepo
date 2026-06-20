#pragma once
#include "Bush.h"

class Random;

class FlowerTile : public Bush
{
	friend class Tile;
public:
	static const int YELLOW = 0;
	static const int WHITE = 1;
	static const int PURPLE = 2;

	static const int FLOWER_NAMES_LENGTH = 3;

	static const unsigned int FLOWER_NAMES[FLOWER_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	Icon** icons;

protected:
	FlowerTile(int id);

public:
    virtual void updateDefaultShape(); // 4J Added override
	virtual bool canSurvive(Level *level, int x, int y, int z);
	virtual bool mayPlace(Level *level, int x, int y, int z);

	virtual Icon* getTexture(int face, int data);
	void registerIcons(IconRegister* iconRegister);
};
