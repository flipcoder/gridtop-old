#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "Args.h"
#include "Common.h"
#include "Daemon.h"
#include "Window.h"
#include "Freq.h"
#include "ICommand.h"
#include "CommandResolver.h"
#include "Operator.h"
#include "Util.h"
#include <memory>
#include <vector>
#include <list>
#include <string>

class gtWindowManager:
    public IRealtime,
    public Daemon
{
    public:
        
        gtWindowManager(const Args& args);
        virtual ~gtWindowManager();

        virtual void run() override;

        enum class eContext: unsigned
        {
            WINDOW = 0, //default
            DISPLAY,
            WORKSPACE
        };

        /*
         * Perform the args action on the daemon side
         */
        virtual std::string action(const Args& args) override;

        void logic(Freq::Time t) override;

        std::vector<std::shared_ptr<gtWindow>> windows() {
            return m_Windows;
        }

        std::shared_ptr<gtWindow> active_window() {
            auto itr = std::find_if(
                ENTIRE(m_Windows),
                [](const std::shared_ptr<gtWindow>& w){
                    return w->active();
                }
            );
            return (itr != m_Windows.end())?
                *itr:
                std::shared_ptr<gtWindow>();
        }
        
        std::list<std::shared_ptr<ICommand>>& pending() {
            return m_Pending;
        }
        //std::shared_ptr<Operator>& default_operator() {
        //    return m_pDefaultOperator;
        //}

        void execute_default_operator();

        //std::vector<std::shared_ptr<gtWindow>> window_matches(
        //    std::vector<Motion*> motions
        //);

        /*
         * Return the next window match, given the provided motions and the
         * window manager's current current targeting algorithm
         */
        std::shared_ptr<gtWindow> next_window(
            std::vector<Motion*> motions
        );

        GETSET(eContext, context, m_Context);

        glm::vec2 workspace_size() const {
            assert(m_pScreen);
            return glm::vec2(
                wnck_screen_get_width(m_pScreen),
                wnck_screen_get_height(m_pScreen)
            );
        }

    private:

        eContext m_Context = eContext::WINDOW;

        mutable WnckScreen* m_pScreen = nullptr;
        std::vector<std::shared_ptr<gtWindow>> m_Windows;

        Freq::Timeline m_Timeline;
        Freq::Alarm m_PendAlarm;
        Freq::Time m_PendTime;

        Commands m_Commands;
        CommandResolver m_CommandResolver;

        std::list<std::shared_ptr<ICommand>> m_Pending;

        Freq m_Timer;
};

#endif

