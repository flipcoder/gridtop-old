#include "WindowManager.h"
#include "Util.h"
#include "Log.h"
#include "FocusOperator.h"
#include "Motion.h"
#include "Geometry.h"
using namespace std;

gtWindowManager :: gtWindowManager(const Args& args):
    Daemon(args),
    m_CommandResolver(&m_Commands),
    m_PendAlarm(&m_Timeline)
{
    util::scoped_dtor<gtWindowManager> dtor(this);

    // TODO: before we do anything, load in user configuration
    m_PendTime = Freq::Time(100);

    // TODO: calculate grid

    wnck_set_client_type(WNCK_CLIENT_TYPE_PAGER);

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
            m_Windows.emplace_back(make_shared<gtWindow>(win, this));
            //if(active_window && (win == active_window)) {
                //m_ActiveWindowIndex = m_Windows.size();
                //m_pActiveWindow = m_Windows.back();
            //}
        }
    }

    dtor.resolve();
}

void gtWindowManager :: execute_default_operator()
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

void gtWindowManager :: logic(Freq::Time t)
{
    //LOGf("logic, t=%s", t.ms());
    m_Timeline.logic(t);

    wnck_screen_force_update(m_pScreen);

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

void gtWindowManager:: run()
{
    GMainLoop* loop = g_main_loop_new(NULL, false);
    GMainContext* context = g_main_loop_get_context(loop);
    while(!killed())
    {
        Freq::Time t;
        while(!(t = m_Timer.tick()).ms())
            this_thread::yield();
        g_main_context_iteration(context, false);
        logic(t);
        Daemon::run_one();
    }
    g_main_loop_unref(loop);
}

gtWindowManager :: ~gtWindowManager()
{
    wnck_shutdown();
}

string gtWindowManager :: action(const Args& args)
{
    if(args.has("-q") || args.has("--quit") || args.has("quit")) {
        kill();
        return string();
    }

    vector<string> cmd_args = args.other();
    //vector<tuple<gtWindowManager*, string>> cmd_args_tuple;
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

//vector<shared_ptr<gtWindow>> gtWindowManager :: window_matches(
//    const std::vector<Motion*> motions
//){
//    return m_Windows;
//}

std::shared_ptr<gtWindow> gtWindowManager :: next_window(
    std::vector<Motion*> motions
){
    unsigned motion_bits = Motion::bits(motions);
    auto wins = windows();

    if(!active_window())
        return std::shared_ptr<gtWindow>();

    std::shared_ptr<gtWindow> target;

    // horizontal
    if(motion_bits & (
        Motion::bit(eMotion::LEFT) |
        Motion::bit(eMotion::RIGHT) | 
        Motion::bit(eMotion::LEFT_EDGE) | 
        Motion::bit(eMotion::RIGHT_EDGE)
    )){
        LOG("horizontal motion");
        sort(ENTIRE(wins), [](
            const shared_ptr<gtWindow>& a, 
            const shared_ptr<gtWindow>& b
        ){
            return a->center().x < b->center().x;
        });
    }
    // vertical
    else if(motion_bits & (
        Motion::bit(eMotion::UP) |
        Motion::bit(eMotion::DOWN) | 
        Motion::bit(eMotion::TOP) | 
        Motion::bit(eMotion::BOTTOM)
    )){
        LOG("vertical motion");
        sort(ENTIRE(wins), [](
            const shared_ptr<gtWindow>& a, 
            const shared_ptr<gtWindow>& b
        ){
            return a->center().y < b->center().y;
        });
    }
    else
    {
        WARN("unsorted window list");
        return std::shared_ptr<gtWindow>();
    }

    // log some info
    for(auto& w: wins)
    {
        LOGf("%s: p(%s, %s) s(%s, %s)%s",
            w->name() %
            w->pos().x % w->pos().y %
            w->size().x % w->size().y %
            (w->active()?" <- active":"")
        );
    }

    auto itr = find_if(ENTIRE(wins), [](const shared_ptr<gtWindow>& w) {
        return w->active();
    });
    assert(itr != wins.end());

    // negative direction
    if(motion_bits & (
        Motion::bit(eMotion::LEFT) |
        Motion::bit(eMotion::UP)
    )){
        LOG("offset (-1)");
        target = (itr - 1 != wins.end()) ? *(itr - 1) : shared_ptr<gtWindow>();
    }
    else if(motion_bits & (
        Motion::bit(eMotion::RIGHT) |
        Motion::bit(eMotion::DOWN)
    )){
        LOG("offset (+1)");
        target = (itr + 1 != wins.end()) ? *(itr + 1) : shared_ptr<gtWindow>();
    }
    if(!target)
        WARN("no target");

    //if(motion_bits & Motion::bit(eMotion::LEFT))
    //{
    //    // TODO: Build line, extrude line to display edge to make zone
    //    //  rectangle
    //    // Match windows whose centers lie in that zone
    //    // if none, extend zone and try again

    //    Rectangle zone = Rectangle::ltrb(
    //        0.0f,
    //        active->pos().y,

    //        active->center().x,
    //        active->size().y
    //    );

    //    // TODO: x_sort
        
    //    // TODO: do window-in-zone overlap test
    //}
    //else if(motion_bits & Motion::bit(eMotion::RIGHT))
    //{
    //}
    //else if(motion_bits & Motion::bit(eMotion::UP))
    //{
    //}
    //else if(motion_bits & Motion::bit(eMotion::DOWN))
    //{
    //}
  
    return target;

    // 1) get active window
    // 2) calculate matching based on motions and window geometry
    // 3) sort based on motion direction
}

