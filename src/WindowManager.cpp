#include "WindowManager.h"
#include "Util.h"
#include "Log.h"
using namespace std;

WindowManager :: WindowManager(const Args& args):
    Daemon(args),
    m_CommandResolver(&m_Commands)
{
    util::scoped_dtor<WindowManager> dtor(this);

    // TODO: before we do anything, load in user configuration

    WnckScreen* screen = wnck_screen_get_default();
    wnck_screen_force_update(screen);
    
    m_pScreen = wnck_screen_get_default();
    wnck_screen_force_update(m_pScreen);

    //WnckWindow* active_window = wnck_screen_get_active_window(m_pScreen);
    for(GList* winlist = wnck_screen_get_windows(m_pScreen);
        winlist != NULL;
        winlist = winlist->next)
    {
        WnckWindow* win = WNCK_WINDOW(winlist->data);
        auto flags = wnck_window_get_window_type(win);
        if(!(flags & WNCK_WINDOW_DESKTOP) &&
           !(flags & WNCK_WINDOW_DOCK))
        {
            m_Windows.emplace_back(make_shared<Window>(win, this));
            //if(active_window && (win == active_window)) {
                //m_ActiveWindowIndex = m_Windows.size();
                //m_pActiveWindow = m_Windows.back();
            //}
        }
    }

    dtor.resolve();
}


void WindowManager :: logic(Freq::Time t)
{
    for(auto& command: m_Pending)
        command->logic(t);

    util::remove_if(m_Pending, [](const std::shared_ptr<ICommand>& c){
        return c->expired();
    });

    // TODO: update window list with new windows
    for(auto& win: m_Windows)
        win->logic(t);
}

void WindowManager:: run()
{
    while(true)
    {
        Freq::Time t;
        while(!(t = m_Timer.tick()).ms())
            this_thread::yield();
        Daemon::run_one();
        logic(t);
    }
}

WindowManager :: ~WindowManager()
{
    wnck_shutdown();
}

string WindowManager :: action(const Args& args)
{
    std::vector<std::string> cmd_args = args.other();
    std::vector<std::tuple<WindowManager*, std::string>> cmd_args_tuple;
    transform(ENTIRE(cmd_args), cmd_args_tuple.begin(),
        [this](const std::string& s){
            return make_tuple(this, s);
        }
    );
    m_Commands.create_all(cmd_args_tuple);
    return string();
}

