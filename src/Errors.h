#ifndef _ERRORS_DEF_HGR1PXJD
#define _ERRORS_DEF_HGR1PXJD

#include <stdexcept>
#include <cassert>
#include <iostream>

enum class ErrorCode: unsigned int
{
    UNKNOWN = 0,

    INIT,
    DISPLAY,
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

static const char* ErrorString[] =  {
    "",

    "Unable to initialize library",
    "Failed to set display mode",
    "Failed to load library",
    "Version is unsupported",

    "Unable to read resource",
    "Unable to write resource",
    "Resource parsing error",
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
        //    std::runtime_error(ErrorString[(int)code])
        //{
        //}
        Error(ErrorCode code = ErrorCode::UNKNOWN, const std::string& desc = std::string()):
            std::runtime_error(ErrorString[(int)code])
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
                m_Msg = std::string(ErrorString[(int)code]) + ": " + desc;
            else
                m_Msg = ErrorString[(int)code];

            std::cout << m_Msg << std::endl;
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

