#ifndef _OPERATORS_DEF_H_G6UPB6OZ
#define _OPERATORS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>

enum class eOperator : unsigned
{
    FOCUS = 0, // default
    //FOCUS = (unsigned)eCommand::OPERATORS_START,
    MAX
};

const std::vector<std::string> g_OperatorString = {
    "focus"
};


#endif

