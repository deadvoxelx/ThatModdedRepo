#pragma once
#include <map>
#include <string>
#include "json/json.hpp"
#include "sol/sol.hpp"
class ModStore {
public:
    static ModStore& Get();
    void setModData(const std::string& modId, const std::string& key, sol::object value, sol::this_state state);
    sol::object getModData(const std::string& modId, const std::string& key, sol::this_state state);
    void removeModData(const std::string& modId, const std::string& key);
    void setPlayerData(const std::string& modId, const std::string& player, const std::string& key, sol::object value, sol::this_state state);
    sol::object getPlayerData(const std::string& modId, const std::string& player, const std::string& key, sol::this_state state);
    void save(const std::string& modId);

private:
    ModStore() = default;
    nlohmann::json& docFor(const std::string& modId);
    std::string fileFor(const std::string& modId) const;
    static void luaToJson(sol::object value, nlohmann::json& out, lua_State* L, const std::string& context);
    static sol::object jsonToLua(sol::this_state state, const nlohmann::json& json);
    std::map<std::string, nlohmann::json> m_docs;
};
