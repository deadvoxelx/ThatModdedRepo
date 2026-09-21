#include "ModCommandRegistry.h"
#include "Common/EventSystem/EventBus.h"
#include "Common/ModPaths.h"
#include "CommandDispatcher.h"
#include "InputOutputStream.h"
#include "Loader.h"
#include "MinecraftServer.h"
#include "ModCommand.h"
#include "ServerPlayer.h"
class LoaderConsoleSender : public CommandSender {
public:
    void sendMessage(const wstring &message, ChatPacket::EChatPacketMessage, int, const wstring &) override
    {
        Loader::log(RubyPaths::toNarrow(message));
    }

    bool hasPermission(EGameCommand)
    {
        return true;
    }
};

ModCommandRegistry::~ModCommandRegistry()
{
    m_commands.clear();
    m_entries.clear();
    m_byName.clear();
    m_dispatcher = nullptr;
}

void ModCommandRegistry::init(MinecraftServer *server)
{
    m_dispatcher = (server != nullptr) ? server->getCommandDispatcher() : nullptr;
}

byteArray ModCommandRegistry::encodeArgs(const std::vector<std::string> &args)
{
    ByteArrayOutputStream baos;
    DataOutputStream dos(&baos);
    dos.writeInt(static_cast<int>(args.size()));
    for (const auto &arg : args)
    {
        dos.writeUTF(RubyPaths::toWide(arg));
    }

    return baos.toByteArray();
}

int ModCommandRegistry::registerCommand(const std::string &name, int permissionLevel, sol::protected_function handler)
{
    if (m_dispatcher == nullptr) return -1;
    if (name.empty() || !handler.valid()) return -1;
    if (m_byName.count(name) > 0) return -1;
    size_t index = m_entries.size();
    m_entries.push_back(Entry{ name, permissionLevel, std::move(handler) });
    m_byName[name] = index;
    auto command = std::make_unique<ModCommand>(
        static_cast<EGameCommand>(eGameCommand_COUNT + index),
        permissionLevel,
        [this, index](shared_ptr<CommandSender> source, const std::vector<std::string> &args) {
            if (index >= m_entries.size()) return;
            Entry &entry = m_entries[index];
            sol::protected_function fn = entry.handler;
            if (!fn.valid()) return;
            ServerPlayer *player = dynamic_cast<ServerPlayer *>(source.get());
            sol::protected_function_result result;
            if (player != nullptr)
            {
                result = fn(player, sol::as_args(args));
            }
            else
            {
                result = fn(sol::lua_nil, sol::as_args(args));
            }

            if (!result.valid())
            {
                Loader::_debugPrint("Error in command '" + entry.name + "': " + safeLuaErrorText(result));
            }
        });

    m_dispatcher->addCommand(command.get());
    m_commands.push_back(std::move(command));
    return static_cast<int>(index);
}

bool ModCommandRegistry::performCommand(const std::string &name, CommandSender *sender, const std::vector<std::string> &args)
{
    if (m_dispatcher == nullptr) return false;
    auto it = m_byName.find(name);
    if (it == m_byName.end()) return false;
    EGameCommand id = static_cast<EGameCommand>(eGameCommand_COUNT + it->second);
    byteArray data = encodeArgs(args);
    if (sender != nullptr)
    {
        shared_ptr<CommandSender> sharedSender(sender, [](CommandSender *) {});
        m_dispatcher->performCommand(sharedSender, id, data);
    }
    else
    {
        m_dispatcher->performCommand(std::make_shared<LoaderConsoleSender>(), id, data);
    }
    return true;
}

int ModCommandRegistry::commandCount() const
{
    return static_cast<int>(m_entries.size());
}
