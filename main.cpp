#include <stdio.h>

typedef int ListElem_t;

#include "Libs\Logging\Logging.h"

void PrintElemInLog(ListElem_t val);

#include "List\List.h"

const char logs[] = "ListLogs.html";

#define DEBUG

int main()
{
    OpenHtmlLogFile(logs);

    printf("Start\n");

    List list = {};
    ListCtor(&list, 0);
    DUMP_L(&list);

    ListInsert(&list, 100, 0);
    DUMP_L(&list);
    ListInsert(&list, 200, 2);
    DUMP_L(&list);
    ListInsert(&list, 300, 1);

    DUMP_L(&list);

    printf("End\n");
    CloseHtmlLogFile();
}

void PrintElemInLog(ListElem_t val)
{
    LogPrintf("%10d", val);
}