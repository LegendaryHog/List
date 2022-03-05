#include "List.h"

int main (void)
{
    List* list = (List*) calloc (1, sizeof (List));
    List_Ctor (list);
    Dbg_Dump (list);
    /*List_Ins_Aft (list, 0, 10);
    List_Ins_Aft (list, 1, 20);
    List_Ins_Aft (list, 2, 30);
    List_Ins_Aft (list, 3, 40);
    List_Delete  (list, 3);
    List_Delete  (list, 4);
    List_Delete  (list, 2);
    List_Delete  (list, 1);*/
    for (long long i = 0; i < 7; i++)
    {
        List_Ins_Aft (list, i, 10*i + 10);
    }
    List_Delete (list, 4);
    List_Ins_Aft (list, 0, 171);
    Dbg_Dump (list);
    List_Delete (list, 1);
    List_Dtor (list);
    free (list);
    return 0;
}