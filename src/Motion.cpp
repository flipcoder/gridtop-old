#include "Motion.h"

Motion :: Motion(const std::tuple<WindowManager*, std::string>& args):
    m_pWM(std::get<0>(args))
{
    // search command stack for operator, otherwise, this is a focus event
    // (in which case, we use WindowManager::default_operator()
}

