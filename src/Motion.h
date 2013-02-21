#ifndef _MOTION_H
#define _MOTION_H

#include "ICommand.h"
#include "Util.h"
class WindowManager;

class Motion:
    public ICommand
{
    public:

        enum class eMotion : uint32_t {
            LEFT = util::bit(0),
            RIGHT = util::bit(1),
            UP = util::bit(2),
            DOWN = util::bit(3),
            LEFT_EDGE = util::bit(4),
            RIGHT_EDGE = util::bit(5),
            TOP = util::bit(6),
            BOTTOM = util::bit(7),
        };

        explicit Motion(const std::tuple<WindowManager*, std::string>& args);

        virtual ~Motion() {}
        virtual bool expired() const override {
            return false;
        }
    private:
        WindowManager* m_pWM;
};

#endif

