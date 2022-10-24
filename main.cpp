#include <stdio.h>

typedef int ListElem_t;
#include "List\List.h"
#include "Libs\Logging\Logging.h"

const char logs[] = "ListLogs.html";

#define DEBUG

int main()
{
    OpenHtmlLogFile(logs);

    printf("Start\n");

    List list = {};
    ListCtor(&list, 0);

    ListInsert(&list, 100, 0);
    ListDtor(&list);

    DUMP_L(&list);

    ListInsert(&list, 200, 0);

    DUMP_L(&list);
    
    ListInsert(&list, 300, list.T);

    DUMP_L(&list);

    
    ListPop(&list, 1);

    DUMP_L(&list);

    ListPop(&list, 2);

    DUMP_L(&list);
    
    ListPop(&list, 3);

    DUMP_L(&list);

    ListDtor(&list);

    DUMP_L(&list);


    CloseHtmlLogFile();
}