#ifndef LISTH
#define LISTH

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CAPACITY_0 8
#define POISON     (1000 - 7)

enum ERRORS{
    NO_ERR = 0,
    ERR = 1,
};

typedef int data_t;

typedef struct List {
    data_t*    data;
    long long* next;
    long long* prev;
    long long  free;
    long long  head;
    long long  tail;
    long long  size;
    long long  capacity;
} List;

int List_Ctor    (List* list);
int List_Dtor    (List* list);
int List_Ins_Aft (List* list, long long last, data_t push);
int List_Delete  (List* list);

#endif //LISTH
