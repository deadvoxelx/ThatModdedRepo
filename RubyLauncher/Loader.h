#pragma once
#include <map>
#include <string>
#include <vector>
#include "Common/EventSystem/EventBus.h"
#include "Common/Scheduler.h"
#include "RubyMod.h"
#include "Server/ModCommandRegistry.h"

#include "sol/sol.hpp"
#include "json/json.hpp"

class MinecraftServer;

constexpr const char* loaderRequiredMetadata[] = {"modId","version","name"};
constexpr const char* loaderOptionalEntries[] = {"serverMain","clientMain"};
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
    static Loader *getInstance();
    RubyMod *findMod(const std::string &modId);
    void tickServer(MinecraftServer *server);
    void tickClient();

    std::map<std::string, RubyMod> mods_;
    std::vector<std::string> loadOrder_;
    sol::state luaServer;
    sol::state luaClient;
    Scheduler m_serverScheduler;
    Scheduler m_clientScheduler;
    ModCommandRegistry m_commandRegistry;
    int m_clientTickCount = 0;
    MinecraftServer *m_server = nullptr;

private:
    static nlohmann::json getManifest(const std::string &filePath);
    static std::string loadFile(std::string fileName);
    std::vector<std::string> computeLoadOrder();
    void buildLoadOrder();
    static Loader *s_instance;
};
