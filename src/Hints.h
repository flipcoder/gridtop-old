#ifndef _HINTS_DEF_H_G6UPB6OZ
#define _HINTS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>

enum class eHint : unsigned
{
    // context hints
    WINDOW = 0, // default
    WORKSPACE,

    // subject hints
    SELF,
    OTHERS,
    ALL,

    MAX
};

const std::vector<std::string> g_HintString = {
    "window",
    "workspace",

    "self",
    "others",
    "all"
};


#endif

