#include "MoveOperator.h"
#include "WindowManager.h"
#include "Motion.h"
#include "Util.h"
using namespace std;

bool MoveOperator :: execute()
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
    move_window(motions);
    
    return true;
}

void MoveOperator :: move_window(const std::vector<Motion*>& motions)
{
    unsigned motion_bits = Motion::bits(motions);
    if(!motion_bits || motion_bits == Motion::bit(eMotion::SELF))
    {
        // TODO: snap window to grid
        auto active = m_pWM->active_window();
        if(active)
            active->snap();
    }

    // TODO: snap active window to grid
    // TODO: move window along grid in motions' direction(s)
}

