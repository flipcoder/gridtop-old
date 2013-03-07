#ifndef _OPERATOR_H
#define _OPERATOR_H

#include "ICommand.h"
class WindowManager;
class Motion;

class Operator:
    public ICommand
{
    public:
        explicit Operator(const std::tuple<gtWindowManager*, std::string>& args):
            m_pWM(std::get<0>(args))
        {}
        virtual ~Operator() {}
        virtual bool pending() const override {
            return true;
        }
    protected:
        gtWindowManager* m_pWM;
};

#endif

