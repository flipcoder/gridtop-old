#ifndef _COMMANDRESOLVER_H
#define _COMMANDRESOLVER_H

#include "ICommand.h"
#include "Commands.h"
#include <map>
class gtWindowManager;

class CommandResolver
{
    public:
        CommandResolver(Commands* factory);
        virtual ~CommandResolver(){}

        unsigned resolve(
            const std::tuple<gtWindowManager*,
            std::string>& args
        ) const;

    private:

        void init_operators(unsigned& offset);
        void init_motions(unsigned& offset);
        void init_hints(unsigned& offset);
        void init_actions(unsigned& offset);

        Commands* m_pCommands;
        std::map<std::string, unsigned> m_CommandString;
};

#endif

