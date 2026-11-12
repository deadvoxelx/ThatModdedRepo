#include "DimensionRegistry.h"
#include "RubyDimension.h"
#include "Loader.h"
#include "Registry/Biome/BiomeRegistry.h"
#include "ChunkSource.h"
#include "DimensionSlots.h"
#include "Level.h"
#include <algorithm>
#include <vector>

namespace
{
	std::vector<DimensionDefinition> g_pending;
	std::vector<DimensionDefinition> g_created;

	std::string describe(const DimensionDefinition &definition)
	{
		return definition.modId + ":" + definition.dimensionId;
	}

	bool duplicates(const DimensionDefinition &definition)
	{
		for (const DimensionDefinition &pending : g_pending)
		{
			if (pending.modId == definition.modId && pending.dimensionId == definition.dimensionId) return true;
		}
		return false;
	}

	bool slotClaimed(int slot)
	{
		for (const DimensionDefinition &created : g_created)
		{
			if (created.hasSlot && created.slot == slot) return true;
		}
		return false;
	}

	int levelXZSize(const DimensionDefinition &definition)
	{
		return definition.xzSize > 0 ? definition.xzSize : 0;
	}
}

bool DimensionRegistry::registerDimension(const DimensionDefinition &definition)
{
	const std::string context = describe(definition);

	if (definition.modId.empty() || definition.dimensionId.empty())
	{
		Loader::_debugPrint("dimension " + context + ": modId and dimensionId are required");
		return false;
	}
	if (definition.hasSlot && (definition.slot < FIRST_SLOT || definition.slot > LAST_SLOT))
	{
		Loader::_debugPrint("dimension " + context + ": slot " + std::to_string(definition.slot) + " is outside -1 to " + std::to_string(LAST_SLOT));
		return false;
	}
	if (definition.xzSize < 0)
	{
		Loader::_debugPrint("dimension " + context + ": xzSize cannot be negative");
		return false;
	}
	if (!definition.generator.empty() && definition.generator != "surface" && definition.generator != "underground" && definition.generator != "floatingIslands")
	{
		Loader::_debugPrint("dimension " + context + ": unknown generator '" + definition.generator + "' (expected 'surface', 'underground' or 'floatingIslands')");
		return false;
	}
	if (duplicates(definition))
	{
		Loader::_debugPrint("dimension " + context + ": already registered");
		return false;
	}

	g_pending.push_back(definition);
	return true;
}

void DimensionRegistry::finalize()
{
	if (g_pending.empty()) return;

	std::stable_sort(g_pending.begin(), g_pending.end(), [](const DimensionDefinition &a, const DimensionDefinition &b)
	{
		if (a.modId != b.modId) return a.modId < b.modId;
		return a.dimensionId < b.dimensionId;
	});

	int added = 0;
	int nextFreeSlot = FIRST_MOD_SLOT;

	for (const DimensionDefinition &pending : g_pending)
	{
		const std::string context = describe(pending);

		if (!pending.biome.empty() && BiomeRegistry::findBiome(pending.biome) == nullptr)
		{
			Loader::_debugPrint("dimension " + context + ": no biome '" + pending.biome + "' (register the biome first)");
			continue;
		}

		DimensionDefinition definition = pending;

		if (!definition.hasSlot)
		{
			while (nextFreeSlot <= LAST_SLOT && slotClaimed(nextFreeSlot)) ++nextFreeSlot;
			if (nextFreeSlot > LAST_SLOT)
			{
				Loader::_debugPrint("dimension " + context + ": no free dimension slot left");
				continue;
			}

			definition.slot = nextFreeSlot++;
			definition.hasSlot = true;
		}

		if (slotClaimed(definition.slot))
		{
			Loader::_debugPrint("dimension " + context + ": slot " + std::to_string(definition.slot) + " is already claimed by another mod");
			continue;
		}

		g_created.push_back(definition);

		DimensionSlots::add(definition.slot, levelXZSize(definition));

		Loader::_debugPrint("dimension " + context + ": registered for slot " + std::to_string(definition.slot));
		++added;
	}

	g_pending.clear();

	if (added == 0) Loader::_debugPrint("DimensionRegistry: no dimensions could be registered");
}

Dimension *DimensionRegistry::create(int id)
{
	for (const DimensionDefinition &definition : g_created)
	{
		if (definition.hasSlot && definition.slot == id) return new RubyDimension(definition);
	}
	return nullptr;
}

namespace
{
	struct BuiltInDimensionName
	{
		const char *name;
		int slot;
	};

	const BuiltInDimensionName BUILT_IN_NAMES[] = {
		{ "overworld", 0 },
		{ "nether", -1 },
		{ "end", 1 },
		{ "outer_end", 2 },
		{ "aether", 3 },
		{ "nurealm", 4 },
	};
}

int DimensionRegistry::slotForName(const std::string &name)
{
	if (name.empty()) return NO_SLOT;

	std::string bare = name;
	if (bare.compare(0, 10, "minecraft:") == 0) bare = bare.substr(10);
	for (const BuiltInDimensionName &builtIn : BUILT_IN_NAMES)
	{
		if (bare == builtIn.name) return builtIn.slot;
	}

	for (const DimensionDefinition &definition : g_created)
	{
		if (describe(definition) == name) return definition.slot;
	}

	int found = NO_SLOT;
	for (const DimensionDefinition &definition : g_created)
	{
		if (definition.dimensionId != name) continue;
		if (found != NO_SLOT) return NO_SLOT;
		found = definition.slot;
	}
	return found;
}

int DimensionRegistry::safeArrivalY(Level *level, int x, int z)
{
	if (level == nullptr) return 0;

	int y = level->getTopSolidBlock(x, z);
	if (y <= 0) return 0;

	for (; y < Level::genDepthMinusOne; y++)
	{
		if (level->isEmptyTile(x, y, z) && level->isEmptyTile(x, y + 1, z)) return y;
	}
	return 0;
}

bool DimensionRegistry::isModDimension(int slot)
{
	return slotClaimed(slot);
}
