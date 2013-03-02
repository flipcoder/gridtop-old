#ifndef _ACTIONS_DEF_H_G6UPB6OZ
#define _ACTIONS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>

enum class eAction : unsigned
{
    DECORATE = 0, // default
    ENABLE_DECORATE = 1, // default
    DISABLE_DECORATE = 2, // default

    MAX
};

const std::vector<std::string> g_ActionString = {
    "decorate",
    "+decorate",
    "-decorate",
};

#endif

