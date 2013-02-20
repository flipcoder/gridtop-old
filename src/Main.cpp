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
            auto daemon = std::make_shared<WindowManager>(args);
            daemon->run();
        } catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    // Fork daemon process into background
    if(args.has("-d") || args.has("--daemon"))
    {
        try{
            Daemon::ensure_process();
        }catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    if(!args.empty())
        Daemon::pipe(args);
    
    return 0;
}

