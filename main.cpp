#include <stdio.h>
#include <string.h>

typedef int ListElem_t;

#include "Libs\Logging\Logging.h"

void PrintElemInLog(ListElem_t val);

#include "List\List.h"

const char logs[] = "ListLogs.html";

int main()
{
    OpenHtmlLogFile(logs);

    printf("Start\n");

    List list = {};
    ListCtor(&list, 3);

    LogPrintf("<h2> ???? ??????, capacity = 3</h2>\n");
    GraphicDump(&list);

    ListInsert(&list, 100, 0);
    LogPrintf("<h2> Insert 100 ????? ????????\n");
    GraphicDump(&list);

    int index = 0;
    ListEnd(&list, &index);
    ListInsert(&list, 200, index);
    LogPrintf("<h2> Insert 200 ????? ??????????\n");
    GraphicDump(&list);

    ListInsert(&list, 300, 1);
    LogPrintf("<h2> Insert 300 ????? ???????\n");
    GraphicDump(&list);
    
    ListInsert(&list, 400, 1);
    LogPrintf("<h2> Insert 400 ????? ???????\n");
    GraphicDump(&list);
    
    ListInsert(&list, 500, 1);
    LogPrintf("<h2> Insert 500 ????? ???????\n");
    GraphicDump(&list);

    ListRemove(&list, 4);

    for(int i = 1; i <= list.capacity; i++)
    {
        int index = 0;
        LogicalIndexToPhys(&list, i, &index);
        printf("[%d] = %d\n", i, index);
    }
    
    ReturnIfError(ListLinerization(&list));
    LogPrintf("<h2> ???????????\n");
    GraphicDump(&list);

    ListRemove(&list, 1);
    LogPrintf("<h2> Remove 1-?? ????????\n");
    GraphicDump(&list);

    ListRemove(&list, 3);
    LogPrintf("<h2> Remove 3-?? ????????\n");
    GraphicDump(&list);

    ReturnIfError(ListLinerization(&list));
    LogPrintf("<h2> ???????????\n");
    GraphicDump(&list);
    
    ListRemove(&list, 1);
    LogPrintf("<h2> Remove 1-?? ????????\n");
    GraphicDump(&list);
    
    ListRemove(&list, 2);
    LogPrintf("<h2> Remove 2-?? ????????\n");
    GraphicDump(&list);

    ListRemove(&list, 1);
    GraphicDump(&list);

    printf("End\n");
    CloseHtmlLogFile();
}

void PrintElemInLog(ListElem_t val)
{
    LogPrintf("%10d", val);
}