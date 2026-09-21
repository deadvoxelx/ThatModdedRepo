#include "Loader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <set>
#include <utility>
#include <string>

#include "Common/ModPaths.h"
#include "Common/ModStore.h"
#include "Common/EventSystem/ClientTickEvent.h"
#include "Common/EventSystem/EventBus.h"
#include "Common/EventSystem/ServerTickEvent.h"
#include "Lua/LuaBindings.h"

class MinecraftServer;

namespace fs = std::filesystem;
using string = std::string;

Loader *Loader::s_instance = nullptr;
Loader *Loader::getInstance()
{
    return s_instance;
}

Loader::Loader() {
    s_instance = this;
    luaServer.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::package, sol::lib::math);
    luaClient.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::package, sol::lib::math);
    auto panicHandler = [](lua_State* L) -> int {
        fprintf(stderr, "[Lua] PANIC: %s\n", lua_tostring(L, -1));
        return 0;
    };
    lua_atpanic(luaServer.lua_state(), panicHandler);
    lua_atpanic(luaClient.lua_state(), panicHandler);

    RubyPaths::ensureModsFolderExists();

    LuaBindings::bindCommonFunctions({ &luaServer, &luaClient });
    LuaBindings::bindClientFunctions(luaClient);
    LuaBindings::bindServerEvents(luaServer);
    app.DebugPrintf("Ruby Launcher initialized!\n");
}

void Loader::_debugPrint(const string &output) {
    app.DebugPrintf(("Ruby Launcher: "+output+"\n").c_str());
}

nlohmann::json Loader::getManifest(const string &filePath) {
    string fullPath = filePath + "/manifest.json";
    auto data = loadFile(fullPath);
    return nlohmann::json::parse(data);
}

void Loader::log(const string& message) {
    app.DebugPrintf(("Ruby Launcher: " + message + "\n").c_str());
}

string Loader::loadFile(string fileName) {
    std::ifstream file(fileName);
    stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

RubyMod *Loader::findMod(const std::string &modId) {
    for (auto &entry : mods_) {
        if (std::string(entry.second.getModID()) == modId) return &entry.second;
    }
    return nullptr;
}

void Loader::tickServer(MinecraftServer *server) {
    if (server == nullptr) return;
    ServerTickEvent event(server, server->tickCount);
    EventBus::Get().fire(event);
    m_serverScheduler.tick();
}

void Loader::tickClient() {
    ++m_clientTickCount;
    ClientTickEvent event(m_clientTickCount);
    EventBus::Get().fire(event);
    m_clientScheduler.tick();
}

void Loader::registerServerFunctions(MinecraftServer *server) {
    m_server = server;
    m_serverScheduler.clear();
    LuaBindings::bindServerFunctions(luaServer, server);
}

bool hasMetadata(nlohmann::json& json, const std::string& key) {
    return !json[key].is_null();
}

void Loader::collectMods() {
    for (const auto & modEntry : fs::directory_iterator(RubyPaths::modsRoot())) {
        if (!modEntry.is_directory()) continue;
        string modPath = modEntry.path().string();

        nlohmann::json json;
        try {
            json = getManifest(modEntry.path().string());
        } catch (const std::exception& e) {
            _debugPrint("Couldnt read manifest.json for '"+modPath+"' exception: "+e.what());
            continue;
        }

        RubyMod m;
        bool missingRequired = false;

        for ( const auto & metadata : loaderRequiredMetadata ) {
            if (!hasMetadata(json, metadata)) {
                _debugPrint((modPath+" missing metadata '"+metadata+"' add it to your manifest.json"));
                missingRequired = true;
                break;
            }
            m.metadata[metadata] = json[metadata].get<std::string>();
        }

        if (missingRequired) continue;

        bool hasServer = hasMetadata(json, "serverMain");
        bool hasClient = hasMetadata(json, "clientMain");
        if (!hasServer && !hasClient) {
            _debugPrint((modPath+" needs at least one of 'serverMain'/'clientMain' in your manifest.json"));
            continue;
        }
        if (hasServer) m.metadata["serverMain"] = json["serverMain"].get<std::string>();
        if (hasClient) m.metadata["clientMain"] = json["clientMain"].get<std::string>();
        if (json["dependencies"].is_array()) {
            for (const auto &dep : json["dependencies"]) {
                if (dep.is_string()) m.dependencies.push_back(dep.get<std::string>());
            }
        }
        if (json["loadAfter"].is_array()) {
            for (const auto &dep : json["loadAfter"]) {
                if (dep.is_string()) m.loadAfter.push_back(dep.get<std::string>());
            }
        }

        string folderName = modEntry.path().filename().string();
        m.setPathName(folderName);
        mods_[std::string(m.getName())] = m;
    }

    buildLoadOrder();
}

std::vector<std::string> Loader::computeLoadOrder() {
    std::map<std::string, std::set<std::string>> dependents;
    std::map<std::string, unsigned int> indegree;
    for (const auto &[id, mod] : mods_) indegree[id] = 0;
    for (const auto &[id, mod] : mods_) {
        std::set<std::string> deps;
        for (const auto &dep : mod.getDependencies()) if (mods_.count(dep) > 0) deps.insert(dep);
        for (const auto &dep : mod.getLoadAfter()) if (mods_.count(dep) > 0) deps.insert(dep);
        indegree[id] = static_cast<unsigned int>(deps.size());
        for (const auto &dep : deps) dependents[dep].insert(id);
    }

    std::vector<std::string> order;
    std::set<std::string> ready;
    for (const auto &[id, deg] : indegree) if (deg == 0) ready.insert(id);
    while (!ready.empty()) {
        std::string id = *ready.begin();
        ready.erase(ready.begin());
        order.push_back(id);
        for (const auto &dependent : dependents[id]) {
            if (--indegree[dependent] == 0) ready.insert(dependent);
        }
    }

    bool cycle = false;
    for (const auto &[id, deg] : indegree) {
        if (deg > 0) {
            cycle = true;
            order.push_back(id);
        }
    }
    if (cycle) _debugPrint("dependency cycle detected, falling back to name order for cyclic mods");
    return order;
}

void Loader::buildLoadOrder() {
    for (auto it = mods_.begin(); it != mods_.end();) {
        bool missing = false;
        std::string missingDep;
        for (const auto &dep : it->second.getDependencies()) {
            if (mods_.count(dep) == 0) {
                missing = true;
                missingDep = dep;
                break;
            }
        }
        if (missing) {
            _debugPrint("Unloading '"+it->first+"' due to missing dependency '"+missingDep+"'");
            it = mods_.erase(it);
        } else {
            ++it;
        }
    }

    loadOrder_ = computeLoadOrder();
}

void Loader::refresh(sol::state& luaState,std::string_view (RubyMod::*getEntry)() const,bool prependPath) {
    std::vector<std::string> failedMods;
    for (const std::string &name : loadOrder_) {
        auto modIterator = mods_.find(name);
        if (modIterator == mods_.end()) continue;
        RubyMod& mod = modIterator->second;
        std::string modName = std::string(mod.getName());
        std::string modId = std::string(mod.getModID());
        std::string folderName = std::string(mod.getPathName());
        std::string modEntry = std::string((mod.*getEntry)());
        if (modEntry.empty()) continue;
        std::string scriptPath = RubyPaths::modScriptPath(folderName, modEntry);
        if (!fs::exists(scriptPath)) {
            _debugPrint("Couldnt find file for " + scriptPath + " for mod " + modName);
            continue;
        }

        if (prependPath) {
            std::string currentPath = luaState["package"]["path"];
            luaState["package"]["path"] = RubyPaths::modsRoot() + "/" + folderName + "/?.lua;" + currentPath;
        }

        sol::environment modEnv(luaState, sol::create, luaState.globals());
        modEnv["modId"] = modId;
        modEnv["pathName"] = folderName;
        sol::table modTable(luaState, sol::create);
        modTable["modId"] = modId;
        modTable["name"] = modName;
        modTable["version"] = std::string(mod.metadata.at("version"));
        modTable["folder"] = folderName;
        modTable["setData"] = [modId](const std::string &key, sol::object value, sol::this_state state) -> bool {
            ModStore::Get().setModData(modId, key, value, state);
            return true;
        };
        modTable["getData"] = [modId](const std::string &key, sol::this_state state) -> sol::object {
            return ModStore::Get().getModData(modId, key, state);
        };
        modTable["removeData"] = [modId](const std::string &key) -> bool {
            ModStore::Get().removeModData(modId, key);
            return true;
        };
        modEnv["mod"] = modTable;
        sol::protected_function_result result;
        try {
            result = luaState.safe_script_file(scriptPath, modEnv, sol::script_pass_on_error);
        } catch (const sol::error& e) {
            _debugPrint("Lua error in '"+folderName+"/"+modEntry+"' error: "+e.what());
            failedMods.push_back(name);
            continue;
        }

        if (result.valid()) {
            if (&luaState == &luaServer) {
                mod.setServerEnv(std::move(modEnv));
            } else {
                mod.setClientEnv(std::move(modEnv));
            }
            _debugPrint(modId + "'s '" + modEntry + "' has been loaded successfully");
        } else {
            _debugPrint("Lua error in '"+folderName+"/"+modEntry+"' error: "+safeLuaErrorText(result));
            failedMods.push_back(name);
        }
    }

    for (const auto& failedMod : failedMods) {
        _debugPrint("Unloading '"+failedMod+"' due to script error");
        mods_.erase(failedMod);
    }

    if (!failedMods.empty()) buildLoadOrder();
}

void Loader::refreshServerScripts() {
    refresh(luaServer,&RubyMod::getServerEntry,true);
}

void Loader::refreshClientScripts() {
    refresh(luaClient,&RubyMod::getClientEntry,false);
}

void Loader::execute(sol::environment& (RubyMod::*getEnv)(), std::string funcName, bool warn) {
    for (const std::string &name : loadOrder_) {
        auto modIterator = mods_.find(name);
        if (modIterator == mods_.end()) continue;
        RubyMod& mod = modIterator->second;
        std::string modName = std::string(mod.getName());
        sol::environment& env = (mod.*getEnv)();
        if (!env.valid()) continue;

        sol::protected_function fn = env[funcName];

        if (fn.valid()) {
            auto result = fn();
            if (!result.valid()) {
                _debugPrint(("Error in '"+modName+"': "+safeLuaErrorText(result)).c_str());
            }
        } else {
            if (warn) _debugPrint(("Mod "+modName+" must have a '"+funcName+"()' function in its global table, 'function "+modName+"."+funcName+"()' is missing"));
        }
    }
}

void Loader::executeServerScripts(std::string name, bool warn) {
    execute(&RubyMod::getServerEnv, name, warn);
}

void Loader::executeClientScripts(std::string name, bool warn) {
    execute(&RubyMod::getClientEnv, name, warn);
}
