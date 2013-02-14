#include <iostream>
#include <string>
#include <vector>
#include <boost/process.hpp>
#include <sstream>
#include "Args.h"
#include "Log.h"
using namespace std;
using namespace util;

class Window
{
public:
    /*
     *  Parse details from "wmctrl -G -l" entry
     */
    explicit Window(const string& s) {
        istringstream ss(s);

        read_val(ss, m_ID);
        read_val(ss, m_Desktop);

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

    bool ignore() const {
        return is_desktop();
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

    bool is_desktop() const {
        return m_Desktop == -1;
    }

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

private:

    string m_ID;
    bool m_Active = false;
    int m_Desktop = 0;
};

struct Workspace
{

};

struct Display
{
};

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

void x_sort(vector<Window>& windows)
{
    sort(ENTIRE(windows),
        [](const Window& a, const Window& b){
            return a.x_center() < b.x_center();
        }
    );
}

void y_sort(vector<Window>& windows)
{
    sort(ENTIRE(windows),
        [](const Window& a, const Window& b){
            return a.y_center() < b.y_center();
        }
    );
}

int main(int argc, const char** argv)
{
    Args args(argc,argv);

    vector<Window> windows;
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
        windows.emplace_back(line);
        if(windows.back().id() == active_id)
            windows.back().active(true);
    }

    // only consider ignored windows if they're the active one
    // otherwise, remove them
    util::remove_if(windows, [](const Window& win) {
        return win.ignore() && !win.active();
    });

    string dir = args.value("focus");
    if(!dir.empty())
    {
        string target_id;
        if(dir=="left"){
            x_sort(windows);
            auto itr = find_if(ENTIRE(windows),[](const Window& a){
                return a.active();
            });
            if(itr != windows.end() && itr != windows.begin())
            {
                --itr;
                target_id = itr->id();
            }
        }else if(dir=="right"){
            x_sort(windows);
            auto itr = find_if(ENTIRE(windows),[](const Window& a){
                return a.active();
            });
            if(itr != windows.end() && itr != windows.end()-1)
            {
                ++itr;
                target_id = itr->id();
            }
        }else if(dir=="up"){
            y_sort(windows);
            auto itr = find_if(ENTIRE(windows),[](const Window& a){
                return a.active();
            });
            if(itr != windows.end() && itr != windows.begin())
            {
                --itr;
                target_id = itr->id();
            }
        }else if(dir=="down"){
            y_sort(windows);
            auto itr = find_if(ENTIRE(windows),[](const Window& a){
                return a.active();
            });
            if(itr != windows.end() && itr != windows.end()-1)
            {
                ++itr;
                target_id = itr->id();
            }
        }

        if(!target_id.empty() && target_id != active_id)
            call("wmctrl", {"wmctrl", "-i", "-a", target_id});
    }

    return 0;
}

