#pragma once

#include <string>

#include "ArmorItem.h"

namespace ArmorMaterialRegistry
{
	constexpr int CUSTOM_ARMOR_MATERIAL_FIRST = 64;

	int registerMaterial(const std::string &modId, const std::string &materialId, int durabilityMultiplier, int headProt, int torsoProt, int legsProt, int feetProt, int enchantmentValue);
    const ArmorItem::ArmorMaterial *materialForIndex(int handle);
}