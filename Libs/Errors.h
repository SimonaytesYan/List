#ifndef __LIST_ERRORS_SYM__
#define __LIST_ERRORS_SYM__

const int ERRORS_NUMBER = 8;

enum ErrorCodes
{
    NO_ERROR                = 0,
    NULL_LIST_POINTER       = 1<<0,
    MEMORY_ALLOCATION_ERROR = 1<<1,
    ERROR_LOGS_OPEN         = 1<<2,
    WRONG_SIZE              = 1<<3,
    WRONG_CAPACIRY          = 1<<4,
    DAMAGED_DATA            = 1<<5,
    WRONG_HEAD              = 1<<6,
    WRONG_TAIL              = 1<<7,
};

const char* const ERROR_DESCRIPTION[] = { "Pointer to stack = nullptr\n",
                                          "Error during memmory allocation\n",
                                          "Error during open logs\n",
                                          "Poisoned size\n",
                                          "Poisoned capacity\n",
                                          "Pointer to data array in list is poison ptr or nullptr\n",
                                          "Poison head\n",
                                          "Poison tail\n",};
#endif  //__LIST_ERRORS_SYM__