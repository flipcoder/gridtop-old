#ifndef _COMMANDRESOLVER_H
#define _COMMANDRESOLVER_H

#include "ICommand.h"
#include "Commands.h"
class WindowManager;

class CommandResolver
{
    public:
        CommandResolver(Commands* factory);
        virtual ~CommandResolver(){}

        unsigned resolve(
            const std::tuple<WindowManager*,
            std::string>& args
        ) const;

    private:
        Commands* m_pCommands;
};

#endif

