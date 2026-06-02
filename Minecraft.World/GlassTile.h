#pragma once
#include "HalfTransparentTile.h"

class Random;

class GlassTile : public HalfTransparentTile
{
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_SOUL = 1;
	static const int TYPE_QUICKSOIL = 2;

	static const int GLASS_NAMES_LENGTH = 3;

	static const unsigned int GLASS_NAMES[GLASS_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	Icon** icons;

public:
	using HalfTransparentTile::isSolidRender;

	GlassTile(int id, Material *material, bool allowSame);
	virtual int getResourceCount(Random *random);
	virtual int getRenderLayer();
	virtual bool shouldRenderFace(LevelSource *level, int x, int y, int z, int face);
	virtual bool isSolidRender();
	virtual bool isCubeShaped();
	virtual bool isSilkTouchable();

	virtual Icon* getTexture(int face, int data);
	void registerIcons(IconRegister* iconRegister);
};
