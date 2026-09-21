#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "CommandsEnum.h"
#include "ModCommand.h"
#include "sol/sol.hpp"
class CommandDispatcher;
class CommandSender;
class MinecraftServer;
class ModCommandRegistry {
public:
    ~ModCommandRegistry();
    void init(MinecraftServer *server);
    int registerCommand(const std::string &name, int permissionLevel, sol::protected_function handler);
    bool performCommand(const std::string &name, CommandSender *sender, const std::vector<std::string> &args);
    int commandCount() const;

private:
    struct Entry {
        std::string name;
        int level;
        sol::protected_function handler;
    };

    static byteArray encodeArgs(const std::vector<std::string> &args);
    CommandDispatcher *m_dispatcher = nullptr;
    std::vector<std::unique_ptr<ModCommand>> m_commands;
    std::vector<Entry> m_entries;
    std::map<std::string, size_t> m_byName;
};
