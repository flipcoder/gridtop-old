#include "MoveOperator.h"
#include "WindowManager.h"
#include "Motion.h"
#include "Util.h"
using namespace std;

bool MoveOperator :: execute()
{
    // TODO: check for context hint and run the specific 
    // function (window, workspace?)

    int move_op_count = 0;

    vector<Motion*> motions;
    auto pending = m_pWM->pending();
    for(auto& cmd: pending)
    {
        auto move_op = dynamic_cast<MoveOperator*>(cmd.get());
        if(move_op)
            ++move_op_count;

        auto motion = dynamic_cast<Motion*>(cmd.get());
        if(motion)
            motions.push_back(motion);
    }

    // TODO: read wm states
    if(move_op_count > 1)
        snap_window();
    if(!motions.empty())
        move_window(motions);
    
    return true;
}

void MoveOperator :: move_window(const std::vector<Motion*>& motions)
{
    unsigned motion_bits = Motion::bits(motions);
    //if(motion_bits)
    //{
        // TODO: snap window to grid
        auto active = m_pWM->active_window();
        if(active)
            active->snap();
    //}

    // TODO: snap active window to grid
    // TODO: move window along grid in motions' direction(s)
}

void MoveOperator :: snap_window()
{
}

