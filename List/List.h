#ifndef __SYM_LIST__
#define __SYM_LIST__

#include <stdlib.h>

#include "..\Libs\Logging\Logging.h"
#include "..\Libs\Errors.h"

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
    size_t    H        = 0;   //!<Head
    size_t    T        = 0;   //!<Tail
    ListElem* data     = nullptr;
    LogInfo   debug    = {};
}List;

int  ListCheck(List* list);
int  ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file);
int  ListDtor(List* list);
void DumpList(List* list, const char* function, const char* file, int line);

//TODO: html dump, для равнения

//!---------------------
//!@param [in]  list        List for inserting an element
//!@param [in]  val         Value of new element
//!@param [in]  after_which Index of element after which put new element
//!@param [out] index       (Optional).Index in which new element will insert
//!@return Error code according to Errors.h
//!
//!---------------------
int ListInsert(List* list, int value, int after_which, int* index = nullptr);

int  ListPop(List* list, int index);
int  FindFree(List* list, int* index);

#define DUMP_L(list) DumpList(list, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define ReturnIfError(func)                 \
    {                                       \
        int error = func;                   \
        if (error != NO_ERROR)              \
            return error;                   \
    }

void DumpList(List* list, const char* function, const char* file, int line)
{
    LogPrintf("\nDump in %s(%d) in function %s\n", file, line, function);

    int errors = ListCheck(list); 
    if (errors != 0)
        ParseErrorCode(errors);
    
    if (list == nullptr || list == POISON_PTR) 
        return;

    LogPrintf("Stack[%p] \"%s\" created at %s at %s(%d):\n", 
                    list, list->debug.name, list->debug.function, list->debug.file, list->debug.line);

    LogPrintf("Status: ");
    if (list->debug.status)
        LogPrintf("enable\n");
    else
        LogPrintf("disable\n");

    LogPrintf("{\n");
    LogPrintf("\tsize     = %zu\n", list->size);
    LogPrintf("\tcapacity = %zu\n", list->capacity);
    LogPrintf("\thead     = %zu\n", list->H);
    LogPrintf("\ttail     = %zu\n", list->T);

    if (list->data != nullptr && list->data != POISON_PTR)
    {
        
        LogPrintf("\n\tdata:\t next:\t prev:\n");
        for(int i = 0; i < list->capacity; i++)
            LogPrintf("\t|%d|\t\t|%d|\t\t|%d|\n", list->data[i].val, list->data[i].next, list->data[i].prev);
        LogPrintf("\n");
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

    LogAndParseErr(error != 0, error);
    
    return error;
}

#define ListCtor(list, capacity)  ListConstructor(list, capacity, __LINE__, #list, __PRETTY_FUNCTION__, __FILE__)

int ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file)
{
    LogAndParseErr(list == nullptr, NULL_LIST_POINTER);

    list->size     = 0;
    list->capacity = capacity;
    list->H        = 0;
    list->T        = 0;
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

int FindFree(List* list, int* index)
{
    ReturnIfError(ListCheck(list));

    for(int i = 0; i < list->capacity; i++)
        if (list->data[i].next == -1)
        {
            *index = i;
            return 0;
        }

    LogPrintf("Free element not found\n\n");
    return -1;
}

int ListPop(List* list, int index)
{
    ReturnIfError(ListCheck(list));

    CHECK(index >= list->size || index <= 0, "Error index", -1);
    
    int next_ind = list->data[index].next;
    int prev_ind = list->data[index].prev;

    if (index != list->H && index != list->T)
    {
        list->data[next_ind].prev = prev_ind;
        list->data[prev_ind].next = next_ind;
    }

    list->data[index].val  = POISON;
    list->data[index].prev = -1;
    list->data[index].next = -1;

    list->size--;

    return 0;
}

int ListInsert(List* list, int value, int after_which, int* index) 
{
    ReturnIfError(ListCheck(list));

    CHECK(after_which > list->size || after_which < 0, "Error index", -1);

    int free_elem_index = -1;
    ReturnIfError(FindFree(list, &free_elem_index));
    if (index != nullptr && index != POISON_PTR)
        *index = free_elem_index;

    list->size++;
    ListElem* new_elem = &list->data[free_elem_index];
    new_elem->val = value;

    if (after_which != -1 && after_which != list->H && after_which != list->T)
    {
        if (after_which != list->H && after_which != list->T)
        {
            int next       = list->data[after_which].next;
            new_elem->next = next;
            new_elem->prev = after_which;

            list->data[next].prev        = free_elem_index; 
            list->data[after_which].next = free_elem_index;
        }
    }    

    return 0;
}

#endif //__SYM_LIST__