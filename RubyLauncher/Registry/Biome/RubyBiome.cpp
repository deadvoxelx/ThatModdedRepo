#include "RubyBiome.h"
#include "BiomeDecorator.h"
#include "TallGrassFeature.h"

RubyBiome::RubyBiome(int id) : Biome(id)
{
	m_grassRGB = 0x79c05a;
	m_foliageRGB = 0x59ae30;
	m_waterRGB = 0x3f76e4;
	m_skyRGB = 0x78a7ff;
}

void RubyBiome::applyDefinition(const std::wstring &name, int surface, int filler, float depth, float scale, float temperature, float downfall, bool snow, bool rain, int grassColor, int foliageColor, int waterColor, int skyColor)
{
	topMaterial = static_cast<byte>(surface);
	material = static_cast<byte>(filler);

	setName(name);
	setDepthAndScale(depth, scale);
	setTemperatureAndDownfall(temperature, downfall);

	if (snow) setSnowCovered();
	if (!rain) setNoRain();

	m_grassRGB = grassColor & 0xffffff;
	m_foliageRGB = foliageColor & 0xffffff;
	m_waterRGB = waterColor & 0xffffff;
	m_skyRGB = skyColor & 0xffffff;
}

void RubyBiome::setVegetationCounts(int grass, int flowers)
{
	if (decorator == nullptr) return;

	decorator->grassCount = grass;
	decorator->flowerCount = flowers;
	decorator->treeCount = -999;
}

void RubyBiome::setGrassBlock(int tileId)
{
	m_grassTile = tileId;
}

Feature *RubyBiome::getGrassFeature(Random *random)
{
	if (m_grassTile > 0) return new TallGrassFeature(m_grassTile, 0);

	return Biome::getGrassFeature(random);
}

int RubyBiome::getGrassColor()
{
	return m_grassRGB;
}

int RubyBiome::getFolageColor()
{
	return m_foliageRGB;
}

int RubyBiome::getWaterColor()
{
	return m_waterRGB;
}

int RubyBiome::getSkyColor(float temp)
{
	return m_skyRGB;
}
