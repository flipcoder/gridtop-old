#ifndef _HINT_H
#define _HINT_H

#include "ICommand.h"
#include "Util.h"
class gtWindowManager;

class Hint:
    public ICommand
{
    public:

        explicit Hint(
            const std::tuple<gtWindowManager*,
            std::string>& args
        ) {}

        virtual ~Hint() {
            // TODO: pop any window manager states here
        }

        virtual bool pending() const override {
            return false;
        }

        virtual bool execute() override {}
    protected:
        gtWindowManager* m_pWM;
};

#endif

