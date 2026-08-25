#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "NurealmBiomeDecorator.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "NusaTowerFeature.h"
#include "NusaVineFeature.h"
#include "NusaTreeFeature.h"

const double M_PI = 3.14159265358979323846;

NurealmBiomeDecorator::NurealmBiomeDecorator(Biome *biome) : BiomeDecorator(biome)
{
	nusaTowerFeature = new NusaTowerFeature(Tile::nustone_Id);
	nusaShrubFeature = new NusaShrubFeature();
	treeCount = 1;
	grassCount = 4;
}

void NurealmBiomeDecorator::decorate()
{
	decorateOres();

	int y = level->getTopSolidBlock(xo+8, zo+8);
	if(y>level->GetHighestY()) level->SetHighestY(y);

	PIXBeginNamedEvent(0,"Nurealm structures");
	if (random->nextInt(8) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		nusaTowerFeature->place(level, random, x, y, z);
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

	int forests = treeCount;
	if (random->nextInt(10) == 0) forests += 1;
	for (int i = 0; i < forests; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		Feature *tree = biome->getTreeFeature(random);
		tree->init(1, 1, 1);
		tree->place(level, random, x, level->getHeightmap(x, z), z);
		delete tree;
	}

	NusaVineFeature nusaVineFeature;
	for (int i = 0; i < 32; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		nusaVineFeature.place(level, random, x, y, z);
	}

	for (int i = 0; i < grassCount; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		MemSect(50);
		Feature *grassFeature = biome->getGrassFeature(random);
		MemSect(0);
		grassFeature->place(level, random, x, y, z);
		delete grassFeature;
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
