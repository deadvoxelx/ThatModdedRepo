#include "stdafx.h"
#include "TheOuterEndDimension.h"
//#include "TheOuterEndLevelRandomLevelSource.h"
#include "FixedBiomeSource.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.levelgen.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "..\Minecraft.Client\Common\Colours\ColourTable.h"

void TheOuterEndDimension::init()
{
    biomeSource = new FixedBiomeSource(Biome::outerIslands, 0.5f, 0);
    id = 2;
    hasCeiling = true;
}

float TheOuterEndDimension::getTimeOfDay(int64_t time, float a) const
{
	return 0.0f;
}

float *TheOuterEndDimension::getSunriseColor(float td, float a)
{
	return nullptr;
}

Vec3 *TheOuterEndDimension::getFogColor(float td, float a) const
{
	int fogColor = Minecraft::GetInstance()->getColourTable()->getColor( eMinecraftColour_End_Fog_Colour ); //0xa080a0;
	float br = Mth::cos(td * PI * 2) * 2 + 0.5f;
	if (br < 0.0f) br = 0.0f;
	if (br > 1.0f) br = 1.0f;

	float r = ((fogColor >> 16) & 0xff) / 255.0f;
	float g = ((fogColor >> 8) & 0xff) / 255.0f;
	float b = ((fogColor) & 0xff) / 255.0f;
	r *= br * 0.0f + 0.15f;
	g *= br * 0.0f + 0.15f;
	b *= br * 0.0f + 0.15f;

	return Vec3::newTemp(r, g, b);
}

bool TheOuterEndDimension::hasGround()
{
	return false;
}

bool TheOuterEndDimension::mayRespawn() const
{
	return false;
}

bool TheOuterEndDimension::isNaturalDimension()
{
	return false;
}

float TheOuterEndDimension::getCloudHeight()
{
	return 8;
}

bool TheOuterEndDimension::isValidSpawn(int x, int z) const
{
    int topTile = level->getTopTile(x, z);

    if (topTile == 0) return false;

    return Tile::tiles[topTile]->material->blocksMotion();
}

Pos *TheOuterEndDimension::getSpawnPos()
{
    return new Pos(0, 80, 0);
}

ChunkSource *TheOuterEndDimension::createRandomLevelSource() const
{
    if (level == nullptr)
    {
        return nullptr; // prevents crashes if init order is wrong
    }

    return new TheOuterEndLevelRandomLevelSource(level, level->getSeed());
}

bool TheOuterEndDimension::isFoggyAt(int x, int z)
{
	return false;
}

int TheOuterEndDimension::getSpawnYPosition()
{
	return 80;
}

/*void TheOuterEndDimension::updateLightRamp()
{
    float ambientLight = 0.10f;
    for (int i = 0; i <= Level::MAX_BRIGHTNESS; i++)
	{
        float v = (1 - i / (float) (Level::MAX_BRIGHTNESS));
        brightnessRamp[i] = ((1 - v) / (v * 3 + 1)) * (1 - ambientLight) + ambientLight;
    }
}*/
