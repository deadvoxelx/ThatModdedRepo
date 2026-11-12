#pragma once

#include <functional>
#include <string>
#include <vector>

class Biome;
class Level;
class Random;

struct FeatureBlock
{
	int x = 0;
	int y = 0;
	int z = 0;
	int tile = 0;
	int aux = 0;
	bool airOnly = false;
};

class FeatureContext
{
public:
	FeatureContext(Level *level, Random *random, int x, int y, int z);

	int getX() const { return m_x; }
	int getY() const { return m_y; }
	int getZ() const { return m_z; }

	void set(int dx, int dy, int dz, int tile, int aux = 0);
	void replace(int dx, int dy, int dz, int tile, int aux = 0);
	void fill(int x0, int y0, int z0, int x1, int y1, int z1, int tile, int aux = 0);

	int get(int dx, int dy, int dz) const;
	bool isAir(int dx, int dy, int dz) const;
	int height(int dx, int dz) const;

	int randomInt(int minInclusive, int maxInclusive);
	float randomFloat();

	const std::vector<FeatureBlock> &blocks() const { return m_blocks; }

private:
	Level *m_level;
	Random *m_random;
	int m_x;
	int m_y;
	int m_z;
	std::vector<FeatureBlock> m_blocks;
};

struct FeaturePlacement
{
	enum Mode
	{
		SURFACE,
		RANGE,
		FIXED
	};

	Mode mode = SURFACE;
	int minY = 0;
	int maxY = 0;

	std::vector<std::string> ground;
};

struct FeatureDefinition
{
	std::string modId;
	std::string featureId;
	std::vector<std::string> biomes;
	std::vector<std::string> dimensions;

	int count = 1;
	float chance = 1.0f;

	FeaturePlacement placement;

	std::function<bool(FeatureContext &)> builder;
};

namespace FeatureRegistry
{
	const int DIMENSION_ALL = -999;

	bool registerFeature(const FeatureDefinition &definition);

	void finalize();
	void reset();
	void decorateChunk(Level *level, Random *random, int xo, int zo, Biome *biome);

	int blockId(const std::string &name);
}
