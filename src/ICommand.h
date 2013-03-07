#ifndef _ICOMMAND_H
#define _ICOMMAND_H

#include "Factory.h"
#include "IRealtime.h"
#include <tuple>
#include <string>
#include <boost/optional.hpp>
class gtWindowManager;

class ICommand:
    public IRealtime
{
    public:
        /*
         *  Constructor is called before the WindowManager has record of this
         *  in its command stack.  So always assum e*self* is not included in
         *  stack during ctor, but included during execute()
         */
        virtual ~ICommand() {}
        //virtual bool expired() const = 0;

        /*
         * Whether or not the command can pend
         */
        virtual bool pending() const = 0;

        /*
         * Length of time to pend (optional)
         *
         * 0 (default ctor) time is considered infinite
         * Possibly modified by user settings
         *
         * operator bool is supported for Time()
         */
        Freq::Time pend() const {
            return Freq::Time();
        }

        /*
         * Command persists inside stack post-execution
         * Can be removed by another command or a stack clear
         */
        virtual bool persists() {
            return false;
        }

        virtual bool execute() {}
};

typedef Factory<ICommand, std::tuple<
    gtWindowManager*,
    std::string
>> Commands;

#endif

