#include "ArmorMaterialRegistry.h"

#include "Loader.h"

#include <map>
#include <utility>
#include <vector>

namespace
{
	std::map<std::string, int> g_indexByKey;
	std::vector<ArmorItem::ArmorMaterial *> g_materials;
}

int ArmorMaterialRegistry::registerMaterial(const std::string &modId, const std::string &materialId, int durabilityMultiplier, int headProt, int torsoProt, int legsProt, int feetProt, int enchantmentValue)
{
	const std::string context = modId + ":" + materialId;

	if (materialId.empty())
	{
		Loader::_debugPrint("armor material: no materialId was given");
		return -1;
	}
	if (durabilityMultiplier <= 0)
	{
		Loader::_debugPrint("armor material " + context + ": durabilityMultiplier must be positive");
		return -1;
	}
	if (headProt < 0 || torsoProt < 0 || legsProt < 0 || feetProt < 0)
	{
		Loader::_debugPrint("armor material " + context + ": protections must be >= 0");
		return -1;
	}

	auto found = g_indexByKey.find(context);
	if (found != g_indexByKey.end())
	{
		return found->second;
	}

	int *protections = new int[4];
	protections[0] = headProt;
	protections[1] = torsoProt;
	protections[2] = legsProt;
	protections[3] = feetProt;

	ArmorItem::ArmorMaterial *material = new ArmorItem::ArmorMaterial(durabilityMultiplier, protections, enchantmentValue);

	const int handle = CUSTOM_ARMOR_MATERIAL_FIRST + static_cast<int>(g_materials.size());
	g_materials.push_back(material);
	g_indexByKey[context] = handle;

	Loader::_debugPrint("registered armor material " + context + " as handle " + std::to_string(handle));
	return handle;
}

const ArmorItem::ArmorMaterial *ArmorMaterialRegistry::materialForIndex(int handle)
{
	const int index = handle - CUSTOM_ARMOR_MATERIAL_FIRST;
	if (index < 0 || index >= static_cast<int>(g_materials.size()))
	{
		return nullptr;
	}
	return g_materials[index];
}
