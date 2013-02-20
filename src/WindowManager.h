#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "Args.h"
#include "Common.h"
#include "Daemon.h"
#include "Window.h"
#include "Freq.h"
#include <memory>
#include <vector>
#include <string>

class WindowManager:
    public IRealtime,
    public Daemon
{
    public:

        enum class Motion {
            LEFT,RIGHT,UP,DOWN,
            START,END,TOP,BOTTOM
        };

        WindowManager(const Args& args);

        virtual ~WindowManager();

        virtual void run() override;

        /*
         * Executes the provided args as WindowManager commands.
         * Eventually, this function will pipe to the daemon process.
         * For now, it just performs the commands locally.
         *
         * Method will be changed to static when WindowManager is a daemon
         */
        virtual std::string action(const Args& args) override;

        void logic(Freq::Time t) override {
            for(auto& win: m_Windows)
                win->logic(t);
        }
        
    private:

        void recache();

        WnckScreen* m_pScreen = nullptr;
        std::vector<std::shared_ptr<Window>> m_Windows;

        std::weak_ptr<Window>  m_pActiveWindow;
        unsigned int m_ActiveWindowIndex;

        Freq m_Timer;

        /*
         * If realtime is false, recacheing is done only on specific commands
         * If realtime is true, recacheing is done every frame
         */
        bool m_bRealtime = false;
};

#endif

