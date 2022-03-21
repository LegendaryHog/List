#include "List.h"

//!@(
/*Masks of errors*/
#define DATA_NULL  (1<<0)
#define NEXT_NULL  (1<<1)
#define PREV_NULL  (1<<2)
#define BAD_CAP    (1<<3)
#define BAD_FREE   (1<<4)
#define BAD_SIZE   (1<<5)
#define LOG_NULL   (1<<6)
//!@)

#define ERR_NUM    7 ///< number of possible erros

#define MAX_LEN_ERR 24 ///< maximum of length for string in massive with errors messages

//! Massive of errors messages
char err_names [][MAX_LEN_ERR] = {
    {"data pointer is NULL\n"},
    {"next pointer is NULL\n"},
    {"prev pointer is NULL\n"},
    {"bad capacity\n"},
    {"free element is null\n"},
    {"bad size\n"},
    {"logfile pointer is NULL"}
};


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
    list->free    = START_END + 1;
    list->linflag = 1;
    list->logfile = fopen ("logfile.txt", "w");
    list->okmask = 0;
    LIST_CHECK
    return NO_ERR;
}

int List_Dtor (List* list)
{
    assert (list != NULL);
    LIST_CHECK
    free (list->data);
    free (list->next);
    free (list->prev);
    fclose (list->logfile);
    list->data = (data_t*)POISON;
    list->next = (long long*)POISON;
    list->prev = (long long*)POISON;
    list->free = POISON;
    return NO_ERR;
}

int List_Ins_Aft (List* list, long long last, data_t push)
{
    assert (list != NULL);
    LIST_CHECK
    if (last < 0 || last >= list->capacity)
    {
        fprintf (list->logfile, "Illegal last elem for insert last = %lld\nNothing was done in function.\n", last);
        return ERR;
    }

    if (list->linflag == 1 && last != list->size)
    {
        list->linflag = 0;
    }

    if (list->size + 1 == list->capacity)
    {
        List_Resize (list);
    }
    list->size++;
    long long free  = list->free;          //free save
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
    LIST_CHECK
    if (list->size == 0)
    {
        fprintf (list->logfile, "Delete from empty list\n");
        fprintf (list->logfile, "Nothing was done in function, returned ERR\n");
        return ERR;
    }
    else if (elem == START_END)
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

    if (list->linflag == 1 && elem != list->size - 1)
    {
        list->linflag = 0;
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
    if ((list->okmask & LOG_NULL) != 0)
    {
        printf ("%u\n", list->okmask);
        printf ("LOGFILE POINTER IS NULL, U FUCKED UP\n");
        return ERR;
    }
    
    fprintf (list->logfile, "List [%p].", list);
    if (list->okmask != 0)
    {
        fprintf (list->logfile, "List is BAD:");
        for (int i = 0; i < ERR_NUM; i++)
        {
            if ((list->okmask & 1<<i) != 0)
            {
                fputs (err_names[i], list->logfile);
            }
        }
    }
    else
    {
        fprintf (list->logfile, "List is Ok\n");
    }
    fprintf (list->logfile, "\tsize = %lld\n\tcapacity = %lld\n\tfree = %lld\n\tlinearized = %d\n", list->size, list->capacity, list->free, list->linflag);
    fprintf (list->logfile, "\tdata [%p]\n\t", list->data);
    if ((list->okmask & DATA_NULL) == 0)
    {
        for (long long i = 0; i < list->capacity; i++)
        {
            fprintf (list->logfile, "[%3lld] ", i);
        }
        fprintf (list->logfile, "\n\t");
        for (long long i = 0; i < list->capacity; i++)
        {
            fprintf (list->logfile, "[%3d] ", list->data[i]);
        }
    }
    fprintf (list->logfile, "\n\t");
    fprintf (list->logfile, "next [%p]\n\t", list->next);
    if ((list->okmask & NEXT_NULL) == 0)
    {
        for (long long i = 0; i < list->capacity; i++)
        {
            fprintf (list->logfile, "[%3lld] ", i);
        }
        fprintf (list->logfile, "\n\t");
        for (long long i = 0; i < list->capacity; i++)
        {
            fprintf (list->logfile, "[%3lld] ", list->next[i]);
        }
    }
    fprintf (list->logfile, "\n\t");
    fprintf (list->logfile, "prev [%p]\n\t", list->prev);
    if ((list->okmask & PREV_NULL) == 0)
    {
        for (long long i = 0; i < list->capacity; i++)
        {
            fprintf (list->logfile, "[%3lld] ", i);
        }
        fprintf (list->logfile, "\n\t");
        for (long long i = 0; i < list->capacity; i++)
        {
            fprintf (list->logfile, "[%3lld] ", list->prev[i]);
        }
    }
    fprintf (list->logfile, "\n");
    return NO_ERR;
}

int List_Resize (List* list)
{
    assert (list != NULL);
    assert (list->data != NULL);
    assert (list->next != NULL);
    assert (list->prev != NULL);

    list->capacity *= 2;

    list->data = (data_t*) realloc (list->data, list->capacity * sizeof (data_t));
    for (long long i = list->capacity / 2; i < list->capacity; i++)
    {
        list->data[i] = 0;
    }
    list->next = (long long*) realloc (list->next, list->capacity * sizeof (long long));
    for (long long i = list->capacity / 2; i < list->capacity; i++)
    {
        list->next[i] = - (i + 1);
    }
    list->prev = (long long*) realloc (list->prev, list->capacity * sizeof (long long));
    for (long long i = list->capacity / 2; i < list->capacity; i++)
    {
        list->prev[i] = -1;
    }
    return NO_ERR;
}

int Linear (List* list)
{
    assert (list != NULL);
    LIST_CHECK
    data_t*    new_data = (data_t*)    calloc (list->capacity, sizeof (data_t));
    long long* new_next = (long long*) calloc (list->capacity, sizeof (long long));
    long long* new_prev = (long long*) calloc (list->capacity, sizeof (long long));

    long long j = 0;
    for (long long i = 0; i < list->size; i++)
    {
        new_data[i + 1] = list->data[list->next[j]];
        j = list->next[j];
    }
    list->free = list->size + 1;
    for (long long i = 0; i < list->free; i++)
    {
        new_next[i] = (i + 1) % (list->free);
    }
    for (long long i = list->free; i < list->capacity; i++)
    {
        new_next[i] = -(i + 1);
    }
    new_prev[START_END] = list->size;
    for (long long i = 1; i < list->free; i++)
    {
        new_prev[i] = (i - 1);
    }
    for (long long i = list->free; i < list->capacity; i++)
    {
        new_prev[i] = -1;
    }

    free (list->data);
    list->data = new_data;
    free (list->next);
    list->next = new_next;
    free (list->prev);
    list->prev = new_prev;

    list->linflag = 1;
    LIST_CHECK
    //Delay (30);
    return NO_ERR;
}

int List_Ok (List* list)
{
    assert (list != NULL);

    list->okmask |= DATA_NULL * (list->data == NULL);
    list->okmask |= NEXT_NULL * (list->next == NULL);
    list->okmask |= PREV_NULL * (list->prev == NULL);
    list->okmask |= BAD_CAP   * (list->capacity < CAPACITY_0);
    list->okmask |= BAD_FREE  * (list->free <= 0 || list->free > list->capacity);
    list->okmask |= BAD_SIZE  * (list->size >= list->capacity || list->size < 0);
    list->okmask |= LOG_NULL  * (list->logfile == NULL);

    if (list->okmask != 0)
    {
        return ERR;
    }
    else
    {
        return NO_ERR;
    }
}

int Delay (float sec)
{
    clock_t start_time = clock ();

    while ((double)((clock() - start_time)/CLOCKS_PER_SEC) < sec) {;}

    return 0;
}

long long Logic_To_Phys (List* list, long long lognum)
{
    assert (list != NULL);
    LIST_CHECK

    if (lognum < 0)
    {
        fprintf (list->logfile, "lOgical number less null\n");
        return ERR;
    }

    long long physnum = 0;
    for (long long i = 0; i < lognum; i++)
    {
        physnum = list->next[physnum];
    }
    return physnum;
}

long long Phys_To_Logic (List* list, long long physnum)
{
    assert (list != NULL);
    LIST_CHECK
    if (physnum >= list->capacity || physnum < 0)
    {
        fprintf (list->logfile, "Physnum not index inside allocated memory for data. physnum = %lld\n", physnum);
        return ERR;
    }
    else if (list->prev[physnum] < 0)
    {
        fprintf (list->logfile, "Node witn physical number physnum = %lld dont actually exist\n", physnum);
        return ERR;
    }
    long long lognum = 0;
    long long i      = 0;
    while (i != physnum)
    {
        i = list->next[i];
        lognum++;
    }
    return lognum;
}

int Graph_Dump (List* list)
{
    assert (list != NULL);
    LIST_CHECK
    FILE* graph = fopen ("logs/graph_dump.dot", "w");
    fprintf (graph, "digraph G{\n");
    fprintf (graph, "\trankdir=LR;\n");
    fprintf (graph, "\tnode[color=\"red\",shape=record];\n");
    fprintf (graph, "\tLIST [color=black, style=filled, fillcolor=aqua, label=\" List:\\n%p |  data:\\n%p | next:\\n%p |", list, list->data, list->next);
    fprintf (graph, " prev:\\n%p | size: %lld | capacity: %lld | logfile:\\n%p |", list->prev, list->size, list->capacity, list->logfile);
    fprintf (graph, " okmask: 0x%02X | linflag: %d \"];\n", list->okmask, list->linflag);
    long long i = 0;
    for (long long iter = 0; iter < list->size + 1; iter++)
    {
        fprintf (graph, "\tNODE%lld ", iter);
        fprintf (graph, "[label=\" <elem%lld> ", iter);
        if (i == START_END)
        {
            fprintf (graph, "FicNode\\n");
        }
        fprintf (graph, " %lld | ", i);
        if (i != START_END)
        {
            fprintf (graph, "data:\\n %d |", list->data[i]);
        }
        fprintf (graph, " <next%lld>  next:\\n %lld | <prev%lld> prev:\\n %lld \"", iter, list->next[i], iter, list->prev[i]);
        if (i == START_END)
        {
            fprintf (graph, "style=filled, fillcolor=aquamarine];\n");
        }
        else
        {
            fprintf (graph, "style=filled, fillcolor=greenyellow];\n");
        }
        i = list->next[i];
    }
    fprintf (graph, "\tLIST -> NODE0[color=invis];\n");
    for (long long j = 0; j < list->size; j++)
    {
        fprintf (graph, "\tNODE%lld -> NODE%lld [color=invis];\n", j, j + 1);
    }
    for (long long j = 0; j < list->size; j++)
    {
        fprintf (graph, "\tNODE%lld:<next%lld> -> NODE%lld:<elem%lld>[color=green, dir=both, arrowhead=normal, arrowtail=dot, style=\"bold\"];\n", j, j, j + 1, j + 1);
    }
    for (long long j = 1; j < list->size + 1; j++)
    {
        fprintf (graph, "\tNODE%lld:<prev%lld> -> NODE%lld:<elem%lld>[color=blue, dir=both, arrowhead=normal, arrowtail=dot, style=\"bold\"];\n", j, j, j - 1, j - 1);
    }
    fprintf (graph, "}\n");
    fclose (graph);
    system ("dot -Tpng logs/graph_dump.dot -o logs/Graph_Dump.png\n");
    return NO_ERR;
}
