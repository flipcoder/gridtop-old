#ifndef _FOCUS_OPERATOR_H
#define _FOCUS_OPERATOR_H

#include "Operator.h"
#include <glm/glm.hpp>

class FillOperator:
    public Operator
{
    public:
        explicit FillOperator(const std::tuple<
            WindowManager*,
            std::string>& args
        ):
            Operator(args)
        {}

        virtual ~FillOperator() {}

        void fill_window(const std::vector<Motion*>& motions);

        virtual bool execute() override;
};

#endif

