#pragma once
#include <functional>
#include <string>
#include <vector>
#include "CommandsEnum.h"
#include "Command.h"
class CommandSender;
class ModCommand : public Command {
public:
    using Handler = std::function<void(shared_ptr<CommandSender> source, const std::vector<std::string>& args)>;
    ModCommand(EGameCommand id, int permissionLevel, Handler handler);
    EGameCommand getId() override;
    int getPermissionLevel() override;
    void execute(shared_ptr<CommandSender> source, byteArray commandData) override;

private:
    EGameCommand m_id;
    int m_permissionLevel;
    Handler m_handler;
};
