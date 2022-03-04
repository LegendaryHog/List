#include "List.h"

int List_Ctor (List* list)
{
    assert (list != NULL);
    list->data = (data_t*)    calloc (CAPACITY_0, sizeof (data_t));
    list->next = (long long*) calloc (CAPACITY_0, sizeof (long long));
    for (long long i = 1; i < CAPACITY_0; i++)
    {
        list->next[i] = -(i + 1);
    }
    list->prev = (long long*) calloc (CAPACITY_0, sizeof (long long));
    for (long long i = 1; i < CAPCITY_0; i++)
    {
        list->prev[i] = -1;
    }
    list->free = 1;
    return NO_ERR;
}

int List_Dtor (List* list)
{
    assert (list != NULL);
    
    free (list->data);
    free (list->next);
    free (list->prev);
    list->data = (void*)POISON;
    list->next = (void*)POISON;
    list->prev = (void*)POISON;
    list->free = POISON:
    return NO_ERR;
}

int List_Ins_Aft (List* list, long long last, data_t push)
{
    long free = list->free;                //free save
    list->free = - list->next[list->free]; //free up

    list->data[list->free] = push;       //push 

    list->next[free] = list->next[last]; //next[free] = next[last] (free -> next[last])
    list->next[last] = free;             //next[last] = free (last -> free)

    list->prev[free] = last;             //last -> free
    list->prev[list->next[free]] = free; // free -> next[free]

    return NO_ERR;
}

int List_Delete (List* list, long long elem)
{
    assert (list != NULL);
    list->data[elem] = 0; //clean elem

    list->next[list->prev[elem]] = list->next[elem];
    list->prev[list->next[elem]] = list->prev[elem];
    
    list->next[elem] = list->free;
    list->free = elem;

    return NO_ERR;
}