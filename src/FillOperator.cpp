#include "FillOperator.h"
#include "WindowManager.h"
#include "Motion.h"
#include "Util.h"
using namespace std;

bool FillOperator :: execute()
{
    // TODO: check for context hint and run the specific 
    // function (window, workspace?)
    
    vector<Motion*> motions;
    auto pending = m_pWM->pending();
    for(auto& cmd: pending)
    {
        auto motion = dynamic_cast<Motion*>(cmd.get());
        motions.push_back(motion);
        //if(motion)
        //    motions |= motion->bit();
    }

    if(motions.empty())
        return false;

    // TODO: read wm states
    fill_window(motions);
    
    return true;
}

void FillOperator :: fill_window(const std::vector<Motion*>& motions)
{
    
}

