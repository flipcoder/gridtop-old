#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <memory>
#include <algorithm>
#include <boost/process.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "Args.h"
#include "Log.h"
using namespace std;
using namespace util;

enum class Motion {
    LEFT,RIGHT,UP,DOWN,
    START,END,TOP,BOTTOM
};
const map<string,Motion> MotionFromString = {
    make_pair("left",Motion::LEFT),
    make_pair("right",Motion::RIGHT),
    make_pair("up",Motion::UP),
    make_pair("down",Motion::DOWN),

    make_pair("start",Motion::START),
    make_pair("end",Motion::END),
    make_pair("top",Motion::TOP),
    make_pair("bottom",Motion::BOTTOM)
};

class Window//:
    //public enable_shared_from_this<Window>
{
public:
    /*
     *  Parse details from "wmctrl -G -l" entry
     */
    explicit Window(const string& s) {
        istringstream ss(s);

        read_val(ss, m_ID);
        read_val(ss, m_Workspace);

        read_val(ss, x);
        read_val(ss, y);
        read_val(ss, w);
        read_val(ss, h);
    }

    template<class T>
    static void read_val(istringstream& ss, T& val) {
        if((ss >> val).fail())
            ERROR(PARSE, "wmctrl window attributes");
    }

    std::string id() const {
        return m_ID;
    }

    operator bool() const { return !m_ID.empty(); }

    bool active() const { return m_Active; }
    void active(bool b) { m_Active = b; }

    bool minimized() const { return m_Minimized; }
    void minimized(bool b) { m_Minimized = b; }

    bool ignore() const {
        // TODO: replace this with a check for the window being the desktop
        return sticky();
    }

    int x_center() const{
        if(w==0)
            return x;
        return x + w/2;
    }
    int y_center() const{
        if(h==0)
            return y;
        return y + h/2;
    }

    bool sticky() const {
        return m_Workspace == -1;
    }

    unsigned index_in(const vector<shared_ptr<Window>>& windows)
    {
        for(unsigned i=0; i<windows.size(); ++i)
            if(windows[i].get() == this){
                //LOGf("active window in position %s ", i);
                return i;
            }
        throw std::out_of_range("window index");
        return 0;
    }

    /*
     *  Next window in a specific direction
     *
     *  TODO: eventually add a smarter axis overlap check which will require
     *        better searching.
     */
    shared_ptr<Window> motion(Motion motion, vector<shared_ptr<Window>> windows) {
        // only consider ignored windows if they're the active one
        // otherwise, remove them
        util::remove_if(windows, [](const shared_ptr<Window>& win) {
            return (win->ignore() && !win->active()) || win->minimized();
        });

        // at() and index_in() may throw out_of_range
        try{
            switch(motion)
            {
                case Motion::LEFT:
                    x_sort(windows);
                    return windows.at(index_in(windows)-1);
                case Motion::RIGHT:
                    x_sort(windows);
                    return windows.at(index_in(windows)+1);
                case Motion::UP:
                    y_sort(windows);
                    return windows.at(index_in(windows)-1);
                case Motion::DOWN:
                    y_sort(windows);
                    return windows.at(index_in(windows)+1);

                case Motion::START:
                    x_sort(windows);
                    return windows.at(0);
                case Motion::END:
                    x_sort(windows);
                    return windows.at(max(windows.size(),size_t()+1)-1);
                case Motion::TOP:
                    y_sort(windows);
                    return windows.at(0);
                case Motion::BOTTOM:
                    y_sort(windows);
                    return windows.at(max(windows.size(),size_t()+1)-1);
                default:
                    break; // silence warnings
            }
        }catch(const out_of_range&){
            //LOG("active window is on edge (target out of range)");
        }

        return std::shared_ptr<Window>();
    }

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

private:
    static void x_sort(vector<shared_ptr<Window>>& windows)
    {
        sort(ENTIRE(windows),
            [](const shared_ptr<Window> a,
               const shared_ptr<Window>& b
            ){
                return a->x_center() < b->x_center();
            }
        );
    }

    static void y_sort(vector<shared_ptr<Window>>& windows)
    {
        sort(ENTIRE(windows),
            [](const shared_ptr<Window> a,
               const shared_ptr<Window>& b
            ){
                return a->y_center() < b->y_center();
            }
        );
    }

    string m_ID;
    bool m_Active = false;
    bool m_Minimized = false;
    int m_Workspace= 0;
};

struct Workspace
{
    
};

struct Display
{
    
};

shared_ptr<Window> get_active_window(vector<shared_ptr<Window>>& windows) {
    auto itr = find_if(ENTIRE(windows),[](const shared_ptr<Window>& a){
        return a->active();
    });
    if(itr == windows.end())
        return shared_ptr<Window>();
    return *itr;
}

enum eOutput {
    USE_STDOUT = 0,
    USE_STDERR
};
vector<string> call(
    const string& prog,
    const vector<string> args,
    eOutput choice = USE_STDOUT
){
    vector<string> output;

    boost::process::context ctx;
    ctx.environment = boost::process::self::get_environment();
    ctx.stdout_behavior = boost::process::capture_stream();
    ctx.stderr_behavior = boost::process::capture_stream();

    boost::process::child child = boost::process::launch(
        boost::process::find_executable_in_path(prog),
        args,
        ctx
    );

    boost::process::pistream& stream = (choice==USE_STDOUT ?
        child.get_stdout() :
        child.get_stderr()

    );

    string line;
    while(getline(stream, line))
        output.emplace_back(line);

    return output;
}

int main(int argc, const char** argv)
{
    Args args(argc,argv);

    vector<shared_ptr<Window>> windows;
    string active_id;
    vector<string> out;
    
    out = call("wmctrl",
        { "wmctrl", "-v", "-r", ":ACTIVE:", "-e", "dummy" },
        USE_STDERR
    );
    active_id = out.at(1).substr(14); // get win ID from verbose output

    out = call("wmctrl", { "wmctrl", "-G", "-l" });
    for(const auto& line: out)
    {
        //LOG(line);
        windows.push_back(make_shared<Window>(line));

        // check if active
        if(windows.back()->id() == active_id)
        {
            windows.back()->active(true);
            //LOG("active ^");
        }

        // TODO: check if minimized
        auto wininfo = call("xwininfo", {
            "xwininfo",
            "-wm",
            "-id",
            windows.back()->id()
        });
        try{
            for(unsigned i=0; i<wininfo.size(); ++i)
                if(boost::ends_with(wininfo[i], "Window state:"))
                {
                    LOGf("state %s", wininfo.at(i));
                    if(boost::ends_with(wininfo.at(i+1), "Hidden"))
                    {
                        LOGf("hidden %s", wininfo.at(i+1));
                        windows.back()->minimized(true);
                        break;
                    }
                }
        }catch(out_of_range&) {}
    }

    string dir_string = args.value("focus");
    if(!dir_string.empty())
    {
        shared_ptr<Window> target;
        Motion dir;
        try{
            dir = MotionFromString.at(dir_string); // might throw
        }catch(out_of_range&){
            ERROR(PARSE, "direction");
        }
        target = get_active_window(windows)->motion(dir,windows);
        if(target && !target->active())
        {
            //LOGf("switching to: %s", target->id());
            call("wmctrl", {"wmctrl", "-i", "-a", target->id()});
        }else{
            //LOG("no target");
        }

        return 0;
    }

    if(args.has("fill"))
        {}//fill(args, windows);

    return 0;
}

