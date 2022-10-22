#ifndef __LOGGING_SYM__
#define __LOGGING_SYM__

#include <stdio.h>

int OpenLogFile(const char* file_name);

int CloseLogFile();

int ParseErrorCode(int error);

int LogPrintf(const char *format, ...);

#define CHECK(cond, return_code)                                                               \
    if (cond)                                                                                  \
    {                                                                                          \
        LogPrintf("In %s in %s(%d)\n", __PRETTY_FUNCTION__ , __FILE__, __LINE__);              \
        ParseErrorCode(return_code);                                                           \
        return return_code;                                                                    \
    }


#endif //__LOGGING_SYM__
