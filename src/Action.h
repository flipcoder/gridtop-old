#ifndef _ACTION_H
#define _ACTION_H

#include "ICommand.h"
class gtWindowManager;

/*
 *  Special actions that don't fit into operators or motions
 *
 *  Examples:
 *      - Cancelation of all queued operators
 *      - Modal changes
 */

class Action:
    public ICommand
{
    public:
        explicit Action(const std::tuple<gtWindowManager*, std::string>& args):
            m_pWM(std::get<0>(args))
        {}

        virtual ~Action() {}
        virtual bool pending() const override {
            return false;
        }
        virtual bool execute() {}
    private:
        gtWindowManager* m_pWM;
};

#endif

