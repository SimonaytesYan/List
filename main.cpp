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
    ListCtor(&list, 3);

    ListInsert(&list, 100, 0, nullptr);
    ListInsert(&list, 200, 1, nullptr);
    ListInsert(&list, 300, 0, nullptr);
    ListInsert(&list, 400, 2, nullptr);

    GraphicDump(&list);

    for(int i = 0; i < 4; i++)
    {
        int index = 0;
        LogicalIndexToPhys(&list, i, &index);
        printf("[%d] = %d\n", i, index);
    }

    ListLinerization(&list);
    GraphicDump(&list);

    printf("\n");
    for(int i = 0; i < 4; i++)
    {
        int index = 0;
        LogicalIndexToPhys(&list, i, &index);
        printf("[%d] = %d\n", i, index);
    }

    /*LogPrintf("<h2> Лист создан, capacity = 3</h2>\n");
    GraphicDump(&list);

    ListInsert(&list, 100, 0);
    LogPrintf("<h2> Insert 100 после нулевого\n");
    GraphicDump(&list);

    int index = 0;
    ListEnd(&list, &index);
    ListInsert(&list, 200, index);
    LogPrintf("<h2> Insert 200 после последнего\n");
    GraphicDump(&list);

    ListInsert(&list, 300, 1);
    LogPrintf("<h2> Insert 300 после первого\n");
    GraphicDump(&list);
    
    ListInsert(&list, 400, 1);
    LogPrintf("<h2> Insert 400 после первого\n");
    GraphicDump(&list);
    
    ListInsert(&list, 500, 1);
    LogPrintf("<h2> Insert 500 после первого\n");
    GraphicDump(&list);

    ListRemove(&list, 4);
    
    ReturnIfError( ListLinerization(&list));
    LogPrintf("<h2> Линеризация\n");
    GraphicDump(&list);

    ListRemove(&list, 1);
    LogPrintf("<h2> Remove 1-го элемента\n");
    GraphicDump(&list);

    ListRemove(&list, 3);
    LogPrintf("<h2> Remove 3-го элемента\n");
    GraphicDump(&list);

    ReturnIfError( ListLinerization(&list));
    LogPrintf("<h2> Линеризация\n");
    GraphicDump(&list);
    
    ListRemove(&list, 1);
    LogPrintf("<h2> Remove 1-го элемента\n");
    GraphicDump(&list);
    
    ListRemove(&list, 2);
    LogPrintf("<h2> Remove 2-го элемента\n");
    GraphicDump(&list);

    ListRemove(&list, 1);
    GraphicDump(&list);*/

    printf("End\n");
    CloseHtmlLogFile();
}

void PrintElemInLog(ListElem_t val)
{
    LogPrintf("%10d", val);
}