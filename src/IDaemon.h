#ifndef _IDAEMON_H_EJV0YIN6
#define _IDAEMON_H_EJV0YIN6

#include "Args.h"

class IDaemon
{
    public:
        virtual ~IDaemon() {}
        virtual std::string action(const Args& args) = 0;
    private:
};

#endif
