#pragma once

#include <string>

class Level; class Random; class Biome;

namespace SaplingRegistry
{
	int registerSapling(const std::string &modId, const std::string &saplingId, const std::string &name, const std::wstring &modPath, const std::string &texturePath, const std::string &shape, const std::string &trunk, const std::string &leaves, int height);
}
