#include "stdafx.h"
#include "AetherBiomeDecorator.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.biome.h"
#include "QuicksoilShelfFeature.h"
#include "AerCloudFeature.h"
#include "BerryBushFeature.h"
#include "HolidayTreeFeature.h"

AetherBiomeDecorator::AetherBiomeDecorator(Biome *biome) : BiomeDecorator(biome)
{
	ambrosiumOreFeature = new OreFeature(Tile::ambrosiumOre_Id, 0, 15, Tile::holystone_Id);
	zaniteOreFeature = new OreFeature(Tile::zaniteOre_Id, 0, 12, Tile::holystone_Id);
	gravititeOreFeature = new OreFeature(Tile::gravititeOre_Id, 0, 8, Tile::holystone_Id);
	icestoneFeature = new OreFeature(Tile::icestone_Id, 0, 10, Tile::holystone_Id);
	mossyHolystoneFeature = new OreFeature(Tile::holystone_Id, 1, 8, Tile::holystone_Id);
	airFossilFeature = new OreFeature(Tile::fossilOre_Id, 2, 7, Tile::holystone_Id);

	quicksoilShelfFeature = new QuicksoilShelfFeature();

	holidayTreeFeature = new HolidayTreeFeature(Tile::skyrootLog_Id);

	largeAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 0, 32);
	smallAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 0, 16);
	smallGoldAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 1, 8);
	smallBlueAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 2, 4);

	whiteFlowerFeature = new FlowerFeature(Tile::flower_Id, 1);
	purpleFlowerFeature = new FlowerFeature(Tile::flower_Id, 2);

	treeCount = 2;
	grassCount = 5;
	flowerCount = 2;

	sandCount = 0;
	clayCount = 0;
	gravelCount = 0;
	deadBushCount = 0;
	mushroomCount = 0;
	reedsCount = 0;
	cactusCount = 0;
	waterlilyCount = 0;
	hugeMushrooms = 0;
	liquids = true;
}

void AetherBiomeDecorator::decorate()
{
	PIXBeginNamedEvent(0, "Decorate Ores");
	decorateAetherOres();
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0,"Decorate Liquids");
	if( liquids )
	{
		AetherSpringFeature *aetherSpringFeature = new AetherSpringFeature(Tile::water_Id);
		for (int i = 0; i < 50; i++)
		{
			int x = xo + random->nextInt(16) + 8;
			int y = random->nextInt(random->nextInt(Level::genDepth - 8) + 8);
			int z = zo + random->nextInt(16) + 8;
			aetherSpringFeature->place(level, random, x, y, z);
		}
		delete aetherSpringFeature;
	}
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0, "Decorate Foliage");
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

	for (int i = 0; i < 3; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		holidayTreeFeature->place(level, random, x, y, z);
	}

	for (int i = 0; i < flowerCount; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		whiteFlowerFeature->place(level, random, x, y, z);
	}

	for (int i = 0; i < flowerCount; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;
		purpleFlowerFeature->place(level, random, x, y, z);
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

	for (int i = 0; i < 9; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int y = random->nextInt(Level::genDepth);
		int z = zo + random->nextInt(16) + 8;		
		BerryBushFeature *berryBushFeature = new BerryBushFeature();
		berryBushFeature->place(level, random, x, y, z);
		delete berryBushFeature;
	}
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0, "Decorate Quicksoil");
	for (int i = 0; i < 5; i++)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = level->getHeightmap(x, z);
		if (y > 0)
		{
			quicksoilShelfFeature->place(level, random, x, y, z);
		}
	}
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0, "Decorate Aerclouds");
	// Large aercloud
	if (random->nextInt(10) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = 12 + random->nextInt(20);
		if (y > 0)
		{
			largeAerCloudFeature->place(level, random, x, y, z);
		}
	}

	// Normal aercloud
	if (random->nextInt(11) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = 56 + random->nextInt(64);
		smallAerCloudFeature->place(level, random, x, y, z);
	}

	// Gold aercloud
	if (random->nextInt(22) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = 56 + random->nextInt(32);
		smallGoldAerCloudFeature->place(level, random, x, y, z);
	}

	// Blue aercloud
	if (random->nextInt(40) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = 56 + random->nextInt(96);
		smallBlueAerCloudFeature->place(level, random, x, y, z);
	}
	PIXEndNamedEvent();
}

void AetherBiomeDecorator::decorateAetherOres()
{
	level->setInstaTick(true);
	decorateDepthSpan(16, ambrosiumOreFeature, 0, Level::genDepth);
	decorateDepthSpan(12, zaniteOreFeature, 0, Level::genDepth / 2);
	decorateDepthSpan(9, gravititeOreFeature, 0, Level::genDepth / 4);
	decorateDepthSpan(16, icestoneFeature, 0, Level::genDepth);
	decorateDepthSpan(12, mossyHolystoneFeature, 0, Level::genDepth);
	decorateDepthSpan(10, airFossilFeature, 0, Level::genDepth / 2);
	level->setInstaTick(false);
}
