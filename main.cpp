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

    printf("End\n");
    CloseHtmlLogFile();
}