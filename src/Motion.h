#ifndef _MOTION_H
#define _MOTION_H

#include "ICommand.h"
#include "Commands.h"
#include "Util.h"
class WindowManager;

class Motion:
    public ICommand
{
    public:

        explicit Motion(const std::tuple<WindowManager*, std::string>& args);

        virtual ~Motion() {}

        virtual bool pending() const override {
            return false;
        }

        virtual void execute() override;

        unsigned bit() const {
            return Motion::bit(m_ID);
        }
        eMotion id() const {
            return m_ID;
        }
        static unsigned bit(eMotion m) {
            return 1<<(unsigned)m;
        }
    protected:
        WindowManager* m_pWM;
    private:
        eMotion m_ID;
};

#endif

