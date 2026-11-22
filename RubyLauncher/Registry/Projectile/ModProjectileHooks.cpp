#include "ModProjectileHooks.h"
#include "ModProjectileLuaHooks.h"

#include <exception>
#include <string>
#include <vector>

#include "Entity.h"
#include "Level.h"
#include "Loader.h"

namespace
{
	struct HookEntry
	{
		sol::protected_function onHit = sol::nil;
		sol::protected_function onTick = sol::nil;
	};

	std::vector<HookEntry> &hooks()
	{
		static std::vector<HookEntry> entries;
		return entries;
	}

	HookEntry *entryFor(int specId)
	{
		std::vector<HookEntry> &entries = hooks();
		if (specId < 0 || specId >= static_cast<int>(entries.size())) return nullptr;
		return &entries[specId];
	}

	bool callHit(HookEntry &entry, Entity *projectile, Entity *target, Level *level)
	{
		try
		{
			sol::protected_function_result result = entry.onHit(projectile, target, level);
			if (!result.valid())
			{
				sol::error err = result;
				Loader::_debugPrint("[Lua] Error in projectile onHit: " + std::string(err.what()));
				return false;
			}
			return result.get_type() == sol::type::boolean && result.get<bool>();
		}
		catch (const std::exception &e)
		{
			Loader::_debugPrint("[Lua] Error in projectile onHit: " + std::string(e.what()));
			return false;
		}
	}

	void callTick(HookEntry &entry, Entity *projectile, Level *level)
	{
		try
		{
			sol::protected_function_result result = entry.onTick(projectile, level);
			if (!result.valid())
			{
				sol::error err = result;
				Loader::_debugPrint("[Lua] Error in projectile onTick: " + std::string(err.what()));
			}
		}
		catch (const std::exception &e)
		{
			Loader::_debugPrint("[Lua] Error in projectile onTick: " + std::string(e.what()));
		}
	}
}

void ModProjectileLuaHooks::registerHooks(int specId, sol::function onHit, sol::function onTick)
{
	if (specId < 0) return;

	std::vector<HookEntry> &entries = hooks();
	if (static_cast<size_t>(specId) >= entries.size()) entries.resize(specId + 1);

	HookEntry &entry = entries[specId];
	if (onHit.valid()) entry.onHit = sol::protected_function(std::move(onHit));
	if (onTick.valid()) entry.onTick = sol::protected_function(std::move(onTick));
}

bool ModProjectileHooks::fireHit(int specId, Entity *projectile, Entity *target, Level *level)
{
	HookEntry *entry = entryFor(specId);
	if (entry == nullptr || !entry->onHit.valid()) return false;
	return callHit(*entry, projectile, target, level);
}

void ModProjectileHooks::fireTick(int specId, Entity *projectile, Level *level)
{
	HookEntry *entry = entryFor(specId);
	if (entry == nullptr || !entry->onTick.valid()) return;
	callTick(*entry, projectile, level);
}
