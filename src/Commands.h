#ifndef _COMMANDS_DEF_H_G6UPB6OZ
#define _COMMANDS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>

enum class eCommand : unsigned int
{
    MOTION,
    OPERATOR,
    MAX
};

const std::vector<std::string> g_CommandNames = {
    "motion"
    "operator",
};


#endif

