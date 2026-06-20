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
	ambrosiumOreFeature = new OreFeature(Tile::ambrosiumOre_Id, 0, 16, Tile::holystone_Id);
	zaniteOreFeature = new OreFeature(Tile::zaniteOre_Id, 0, 9, Tile::holystone_Id);
	gravititeOreFeature = new OreFeature(Tile::gravititeOre_Id, 0, 5, Tile::holystone_Id);
	icestoneFeature = new OreFeature(Tile::icestone_Id, 0, 8, Tile::holystone_Id);
	mossyHolystoneFeature = new OreFeature(Tile::holystone_Id, 1, 8, Tile::holystone_Id);

	quicksoilShelfFeature = new QuicksoilShelfFeature();

	holidayTreeFeature = new HolidayTreeFeature(false);

	largeAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 0/*AercloudTile::TYPE_DEFAULT*/, 6, 10, 2, 4, true);
	smallAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 0/*AercloudTile::TYPE_DEFAULT*/, 3, 6, 1, 2, false);
	smallGoldAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 2/*AercloudTile::TYPE_GOLD*/, 2, 4, 1, 2, false);
	smallBlueAerCloudFeature = new AerCloudFeature(Tile::aercloud_Id, 1/*AercloudTile::TYPE_BLUE*/, 2, 4, 1, 2, false);

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
	liquids = false;
}

void AetherBiomeDecorator::decorate()
{
	PIXBeginNamedEvent(0, "Decorate Aether ores");
	decorateAetherOres();
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0, "Decorate Aether forests");
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
	PIXEndNamedEvent();

	PIXBeginNamedEvent(0, "Decorate Aether flowers/grass");
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

	PIXBeginNamedEvent(0, "Decorate Aether quicksoil shelves");
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

	PIXBeginNamedEvent(0, "Decorate Aether clouds");

	if (random->nextInt(3) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = level->getHeightmap(x, z);
		if (y > 0)
		{
			largeAerCloudFeature->place(level, random, x, y, z);
		}
	}

	const int minCloudY = 80;

	if (random->nextInt(10) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = minCloudY + random->nextInt(Level::genDepth - 10 - minCloudY);
		smallAerCloudFeature->place(level, random, x, y, z);
	}

	if (random->nextInt(20) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = minCloudY + random->nextInt(Level::genDepth - 10 - minCloudY);
		smallGoldAerCloudFeature->place(level, random, x, y, z);
	}

	if (random->nextInt(30) == 0)
	{
		int x = xo + random->nextInt(16) + 8;
		int z = zo + random->nextInt(16) + 8;
		int y = minCloudY + random->nextInt(Level::genDepth - 10 - minCloudY);
		smallBlueAerCloudFeature->place(level, random, x, y, z);
	}

	PIXEndNamedEvent();
}

void AetherBiomeDecorator::decorateAetherOres()
{
	level->setInstaTick(true);
	decorateDepthSpan(20, ambrosiumOreFeature, 0, Level::genDepth);
	decorateDepthSpan(10, zaniteOreFeature, 0, Level::genDepth / 2);
	decorateDepthSpan(7, gravititeOreFeature, 0, Level::genDepth / 4);
	decorateDepthSpan(10, icestoneFeature, 0, Level::genDepth);
	decorateDepthSpan(12, mossyHolystoneFeature, 0, Level::genDepth);
	level->setInstaTick(false);
}
