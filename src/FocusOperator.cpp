#include "FocusOperator.h"
#include "WindowManager.h"
#include "Motion.h"
#include "Log.h"
#include "Util.h"
using namespace std;

bool FocusOperator :: execute()
{
    // TODO: check for context hint and run the specific 
    // function (window, workspace?)
    
    vector<Motion*> motions;
    auto pending = m_pWM->pending();
    for(auto& cmd: pending)
    {
        auto motion = dynamic_cast<Motion*>(cmd.get());
        if(motion)
            motions.push_back(motion);
    }

    if(motions.empty())
        return false;

    // TODO: read wm states
    focus_window(motions);
    
    return true;
}

void FocusOperator :: focus_window(const std::vector<Motion*>& motions)
{
    auto window = m_pWM->next_window(motions);
    if(window)
        window->activate();
}

