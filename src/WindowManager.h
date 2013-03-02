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

class WindowManager:
    public IRealtime,
    public Daemon
{
    public:
        
        WindowManager(const Args& args);
        virtual ~WindowManager();

        virtual void run() override;

        enum class eContext: unsigned
        {
            WINDOW = 0, //default
        };

        /*
         * Perform the args action on the daemon side
         */
        virtual std::string action(const Args& args) override;

        void logic(Freq::Time t) override;

        std::vector<std::shared_ptr<Window>> windows() {
            return m_Windows;
        }

        std::shared_ptr<Window> active_window() {
            auto itr = std::find_if(
                ENTIRE(m_Windows),
                [](const std::shared_ptr<Window>& w){
                    return w->active();
                }
            );
            return (itr != m_Windows.end())?
                *itr:
                std::shared_ptr<Window>();
        }
        
        std::list<std::shared_ptr<ICommand>>& pending() {
            return m_Pending;
        }
        //std::shared_ptr<Operator>& default_operator() {
        //    return m_pDefaultOperator;
        //}

        void execute_default_operator();

        //std::vector<std::shared_ptr<Window>> window_matches(
        //    std::vector<Motion*> motions
        //);

        std::shared_ptr<Window> next_window(
            std::vector<Motion*> motions
        );

    private:

        WnckScreen* m_pScreen = nullptr;
        std::vector<std::shared_ptr<Window>> m_Windows;

        Freq::Timeline m_Timeline;
        Freq::Alarm m_PendAlarm;
        Freq::Time m_PendTime;

        Commands m_Commands;
        CommandResolver m_CommandResolver;

        std::list<std::shared_ptr<ICommand>> m_Pending;

        Freq m_Timer;
};

#endif

