#ifndef _FOCUS_OPERATOR_H
#define _FOCUS_OPERATOR_H

#include "Operator.h"

class FocusOperator:
    public Operator
{
    public:
        explicit FocusOperator(const std::tuple<
            WindowManager*,
            std::string>& args
        ):
            Operator(args)
        {}

        virtual ~FocusOperator() {}

        virtual void execute() override;
};

#endif

