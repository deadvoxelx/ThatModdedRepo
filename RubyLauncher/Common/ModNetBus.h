#pragma once
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "sol/sol.hpp"
class ServerPlayer;
struct ModNetEvent {
    ServerPlayer* player;
    std::string channel;
    std::string data;
};

inline byteArray stringToByteArray(const std::string& text)
{
    if (text.empty()) return byteArray();
    byteArray bytes(static_cast<unsigned int>(text.size()));
    memcpy(bytes.data, text.data(), text.size());
    return bytes;
}

inline std::string byteArrayToString(const byteArray& bytes)
{
    if (bytes.data == nullptr || bytes.length == 0) return std::string();
    return std::string(reinterpret_cast<const char*>(bytes.data), bytes.length);
}

class ModNetBus {
public:
    using Listener = sol::protected_function;
    static ModNetBus& Get()
    {
        static ModNetBus instance;
        return instance;
    }

    void listenServer(const std::string& channel, Listener listener);
    void listenClient(const std::string& channel, Listener listener);
    void clearServer();
    void clearClient();
    void fireServer(ServerPlayer* player, const std::string& channel, const std::string& data);
    void fireClient(const std::string& channel, const std::string& data);

private:
    ModNetBus() = default;
    std::map<std::string, std::vector<Listener>> m_serverListeners;
    std::map<std::string, std::vector<Listener>> m_clientListeners;
};
