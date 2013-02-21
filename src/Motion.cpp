#include "Motion.h"

Motion :: Motion(const std::tuple<WindowManager*, std::string>& args):
    m_pWM(std::get<0>(args))
{
    
}

