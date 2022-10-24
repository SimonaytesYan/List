#ifndef __SYM_LIST__
#define __SYM_LIST__

#include <stdlib.h>

#include "..\Libs\Logging\Logging.h"
#include "..\Libs\Errors.h"

const int         ResizeCoef = 2; 
const void*       POISON_PTR = (void*)13;
const ListElem_t  POISON     = 0X7FFFFFFF;

typedef struct ListElem 
{
    ListElem_t val  = 0;
    int        next = -1;
    int        prev = -1;
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
    ListElem* data     = nullptr;
    LogInfo   debug    = {};
}List;

int  ListCheck(List* list);

int  ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file);

int  ListDtor(List* list);

void DumpList(List* list, const char* function, const char* file, int line);

//!---------------------
//!@param [in]  list        List for inserting an element
//!@param [in]  val         Value of new element
//!@param [in]  after_which Index of element after which put new element
//!@param [out] index       (Optional).Index in which new element will insert
//!@return Error code according to Errors.h
//!
//!---------------------
int ListInsert(List* list, int value, int after_which, int* index = nullptr);

int ListPop(List* list, int index);

int FindFree(List* list, int* index);

int ResizeUp(List* list, int new_capacity);

#define DUMP_L(list) DumpList(list, __PRETTY_FUNCTION__, __FILE__, __LINE__)

void DumpList(List* list, const char* function, const char* file, int line)
{
    LogPrintf("\nDump in %s(%d) in function %s\n", file, line, function);

    ListCheck(list);
    
    if (list == nullptr || list == POISON_PTR) 
        return;

    LogPrintf("Stack[%p] ", list);

    if (list->debug.name != nullptr && list->debug.name != POISON_PTR)
        LogPrintf("\"%s\"", list->debug.name);
    if (list->debug.function != nullptr && list->debug.function != POISON_PTR)
        LogPrintf("created at %s ", list->debug.function);
    if (list->debug.file != nullptr && list->debug.file != POISON_PTR)
        LogPrintf("at %s(%d):", list->debug.file);
    LogPrintf("\n");

    LogPrintf("Status: ");
    if (list->debug.status)
        LogPrintf("enable\n");
    else
        LogPrintf("disable\n");

    LogPrintf("{\n");
    LogPrintf("\tsize     = %zu\n", list->size);
    LogPrintf("\tcapacity = %zu\n", list->capacity);

    if (list->data != nullptr && list->data != POISON_PTR)
    {
        LogPrintf("\tdata:");
        for(int i = 0; i <= list->capacity; i++)
        {
            LogPrintf("|");
            PrintElemInLog(list->data[i].val);
        }
        LogPrintf("|");

        LogPrintf("\n\tnext:");
        for(int i = 0; i <= list->capacity; i++)
            LogPrintf("|%10d", list->data[i].next);    
        LogPrintf("|");
            
        LogPrintf("\n\tprev:");
        for(int i = 0; i <= list->capacity; i++)
            LogPrintf("|%10d", list->data[i].prev);
            
        LogPrintf("|\n");
    }

    LogPrintf("}\n\n");
}

int ListCheck(List* list)
{
    int error = 0;
    if (list == nullptr || list == POISON_PTR)
        error |= NULL_LIST_POINTER;
    else
    {
        if (list->size     == POISON)     error |= WRONG_SIZE;
        if (list->capacity == POISON)     error |= WRONG_CAPACITY;
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

    LogAndParseErr(error != 0, error);
    
    return error;
}

#define ListCtor(list, capacity)  ListConstructor(list, capacity, __LINE__, #list, __PRETTY_FUNCTION__, __FILE__)

int ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file)
{
    LogAndParseErr(list == nullptr, NULL_LIST_POINTER);

    list->size     = 0;
    list->capacity = capacity;
    list->data     = (ListElem*)calloc(capacity + 1, sizeof(ListElem));
    if (list->data != nullptr)
        for(int i = 1; i <= list->capacity; i++)
        {
            list->data[i].val  = POISON;
            list->data[i].prev = -1;
            list->data[i].next = -1;
        }

    list->debug.name     = name;
    list->debug.function = function;
    list->debug.file     = file;
    list->debug.line     = line;
    list->debug.status   = true;

    return ListCheck(list);
}

int ListDtor(List* list)
{
    ListCheck(list);

    list->capacity = POISON;
    list->size     = POISON;

    free(list->data);
    list->data = (ListElem*)POISON_PTR;

    list->debug.file     = (const char*)POISON_PTR;
    list->debug.function = (const char*)POISON_PTR;
    list->debug.name     = (const char*)POISON_PTR;
    list->debug.line     = POISON;

    return 0;
}

int FindFree(List* list, int* index)
{
    ReturnIfError(ListCheck(list));

    for(int i = 0; i <= list->capacity; i++)
        if (list->data[i].next == -1)
        {
            *index = i;
            return 0;
        }

    *index = -1;
    return 0;
}

int ListPop(List* list, int index)
{
    ReturnIfError(ListCheck(list));

    CHECK(index > list->capacity || index <= 0, "Error index", -1);

    int next_ind = list->data[index].next;
    int prev_ind = list->data[index].prev;

    CHECK(next_ind == -1 || prev_ind == -1, "Index to not inserted element", -1);

    list->data[next_ind].prev = prev_ind;
    list->data[prev_ind].next = next_ind;

    list->data[index].val  = POISON;
    list->data[index].prev = -1;
    list->data[index].next = -1;

    list->size--;

    return 0;
}

int ResizeUp(List* list, int new_capacity)
{
    ReturnIfError(ListCheck(list));

    list->data = (ListElem*)realloc(list->data, sizeof(ListElem)*(new_capacity + 1));

    if (list->data == nullptr)
        return MEMORY_ALLOCATION_ERROR;

    for(int i = list->capacity + 1; i <= new_capacity; i++)
        list->data[i] = {POISON, -1, -1};
    
    list->capacity = new_capacity;

    return 0;
}

int ListInsert(List* list, int value, int after_which, int* index) 
{
    ReturnIfError(ListCheck(list));

    CHECK(after_which > list->capacity || after_which < 0, "Error index", -1);
    
    CHECK(list->data[after_which].next == -1 || list->data[after_which].prev == -1, "Index to not inserted element", -1);

    if (list->capacity == list->size)
    {
        int new_capacity = 0;
        if (list->capacity == 0)
            new_capacity = 2;
        else
            new_capacity = list->capacity * ResizeCoef;
        ReturnIfError(ResizeUp(list, new_capacity));
    }

    int free_elem_index = -1;
    ReturnIfError(FindFree(list, &free_elem_index));

    if (index != nullptr && index != POISON_PTR)
        *index = free_elem_index;
    
    ListElem* new_elem = &list->data[free_elem_index];
    new_elem->val = value;
    
    int next       = list->data[after_which].next;
    new_elem->next = next;
    new_elem->prev = after_which;

    list->data[next].prev        = free_elem_index; 
    list->data[after_which].next = free_elem_index;

    list->size++;

    return 0;
}

#endif //__SYM_LIST__