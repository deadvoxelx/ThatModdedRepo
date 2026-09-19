#pragma once

#include "Recipy.h"

#include <map>
#include <string>
#include <vector>

namespace RecipeRegistry
{
	using Group = Recipy::_eGroupType;

	bool registerShaped(const std::string &modId, const std::string &recipeId, const std::vector<std::string> &pattern, const std::map<std::string, std::string> &key, const std::string &result, int count, int aux, Group group);
	bool registerShapeless(const std::string &modId, const std::string &recipeId, const std::vector<std::string> &ingredients, const std::string &result, int count, int aux, Group group);
	bool registerSmelting(const std::string &modId, const std::string &recipeId, const std::string &input, const std::string &result, int count, float xp);
	int pendingCount();
	void finalize();
}
