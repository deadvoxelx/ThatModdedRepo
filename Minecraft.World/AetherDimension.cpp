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
	int colour = Minecraft::GetInstance()->getColourTable()->getColor( eMinecraftColour_Aether_Fog_Colour );
	byte redComponent = ((colour>>16)&0xFF);
	byte greenComponent = ((colour>>8)&0xFF);
	byte blueComponent = ((colour)&0xFF);

	float rr = (float)redComponent/256;
	float gg = (float)greenComponent/256;
	float bb = (float)blueComponent/256;
	return Vec3::newTemp(rr, gg, bb);
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
	return true;
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

	if (levelType == LevelType::lvl_farlands) 
	{
		return new AetherFarlandsLevelSource(level, level->getSeed());
	}
	else
	{
		return new AetherLevelSource(level, level->getSeed());
	}
}

bool AetherDimension::isFoggyAt(int x, int z)
{
	return false;
}

int AetherDimension::getSpawnYPosition()
{
	return 64;
}
