#include "WindowManager.h"
#include "Util.h"
#include "Log.h"
#include "FocusOperator.h"
#include "Motion.h"
#include "Geometry.h"
using namespace std;

WindowManager :: WindowManager(const Args& args):
    Daemon(args),
    m_CommandResolver(&m_Commands),
    m_PendAlarm(&m_Timeline)
{
    util::scoped_dtor<WindowManager> dtor(this);

    // TODO: before we do anything, load in user configuration
    m_PendTime = Freq::Time(1000);

    // TODO: calculate grid

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
    m_Pending.front()->execute();
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
        {
            m_PendAlarm.set(cmd->pend());
        }
        else
        {
            cmd->execute();
            m_Pending.clear();
            m_PendAlarm.stop();
        }

        // check if any other commands have _stopped_ pending as a result of
        // the newest entry on the stack (we'll iterate from back to front)
        for(auto itr = m_Pending.rbegin();
            itr != m_Pending.rend();
            ++itr)
        {
            if(!(*itr)->pending())
            {
                (*itr)->execute();
                m_Pending.clear();
                m_PendAlarm.stop();
                break;
            }
        }
    }
    return string();
}

//vector<shared_ptr<Window>> WindowManager :: window_matches(
//    const std::vector<Motion*> motions
//){
//    return m_Windows;
//}

std::shared_ptr<Window> WindowManager :: next_window(
    std::vector<Motion*> motions
){
    unsigned motion_bits = Motion::bits(motions);

    Window* active = active_window().get();
    if(!active)
        return std::shared_ptr<Window>();

    std::shared_ptr<Window> target;

    if(motion_bits & Motion::bit(eMotion::LEFT))
    {
        // TODO: Build line, extrude line to display edge to make zone
        //  rectangle
        // Match windows whose centers lie in that zone
        // if none, extend zone and try again

        Rectangle zone = Rectangle::xywh(
            0.0f,
            active->pos().y,

            active->center().x,
            active->size().y
        );

        auto wins = windows();
        // TODO: x_sort
        // TODO: do window-in-zone overlap test
    }
    else if(motion_bits & Motion::bit(eMotion::RIGHT))
    {
    }
    else if(motion_bits & Motion::bit(eMotion::UP))
    {
    }
    else if(motion_bits & Motion::bit(eMotion::DOWN))
    {
    }
  
    return target;

    // 1) get active window
    // 2) calculate matching based on motions and window geometry
    // 3) sort based on motion direction
}

