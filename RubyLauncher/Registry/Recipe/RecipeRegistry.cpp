#include "RecipeRegistry.h"

#include "Loader.h"
#include "Registry/IDs.h"

#include "Item.h"
#include "ItemInstance.h"
#include "Recipes.h"
#include "FurnaceRecipes.h"
#include "ShapedRecipy.h"
#include "ShapelessRecipy.h"

#include <utility>

namespace
{
	struct PendingRecipe
	{
		bool shaped = true;
		std::string modId;
		std::string recipeId;
		std::vector<std::string> pattern;
		std::vector<std::pair<char, std::string>> key;
		std::vector<std::string> ingredients;
		std::string result;
		int count = 1;
		int aux = -1;
		RecipeRegistry::Group group = Recipy::eGroupType_Decoration;
	};

	struct PendingSmeltingRecipe
	{
		std::string modId;
		std::string recipeId;
		std::string input;
		std::string result;
		int count = 1;
		float xp = 0.0f;
	};

	std::vector<PendingRecipe> g_pending;
	std::vector<PendingSmeltingRecipe> g_pendingSmelting;

	std::string describe(const PendingRecipe &pending)
	{
		return pending.modId + ":" + pending.recipeId;
	}

	ItemInstance *resolveIngredient(const std::string &name, const std::string &context)
	{
		IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
		if (mapped.id == 0)
		{
			Loader::_debugPrint("recipe " + context + ": unknown ingredient '" + name + "'");
			return nullptr;
		}
		if (Item::items[mapped.id] == nullptr)
		{
			Loader::_debugPrint("recipe " + context + ": ingredient '" + name + "' has no Item entry (id " + std::to_string(mapped.id) + ")");
			return nullptr;
		}

		return new ItemInstance(mapped.id, 1, Recipes::ANY_AUX_VALUE);
	}

	ItemInstance *resolveResult(const std::string &name, int count, int aux, const std::string &context)
	{
		IDMapping::MappedItem mapped = IDMapping::get()->getID(name);
		if (mapped.id == 0)
		{
			Loader::_debugPrint("recipe " + context + ": unknown result '" + name + "'");
			return nullptr;
		}
		if (Item::items[mapped.id] == nullptr)
		{
			Loader::_debugPrint("recipe " + context + ": result '" + name + "' has no item entry (id " + std::to_string(mapped.id) + ")");
			return nullptr;
		}

		return new ItemInstance(mapped.id, count, aux >= 0 ? aux : mapped.aux);
	}

	Recipy *buildShaped(const PendingRecipe &pending)
	{
		const std::string context = describe(pending);

		const int height = static_cast<int>(pending.pattern.size());
		const int width = static_cast<int>(pending.pattern[0].size());

		for (const std::string &row : pending.pattern)
		{
			if (static_cast<int>(row.size()) != width)
			{
				Loader::_debugPrint("recipe " + context + ": every pattern row must be the same width");
				return nullptr;
			}
		}

		std::map<char, ItemInstance *> keyMap;
		for (const auto &entry : pending.key)
		{
			ItemInstance *ingredient = resolveIngredient(entry.second, context);
			if (ingredient == nullptr) return nullptr;
			keyMap[entry.first] = ingredient;
		}

		ItemInstance **items = new ItemInstance *[width * height];
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const char symbol = pending.pattern[y][x];

				if (symbol == ' ')
				{
					items[y * width + x] = nullptr;
					continue;
				}

				auto found = keyMap.find(symbol);
				if (found == keyMap.end())
				{
					Loader::_debugPrint("recipe " + context + ": the pattern uses '" + std::string(1, symbol) + "' but the key has no entry for it");
					delete [] items;
					return nullptr;
				}

				items[y * width + x] = found->second;
			}
		}

		ItemInstance *result = resolveResult(pending.result, pending.count, pending.aux, context);
		if (result == nullptr)
		{
			delete [] items;
			return nullptr;
		}

		return new ShapedRecipy(width, height, items, result, pending.group);
	}

	Recipy *buildShapeless(const PendingRecipe &pending)
	{
		const std::string context = describe(pending);

		std::map<int, ItemInstance *> cache;

		vector<ItemInstance *> *ingredients = new vector<ItemInstance *>();
		for (const std::string &name : pending.ingredients)
		{
			const int id = IDMapping::get()->getID(name).id;

			ItemInstance *ingredient = nullptr;
			auto cached = cache.find(id);
			if (cached != cache.end())
			{
				ingredient = cached->second;
			}
			else
			{
				ingredient = resolveIngredient(name, context);
				if (ingredient == nullptr)
				{
					for (auto &entry : cache) delete entry.second;
					delete ingredients;
					return nullptr;
				}
				cache[id] = ingredient;
			}

			ingredients->push_back(ingredient);
		}

		ItemInstance *result = resolveResult(pending.result, pending.count, pending.aux, context);
		if (result == nullptr)
		{
			delete ingredients;
			return nullptr;
		}

		return new ShapelessRecipy(result, ingredients, pending.group);
	}
}

bool RecipeRegistry::registerShaped(const std::string &modId, const std::string &recipeId, const std::vector<std::string> &pattern, const std::map<std::string, std::string> &key, const std::string &result, int count, int aux, Group group)
{
	const std::string context = modId + ":" + recipeId;

	if (pattern.empty() || pattern.size() > 3)
	{
		Loader::_debugPrint("recipe " + context + ": a shaped recipe needs 1-3 pattern rows");
		return false;
	}
	if (pattern[0].empty() || pattern[0].size() > 3)
	{
		Loader::_debugPrint("recipe " + context + ": a shaped recipe needs 1-3 columns per pattern row");
		return false;
	}
	if (result.empty())
	{
		Loader::_debugPrint("recipe " + context + ": no result was given");
		return false;
	}

	PendingRecipe pending;
	pending.shaped = true;
	pending.modId = modId;
	pending.recipeId = recipeId;
	pending.pattern = pattern;
	for (const auto &entry : key)
	{
		if (entry.first.empty()) continue;
		pending.key.push_back(std::make_pair(entry.first[0], entry.second));
	}
	pending.result = result;
	pending.count = count;
	pending.aux = aux;
	pending.group = group;

	g_pending.push_back(std::move(pending));
	return true;
}

bool RecipeRegistry::registerShapeless(const std::string &modId, const std::string &recipeId, const std::vector<std::string> &ingredients, const std::string &result, int count, int aux, Group group)
{
	const std::string context = modId + ":" + recipeId;

	if (ingredients.empty() || ingredients.size() > 9)
	{
		Loader::_debugPrint("recipe " + context + ": a shapeless recipe needs 1-9 ingredients");
		return false;
	}
	if (result.empty())
	{
		Loader::_debugPrint("recipe " + context + ": no result was given");
		return false;
	}

	PendingRecipe pending;
	pending.shaped = false;
	pending.modId = modId;
	pending.recipeId = recipeId;
	pending.ingredients = ingredients;
	pending.result = result;
	pending.count = count;
	pending.aux = aux;
	pending.group = group;

	g_pending.push_back(std::move(pending));
	return true;
}

bool RecipeRegistry::registerSmelting(const std::string &modId, const std::string &recipeId, const std::string &input, const std::string &result, int count, float xp)
{
	const std::string context = modId + ":" + recipeId;

	if (input.empty())
	{
		Loader::_debugPrint("smelting recipe " + context + ": no input was given");
		return false;
	}
	if (result.empty())
	{
		Loader::_debugPrint("smelting recipe " + context + ": no result was given");
		return false;
	}

	PendingSmeltingRecipe pending;
	pending.modId = modId;
	pending.recipeId = recipeId;
	pending.input = input;
	pending.result = result;
	pending.count = count;
	pending.xp = xp;

	g_pendingSmelting.push_back(std::move(pending));
	return true;
}

int RecipeRegistry::pendingCount()
{
	return static_cast<int>(g_pending.size());
}

void RecipeRegistry::finalize()
{
	if (g_pending.empty() && g_pendingSmelting.empty()) return;

	Recipes *recipes = Recipes::getInstance();
	FurnaceRecipes *furnace = FurnaceRecipes::getInstance();

	int added = 0;
	if (recipes == nullptr)
	{
		Loader::_debugPrint("RecipeRegistry: error, " + std::to_string(g_pending.size()) + " recipe(s) dropped");
		g_pending.clear();
	}
	else
	{
		for (const PendingRecipe &pending : g_pending)
		{
			Recipy *recipe = pending.shaped ? buildShaped(pending) : buildShapeless(pending);
			if (recipe == nullptr) continue;

			recipes->addRecipe(recipe);
			++added;
		}

		g_pending.clear();

		if (added == 0)
		{
			Loader::_debugPrint("RecipeRegistry: recipes couldnt be registered");
		}
		else
		{
			recipes->rebuildIngredientsArray();

			Loader::_debugPrint("RecipeRegistry: " + std::to_string(added) + " recipe(s) registered, ingredients array rebuilt");
		}
	}

	int smelted = 0;
	for (const PendingSmeltingRecipe &pending : g_pendingSmelting)
	{
		const std::string context = pending.modId + ":" + pending.recipeId;

		if (furnace == nullptr)
		{
			Loader::_debugPrint("smelting recipe " + context + ": the FurnaceRecipes singleton is missing");
			continue;
		}

		ItemInstance *input = resolveIngredient(pending.input, context);
		if (input == nullptr) continue;

		ItemInstance *result = resolveResult(pending.result, pending.count, -1, context);
		if (result == nullptr)
		{
			delete input;
			continue;
		}

		const int inputId = input->id;
		if (furnace->isFurnaceItem(inputId))
		{
			Loader::_debugPrint("smelting recipe " + context + ": overriding an existing smelting recipe for input '" + pending.input + "'");
		}

		delete input;
		furnace->addFurnaceRecipy(inputId, result, pending.xp);
		++smelted;
	}

	g_pendingSmelting.clear();

	if (smelted > 0)
	{
		Loader::_debugPrint("RecipeRegistry: " + std::to_string(smelted) + " smelting recipe(s) registered");
	}
}
