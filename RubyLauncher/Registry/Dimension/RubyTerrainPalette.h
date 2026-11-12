#pragma once

#include "DimensionRegistry.h"
#include "Registry/IDs.h"
#include "Biome.h"
#include "Tile.h"

struct RubyTerrainPalette
{
	RubyTerrainPalette(const DimensionTerrain &terrain, const Biome *biome)
	{
		stone = resolve(terrain.stone, Tile::stone_Id);
		surface = resolve(terrain.surface, biome != nullptr ? biome->topMaterial : Tile::grass_Id);
		filler = resolve(terrain.filler, biome != nullptr ? biome->material : Tile::dirt_Id);
	}

	int stone;
	int surface;
	int filler;

private:
	static int resolve(const std::string &name, int fallback)
	{
		if (name.empty()) return fallback;

		IDMapping *mapping = IDMapping::get();
		if (mapping == nullptr) return fallback;

		IDMapping::MappedItem mapped = mapping->getID(name);
		return mapped.id > 0 ? mapped.id : fallback;
	}
};
