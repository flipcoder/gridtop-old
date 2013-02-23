#ifndef _MOTION_H
#define _MOTION_H

#include "ICommand.h"
#include "Util.h"
class WindowManager;

class Motion:
    public ICommand
{
    public:

        explicit Motion(const std::tuple<WindowManager*, std::string>& args);

        virtual ~Motion() {}

        virtual bool pend() const override {
            return true;
        }

    private:
        WindowManager* m_pWM;
};

#endif

