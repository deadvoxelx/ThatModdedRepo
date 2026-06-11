#include "stdafx.h"
#include "StainedGlassBlock.h"
#include "net.minecraft.world.h"
#include "net.minecraft.h"

const unsigned int StainedGlassBlock::STAINED_GLASS_NAMES[STAINED_GLASS_NAMES_LENGTH] = {
	IDS_TILE_STAINED_GLASS_BLACK,
	IDS_TILE_STAINED_GLASS_RED,
	IDS_TILE_STAINED_GLASS_GREEN,
	IDS_TILE_STAINED_GLASS_BROWN,
	IDS_TILE_STAINED_GLASS_BLUE,
	IDS_TILE_STAINED_GLASS_PURPLE,
	IDS_TILE_STAINED_GLASS_CYAN,
	IDS_TILE_STAINED_GLASS_SILVER,
	IDS_TILE_STAINED_GLASS_GRAY,
	IDS_TILE_STAINED_GLASS_PINK,
	IDS_TILE_STAINED_GLASS_LIME,
	IDS_TILE_STAINED_GLASS_YELLOW,
	IDS_TILE_STAINED_GLASS_LIGHT_BLUE,
	IDS_TILE_STAINED_GLASS_MAGENTA,
	IDS_TILE_STAINED_GLASS_ORANGE,
	IDS_TILE_STAINED_GLASS_WHITE,
};

const wstring StainedGlassBlock::TEXTURE_NAMES[] = {
	L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass", L"stainedGlass"
};

StainedGlassBlock::StainedGlassBlock(int id, Material *material) : HalfTransparentTile(id, L"glass", material, false)
{
	icons = nullptr;
}

Icon *StainedGlassBlock::getTexture(int face, int data)
{
	if (data < 0 || data >= STAINED_GLASS_NAMES_LENGTH)
	{
		data = 0;
	}
	return icons[data];
}

void StainedGlassBlock::registerIcons(IconRegister *iconRegister)
{
	icons = new Icon * [STAINED_GLASS_NAMES_LENGTH];

	for (int i = 0; i < STAINED_GLASS_NAMES_LENGTH; i++)
	{
		icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
	}
}

int StainedGlassBlock::getSpawnResourcesAuxValue(int data)
{
	return data;
}

int StainedGlassBlock::getItemAuxValueForBlockData(int data)
{
	return (~data & 0xf);
}

int StainedGlassBlock::getRenderLayer()
{
	return 1;
}

int StainedGlassBlock::getResourceCount(Random *random) 
{
	return 0;
}

bool StainedGlassBlock::isSilkTouchable()
{
	return true;
}

bool StainedGlassBlock::isCubeShaped()
{
	return false;
}

int StainedGlassBlock::getColor() const
{
	return 0xffffff;
}

int StainedGlassBlock::getColor(int auxData)
{
	if (auxData == RED) return 0x993333;
	if (auxData == ORANGE) return 0xd87f33;
	if (auxData == YELLOW) return 0xe5e533;
	if (auxData == LIME) return 0x7fcc19;
	if (auxData == GREEN) return 0x667f33;
	if (auxData == CYAN) return 0x4c7f99;
	if (auxData == LIGHTBLUE) return 0x6699d8;
	if (auxData == BLUE) return 0x334cb2;
	if (auxData == PURPLE) return 0x7f3fb2;
	if (auxData == MAGENTA) return 0xb24cd8;
	if (auxData == PINK) return 0xf27fa5;
	if (auxData == BROWN) return 0x664c33;
	if (auxData == LIGHTGRAY) return 0x999999;
	if (auxData == GRAY) return 0x4c4c4c;
	if (auxData == BLACK) return 0x191919;
	return 0xffffff;
}

int StainedGlassBlock::getColor(LevelSource *level, int x, int y, int z)
{
	return getColor( level, x, y, z, level->getData(x, y, z) );
}

int StainedGlassBlock::getColor(LevelSource *level, int x, int y, int z, int data)
{
	int d = data;
	if (d == RED) return 0x993333;
	if (d == ORANGE) return 0xd87f33;
	if (d == YELLOW) return 0xe5e533;
	if (d == LIME) return 0x7fcc19;
	if (d == GREEN) return 0x667f33;
	if (d == CYAN) return 0x4c7f99;
	if (d == LIGHTBLUE) return 0x6699d8;
	if (d == BLUE) return 0x334cb2;
	if (d == PURPLE) return 0x7f3fb2;
	if (d == MAGENTA) return 0xb24cd8;
	if (d == PINK) return 0xf27fa5;
	if (d == BROWN) return 0x664c33;
	if (d == LIGHTGRAY) return 0x999999;
	if (d == GRAY) return 0x4c4c4c;
	if (d == BLACK) return 0x191919;
	return 0xffffff;
}
