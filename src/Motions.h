#ifndef _MOTIONS_DEF_H_G6UPB6OZ
#define _MOTIONS_DEF_H_G6UPB6OZ

#include <string>
#include <vector>

enum class eMotion : unsigned
{
    SELF = 0, // default
    LEFT,
    RIGHT,
    UP,
    DOWN,
    LEFT_EDGE,
    RIGHT_EDGE,
    TOP,
    BOTTOM,
    MAX
};

const std::vector<std::string> g_MotionString = {
    "self",
    "left",
    "right",
    "up",
    "down",
    "left-edge",
    "right-edge",
    "top",
    "bottom"
};


#endif

