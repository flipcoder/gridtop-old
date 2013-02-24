#include "WindowManager.h"
#include "Util.h"
#include "Log.h"
#include "FocusOperator.h"
using namespace std;

WindowManager :: WindowManager(const Args& args):
    Daemon(args),
    m_CommandResolver(&m_Commands),
    m_PendAlarm(&m_Timeline)
{
    util::scoped_dtor<WindowManager> dtor(this);

    // TODO: before we do anything, load in user configuration
    m_PendTime = Freq::Time(1000);

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

void WindowManager :: execute_default_operator()
{
    // TODO: probably better to insert left of the motions
    //       but this is good for now
    m_Pending.push_front(make_shared<FocusOperator>(
        make_tuple(this, string())
    ));
    (*m_Pending.begin())->execute();
    m_Pending.clear();
    m_PendAlarm.stop();
}

void WindowManager :: logic(Freq::Time t)
{
    m_Timeline.logic(t);
    if(m_PendAlarm.elapsed())
    {
        if(!m_Pending.empty())
        {
            LOG("execute!");
            m_Pending.back()->execute();
            m_Pending.clear();
        }
    }

    for(auto& command: m_Pending)
        command->logic(t);

    //util::remove_if(m_Pending, [](const shared_ptr<ICommand>& c){
    //    return c->expired();
    //});

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
    vector<string> cmd_args = args.other();
    //vector<tuple<WindowManager*, string>> cmd_args_tuple;
    //transform(ENTIRE(cmd_args), std::back_inserter(cmd_args_tuple),
    //    [this](const string& s){
    //        return make_tuple(this, s);
    //    }
    //);
    
    for(auto& c: cmd_args)
    {
        auto cmd = m_Commands.create(make_tuple(this, c));
        if(!cmd)
            ERRORf(PARSE, "command \"%s\"", c);

        m_Pending.push_back(cmd);

        if(cmd->pending())
            m_PendAlarm.set(m_PendTime);
        else
        {
            cmd->execute();
            c.clear();
            m_PendAlarm.stop();
        }
    }
    return string();
}

