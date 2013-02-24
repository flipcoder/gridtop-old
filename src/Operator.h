#ifndef _OPERATOR_H
#define _OPERATOR_H

#include "ICommand.h"
class WindowManager;
class Motion;

class Operator:
    public ICommand
{
    public:
        explicit Operator(const std::tuple<WindowManager*, std::string>& args):
            m_pWM(std::get<0>(args))
        {}
        virtual ~Operator() {}
        virtual bool pending() const override {
            return true;
        }
    protected:
        WindowManager* m_pWM;
};

#endif

