#pragma once

#include <climits>
#include <string>
#include <vector>

class Dimension;
class Level;

struct DimensionTerrain
{
	std::string stone;
	std::string surface;
	std::string filler;

	int baseHeight = 48;
	int heightVariation = 24;

	float relief = 1.0f;

	int soilDepth = 4;
	int baseDepth = 4;

	int ceilingMargin = 32;

	bool lakes = true;
};

struct DimensionDefinition
{
	std::string modId;
	std::string dimensionId;

	int slot = 0;
	bool hasSlot = false;

	std::string biome;
	std::string generator;

	DimensionTerrain terrain;

	int xzSize = 0;
	int seaLevel = -1;

	bool hasCeiling = false;
	bool ultraWarm = false;
	bool natural = true;
	bool mayRespawn = false;

	int spawnX = 0;
	int spawnY = 72;
	int spawnZ = 0;
};

namespace DimensionRegistry
{
	const int FIRST_SLOT = -1;
	const int LAST_SLOT = 31;
	const int FIRST_MOD_SLOT = 5;

	bool registerDimension(const DimensionDefinition &definition);

	void finalize();

	Dimension *create(int id);

	const int NO_SLOT = INT_MIN;

	int slotForName(const std::string &name);

	bool isModDimension(int slot);

	int safeArrivalY(Level *level, int x, int z);
}
