#pragma once

#include "Common/EventSystem/EventBus.h"
#include "RubyMod.h"

#include "sol/sol.hpp"
#include "json/json.hpp"

class MinecraftServer;

constexpr const char* requiredMetadata[] = {"modId","version","name","serverMain","clientMain"};
class Loader {

public:
    Loader();
    void collectMods();

    void registerServerFunctions(MinecraftServer* server);

    static void log(const std::string &message);

    void refresh(sol::state& luaState,std::string_view (RubyMod::*getEntry)() const,bool prependPath = false);
    void execute(sol::environment& (RubyMod::*getEnv)(), std::string funcName, bool warn);

    void refreshServerScripts();
    void refreshClientScripts();

    void executeServerScripts(std::string name = "main", bool warn = false);
    void executeClientScripts(std::string name = "main", bool warn = false);

    static void _debugPrint(const std::string &output);

    std::map<std::string, RubyMod> mods_;

    sol::state luaServer;
    sol::state luaClient;

private:
    static nlohmann::json getManifest(const std::string &filePath);
    static std::string loadFile(std::string fileName);
};