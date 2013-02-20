#ifndef _INTERPROCESS_H_1Z9XO94K
#define _INTERPROCESS_H_1Z9XO94K

#include <memory>
#include <boost/asio.hpp>
#include "IDaemon.h"

/*
 * Handles interprocess communication between a CLI call and the daemon process
 * This is the daemon process side.
 * The client side of this is dealt with in Daemon::pipe()
 */
class Interprocess
{
    public:
        Interprocess(
            std::shared_ptr<boost::asio::io_service> ioservice,
            IDaemon* daemon
        );
        //virtual ~Interprocess() {}
        uint32_t port() const { return m_Port; }
        void accept();
        void respond(std::shared_ptr<boost::asio::ip::tcp::socket>& sock, const std::string& response);

    private:

        /*
         * Creates a new socket to be the listener
         */
        void renew_listener();
        uint32_t m_Port = 0;

        IDaemon* m_pDaemon = nullptr;

        std::shared_ptr<boost::asio::io_service> m_pIOService;
        std::shared_ptr<boost::asio::ip::tcp::socket> m_pSocket;
        std::shared_ptr<boost::asio::ip::tcp::acceptor> m_pAcceptor;
        std::shared_ptr<boost::asio::signal_set> m_Signals;
};

#endif

