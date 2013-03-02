#ifndef _MOVE_OPERATOR_H
#define _MOVE_OPERATOR_H

#include "Operator.h"
#include <glm/glm.hpp>

class MoveOperator:
    public Operator
{
    public:
        explicit MoveOperator(const std::tuple<
            WindowManager*,
            std::string>& args
        ):
            Operator(args)
        {}

        virtual ~MoveOperator() {}

        void move_window(const std::vector<Motion*>& motions);

        virtual bool execute() override;
};

#endif

