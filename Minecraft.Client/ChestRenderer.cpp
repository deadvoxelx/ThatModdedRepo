#include "stdafx.h"
#include "ChestRenderer.h"
#include "ChestModel.h"
#include "LargeChestModel.h"
#include "ModelPart.h"
#include "../Minecraft.World/net.minecraft.world.level.tile.entity.h"
#include "../Minecraft.World/net.minecraft.world.level.tile.h"
#include "../Minecraft.World/Calendar.h"

ResourceLocation ChestRenderer::CHEST_LARGE_TRAP_LOCATION = ResourceLocation(TN_TILE_LARGE_TRAP_CHEST);
//ResourceLocation ChestRenderer::CHEST_LARGE_XMAS_LOCATION = ResourceLocation(TN_TILE_LARGE_XMAS_CHEST);
ResourceLocation ChestRenderer::CHEST_LARGE_LOCATION = ResourceLocation(TN_TILE_LARGE_CHEST);
ResourceLocation ChestRenderer::CHEST_TRAP_LOCATION = ResourceLocation(TN_TILE_TRAP_CHEST);
//ResourceLocation ChestRenderer::CHEST_XMAS_LOCATION = ResourceLocation(TN_TILE_XMAS_CHEST);
ResourceLocation ChestRenderer::CHEST_LOCATION = ResourceLocation(TN_TILE_CHEST);

ChestRenderer::ChestRenderer() : TileEntityRenderer()
{
	chestModel = new ChestModel();
	largeChestModel = new LargeChestModel();

	xmasTextures = false;

	// 4J Stu - Disable this
#if 0
	if (Calendar::GetMonth() + 1 == 12 && Calendar::GetDayOfMonth() >= 24 && Calendar::GetDayOfMonth() <= 26)
	{
		xmasTextures = true;
	}
#endif
}

ChestRenderer::~ChestRenderer()
{
	delete chestModel;
	delete largeChestModel;
}

void ChestRenderer::render(shared_ptr<TileEntity>  _chest, double x, double y, double z, float a, bool setColor, float alpha, bool useCompiled)
{
	// 4J Convert as we aren't using a templated class
	shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity>(_chest);

	int data;

	if (!chest->hasLevel())
	{
		data = 0;
	}
	else
	{
		Tile *tile = chest->getTile();
		data = chest->getData();

		if (dynamic_cast<ChestTile*>(tile) != nullptr && data == 0)
		{
			static_cast<ChestTile *>(tile)->recalcLockDir(chest->getLevel(), chest->x, chest->y, chest->z);
			data = chest->getData();
		}

		chest->checkNeighbors();
	}

	// Fireblade - get potential cached "neighboring" chests
	shared_ptr<ChestTileEntity> n = chest->n.lock();
	shared_ptr<ChestTileEntity> s = chest->s.lock();
	shared_ptr<ChestTileEntity> w = chest->w.lock();
	shared_ptr<ChestTileEntity> e = chest->e.lock();

	// Fireblade - re-check validity of neighboring chests
	if (n != nullptr) n->checkNeighbors();
	if (s != nullptr) s->checkNeighbors();
	if (w != nullptr) w->checkNeighbors();
	if (e != nullptr) e->checkNeighbors();

	// Fireblade - check if pairing is valid
	bool pairN = n != nullptr && n->s.lock() == chest;
	bool pairS = s != nullptr && s->n.lock() == chest;
	bool pairW = w != nullptr && w->e.lock() == chest;
	bool pairE = e != nullptr && e->w.lock() == chest;

	// Fireblade - prevent unnecessary rendering of chest pairs
	if (pairN || pairW) return;

	ChestModel *model;
	if (pairE || pairS)
	{
		model = largeChestModel;

		if (chest->getType() == ChestTile::TYPE_TRAP)
		{
			bindTexture(&CHEST_LARGE_TRAP_LOCATION);
		}
		//else if (xmasTextures)
		//{
		//	bindTexture(&CHEST_LARGE_XMAS_LOCATION);
		//}
		else
		{
			bindTexture(&CHEST_LARGE_LOCATION);
		}
	}
	else
	{
		model = chestModel;
		if (chest->getType() == ChestTile::TYPE_TRAP)
		{
			bindTexture(&CHEST_TRAP_LOCATION);
		}
		//else if (xmasTextures)
		//{
		//	bindTexture(&CHEST_XMAS_LOCATION);
		//}
		else
		{
			bindTexture(&CHEST_LOCATION);
		}
	}

	glPushMatrix();
	glEnable(GL_RESCALE_NORMAL);
	//if( setColor ) glColor4f(1, 1, 1, 1);
	if( setColor ) glColor4f(1, 1, 1, alpha);
	glTranslatef(static_cast<float>(x), static_cast<float>(y) + 1, static_cast<float>(z) + 1);
	glScalef(1, -1, -1);

	glTranslatef(0.5f, 0.5f, 0.5f);
	int rot = 0;
	if (data == 2) rot = 180;
	if (data == 3) rot = 0;
	if (data == 4) rot = 90;
	if (data == 5) rot = -90;

	if (data == 2 && pairE)
	{
		glTranslatef(1, 0, 0);
	}
	if (data == 5 && pairS)
	{
		glTranslatef(0, 0, -1);
	}
	glRotatef(rot, 0, 1, 0);
	glTranslatef(-0.5f, -0.5f, -0.5f);

	float open = chest->oOpenness + (chest->openness - chest->oOpenness) * a;
	if (pairN)
	{
		float open2 = n->oOpenness + (n->openness - n->oOpenness) * a;
		if (open2 > open) open = open2;
	}
	if (pairW)
	{
		float open2 = w->oOpenness + (w->openness - w->oOpenness) * a;
		if (open2 > open) open = open2;
	}

	open = 1 - open;
	open = 1 - open * open * open;

	model->lid->xRot = -(open * PI / 2);
	model->render(useCompiled);
	glDisable(GL_RESCALE_NORMAL);
	glPopMatrix();
	if( setColor ) glColor4f(1, 1, 1, 1);
}
