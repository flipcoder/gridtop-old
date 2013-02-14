#include <iostream>
#include <string>
#include <vector>
#include <boost/process.hpp>
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
        
    }

    std::string id() const {
        return m_ID;
    }

    operator bool() const { return !m_ID.empty(); }

    bool active() const { return m_Active; }
    void active(bool b) { m_Active = b; }

private:
    string m_ID;
    bool m_Active = false;
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
    LOG(active_id);

    out = call("wmctrl", { "wmctrl", "-G", "-l" });
    for(const auto& line: out)
    {
        windows.emplace_back(line);
        if(windows.back().id() == active_id)
            windows.back().active(true);
    }

    return 0;
}

