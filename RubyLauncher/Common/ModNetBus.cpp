#include "Common/ModNetBus.h"
#include <utility>
#include "Common/EventSystem/EventBus.h"
void ModNetBus::listenServer(const std::string& channel, Listener listener)
{
    m_serverListeners[channel].push_back(std::move(listener));
}

void ModNetBus::listenClient(const std::string& channel, Listener listener)
{
    m_clientListeners[channel].push_back(std::move(listener));
}

void ModNetBus::clearServer()
{
    m_serverListeners.clear();
}

void ModNetBus::clearClient()
{
    m_clientListeners.clear();
}

void ModNetBus::fireServer(ServerPlayer* player, const std::string& channel, const std::string& data)
{
    auto it = m_serverListeners.find(channel);
    if (it == m_serverListeners.end()) return;
    ModNetEvent event{ player, channel, data };
    for (auto& fn : it->second)
    {
        if (!fn.valid()) continue;
        auto result = fn(&event);
        if (!result.valid())
        {
            fprintf(stderr, "[Lua] Error in net listener '%s': %s\n", channel.c_str(), safeLuaErrorText(result).c_str());
        }
    }
}

void ModNetBus::fireClient(const std::string& channel, const std::string& data)
{
    auto it = m_clientListeners.find(channel);
    if (it == m_clientListeners.end()) return;
    ModNetEvent event{ nullptr, channel, data };
    for (auto& fn : it->second)
    {
        if (!fn.valid()) continue;
        auto result = fn(&event);
        if (!result.valid())
        {
            fprintf(stderr, "[Lua] Error in net listener '%s': %s\n", channel.c_str(), safeLuaErrorText(result).c_str());
        }
    }
}
