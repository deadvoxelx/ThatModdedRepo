#include "stdafx.h"
#include "net.minecraft.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.h"
#include "StoneSlab2Tile.h"


const unsigned int StoneSlab2Tile::SLAB2_NAMES[SLAB2_NAMES_LENGTH] = {	
	IDS_TILE_GOLDENCLIN_SLAB,
	IDS_TILE_GOLDENCLIN_BRICK_SLAB,
	IDS_TILE_NETHERBRICK_MOSSY_SLAB,
	IDS_TILE_ENDSTONE_SLAB,
	IDS_TILE_ENDSTONE_MOSSY_SLAB,
	IDS_TILE_ENDBRICK_SLAB,
	IDS_TILE_ENDBRICK_MOSSY_SLAB,
	//IDS_TILE_STONESLAB_QUARTZ,
};

StoneSlab2Tile::StoneSlab2Tile(int id, bool fullSize) : HalfSlabTile(id, fullSize, Material::stone)
{
}

Icon *StoneSlab2Tile::getTexture(int face, int data)
{
	int type = data & TYPE_MASK;
	if (fullSize && (data & TOP_SLOT_BIT) != 0)
	{
		face = Facing::UP;
	}
	if (data == GOLDENCLIN_SLAB)
	{
		return Tile::goldenclin->getTexture(face);
	}
	if (data == GOLDENCLINBRICK_SLAB)
	{
		return Tile::goldenclinBricks->getTexture(face);
	}
	if (data == NETHERBRICK_MOSSY_SLAB)
	{
		return Tile::netherBricksMossy->getTexture(face);
	}
	if (data == ENDSTONE_SLAB)
	{
		return Tile::endStone->getTexture(face);
	}
	if (data == ENDSTONE_MOSSY_SLAB)
	{
		return Tile::endStoneMossy->getTexture(face);
	}
	if (data == ENDBRICK_SLAB)
	{
		return Tile::endBricks->getTexture(face);
	}
	if (data == ENDBRICK_MOSSY_SLAB)
	{
		return Tile::endBricksMossy->getTexture(face);
	}
	//if (data == QUARTZ_SLAB)
	//{
	//	return Tile::quartzBlock->getTexture(face);
	//}
	
	return icon;
}

void StoneSlab2Tile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(L"stoneslab_top");
	iconSide = iconRegister->registerIcon(L"stoneslab_side");
}

int StoneSlab2Tile::getResource(int data, Random *random, int playerBonusLevel)
{
	return Tile::stoneSlabHalf_Id;
}

unsigned int StoneSlab2Tile::getDescriptionId(int iData /*= -1*/)
{
	if(iData < 0 ) iData = 0;
	return StoneSlab2Tile::SLAB2_NAMES[iData];
}

int StoneSlab2Tile::getAuxName(int auxValue) 
{
	if (auxValue < 0 || auxValue >= SLAB2_NAMES_LENGTH) 
	{
		auxValue = 0;
	}
	return SLAB2_NAMES[auxValue];
}

shared_ptr<ItemInstance> StoneSlab2Tile::getSilkTouchItemInstance(int data)
{
	return std::make_shared<ItemInstance>(Tile::stoneSlab2Half_Id, 2, data & TYPE_MASK);
}
