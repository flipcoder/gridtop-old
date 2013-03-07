#ifndef _WINDOW_H
#define _WINDOW_H

#include <glm/glm.hpp>
#include "Common.h"
#include "Animation.h"
#include "Args.h"

class gtWindowManager;

class gtWindow:
    public IRealtime
{
    public:
        explicit gtWindow(WnckWindow* wnck_window, gtWindowManager* wm);
        virtual ~gtWindow() {}

        std::string name() const {
            if(!wnck_window_has_name(m_pWindow)) {
                m_Name = std::string();
                return m_Name;
            }
            m_Name = wnck_window_get_name(m_pWindow);
            return m_Name;
        }

        glm::vec2 pos() {
            return m_Pos.get();
        }

        glm::vec2 size() const {
            return m_Size.get();
        }

        void pos(glm::vec2 v, Freq::Time t = Freq::Time()) {
            m_Pos.stop(v, t,
                [](const glm::vec2& a, const glm::vec2& b, float t){
                    return a+(b-a)*t;
                }
            );
        }

        void activate(){
            wnck_window_activate(
                m_pWindow,
                gdk_x11_display_get_user_time(gdk_display_get_default())
            );
        }

        bool active() const {
            return wnck_window_is_active(m_pWindow);
        }

        bool minimized() const {
            return wnck_window_is_minimized(m_pWindow);
        }
        void minimize() {
        }

        //bool inside(
        //    const glm::vec2& pos,
        //    glm::vec2 size
        //) const {
            
        //}

        void logic(Freq::Time t) override;

        GETSET(bool, respects_border_gap, m_bRespectsBorderGap);

        glm::vec2 center() const {
            return m_Pos.get() + m_Size.get()/2.0f;
        }

        glm::vec2 upper_left() const {
            return m_Pos.get();
        }
        glm::vec2 lower_right() const {
            return m_Pos.get() + m_Size.get();
        }

        void snap(Freq::Time t = Freq::Time()) {
            // TODO: snap to grid
            m_bRespectsBorderGap = true;
        }

    private:

        Animation<glm::vec2> m_Pos;
        Animation<glm::vec2> m_Size;

        mutable WnckWindow* m_pWindow;
        gtWindowManager* m_pWindowManager;

        mutable std::string m_Name;

        bool m_bRespectsBorderGap = false;
};

#endif

