#include "stdafx.h"
#include "NurealmDimension.h"
#include "FixedBiomeSource.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.levelgen.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "..\Minecraft.Client\Common\Colours\ColourTable.h"

void NurealmDimension::init()
{
    biomeSource = new FixedBiomeSource(Biome::nulands, 0.5f, 0);
    id = 4;
    hasCeiling = true;
}

float NurealmDimension::getTimeOfDay(int64_t time, float a) const
{
	return 0.0f;
}

float *NurealmDimension::getSunriseColor(float td, float a)
{
	return nullptr;
}

Vec3 *NurealmDimension::getFogColor(float td, float a) const
{
	int colour = Minecraft::GetInstance()->getColourTable()->getColor( eMinecraftColour_Nurealm_Fog_Colour );
	byte redComponent = ((colour>>16)&0xFF);
	byte greenComponent = ((colour>>8)&0xFF);
	byte blueComponent = ((colour)&0xFF);

	float rr = (float)redComponent/256;
	float gg = (float)greenComponent/256;
	float bb = (float)blueComponent/256;
	return Vec3::newTemp(rr, gg, bb);
}

bool NurealmDimension::hasGround()
{
	return false;
}

bool NurealmDimension::mayRespawn() const
{
	return false;
}

bool NurealmDimension::isNaturalDimension()
{
	return false;
}

float NurealmDimension::getCloudHeight()
{
	return 8;
}

bool NurealmDimension::isValidSpawn(int x, int z) const
{
    int topTile = level->getTopTile(x, z);

    if (topTile == 0) return false;

    return Tile::tiles[topTile]->material->blocksMotion();
}

Pos *NurealmDimension::getSpawnPos()
{
    return new Pos(0, 80, 0);
}

ChunkSource *NurealmDimension::createRandomLevelSource() const
{
    if (level == nullptr)
    {
        return nullptr; // prevents crashes if init order is wrong
    }

    return new NurealmLevelSource(level, level->getSeed());
}

bool NurealmDimension::isFoggyAt(int x, int z)
{
	return true;
}

int NurealmDimension::getSpawnYPosition()
{
	return 64;
}

void NurealmDimension::updateLightRamp()
{
    float ambientLight = 0.01f;
    for (int i = 0; i <= Level::MAX_BRIGHTNESS; i++)
	{
        float v = (1 - i / (float) (Level::MAX_BRIGHTNESS));
        brightnessRamp[i] = ((1 - v) / (v * 3 + 1)) * (1 - ambientLight) + ambientLight;
    }
}