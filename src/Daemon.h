#ifndef DAEMON_H_LAE45GZN
#define DAEMON_H_LAE45GZN

//#include <boost/interprocess/sync/named_mutex.hpp>
//#include <boost/thread/locks.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include "Args.h"
#include "IDaemon.h"
#include "Interprocess.h"

class Daemon:
    virtual public IDaemon
{
    public:

        Daemon(const Args& args);
        virtual ~Daemon();

        /**
         * Obtain lockfile
         */
        void init_lock();

        /**
         * Runs the IO services.
         * run() will block
         * run_one() will not
         */
        virtual void run();
        virtual void run_one();

        /**
         * Checks if a daemon process is running on the system.
         */
        static bool is_running();

        /**
         * Ensures daemon process is running on the system.
         * Might throw.
         */
        static void ensure_process();

        /**
         * Connect to the daemon process and tell it to perform the action.
         */
        static void pipe(const Args& args);

    private:

        const static int MAX_RETRY= 5;

        constexpr static const char* LOCK_NAME = ".lock";
        constexpr static const char* PORT_NAME = ".port";

        boost::interprocess::file_lock m_FileLock;
        std::fstream m_Lockfile;

        std::shared_ptr<boost::asio::io_service> m_pIOService;

        std::shared_ptr<Interprocess> m_pInterprocess;
};

#endif

