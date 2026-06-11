#pragma once
#include "Tile.h"

class Player;

class QuicksoilTile : public Tile
{
	friend class Tile;

public:
	static const int TYPE_NATURAL = 0;
	static const int TYPE_UNNATURAL = 1;

	static const int QUICKSOIL_NAMES_LENGTH = 2;

	static const unsigned int QUICKSOIL_NAMES[QUICKSOIL_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];
private:
	Icon** icons;

protected:
	QuicksoilTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual void stepOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity);
	virtual int getResource(int data, Random *random, int playerBonusLevel);
	virtual void spawnResources(Level *level, int x, int y, int z, int data, float odds, int playerBonusLevel);
	virtual Icon* getTexture(int face, int data);
	void registerIcons(IconRegister* iconRegister);
};
