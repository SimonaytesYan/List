#ifndef __SYM_LIST__
#define __SYM_LIST__

#include <stdlib.h>

#include "..\Libs\Logging\Logging.h"
#include "..\Libs\Errors.h"

const void*       POISON_PTR = (void*)13;
const ListElem_t  POISON     = 0X7FFFFFFF;

typedef struct ListElem 
{
    int        next = -1;
    int        prev = -1;
    ListElem_t val  = 0;
} ListElem;

typedef struct LogInfo 
{
    const char* name     = "(null)";
    const char* function = "(null)";
    const char* file     = "(null)";
    int         line     = POISON;
    bool        status   = false;
} LogInfo;

typedef struct List {
    size_t    size     = 0;
    size_t    capacity = 0;
    size_t    H        = 0;   //!<Head
    size_t    T        = 0;   //!<Tail
    ListElem* data     = nullptr;
    LogInfo   debug    = {};
}List;

int ListCheck(List* list);
int ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file);
int ListDtor(List* list);

int ListCheck(List* list)
{
    int error = 0;
    if (list == nullptr || list == POISON_PTR)
        error |= NULL_LIST_POINTER;
    else
    {
        if (list->size     == POISON)     error |= WRONG_SIZE;
        if (list->capacity == POISON)     error |= WRONG_CAPACITY;
        if (list->H        == POISON)     error |= WRONG_HEAD;
        if (list->T        == POISON)     error |= WRONG_TAIL;
        if (list->data     == nullptr || 
            list->data     == POISON_PTR) error |= DAMAGED_DATA; 
        
        if (list->debug.file     == nullptr ||
            list->debug.file     == POISON_PTR) error |= DEBUG_FILE_DAMAGED;
        if (list->debug.function == nullptr ||
            list->debug.function == POISON_PTR) error |= DEBUG_FUNCTION_DAMAGED;
        if (list->debug.name     == nullptr ||
            list->debug.name     == POISON_PTR) error |= DEBUG_NAME_DAMAGED;
        if (list->debug.line     == POISON)     error |= DEBUG_LINE_DAMAGED;
    }
    return error;
}

#define ListCtor(list, capacity)  ListConstructor(list, capacity, __LINE__, #list, __PRETTY_FUNCTION__, __FILE__)

int ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file)
{
    CHECK(list == nullptr, NULL_LIST_POINTER);

    list->size     = 0;
    list->capacity = capacity;
    list->H        = 0;
    list->T        = 0;
    list->data     = (ListElem*)calloc(capacity, sizeof(ListElem));

    list->debug.name     = name;
    list->debug.function = function;
    list->debug.file     = file;
    list->debug.line     = line;

    return ListCheck(list);
}

int ListDtor(List* list)
{
    ListCheck(list);

    list->capacity = POISON;
    list->size     = POISON;
    list->H        = POISON;
    list->T        = POISON;

    free(list->data);
    list->data = (ListElem*)POISON_PTR;

    list->debug.file     = (const char*)POISON_PTR;
    list->debug.function = (const char*)POISON_PTR;
    list->debug.name     = (const char*)POISON_PTR;
    list->debug.line     = POISON;
    return 0;
}

#endif //__SYM_LIST__