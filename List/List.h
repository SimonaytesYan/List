#ifndef __SYM_LIST__
#define __SYM_LIST__

#include <stdlib.h>

#include "..\Libs\Logging\Logging.h"
#include "..\Libs\Errors.h"

const int         ResizeCoef         = 2;
const void*       POISON_PTR         = (void*)13;
const ListElem_t  POISON             = 0X7FFFFFFF;
const char        COMAND_PROTOTYPE[] = "Dot GraphicDumps/dump%d -o GraphicDumps/Dump%d.png -T png";
      int         GRAPHIC_DUMP_CNT   = 0;

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

typedef struct List 
{
    size_t    size      = 0;
    size_t    capacity  = 0;
    ListElem* data      = nullptr;
    LogInfo   debug     = {};
    int       free      = -1;
    bool      linerized = false;
}List;

int  ListCheck(List* list);

int  ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file);

int  ListDtor(List* list);

void DumpList(List* list, const char* function, const char* file, int line);

void GraphicDump(List* list);

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

//!-----------------------
//!Not iterate if index  indicates to the end of the list
//!index will be -1 if there isn`t next element in list
//!
//!-----------------------
int ListIterate(List* list, int* index);

int ListBegin(List* list, int *index);

int ListEnd(List* list, int *index);

//!---------------------
//!@param [in]  list        List for searching logical index from physical
//!@param [in]  phys_index  Physical index
//!@param [out] log_index   Logical index of element(-1 if element not found)
//!@return Error code according to Errors.h
//!
//!---------------------
int PhysIndexToLogical(List* list, int phys_index, int* log_index);

int ListIterate(List* list, int* index)
{
    ReturnIfError(ListCheck(list));
    CHECK(index == nullptr || index == POISON_PTR, "index = nullptr", -1);

    if (*index < 0 || *index >= list->capacity)
        return 0;

    if (list->data[*index].next != 0)
        *index = list->data[*index].next;

    return 0;
}

int ListBegin(List* list, int *index)
{
    ReturnIfError(ListCheck(list));

    if (list->capacity >= 1)
        *index = list->data[0].next;
    else
        *index = -1;

    return 0;
}

int ListEnd(List* list, int *index)
{
    ReturnIfError(ListCheck(list));

    if (list->capacity >= 1)
        *index = list->data[0].prev;
    else
        *index = -1;

    return 0;    
}

int ListLinerization(List* list)
{
    ReturnIfError(ListCheck(list));

    if (list->size <= 1)
        return 0;

    ListElem* new_data = (ListElem*)calloc(list->capacity + 1, sizeof(ListElem));

    int index = 0;
    for(int i = 0; i < list->size; i++)
    {
        ListIterate(list, &index);
        if (index == -1)
            return -1;

        new_data[i + 1].val  = list->data[index].val;

        if (i + 1 == list->size)
            new_data[i + 1].next = 0;
        else
            new_data[i + 1].next = i + 2;
        new_data[i + 1].prev = i;
    }
    new_data[0].next = 1;
    new_data[0].prev = list->size;

    free(list->data);
    list->data = new_data;

    if (list->capacity != list->size)
    {
        list->free = list->capacity;
        for(int i = list->capacity; i > list->size + 1; i--)
        {
            list->data[i].prev = i + 1;
            list->data[i].next = -1;
        }
        list->data[list->size + 1].prev = -1;
        list->data[list->size + 1].next = -1;
    }

    list->linerized = true;

    return 0;
}

int PhysIndexToLogical(List* list, int phys_index, int* log_index)
{
    ReturnIfError(ListCheck(list));

    if (list->linerized)
    {
        *log_index = phys_index - 1;
        return 0;
    }
    int index = 0;
    *log_index = -1;
    for(int i = 0; i < list->size; i++)
    {
        ListIterate(list, &index);
        if (index == -1)
        {
            LogPrintf("Element in physical index %d not found\n", phys_index);
            return 0;
        }
        if (index == phys_index)
        {
            *log_index = i;
            break;
        }
    }
    
    return 0;
}

void GraphicDump(List* list)
{
    char name[20] = "";
    sprintf(name, "GraphicDumps/dump%d", GRAPHIC_DUMP_CNT);
    FILE* fp = fopen(name, "w");

    fprintf(fp, "digraph{\n");
    fprintf(fp, "rankdir=LR;\n"                                 \
                "node[shape = record, fontsize=14];\n"          \
                "edge[style = invis, constraint = true]\n"             \
                "splines=polyline\n");

    if (list == nullptr || list == POISON_PTR) 
        return;
    if (list->data == nullptr || list->data == POISON_PTR)
        return;

    fprintf(fp, "info[label = \"size = %d\\n | capasity = %d \\n | <f> free = %d \\n | linerized = %d \\n \"]\n", list->size, list->capacity, list->free, list->linerized);

    for(int i = 0; i <= list->capacity; i++)
    {
        fprintf(fp, "Node%d[label = \"<v>%d \\n | {<p> %d |<n>  %d}\"]\n", i, list->data[i].val, list->data[i].prev, list->data[i].next);
        if (i == 0)
            fprintf(fp, "info:s -> Node0:v:n\n");
        else 
            fprintf(fp, "Node%d->Node%d\n", i - 1, i);
    }

    int index = 0;
    fprintf(fp, "edge [style = solid, color = \"red\", constraint = false]\n");
    for(int i = 0; i <= list->size; i++)
    {
        int prev = list->data[index].prev;
        fprintf(fp, "Node%d:<p> -> Node%d\n", index, prev);
        index = prev;
    }
    
    fprintf(fp, "edge [style = solid, color = \"blue\", constraint = false]\n");
    index = 0;
    for(int i = 0; i <= list->size; i++)
    {
        int next = list->data[index].next;
        fprintf(fp, "Node%d:<n> -> Node%d\n", index, next);
        index = next;
    }

    fprintf(fp, "edge [style = solid, color = \"black\", constraint = false]\n");
    index = list->free;
    if (index != -1)
    {
        fprintf(fp, "info:<f> -> Node%d\n", index);
        for(int i = 0; i <= list->capacity; i++)    
        {
            int free_ind = list->data[index].prev;
            if (free_ind == -1)
                break;
            fprintf(fp, "Node%d -> Node%d\n", index, free_ind);
            index = free_ind;
        }
    }

    fprintf(fp, "}");

    fclose(fp);

    char comand[50] = "";
    sprintf(comand, COMAND_PROTOTYPE, GRAPHIC_DUMP_CNT, GRAPHIC_DUMP_CNT);
    system(comand);

    GRAPHIC_DUMP_CNT++;
}

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
    LogPrintf("\tfree     = %zu\n", list->free);

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
    list->free     = -1;
    list->data     = (ListElem*)calloc(capacity + 1, sizeof(ListElem));
    if (list->data != nullptr)
        for(int i = list->capacity; i >= 1; i--)
        {
            list->data[i] = {POISON, -1, list->free};
            list->free    = i;
        }

    list->debug.name     = name;
    list->debug.function = function;
    list->debug.file     = file;
    list->debug.line     = line;
    list->debug.status   = true;
    list->linerized      = true;

    return ListCheck(list);
}

int ListDtor(List* list)
{
    ListCheck(list);

    list->capacity  = POISON;
    list->size      = POISON;
    list->free      = POISON;
    list->linerized = false;

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

    *index = list->free;
    list->free = list->data[list->free].prev;

    return 0;
}

int ListPop(List* list, int index)
{
    ReturnIfError(ListCheck(list));

    CHECK(index > list->capacity || index <= 0, "Error index", -1);

    int next_ind = list->data[index].next;
    int prev_ind = list->data[index].prev;

    CHECK(next_ind == -1 || prev_ind == -1, "Index to not inserted element", -1);

    int tail = 0;
    ReturnIfError(ListEnd(list, &tail));
    if (index != tail)
        list->linerized = false;

    list->data[next_ind].prev = prev_ind;
    list->data[prev_ind].next = next_ind;

    list->data[index].val  = POISON;
    list->data[index].prev = list->free;
    list->data[index].next = -1;

    list->data[list->free].next = list->free;
    list->free = index;
 
    list->size--;

    return 0;
}

int ResizeUp(List* list, int new_capacity)
{
    ReturnIfError(ListCheck(list));

    list->data = (ListElem*)realloc(list->data, sizeof(ListElem)*(new_capacity + 1));

    if (list->data == nullptr)
        return MEMORY_ALLOCATION_ERROR;

    for(int i = new_capacity; i >= list->capacity + 1; i--)
    {
        list->data[i] = {POISON, -1, list->free};
        list->free = i;
    }
    
    list->capacity = new_capacity;

    return 0;
}

int ResizeIfNeed(List *list)
{
    if (list->capacity == list->size)
    {
        int new_capacity = 0;
        if (list->capacity == 0)
            new_capacity = 2;
        else
            new_capacity = list->capacity * ResizeCoef;
        ReturnIfError(ResizeUp(list, new_capacity));
    }

    return 0;
}

int ListInsert(List* list, int value, int after_which, int* index) 
{
    ReturnIfError(ListCheck(list));

    CHECK(after_which > list->capacity || after_which < 0, "Error index", -1);
    
    CHECK(list->data[after_which].next == -1 || list->data[after_which].prev == -1, "Index to not inserted element", -1);

    ReturnIfError(ResizeIfNeed(list));

    int free_elem_index = -1;
    ReturnIfError(FindFree(list, &free_elem_index));

    if (index != nullptr && index != POISON_PTR)
        *index = free_elem_index;

    int tail = 0;
    ReturnIfError(ListEnd(list, &tail));
    if (after_which != tail)
        list->linerized = false;
    
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