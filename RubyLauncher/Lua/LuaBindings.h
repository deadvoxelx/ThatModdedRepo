#pragma once

#include "MinecraftServer.h"

#include "sol/sol.hpp"

class LuaBindings {
public:
    static void bindCommonFunctions(const std::vector<sol::state *> &luaStates);

    static void bindServerEvents(sol::state& lua);
    static void bindServerFunctions(sol::state& lua, MinecraftServer* server);

    static void bindClientFunctions(sol::state& lua);
};
