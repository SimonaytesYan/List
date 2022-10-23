#include <stdio.h>

typedef int ListElem_t;
#include "List\List.h"
#include "Libs\Logging\Logging.h"

const char logs[] = "ListLogs.log";

#define DEBUG

int main()
{
    OpenLogFile(logs);

    List list = {};
    ListCtor(&list, 15);

    for(int i = 1; i <= 10; i++)
        list.data[i] = {i, (i+1)%11, i-1};

    list.size = 11;
    ListInsert(&list, 100, 2);

    DUMP_L(&list);

    ListPop(&list, 3);

    DUMP_L(&list);
    
    ListPop(&list, 2);

    DUMP_L(&list);

    ListDtor(&list);

    CloseLogFile();
}