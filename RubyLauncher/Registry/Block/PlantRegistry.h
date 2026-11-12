#pragma once

#include <string>
#include <vector>

struct PlantDefinition
{
	std::string modId;
	std::string plantId;
	std::string name;
	std::wstring modPath;
	std::string texturePath;
	std::vector<std::string> soil;
	std::string drop;

	bool dropNothing = false;
};

namespace PlantRegistry
{
	int registerPlant(const PlantDefinition &definition);

	void finalize();
}
