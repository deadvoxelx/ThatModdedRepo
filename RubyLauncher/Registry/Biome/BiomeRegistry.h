#pragma once

#include <string>
#include <vector>

class Biome;

struct BiomeDefinition
{
	std::string modId;
	std::string biomeId;

	std::string name;

	std::string surface;
	std::string filler;

	float depth = 0.1f;
	float scale = 1.0f;
	float temperature = 0.5f;
	float downfall = 0.5f;

	bool snow = false;
	bool rain = true;

	int grassCount = 1;
	int flowerCount = 2;

	std::string tallGrass;

	bool isOverworldBiome = true;

	int size = 128;
	float frequency = 16.0f;

	int grassColor = 0x79c05a;
	int foliageColor = 0x59ae30;
	int waterColor = 0x3f76e4;
	int skyColor = 0x78a7ff;
};

struct BiomePlacement
{
	int biomeId = 0;
	int size = 0;
	float frequency = 0.0f;
};

namespace BiomeRegistry
{
	bool registerBiome(const BiomeDefinition &definition);
	void finalize();

	const std::vector<int> &customBiomeIds();
	const std::vector<BiomePlacement> &customBiomePlacements();

	Biome *findBiome(const std::string &name);
}
