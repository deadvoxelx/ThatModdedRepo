#pragma once

#include <string>

class Level; class Random; class Biome; class Feature;

namespace TreeFeatureRegistry
{
	bool registerTree(const std::string &modId, const std::string &treeId, const std::string &shape, const std::string &trunk, const std::string &leaves, const std::string &biome, int count, int height);
	void finalize();
	void reset();
	void decorateChunk(Level *level, Random *random, int xo, int zo, Biome *biome);
	Feature *buildTreeFeature(const std::string &shape, const std::string &trunk, const std::string &leaves, int height, const std::string &context);
}
