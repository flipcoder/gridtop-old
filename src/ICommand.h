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
        virtual ~ICommand() {}
        virtual bool expired() const = 0;
};

typedef Factory<ICommand, std::tuple<
    WindowManager*,
    std::string
>> Commands;

#endif

