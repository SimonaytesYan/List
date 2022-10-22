#ifndef __SYM_LIST__
#define __SYM_LIST__

#include <stdlib.h>

#include "..\Libs\Logging\Logging.h"
#include "..\Libs\Errors.h"

const void*       POISON_PTR = (void*)13;
const ListElem_t  POISON     = 0X7FFFFFFF;

struct ListElem {
    int        next = -1;
    int        prev = -1;
    ListElem_t val  = 0;
} typedef ListElem;

struct List {
    size_t    size     = 0;
    size_t    capacity = 0;
    size_t    H        = 0;   //!<Head
    size_t    T        = 0;   //!<Tail
    ListElem* data     = (ListElem*)POISON_PTR;
} typedef List;

int ListCheck(List* list)
{
    CHECK(list == nullptr,           NULL_LIST_POINTER);
    CHECK(list == (List*)POISON_PTR, NULL_LIST_POINTER);

    CHECK(list->size     == POISON, WRONG_SIZE);
    CHECK(list->capacity == POISON, WRONG_CAPACIRY);
    CHECK(list->H        == POISON, WRONG_HEAD);
    CHECK(list->T        == POISON, WRONG_TAIL);
    CHECK(list->data     == nullptr || 
          list->data     == POISON_PTR, DAMAGED_DATA); 

    return 0;
}

int ListCtor(List* list, int capacity)
{
    CHECK(list == nullptr, NULL_LIST_POINTER);

    list->size     = 0;
    list->capacity = capacity;
    list->H        = 0;
    list->T        = 0;
    list->data     = (ListElem*)calloc(capacity, sizeof(ListElem));

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

    return 0;
}

#endif //__SYM_LIST__