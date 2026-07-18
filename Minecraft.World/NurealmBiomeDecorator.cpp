#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "NurealmBiomeDecorator.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "EndTowerFeature.h"
#include "NusaVineFeature.h"
#include "NusaTreeFeature.h"

const double M_PI = 3.14159265358979323846;

NurealmBiomeDecorator::NurealmBiomeDecorator(Biome *biome) : BiomeDecorator(biome)
{
	endTowerFeature = new EndTowerFeature(Tile::endStone_Id);

	grassCount = 3;
}

void NurealmBiomeDecorator::decorate()
{
	decorateOres();

	int y = level->getTopSolidBlock(xo+8, zo+8);
	if(y>level->GetHighestY()) level->SetHighestY(y);

	PIXBeginNamedEvent(0,"Nurealm structures");
	for (int i = 0; i < 1; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		endTowerFeature->place(level, random, x, y, z);
	}
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0,"Nurealm decor");
	for (int i = 0; i < 1; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = 32 + random->nextInt(64);
		int z = zo + random->nextInt(16) + 8;
		GlowingNustoneFeature().place(level, random, x, y, z);
	}

	for (int i = 0; i < 128; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		NusaTreeFeature(false).place(level, random, x, y, z);
	}

	for (int i = 0; i < 32; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		NusaVineFeature *nusaVineFeature = new NusaVineFeature();
		nusaVineFeature->place(level, random, x, y, z);
	}
	PIXEndNamedEvent();
}

void NurealmBiomeDecorator::decorateDepthSpan(int count, Feature *feature, int y0, int y1)
{
	for (int i = 0; i < count; i++)
	{
		int x = xo + random->nextInt(16);
		int y = random->nextInt(y1 - y0) + y0;
		int z = zo + random->nextInt(16);
		feature->place(level, random, x, y, z);
	}
}

void NurealmBiomeDecorator::decorateDepthAverage(int count, Feature *feature, int yMid, int ySpan)
{
	for (int i = 0; i < count; i++)
	{
		int x = xo + random->nextInt(16);
		int y = random->nextInt(ySpan) + random->nextInt(ySpan) + (yMid - ySpan);
		int z = zo + random->nextInt(16);
		feature->place(level, random, x, y, z);
	}
}
