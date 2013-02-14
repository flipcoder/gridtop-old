#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <fstream>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "Errors.h"
#include "Util.h"

class Log : public util::singleton<Log>
{
public:

    class Message
    {
    public:
        std::string sMessage;
        enum eLoggingLevel { LL_BLANK, LL_INFO, LL_WARNING, LL_ERROR } eLevel;

        Message(std::string message, eLoggingLevel level):
            sMessage(message),
            eLevel(level) {}
    };

private:

    boost::circular_buffer<Message> m_cbLog;
    std::ofstream m_LogFile;
    bool m_bStdOut;

    static const int DEFAULT_LENGTH = 256;

public:
    //static Log& get()
    //{
    //    static Log log;
    //    return log;
    //}
    
    Log();
    virtual ~Log() {}

    void write(const std::string& s, Message::eLoggingLevel lev = Message::LL_INFO);
    void warning(const std::string&s) { write(s,Message::LL_WARNING); }
    void error(const std::string&s) {write(s,Message::LL_ERROR);}

    unsigned int size() const { return m_cbLog.size(); }
    bool empty() const { return (size()==0); }
    void set_stdout(bool b) { m_bStdOut=b; }

    boost::circular_buffer<Message>* getBuffer() { return &m_cbLog; }
    const boost::circular_buffer<Message>* getBuffer_c() const { return &m_cbLog; }
    std::string message(unsigned int idx) {
        if(idx < m_cbLog.size())
            return m_cbLog.at(idx).sMessage;
        return "";
    }
};

#define LOG(X) Log::get().write(X)
#define LOGf(X,Y) Log::get().write((boost::format(X) % Y).str())
#define WARN(X) Log::get().warn(X)
#define WARNf(X,Y) Log::get().warn((boost::format(X) % Y).str())
#define ERROR(CODE,X) {\
    Log::get().error(X);\
    throw Error(ErrorCode::CODE, X);\
}
#define ERRORf(CODE,X,Y) {\
    const std::string _err = (boost::format(X) % Y).str();\
    Log::get().error(_err);\
    throw Error(ErrorCode::CODE, _err);\
}

#endif

