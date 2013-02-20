//#include <array>
#include "Interprocess.h"
#include "Args.h"
#include "Log.h"
#include "IDaemon.h"
using namespace std;

Interprocess :: Interprocess(
    std::shared_ptr<boost::asio::io_service> ioservice,
    IDaemon* daemon
):
    m_pDaemon(daemon),
    m_pIOService(ioservice)
{
    m_pAcceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(
        *m_pIOService,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0)
    );
    m_Port = m_pAcceptor->local_endpoint().port();

    renew_listener();

    // the below 2 lines are POSIX only afaik unless boost handles this
    // TODO: test this on windows and mac
    m_Signals = std::make_shared<boost::asio::signal_set>(
        *m_pIOService, SIGINT, SIGTERM
    );
    m_Signals->async_wait(std::bind(
        &boost::asio::io_service::stop,
        m_pIOService
    ));
}

void Interprocess :: accept()
{
    //boost::asio:write(m_pSocket, boost::asio::buffer());
    // TODO: read in Args and parse them

    const size_t max = 1024;
    boost::array<char, max> buf;
    std::string str;
    boost::system::error_code error;

    std::shared_ptr<boost::asio::ip::tcp::socket> sock(m_pSocket);
    renew_listener();

    for(;;)
    {
        size_t len = sock->read_some(boost::asio::buffer(buf, max), error);
        if(error) // EOF is also considered "error"
            break;
        if(len)
            str += std::string(buf.data(), len);
        //std::cout << "received " << len << " data" << std::endl;
    }
    
    if(!str.empty())
    {
        Args args(str);
        std::string response = m_pDaemon->action(args);
        if(!response.empty())
            respond(sock, response);
    }
}

void Interprocess :: respond(
    std::shared_ptr<boost::asio::ip::tcp::socket>& sock,
    const std::string& response
){
    // TODO: response async
    //boost::asio::async_send();
}

void Interprocess :: renew_listener()
{
    m_pSocket = std::make_shared<boost::asio::ip::tcp::socket>(*m_pIOService);
    m_pAcceptor->async_accept(
        *m_pSocket,
        std::bind(&Interprocess::accept, this)
    );
}

