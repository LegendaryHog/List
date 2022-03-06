#include "List.h"

int main (void)
{
    List* list = (List*) calloc (1, sizeof (List));
    List_Ctor (list);
    Dbg_Dump (list);

    //List_Ins_Aft (list, );
    Linear (list);
    Dbg_Dump (list);
    List_Dtor (list);
    free (list);
    return 0;
}