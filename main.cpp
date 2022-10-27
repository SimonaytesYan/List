#include <stdio.h>
#include <string.h>


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

    ListInsert(&list, 100, 0);
    ListInsert(&list, 200, 1);
    ListInsert(&list, 400, 1);
    ListInsert(&list, 500, 1);
    ListInsert(&list, 600, 1);
    ListInsert(&list, 700, 1);
    ListInsert(&list, 800, 1);
    ListInsert(&list, 900, 1);
    GraphicDump(&list);
    
    ListLinerization(&list);
    
    GraphicDump(&list);

    ListSwap(&list, 1, 5);

    GraphicDump(&list);

    printf("End\n");
    CloseHtmlLogFile();
}

void PrintElemInLog(ListElem_t val)
{
    LogPrintf("%10d", val);
}