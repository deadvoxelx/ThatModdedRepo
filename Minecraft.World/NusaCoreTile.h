#pragma once

#include "Tile.h"

class Nusagar;

class NusaCoreTile : public Tile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_ACTIVE = 1;

	static const int CORE_NAMES_LENGTH = 2;

	static const unsigned int CORE_NAMES[CORE_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	static const int CORE_ACTIVE_CYCLES = 5;
	static const int CORE_CYCLE_DELAY = 80;

	Icon** icons;
	Icon *iconTop;

public:
	NusaCoreTile(int id, Material *material);

	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);
	virtual void tick(Level *level, int x, int y, int z, Random *random);
	virtual int getTickDelay(Level *level) override;

protected:
	virtual bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false);
};
