#include "stdafx.h"
#include "AetherDimension.h"
#include "FixedBiomeSource.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.levelgen.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "..\Minecraft.Client\Common\Colours\ColourTable.h"

void AetherDimension::init()
{
    biomeSource = new FixedBiomeSource(Biome::aether, 0.35f, 0);
    id = 3;
    hasCeiling = false;
}

float AetherDimension::getTimeOfDay(int64_t time, float a) const
{
	return 0.0f;
}

float *AetherDimension::getSunriseColor(float td, float a)
{
	return nullptr;
}

Vec3 *AetherDimension::getFogColor(float td, float a) const
{
	float r = 0.62f;
	float g = 0.80f;
	float b = 1.0f;
	return Vec3::newTemp(r, g, b);
}

bool AetherDimension::hasGround()
{
	return false;
}

bool AetherDimension::mayRespawn() const
{
	return false;
}

bool AetherDimension::isNaturalDimension()
{
	return false;
}

float AetherDimension::getCloudHeight()
{
	return 8;
}

bool AetherDimension::isValidSpawn(int x, int z) const
{
    int topTile = level->getTopTile(x, z);

    if (topTile == 0) return false;

    return Tile::tiles[topTile]->material->blocksMotion();
}

Pos *AetherDimension::getSpawnPos()
{
    return new Pos(0, 64, 0);
}

ChunkSource *AetherDimension::createRandomLevelSource() const
{
    if (level == nullptr)
    {
        return nullptr; // prevents crashes if init order is wrong
    }

    return new AetherLevelSource(level, level->getSeed());
}

bool AetherDimension::isFoggyAt(int x, int z)
{
	return false;
}

int AetherDimension::getSpawnYPosition()
{
	return 64;
}