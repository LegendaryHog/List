#include "List.h"

int List_Ctor (List* list)
{
    assert (list != NULL);

    list->data = (data_t*) calloc (CAPACITY_0, sizeof (data_t));
    list->size = 0;
    list->capacity = CAPACITY_0;
    list->next = (long long*) calloc (CAPACITY_0, sizeof (long long));
    for (long long i = 1; i < CAPACITY_0; i++)
    {
        list->next[i] = -(i + 1);
    }
    list->prev = (long long*) calloc (CAPACITY_0, sizeof (long long));
    for (long long i = 1; i < CAPACITY_0; i++)
    {
        list->prev[i] = -1;
    }
    list->free = 1;
    list->head = 0;
    list->tail = 0;
    list->logfile = fopen ("logfile.txt", "w");
    return NO_ERR;
}

int List_Dtor (List* list)
{
    assert (list != NULL);
    
    free (list->data);
    free (list->next);
    free (list->prev);
    fclose (list->logfile);
    list->data = (void*)POISON;
    list->next = (void*)POISON;
    list->prev = (void*)POISON;
    list->free = POISON;
    return NO_ERR;
}

int List_Ins_Aft (List* list, long long last, data_t push)
{
    assert (list != NULL);
    if (last < 0 || last >= list->capcity)
    {
        fprintf (list->logfile, "Illegal last elem for insert last = %lld\nNothing was done in function.\n", last);
        return ERR;
    }

    list->size++;
    long free  = list->free;               //free save
    list->free = - list->next[list->free]; //free up

    list->data[free] = push; //push 

    list->next[free] = list->next[last]; //next[free] = next[last] (free -> next[last])
    list->next[last] = free;             //next[last] = free (last -> free)

    list->prev[free] = last;             //last -> free
    list->prev[list->next[free]] = free; // free -> next[free]

    return NO_ERR;
}

int List_Delete (List* list, long long elem)
{
    assert (list != NULL);
    if (elem == 0)
    {
        fprintf (list->logfile, "U can't delete START/END element, elem = %lld\n", elem);
        fprintf (list->logfile, "Nothing was done in function, returned ERR\n");
        return ERR;
    }
    else if (elem < 0 || elem >= list->capacity)
    {
        fprintf (list->logfile, "Illegal elem to delete, elem not in allocated memory, elem = %lld\n", elem);
        fprintf (list->logfile, "Nothing was done in function, returned ERR\n");
        return ERR;
    }
    else if (list->next[elem] < 0)
    {
        fprintf (list->logfile, "Illegal elem to delte, elem does not belong to list, elem = %lld\n", elem);
        fprintf (list->logfile, "Nothing was done in function, returned ERR\n");
        return ERR;
    }
    list->size--;
    list->data[elem] = 0; //clean elem

    list->next[list->prev[elem]] = list->next[elem];
    list->prev[list->next[elem]] = list->prev[elem];

    list->prev[elem] = -1;
    list->next[elem] = -list->free;
    list->free = elem;

    return NO_ERR;
}

int Dbg_Dump (List* list)
{
    assert (list != NULL);

    fprintf (list->logfile, "List [%p]\n", list);
    fprintf (list->logfile, "\tsize = %lld\n\tcapacity = %lld\n\tfree = %lld\n", list->size, list->capacity, list->free);
    fprintf (list->logfile, "\tdata [%p]\n\t", list->data);
    for (long long i = 0; i < list->capacity; i++)
    {
        fprintf (list->logfile, "[%3lld] ", i);
    }
    fprintf (list->logfile, "\n\t");
    for (long long i = 0; i < list->capacity; i++)
    {
        fprintf (list->logfile, "[%3d] ", list->data[i]);
    }
    fprintf (list->logfile, "\n\t");
    fprintf (list->logfile, "next [%p]\n\t", list->next);
    for (long long i = 0; i < list->capacity; i++)
    {
        fprintf (list->logfile, "[%3lld] ", i);
    }
    fprintf (list->logfile, "\n\t");
    for (long long i = 0; i < list->capacity; i++)
    {
        fprintf (list->logfile, "[%3lld] ", list->next[i]);
    }
    fprintf (list->logfile, "\n\t");
    fprintf (list->logfile, "prev [%p]\n\t", list->prev);
    for (long long i = 0; i < list->capacity; i++)
    {
        fprintf (list->logfile, "[%3lld] ", i);
    }
    fprintf (list->logfile, "\n\t");
    for (long long i = 0; i < list->capacity; i++)
    {
        fprintf (list->logfile, "[%3lld] ", list->prev[i]);
    }
    fprintf (list->logfile, "\n");
    return NO_ERR;
}

int List_Resize (List* list)
{
    list->capacity *= 2;

    list->data = (data_t*) realloc (list->data, list->capacity * sizeof (data_t));
    for (long long i = list->capcity / 2; i < list->capacity; i++)
    {
        list->data[i] = 0;
    }
    list->next = (long long*) realloc (list->next, list->capacity * sizeof (data_t));
    for (long long i = list->capcity / 2; i < list->capacity; i++)
    {
        list->next[i] = - (list->capacity/2 + 1);
    }
    list->prev = (long long*) realloc (list->next, list->capacity * sizeof (data_t));
    for (long long i = list->capcity / 2; i < list->capacity; i++)
    {
        list->prev[i] = -1;
    }
    return NO_ERR;
}