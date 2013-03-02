#ifndef _ACTION_H
#define _ACTION_H

#include "ICommand.h"
class WindowManager;

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
        explicit Action(const std::tuple<WindowManager*, std::string>& args):
            m_pWM(std::get<0>(args))
        {}

        virtual ~Action() {}
        virtual bool pending() const override {
            return false;
        }
        virtual bool execute() {}
    private:
        WindowManager* m_pWM;
};

#endif

