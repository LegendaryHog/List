#ifndef LISTH
#define LISTH

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CAPACITY_0 8
#define POISON     (1000 - 7)
#define START_END  0
#define LIST_CHECK if (List_Ok (list) == ERR){Dbg_Dump(list); return ERR;}

enum ERRORS {
    NO_ERR = 0,
    ERR = 1,
};

typedef int data_t;
typedef unsigned char flag_t;

typedef struct List {
    data_t*    data;
    long long* next;
    long long* prev;
    long long  free;
    long long  size;
    long long  capacity;
    FILE*      logfile;
    flag_t     okmask;
    int        linflag;     
} List;

int List_Ctor    (List* list);
int List_Dtor    (List* list);
int List_Ins_Aft (List* list, long long last, data_t push);
int List_Delete  (List* list, long long elem);
int Dbg_Dump     (List* list);
int List_Resize  (List* list);
int List_Check   (List* list);
int List_Ok      (List* list);

#endif //LISTH