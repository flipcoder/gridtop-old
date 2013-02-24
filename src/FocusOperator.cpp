#include "FocusOperator.h"
#include "WindowManager.h"
#include "Motion.h"

void FocusOperator :: execute()
{
    unsigned motions = 0;
    auto pending = m_pWM->pending();
    for(auto& cmd: pending)
    {
        auto motion = dynamic_cast<Motion*>(cmd.get());
        if(motion)
            motions |= motion->bit();
    }

    if(!motions)
        return;

    
}

