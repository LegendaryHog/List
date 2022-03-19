#include "List.h"

int main (void)
{
    List* list = (List*) calloc (1, sizeof (List));

    List_Ctor (list);
    Dbg_Dump (list);

    List_Ins_Aft (list, 0, 20);
    List_Ins_Aft (list, 0, 10);
    List_Ins_Aft (list, 1, 30);
    Dbg_Dump (list);
    Graph_Dump (list);
    Linear (list);
    Dbg_Dump (list);
    List_Dtor (list);
    free (list);
    return 0;
}