#include "WindowManager.h"
#include "Util.h"
#include "Log.h"
#include "Errors.h"

using namespace std;
using namespace util;

int main(int argc, const char** argv)
{
    Args args(argc,argv);
    gdk_init(&argc,(char***)(&argv));

    if(args.has("-s"))
    {
        std::cout << (Daemon::is_running() ? "true" : "false")  << std::endl;
        return 0;
    }

    if(args.has("-D")) // run _as_ daemon process (instead of forking)
    {
        try {
            auto daemon = std::make_shared<gtWindowManager>(args);
            daemon->run();
        } catch(const Error&) {
        } catch(const std::exception& e) {
            ERROR(GENERAL, e.what());
            return 1;
        }
        return 0;
    }

    // Fork daemon process into background
    if(args.has("-d") || args.has("--daemon"))
    {
        try{
            Daemon::ensure_process();
        } catch(const Error&) {
            // logged when thrown
        } catch(const std::exception& e) {
            ERROR(GENERAL, e.what());
            return 1;
        }
        return 0;
    }

    if(!args.empty())
    {
        if(args.has("-q") || args.has("--quit") || args.has("quit"))
        {
            if(Daemon::is_running())
                Daemon::pipe(args);
        }
        else
            Daemon::pipe(args);
    }
    
    return 0;
}

