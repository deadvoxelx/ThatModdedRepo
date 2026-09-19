#pragma once
#include <string>

#include "Item.h"

namespace TierRegistry
{
	constexpr int CUSTOM_TIER_FIRST = 64;

	int registerTier(const std::string &modId, const std::string &tierId, int level, int uses, float speed, float damage, int enchantmentValue);
    const Item::Tier *tierForIndex(int handle);
}