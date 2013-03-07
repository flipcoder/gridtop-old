#ifndef _FOCUS_OPERATOR_H
#define _FOCUS_OPERATOR_H

#include "Operator.h"
#include <glm/glm.hpp>

class FocusOperator:
    public Operator
{
    public:
        explicit FocusOperator(const std::tuple<
            gtWindowManager*,
            std::string>& args
        ):
            Operator(args)
        {}

        virtual ~FocusOperator() {}

        void focus_window(const std::vector<Motion*>& motions);

        virtual bool execute() override;
};

#endif

