#include "Daemon.h"
#include <stdexcept>
#include <boost/process.hpp>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <boost/array.hpp>
#include <boost/format.hpp>
#include "Errors.h"
#include "Info.h"
#include "Log.h"
using namespace std;

Daemon :: Daemon(const Args& args)
{
    // verify daemon version if "-v=VERSION" was specified
    string version = args.value("-v");
    if(!version.empty() && version != string(Info::Version))
        ERROR(VERSION, "daemon");

    init_lock();
    util::scoped_dtor<Daemon> dtor(this);
    m_pIOService = make_shared<boost::asio::io_service>();
    m_pInterprocess = make_shared<Interprocess>(m_pIOService, this);

    fstream file(PORT_NAME, ios_base::out | ios_base::trunc);
    if(!file.is_open() || file.bad())
        ERROR(READ, PORT_NAME);
    file << m_pInterprocess->port(); // write port name to portfile

    dtor.resolve();
}

Daemon :: ~Daemon()
{
    m_Lockfile.close();
    try{
        boost::filesystem::remove(PORT_NAME);
    }catch(...){
    }
    try{
        boost::filesystem::remove(LOCK_NAME);
    }catch(...){
    }
}

void Daemon :: init_lock()
{
    try{
        m_FileLock = boost::interprocess::file_lock(LOCK_NAME);
    }catch(...){
        // create if lock doesn't exist
        m_Lockfile.open(LOCK_NAME, ios_base::out | ios_base::trunc);
        try{
            m_FileLock = boost::interprocess::file_lock(LOCK_NAME);
        }catch(...){
            // can't craete lockfile
            ERROR(ACTION, "lock");
        }
    }

    if(!m_FileLock.try_lock())
    {
        //cout << "daemon already running" << endl;
        ERROR(ACTION, "lock");
    }
}

bool Daemon :: is_running()
{
    boost::interprocess::file_lock mylock;
    try{
        mylock = boost::interprocess::file_lock(LOCK_NAME);
    }catch(const exception& e){
        //cout<< e.what() << endl;
        return false;
    }
    return !mylock.try_lock();
}

void Daemon :: ensure_process()
{
    if(!is_running())
    {
        //cout<<"spawning new daemon"<<endl;
        boost::process::context ctx;
        // TODO: use program path instead of current
        ctx.environment = boost::process::self::get_environment();
        ctx.stdout_behavior = boost::process::redirect_stream_to_stdout();
        ctx.stderr_behavior = boost::process::redirect_stream_to_stdout();
        boost::process::child proc = boost::process::launch(
            string(Info::Program),
            vector<string>{
                string(Info::Program),
                "-D", // daemon
                (boost::format("-v=%s") % Info::Version).str()
            }, //version
            ctx
        );
    }
}

void Daemon :: pipe(const Args& args)
{
    uint16_t port = 0;
    Daemon::ensure_process();

    boost::asio::io_service io;
    boost::asio::ip::tcp::resolver resolver(io);
    boost::asio::ip::tcp::socket sock(io);

    if(!util::retry(MAX_RETRY, chrono::seconds(1), [&]() -> bool
    {
        if(!util::retry(MAX_RETRY, chrono::seconds(1), [&]() -> bool
        {
            fstream file(PORT_NAME, ios_base::in);
            return !(file >> port).fail();
        }
        )){
            ERROR(PARSE, "daemon port");
        }

        //for(int j=0; i<MAX_RETRY; ++j)
        //{
        //    fstream file(PORT_NAME, ios_base::in);
        //    if((file >> port).fail())
        //        break;
        //    if(j==MAX_RETRY-1)
        //        throw Error(ErrorCode::NO_DAEMON_PORT);
        //    this_thread::sleep_for(chrono::seconds(1));
        //}

        boost::asio::ip::tcp::resolver::query query(
            boost::asio::ip::tcp::v4(),
            "localhost",
            to_string(port)
        );
        auto itr = resolver.resolve(query);
        try{
            boost::asio::connect(sock, itr);
        }catch(const exception& e){
            return false;
        }

        return true;
    }
    )){
        ERROR(ACTION, "connection");
    }

    sock.non_blocking(false);

    try
    {
        //boost::array<uint8_t, 128> buffer;
        boost::system::error_code error;
        boost::asio::write(sock, boost::asio::buffer(args.data()), error);
        //size_t len = sock.read_some(boost::asio::buffer(buf), error);
        //
        //if(error == boost::asio::error::eof)
        //    break;
        //else if (error)
        //    throw boost::system::system_error(error);

        //cout << "connected" << endl;
    }
    catch(const exception& e)
    {
        ERROR(GENERAL, "disconnected"); // disconnected
    }
}

void Daemon :: run()
{
    m_pIOService->run();
}

void Daemon :: run_one()
{
    m_pIOService->run_one();
}

