#ifndef _MOTION_H
#define _MOTION_H

#include "ICommand.h"
#include "Commands.h"
#include "Util.h"
#include "Motions.h"
class gtWindowManager;

class Motion:
    public ICommand
{
    public:

        explicit Motion(const std::tuple<gtWindowManager*, std::string>& args);

        virtual ~Motion() {}

        virtual bool pending() const override {
            return false;
        }

        virtual bool execute() override;

        unsigned bit() const {
            return Motion::bit(m_ID);
        }

        static unsigned bit(eMotion m) {
            return 1<<(unsigned)m;
        }

        template<class Container>
        static unsigned bits(Container motions) {
            unsigned r = 0;
            for(auto& m: motions)
                r |= m->bit();
            return r;
        }
        
        eMotion id() const {
            return m_ID;
        }

    protected:
        gtWindowManager* m_pWM;

    private:
        eMotion m_ID;
};

#endif

