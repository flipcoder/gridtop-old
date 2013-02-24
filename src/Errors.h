#ifndef _ERRORS_DEF_HGR1PXJD
#define _ERRORS_DEF_HGR1PXJD

#include <stdexcept>
#include <cassert>
#include <iostream>

enum class ErrorCode: unsigned int
{
    UNKNOWN = 0,

    GENERAL,
    INIT,
    LIBRARY,
    VERSION,

    READ,
    WRITE,
    PARSE,
    ACTION,

    LOCK,
    FATAL,

    MAX
};

static const char* g_ErrorString[] =  {
    "",

    "An error occurred",
    "Unable to initialize",
    "Failed to load library",
    "Version mismatch",

    "Unable to read",
    "Unable to write",
    "Parse error",
    "Failed to perform action",

    "Failed to obtain lock",
    "Fatal error occured",
};

class Error:
    virtual public std::runtime_error
{
    private:
        std::string m_Msg;
    public:
        //Error(ErrorCode code = ErrorCode::UNKNOWN, const char* text):
        //    std::runtime_error(g_ErrorString[(int)code])
        //{
        //}
        Error(ErrorCode code = ErrorCode::UNKNOWN, const std::string& desc = std::string()):
            std::runtime_error(g_ErrorString[(unsigned)code])
        {
            assert(code < ErrorCode::MAX);

            if(code == ErrorCode::UNKNOWN)
            {
                if(!desc.empty())
                    m_Msg = desc;
                else
                    m_Msg = "Unknown Error";
            }
            else if(code == ErrorCode::FATAL)
            {
                //assert(false);
            }
            else if(!desc.empty())
                m_Msg = std::string(g_ErrorString[(unsigned)code]) + ": " + desc;
            else
                m_Msg = g_ErrorString[(unsigned)code];

            //std::cout << m_Msg << std::endl;
        }
        const char* what() const noexcept override {
            if(!m_Msg.empty())
                return m_Msg.c_str();
            else
                return std::runtime_error::what();
        }
        virtual ~Error() noexcept {}
};

#endif

