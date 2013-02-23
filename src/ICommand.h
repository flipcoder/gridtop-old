#ifndef _ICOMMAND_H
#define _ICOMMAND_H

#include "Factory.h"
#include "IRealtime.h"
#include <tuple>
#include <string>
class WindowManager;

class ICommand:
    public IRealtime
{
    public:
        /*
         *  Constructor is called before the WindowManager has record of this
         *  in its command stack.  So assume self is not included.
         */
        virtual ~ICommand() {}
        //virtual bool expired() const = 0;
        virtual bool pend() const = 0;
};

typedef Factory<ICommand, std::tuple<
    WindowManager*,
    std::string
>> Commands;

#endif

