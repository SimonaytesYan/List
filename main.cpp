#include <stdio.h>

typedef int ListElem_t;
#include "List\List.h"
#include "Libs\Logging\Logging.h"

const char logs[] = "ListLogs.log";

int main()
{
    OpenLogFile(logs);

    List list = {};
    ListCtor(&list, 10);

    list.data = nullptr;
    ListCheck(&list);
    ListCheck(nullptr);
    ListCheck((List*)POISON_PTR);

    ListDtor(&list);

    CloseLogFile();
}