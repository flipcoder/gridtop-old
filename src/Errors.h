#ifndef _ERRORS_DEF_HGR1PXJD
#define _ERRORS_DEF_HGR1PXJD

#include <stdexcept>

enum class ErrorCode: unsigned int
{
    UNKNOWN = 0,
    NO_DAEMON_PORT,
    CONNECTION_REFUSED,
    FAILED_TO_LOCK,
    FAILED_TO_OPEN_FILE,
    FAILED_TO_WRITE_FILE,
    WRONG_VERSION,

    MAX
};

static const char* ErrorString[] =  {
    "Unknown error",
    "No daemon port",
    "Connection refused",
    "Failed to obtain lock",
    "Failed to open file",
    "Failed to write file",
    "Wrong version",
};

class Error:
    virtual public std::runtime_error
{
    public:
        //Error(ErrorCode code = ErrorCode::UNKNOWN, const char* text):
        //    std::runtime_error(ErrorString[(int)code])
        //{
        //}
        Error(ErrorCode code = ErrorCode::UNKNOWN):
            std::runtime_error(ErrorString[(int)code])
        {
            assert(code < ErrorCode::MAX);
        }
        virtual ~Error() noexcept {}
};

#endif

