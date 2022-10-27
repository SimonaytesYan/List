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
    int index = 0;
    ListEnd(&list, &index);
    ListInsert(&list, 200, index);
    ListEnd(&list, &index);
    ListInsert(&list, 300, index);
    ListEnd(&list, &index);
    ListInsert(&list, 400, index);
    ListEnd(&list, &index);
    ListInsert(&list, 500, index);
    ListEnd(&list, &index);
    ListInsert(&list, 600, index);
    ListEnd(&list, &index);
    ListInsert(&list, 700, index);
    ListEnd(&list, &index);
    ListInsert(&list, 800, index);
    ListEnd(&list, &index);
    ListInsert(&list, 900, index);

    
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