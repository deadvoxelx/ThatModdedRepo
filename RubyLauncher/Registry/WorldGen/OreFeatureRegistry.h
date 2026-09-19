#pragma once

#include <string>

class Level; class Random; class Biome;

namespace OreFeatureRegistry
{
	constexpr int DIMENSION_ALL = -999;

	bool registerOre(const std::string &modId, const std::string &oreId, const std::string &block, int size, const std::string &target, int dimension, int yMin, int yMax, int count);
	void finalize();
	void reset();
	void decorateChunk(Level *level, Random *random, int xo, int zo, Biome *biome);
}