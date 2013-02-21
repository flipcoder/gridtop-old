#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "Args.h"
#include "Common.h"
#include "Daemon.h"
#include "Window.h"
#include "Freq.h"
#include "ICommand.h"
#include "CommandResolver.h"
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

        /*
         * Perform the args action on the daemon side
         */
        virtual std::string action(const Args& args) override;

        void logic(Freq::Time t) override;
        
        std::list<std::shared_ptr<ICommand>>& pending() {
            return m_Pending;
        }
        const std::list<std::shared_ptr<ICommand>>& commands() const {
            return m_Pending;
        }

    private:

        WnckScreen* m_pScreen = nullptr;
        std::vector<std::shared_ptr<Window>> m_Windows;

        Commands m_Commands;
        CommandResolver m_CommandResolver;

        std::list<std::shared_ptr<ICommand>> m_Pending;

        Freq m_Timer;
};

#endif

