#ifndef _OPERATORS_DEF_H_G6UPB6OZ
#define _OPERATORS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>

enum class eOperator : unsigned
{
    FOCUS = 0, // default
    MOVE,

    //RESIZE, // prefix +/-

    //CLOSE,

    MAX
};

const std::vector<std::string> g_OperatorString = {
    "focus",
    "move",

    //"resize",

    //"close"
};


#endif

