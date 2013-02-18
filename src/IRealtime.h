#ifndef _IREALTIME_H
#define _IREALTIME_H

#include "Freq.h"

class IRealtime
{
    public:
        virtual ~IRealtime() {}
        virtual void logic(Freq::Time) {}
};

#endif

