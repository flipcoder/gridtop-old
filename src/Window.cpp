#include "Window.h"
#include "WindowManager.h"
#include "Log.h"
using namespace std;
using namespace glm;

Window :: Window(WnckWindow* wnck_window, WindowManager* wm):
    m_pWindow(wnck_window),
    m_pWindowManager(wm)
{
    // TODO: get properties from WnckWindow

    //LOGf("window name: %s", wnck_window_get_name(m_pWindow));
}

void Window :: logic(Freq::Time t)
{
    // process animation first
    m_Pos.logic(t);
    m_Size.logic(t);

    if(m_Pos.elapsed() || m_Size.elapsed())
    {
        // if animation is still going
        // TODO: use signals instead of querying
        glm::ivec2 ipos;
        glm::ivec2 isize;
        wnck_window_get_geometry(
            m_pWindow,
            &ipos[0], &ipos[1],
            &isize[0], &isize[1]
        );
        if(m_Pos.elapsed())
            m_Pos = glm::vec2((float)ipos.x, (float)ipos.y);
        if(m_Size.elapsed())
            m_Size = glm::vec2((float)isize.x, (float)isize.y);
        
    }

    // read positions and sizes from animation
    if(!m_Size.elapsed())
    {
        vec2 v = m_Size.get();
        ivec2 iv = ivec2(v.x, v.y);

        wnck_window_set_geometry(
            m_pWindow,
            WNCK_WINDOW_GRAVITY_CURRENT,
            WnckWindowMoveResizeMask((int)WNCK_WINDOW_CHANGE_WIDTH | (int)WNCK_WINDOW_CHANGE_HEIGHT),
            0,0, // unused
            v.x, v.y
        );
    }
    if(!m_Pos.elapsed())
    {
        vec2 v = m_Pos.get();
        ivec2 iv = ivec2(v.x, v.y);

        wnck_window_set_geometry(
            m_pWindow,
            WNCK_WINDOW_GRAVITY_CURRENT,
            WnckWindowMoveResizeMask((int)WNCK_WINDOW_CHANGE_X | (int)WNCK_WINDOW_CHANGE_Y),
            v.x, v.y,
            0,0 // unused
        );
    }
}

