#ifndef _WINDOW_H
#define _WINDOW_H

#include <glm/glm.hpp>
#include "Common.h"
#include "Animation.h"
#include "Args.h"

class WindowManager;

class Window:
    public IRealtime
{
    public:
        explicit Window(WnckWindow* wnck_window, WindowManager* wm);
        virtual ~Window() {}

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
            //wnck_window_activate(m_pWindow);
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

    private:

        Animation<glm::vec2> m_Pos;
        Animation<glm::vec2> m_Size;

        mutable WnckWindow* m_pWindow;
        WindowManager* m_pWindowManager;

        mutable std::string m_Name;
};

#endif

