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
    PhysIndexToLogical(&list, 1, &index);
    printf("[1] = %d\n", index);

    ListInsert(&list, 200, 1);
    index = 0;
    PhysIndexToLogical(&list, 2, &index);
    printf("[2] = %d\n", index);
    ListInsert(&list, 400, 1);
    index = 0;
    PhysIndexToLogical(&list, 3, &index);
    printf("[3] = %d\n", index);
    ListInsert(&list, 500, 1);
    ListInsert(&list, 600, 1);
    ListInsert(&list, 700, 1);
    ListInsert(&list, 800, 1);
    ListInsert(&list, 900, 1);
    GraphicDump(&list);
    
    ListLinerization(&list);
    
    GraphicDump(&list);
    

    printf("End\n");
    CloseHtmlLogFile();
}

void PrintElemInLog(ListElem_t val)
{
    LogPrintf("%10d", val);
}