#include "ModCommand.h"
#include "Common/ModPaths.h"
#include "InputOutputStream.h"
ModCommand::ModCommand(EGameCommand id, int permissionLevel, Handler handler)
	: m_id(id), m_permissionLevel(permissionLevel), m_handler(std::move(handler))
{
}

EGameCommand ModCommand::getId()
{
    return m_id;
}

int ModCommand::getPermissionLevel()
{
    return m_permissionLevel;
}

void ModCommand::execute(shared_ptr<CommandSender> source, byteArray commandData)
{
    std::vector<std::string> args;
    if (commandData.data != nullptr && commandData.length > 0)
    {
        ByteArrayInputStream bais(commandData);
        DataInputStream dis(&bais);
        int argc = dis.readInt();
        for (int i = 0; i < argc; ++i)
        {
            args.push_back(RubyPaths::toNarrow(dis.readUTF()));
        }
    }

    m_handler(source, args);
}
