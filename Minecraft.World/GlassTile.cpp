#include "stdafx.h"
#include "GlassTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int GlassTile::GLASS_NAMES[GLASS_NAMES_LENGTH] = {
	IDS_TILE_GLASS,
	IDS_TILE_GLASS_SOUL,
	IDS_TILE_GLASS_QUICKSOIL,

};

const wstring GlassTile::TEXTURE_NAMES[] = {
	L"glass", L"glass", L"quicksoilGlass"
};

GlassTile::GlassTile(int id, Material *material, bool allowSame) : HalfTransparentTile(id, L"glass", material, allowSame)
{
	icons = nullptr;
}

int GlassTile::getResourceCount(Random *random)
{
	return 0;
}

int GlassTile::getRenderLayer()
{
	return 1;
}

bool GlassTile::shouldRenderFace(LevelSource *level, int x, int y, int z, int face)
{
	return HalfTransparentTile::shouldRenderFace(level, x, y, z, 1 - face);
}

bool GlassTile::isSolidRender()
{
	return false;
}

bool GlassTile::isCubeShaped()
{
	return false;
}

bool GlassTile::isSilkTouchable()
{
	return true;
}

Icon* GlassTile::getTexture(int face, int data)
{
	if (data < 0 || data >= GLASS_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void GlassTile::registerIcons(IconRegister* iconRegister)
{
	icons = new Icon * [GLASS_NAMES_LENGTH];

	for (int i = 0; i < GLASS_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}
