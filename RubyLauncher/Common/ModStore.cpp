#include "Common/ModStore.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>
#include "Common/ModPaths.h"
#include "Loader.h"
namespace fs = std::filesystem;
ModStore& ModStore::Get()
{
    static ModStore instance;
    return instance;
}

std::string ModStore::fileFor(const std::string& modId) const
{
    fs::path modsRoot(RubyPaths::modsRoot());
    fs::path dataDir = modsRoot.parent_path() / "ruby_data";
    return (dataDir / (modId + ".json")).string();
}

nlohmann::json& ModStore::docFor(const std::string& modId)
{
    auto it = m_docs.find(modId);
    if (it != m_docs.end()) return it->second;
    nlohmann::json doc;
    std::ifstream file(fileFor(modId));
    if (file.is_open())
    {
        try
        {
            file >> doc;
        }
        catch (const std::exception& e)
        {
            Loader::_debugPrint("failed to read data for mod '" + modId + "': " + e.what());
        }
    }
    if (!doc.is_object()) doc = nlohmann::json::object();
    return m_docs.emplace(modId, std::move(doc)).first->second;
}

void ModStore::save(const std::string& modId)
{
    auto it = m_docs.find(modId);
    if (it == m_docs.end()) return;
    try
    {
        fs::path modsRoot(RubyPaths::modsRoot());
        fs::path dataDir = modsRoot.parent_path() / "ruby_data";
        fs::create_directories(dataDir);
        std::ofstream out(fileFor(modId));
        out << it->second.dump(1, '\t');
    }
    catch (const std::exception& e)
    {
        Loader::_debugPrint("failed to save data for mod '" + modId + "': " + e.what());
    }
}

void ModStore::luaToJson(sol::object value, nlohmann::json& out, lua_State* L, const std::string& context)
{
    switch (value.get_type())
    {
    case sol::type::none:
    case sol::type::nil:
        out = nullptr;
        return;
    case sol::type::boolean:
        out = value.as<bool>();
        return;
    case sol::type::number:
    {
        double d = value.as<double>();
        if (std::floor(d) == d && std::abs(d) <= 9007199254740992.0)
        {
            out = static_cast<int64_t>(d);
        }
        else
        {
            out = d;
        }
        return;
    }
    case sol::type::string:
        out = value.as<std::string>();
        return;
    case sol::type::table:
    {
        sol::table t = value.as<sol::table>();
        std::vector<std::pair<sol::object, sol::object>> entries;
        bool isArray = true;
        lua_Integer index = 1;
        t.for_each([&](sol::object key, sol::object val) {
            entries.emplace_back(key, val);
            if (isArray && key.get_type() == sol::type::number)
            {
                if (key.as<lua_Integer>() != index) isArray = false;
            }
            else
            {
                isArray = false;
            }
            ++index;
        });

        if (isArray)
        {
            out = nlohmann::json::array();
            for (auto& entry : entries)
            {
                nlohmann::json j;
                luaToJson(entry.second, j, L, context);
                out.push_back(std::move(j));
            }
        }
        else
        {
            out = nlohmann::json::object();
            for (auto& entry : entries)
            {
                if (entry.first.get_type() != sol::type::string)
                {
                    luaL_error(L, "%s: table keys must be strings", context.c_str());
                }
                nlohmann::json j;
                luaToJson(entry.second, j, L, context);
                out[entry.first.as<std::string>()] = std::move(j);
            }
        }
        return;
    }
    default:
        luaL_error(L, "%s: unsupported value type %s", context.c_str(), lua_typename(L, static_cast<int>(value.get_type())));
        return;
    }
}

sol::object ModStore::jsonToLua(sol::this_state state, const nlohmann::json& json)
{
    lua_State* L = state.lua_state();
    switch (json.type())
    {
    case nlohmann::json::value_t::null:
        return sol::lua_nil_t{};
    case nlohmann::json::value_t::boolean:
        return sol::make_object(L, json.get<bool>());
    case nlohmann::json::value_t::number_integer:
    case nlohmann::json::value_t::number_unsigned:
        return sol::make_object(L, json.get<int64_t>());
    case nlohmann::json::value_t::number_float:
        return sol::make_object(L, json.get<double>());
    case nlohmann::json::value_t::string:
        return sol::make_object(L, json.get<std::string>());
    case nlohmann::json::value_t::array:
    {
        sol::table t(L, sol::create);
        int i = 1;
        for (const auto& el : json)
        {
            t[i++] = jsonToLua(state, el);
        }
        return t;
    }
    case nlohmann::json::value_t::object:
    {
        sol::table t(L, sol::create);
        for (const auto& el : json.items())
        {
            t[el.key()] = jsonToLua(state, el.value());
        }
        return t;
    }
    default:
        return sol::lua_nil_t{};
    }
}

void ModStore::setModData(const std::string& modId, const std::string& key, sol::object value, sol::this_state state)
{
    nlohmann::json& doc = docFor(modId);
    nlohmann::json converted;
    luaToJson(value, converted, state.lua_state(), "setData");
    doc[key] = std::move(converted);
    save(modId);
}

sol::object ModStore::getModData(const std::string& modId, const std::string& key, sol::this_state state)
{
    nlohmann::json& doc = docFor(modId);
    if (!doc.contains(key)) return sol::lua_nil_t{};
    return jsonToLua(state, doc[key]);
}

void ModStore::removeModData(const std::string& modId, const std::string& key)
{
    nlohmann::json& doc = docFor(modId);
    if (!doc.contains(key)) return;
    doc.erase(key);
    save(modId);
}

void ModStore::setPlayerData(const std::string& modId, const std::string& player, const std::string& key, sol::object value, sol::this_state state)
{
    nlohmann::json& doc = docFor(modId);
    nlohmann::json& players = doc["players"];
    if (!players.is_object()) players = nlohmann::json::object();
    nlohmann::json& playerEntry = players[player];
    if (!playerEntry.is_object()) playerEntry = nlohmann::json::object();
    nlohmann::json converted;
    luaToJson(value, converted, state.lua_state(), "setData");
    playerEntry[key] = std::move(converted);
    save(modId);
}

sol::object ModStore::getPlayerData(const std::string& modId, const std::string& player, const std::string& key, sol::this_state state)
{
    nlohmann::json& doc = docFor(modId);
    if (!doc.contains("players") || !doc["players"].is_object()) return sol::lua_nil_t{};
    auto& players = doc["players"];
    if (!players.contains(player) || !players[player].is_object()) return sol::lua_nil_t{};
    auto& playerEntry = players[player];
    if (!playerEntry.contains(key)) return sol::lua_nil_t{};
    return jsonToLua(state, playerEntry[key]);
}
