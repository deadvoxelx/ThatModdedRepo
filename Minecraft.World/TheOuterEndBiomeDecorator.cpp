#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "TheOuterEndBiomeDecorator.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "EndGatewayFeature.h"
#include "EndactusFeature.h"
#include "EndesertBushFeature.h"
#include "VeloettShrubFeature.h"
#include "VeloettFlowerFeature.h"
#include "PurulTreeFeature.h"
#include "VeloettVineFeature.h"
#include "EndTowerFeature.h"

const double M_PI = 3.14159265358979323846;

TheOuterEndBiomeDecorator::TheOuterEndBiomeDecorator(Biome *biome) : BiomeDecorator(biome)
{
	endGatewayFeature = new EndGatewayFeature(Tile::unbreakable_Id);
	endesertBushFeature = new EndesertBushFeature();
	veloettShrubFeature = new VeloettShrubFeature();
	veloettFlowerFeature = new VeloettFlowerFeature();

	endTowerFeature = new EndTowerFeature(Tile::endStone_Id);
}

void TheOuterEndBiomeDecorator::decorate()
{
	decorateOres();

	int y = level->getTopSolidBlock(xo+8, zo+8);
	if(y>level->GetHighestY()) level->SetHighestY(y);

	if (xo == 0 && zo == 0)
	{
		for (int i = 0; i < 20; i++)
		{
			
			double angle = (2 * M_PI / 20) * i;

			int gx = (int)(96 * cos(angle));
			int gz = (int)(96 * sin(angle));


			endGatewayFeature->place(level, random, gx, 75, gz);
		}
	}

	PIXBeginNamedEvent(0,"End structures");
	for (int i = 0; i < 1; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		endTowerFeature->place(level, random, x, y, z);
	}
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0,"Decorate end foliage");
	for (int i = 0; i < 192; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		PurulTreeFeature().place(level, random, x, y, z);
	}

	for (int i = 0; i < 32; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		VeloettVineFeature *veloettVineFeature = new VeloettVineFeature();
		veloettVineFeature->place(level, random, x, y, z);
	}

	for (int i = 0; i < 32; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		EndactusFeature *endactusFeature = new EndactusFeature();
		endactusFeature->place(level, random, x, y, z);
	}

	for (int i = 0; i < 32; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		EndesertBushFeature().place(level, random, x, y, z);
	}

	for (int i = 0; i < 64; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		VeloettShrubFeature().place(level, random, x, y, z);
	}

	for (int i = 0; i < 32; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		VeloettFlowerFeature().place(level, random, x, y, z);
	}

	OreFeature endoriumOreFeature(Tile::endoriumOre_Id, 0, 5, Tile::endStone_Id);
	for (int i = 0; i < 5; i++)
	{
		int x = xo + random->nextInt(16);
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16);
		endoriumOreFeature.place(level, random, x, y, z);
	}

	OreFeature mossyEndStoneFeature(Tile::endStone_Id, EndStoneTile::TYPE_MOSSY, 24, Tile::endStone_Id);
	for (int i = 0; i < 17; i++)
	{
		int x = xo + random->nextInt(16);
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16);
		mossyEndStoneFeature.place(level, random, x, y, z);
	}
	PIXEndNamedEvent();
}

void TheOuterEndBiomeDecorator::decorateDepthSpan(int count, Feature *feature, int y0, int y1)
{
	for (int i = 0; i < count; i++)
	{
		int x = xo + random->nextInt(16);
		int y = random->nextInt(y1 - y0) + y0;
		int z = zo + random->nextInt(16);
		feature->place(level, random, x, y, z);
	}
}

void TheOuterEndBiomeDecorator::decorateDepthAverage(int count, Feature *feature, int yMid, int ySpan)
{
	for (int i = 0; i < count; i++)
	{
		int x = xo + random->nextInt(16);
		int y = random->nextInt(ySpan) + random->nextInt(ySpan) + (yMid - ySpan);
		int z = zo + random->nextInt(16);
		feature->place(level, random, x, y, z);
	}
}
