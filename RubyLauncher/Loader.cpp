#include "Loader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>

#include "Common/ModPaths.h"
#include "Common/EventSystem/EventBus.h"
#include "Lua/LuaBindings.h"

class MinecraftServer;

namespace fs = std::filesystem;
using string = std::string;

Loader::Loader() {
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

void Loader::registerServerFunctions(MinecraftServer *server) {
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

        for ( const auto & metadata : requiredMetadata ) {
            if (!hasMetadata(json, metadata)) {
                _debugPrint((modPath+" missing metadata '"+metadata+"' add it to your manifest.json"));
                missingRequired = true;
                break;
            }
            m.metadata[metadata] = json[metadata].get<std::string>();
        }

        if (missingRequired) continue;

        string folderName = modEntry.path().filename().string();
        m.setPathName(folderName);
        mods_[std::string(m.getName())] = m;
    }
}

void Loader::refresh(sol::state& luaState,std::string_view (RubyMod::*getEntry)() const,bool prependPath) {
    std::vector<RubyMod> failedMods;

    for (auto& [name,mod] : mods_) {
        std::string modName = std::string(mod.getName());
        std::string modId = std::string(mod.getModID());
        std::string folderName = std::string(mod.getPathName());
        std::string modEntry = std::string((mod.*getEntry)());

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

        sol::protected_function_result result;
        try {
            result = luaState.safe_script_file(scriptPath, modEnv, sol::script_pass_on_error);
        } catch (const sol::error& e) {
            _debugPrint("Lua error in '"+folderName+"/"+modEntry+"' error: "+e.what());
            failedMods.push_back(mod);
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
            failedMods.push_back(mod);
        }
    }

    for (const auto& failedMod : failedMods) {
        _debugPrint("Unloading '"+std::string(failedMod.getName())+"' due to script error");
        mods_.erase(std::string(failedMod.getName()));
    }
}

void Loader::refreshServerScripts() {
    refresh(luaServer,&RubyMod::getServerEntry,true);
}

void Loader::refreshClientScripts() {
    refresh(luaClient,&RubyMod::getClientEntry,false);
}

void Loader::execute(sol::environment& (RubyMod::*getEnv)(), std::string funcName, bool warn) {
    for (auto& [name, mod] : mods_) {
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
