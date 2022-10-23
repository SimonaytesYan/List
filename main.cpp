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
    ListCtor(&list, 10);

    DUMP_L(&list);

    ListDtor(&list);

    CloseLogFile();
}