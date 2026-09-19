#include "TierRegistry.h"

#include "Loader.h"

#include <map>
#include <utility>
#include <vector>

namespace
{
	std::map<std::string, int> g_indexByKey;
	std::vector<Item::Tier *> g_tiers;
}

int TierRegistry::registerTier(const std::string &modId, const std::string &tierId, int level, int uses, float speed, float damage, int enchantmentValue)
{
	const std::string context = modId + ":" + tierId;

	if (tierId.empty())
	{
		Loader::_debugPrint("item tier: no tierId was given");
		return -1;
	}
	if (uses <= 0)
	{
		Loader::_debugPrint("item tier " + context + ": uses must be positive");
		return -1;
	}
	if (level < 0)
	{
		Loader::_debugPrint("item tier " + context + ": level must be >= 0");
		return -1;
	}

	auto found = g_indexByKey.find(context);
	if (found != g_indexByKey.end())
	{
		return found->second;
	}

	Item::Tier *tier = new Item::Tier(level, uses, speed, damage, enchantmentValue);

	const int handle = CUSTOM_TIER_FIRST + static_cast<int>(g_tiers.size());
	g_tiers.push_back(tier);
	g_indexByKey[context] = handle;

	Loader::_debugPrint("registered item tier " + context + " as handle " + std::to_string(handle));
	return handle;
}

const Item::Tier *TierRegistry::tierForIndex(int handle)
{
	const int index = handle - CUSTOM_TIER_FIRST;
	if (index < 0 || index >= static_cast<int>(g_tiers.size()))
	{
		return nullptr;
	}
	return g_tiers[index];
}
