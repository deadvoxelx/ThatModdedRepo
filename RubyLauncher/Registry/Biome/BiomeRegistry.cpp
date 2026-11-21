#include "BiomeRegistry.h"
#include "RubyBiome.h"
#include "Loader.h"
#include "Registry/IDs.h"
#include "Biome.h"
#include "Tile.h"
#include "EntityIO.h"
#include "MobCategory.h"
#include <algorithm>
#include <vector>

namespace
{
	const int FIRST_CUSTOM_BIOME_ID = 30;
	const int MAX_BIOME_ID = 255;

	struct CreatedBiome
	{
		std::string key;
		RubyBiome *biome = nullptr;
	};

	std::vector<BiomeDefinition> g_pending;
	std::vector<CreatedBiome> g_created;
	std::vector<int> g_ids;
	std::vector<BiomePlacement> g_placements;

	std::string describe(const BiomeDefinition &definition)
	{
		return definition.modId + ":" + definition.biomeId;
	}

	bool duplicates(const BiomeDefinition &definition)
	{
		for (const BiomeDefinition &pending : g_pending)
		{
			if (pending.modId == definition.modId && pending.biomeId == definition.biomeId) return true;
		}
		return false;
	}

	int resolveBlock(const std::string &name, const std::string &context, const std::string &role)
	{
		IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
		if (mapped.id == 0)
		{
			Loader::_debugPrint("biome " + context + ": unknown " + role + " '" + name + "'");
			return -1;
		}
		if (Tile::tiles[mapped.id] == nullptr)
		{
			Loader::_debugPrint("biome " + context + ": " + role + " '" + name + "' has no Tile entry (id " + std::to_string(mapped.id) + ")");
			return -1;
		}
		return mapped.id;
	}
	
	MobCategory *mobCategoryForName(const std::string &name)
	{
		if (name.empty() || name == "monster") return MobCategory::monster;
		if (name == "creature") return MobCategory::creature;
		if (name == "chicken") return MobCategory::creature_chicken;
		if (name == "wolf") return MobCategory::creature_wolf;
		if (name == "mushroomcow") return MobCategory::creature_mushroomcow;
		if (name == "water") return MobCategory::waterCreature;
		if (name == "ambient") return MobCategory::ambient;
		return nullptr;
	}

	eINSTANCEOF mobTypeForName(const std::string &name)
	{
		if (name.empty()) return eTYPE_NOTSET;

		std::wstring wide(name.begin(), name.end());

		if (EntityIO::getEncodeId(EntityIO::getId(wide)) == wide) return EntityIO::getType(wide);

		if (wide[0] >= 'a' && wide[0] <= 'z') wide[0] = static_cast<wchar_t>(wide[0] - 'a' + 'A');

		if (EntityIO::getEncodeId(EntityIO::getId(wide)) == wide) return EntityIO::getType(wide);

		return eTYPE_NOTSET;
	}

	bool resolveMobSpawns(const BiomeDefinition &definition, const std::string &context, std::vector<RubyMobSpawn> &resolved)
	{
		resolved.clear();

		for (const BiomeMobSpawn &spawn : definition.mobSpawns)
		{
			const eINSTANCEOF type = mobTypeForName(spawn.mob);

			if (type == eTYPE_NOTSET)
			{
				Loader::_debugPrint("biome " + context + ": unknown mob '" + spawn.mob + "' in mobSpawns; skipping it");
				continue;
			}

			MobCategory *category = mobCategoryForName(spawn.category);

			if (category == nullptr)
			{
				Loader::_debugPrint("biome " + context + ": unknown spawn category '" + spawn.category + "' for '" + spawn.mob + "'; use monster, creature, chicken, wolf, mushroomcow, water or ambient");
				continue;
			}

			if (spawn.weight <= 0)
			{
				Loader::_debugPrint("biome " + context + ": '" + spawn.mob + "' has a spawn weight of " + std::to_string(spawn.weight) + "; it needs to be above 0, skipping it");
				continue;
			}

			RubyMobSpawn entry;
			entry.type = type;
			entry.category = category;
			entry.weight = spawn.weight;
			entry.minCount = spawn.minCount < 1 ? 1 : spawn.minCount;
			entry.maxCount = spawn.maxCount < entry.minCount ? entry.minCount : spawn.maxCount;

			resolved.push_back(entry);
		}
		if (resolved.empty())
		{
			Loader::_debugPrint("biome " + context + ": mobSpawns named no usable mobs; keeping the default spawns");
			return false;
		}
		return true;
	}
}

bool BiomeRegistry::registerBiome(const BiomeDefinition &definition)
{
	const std::string context = describe(definition);

	if (definition.modId.empty() || definition.biomeId.empty())
	{
		Loader::_debugPrint("biome " + context + ": modId and biomeId are required");
		return false;
	}
	if (definition.surface.empty() || definition.filler.empty())
	{
		Loader::_debugPrint("biome " + context + ": surface and filler blocks are required");
		return false;
	}
	if (definition.scale <= 0.0f)
	{
		Loader::_debugPrint("biome " + context + ": scale must be positive");
		return false;
	}
	if (definition.size <= 0)
	{
		Loader::_debugPrint("biome " + context + ": size must be positive (blocks across a patch)");
		return false;
	}
	if (definition.frequency <= 0.0f || definition.frequency > 100.0f)
	{
		Loader::_debugPrint("biome " + context + ": frequency must be between 0 and 100 (percent of the land)");
		return false;
	}
	if (static_cast<int>(g_pending.size() + g_created.size()) > MAX_BIOME_ID - FIRST_CUSTOM_BIOME_ID)
	{
		Loader::_debugPrint("biome " + context + ": no free biome ids left");
		return false;
	}
	if (duplicates(definition))
	{
		Loader::_debugPrint("biome " + context + ": already registered");
		return false;
	}

	g_pending.push_back(definition);
	return true;
}

void BiomeRegistry::finalize()
{
	if (g_pending.empty()) return;

	std::stable_sort(g_pending.begin(), g_pending.end(), [](const BiomeDefinition &a, const BiomeDefinition &b)
	{
		if (a.modId != b.modId) return a.modId < b.modId;
		return a.biomeId < b.biomeId;
	});

	int nextId = FIRST_CUSTOM_BIOME_ID;
	int added = 0;

	for (const BiomeDefinition &definition : g_pending)
	{
		const std::string context = describe(definition);

		const int surface = resolveBlock(definition.surface, context, "surface");
		if (surface == -1) continue;
		const int filler = resolveBlock(definition.filler, context, "filler");
		if (filler == -1) continue;

		while (nextId <= MAX_BIOME_ID && Biome::biomes[nextId] != nullptr) nextId++;
		if (nextId > MAX_BIOME_ID)
		{
			Loader::_debugPrint("biome " + context + ": no free biome ids left");
			break;
		}

		RubyBiome *biome = new RubyBiome(nextId++);
		const std::wstring name(definition.name.begin(), definition.name.end());
		biome->applyDefinition(name, surface, filler, definition.depth, definition.scale, definition.temperature, definition.downfall, definition.snow, definition.rain, definition.grassColor, definition.foliageColor, definition.waterColor, definition.skyColor);
		RubyDecoratorCounts counts;
		counts.grass = definition.grassCount;
		counts.flowers = definition.flowerCount;
		counts.deadBushes = definition.deadBushCount;
		counts.cacti = definition.cactusCount;
		counts.reeds = definition.reedsCount;
		counts.mushrooms = definition.mushroomCount;
		counts.waterlilies = definition.waterlilyCount;
		counts.sand = definition.sandCount;
		counts.clay = definition.clayCount;
		counts.gravel = definition.gravelCount;
		biome->setVegetationCounts(counts);

		if (!definition.mobSpawns.empty())
		{
			std::vector<RubyMobSpawn> spawns;

			if (resolveMobSpawns(definition, context, spawns))
			{
				biome->setMobSpawns(spawns);
				Loader::_debugPrint("biome " + context + ": " + std::to_string(spawns.size()) + " mob spawn entries of its own");
			}
		}

		if (!definition.tallGrass.empty())
		{
			const int grass = resolveBlock(definition.tallGrass, context, "tall grass");
			if (grass != -1) biome->setGrassBlock(grass);
		}

		g_created.push_back({ context, biome });
		g_ids.push_back(biome->id);
		if (definition.isOverworldBiome)
		{
			g_placements.push_back({ biome->id, definition.size, definition.frequency });
		}
		Loader::_debugPrint("biome " + context + ": registered as id " + std::to_string(biome->id) + " (" + definition.name + ")" + (definition.isOverworldBiome ? "" : ", kept out of the overworld"));
		++added;
	}

	g_pending.clear();

	if (added == 0) Loader::_debugPrint("BiomeRegistry: no biomes could be registered");
}

const std::vector<int> &BiomeRegistry::customBiomeIds()
{
	return g_ids;
}

const std::vector<BiomePlacement> &BiomeRegistry::customBiomePlacements()
{
	return g_placements;
}

Biome *BiomeRegistry::findBiome(const std::string &name)
{
	if (name.empty()) return nullptr;

	for (const CreatedBiome &created : g_created)
	{
		if (created.key == name) return created.biome;
	}

	Biome *match = nullptr;
	for (const CreatedBiome &created : g_created)
	{
		const size_t colon = created.key.find(':');
		if (colon == std::string::npos || created.key.substr(colon + 1) != name) continue;
		if (match != nullptr) return nullptr;
		match = created.biome;
	}
	return match;
}
