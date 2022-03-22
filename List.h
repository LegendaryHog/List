#ifndef LISTH
#define LISTH

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define CAPACITY_0 8 ///< start size of massives
#define POISON     (1000 - 7)
#define START_END  0 ///< number of fictive node in massives
#define LIST_CHECK if (List_Ok (list) == ERR){Dbg_Dump(list); Graph_Dump(list); return ERR;}

/*! Enum with errors codes*/
enum ERRORS {
    NO_ERR = 0,
    ERR = 1,
};

typedef int data_t; ///< generalization for any type
typedef unsigned char mask_t;
/*! Struct with massives and variables*/
typedef struct List {
    data_t*    data;
    long long* next;
    long long* prev;
    long long  free;
    long long  size;
    long long  capacity;
    FILE*      logfile;
    mask_t     okmask;
    int        linflag;     
} List;

/*!
Function that initializes structure fields and allocate memory.
\param [in,out] list - pointer on struct, that must be alloctae on sizeof (List)
\return - ERR - if some errors happened, else - NO_ERR.
*/
int List_Ctor    (List* list);

/*!
Fucntion that destructs your list. Use it if you don't need list
param [in,out] list - pointer on struct, that must be ctored before
\return - ERR if some errors happened, else - NO_ERR.
*/
int List_Dtor    (List* list);

/*!
Fucntion that insert node after "last" node.
param [in,out] list - pointer on struct, that must be ctored before
param [in] last - physical index of node in data massive, after that inseret new node
param [in] push - data, that will be saved in new node
\return - ERR if some errors happened, else - NO_ERR.
*/
int List_Ins_Aft (List* list, long long last, data_t push);

/*!
Function that delete node with index "elem"
param [in,out] list - pointer on struct, that must be ctored before
param [in] elem - physical index in data massive of node that need to delete. This node must exist.
\return - ERR if some errors happened, else - NO_ERR.
*/
int List_Delete  (List* list, long long elem);

/*!
Function that make text dump of list in logs/logfile.txt
param [in,out] list - pointer on struct, that must be ctored before
\return - ERR if some errors happened, else - NO_ERR.
*/
int Dbg_Dump     (List* list);

/*!
Function that reallocate (in high) massives. Call in List_Ins_Aft (). You don't have to call it by yourself. 
param [in,out] list - pointer on struct, that must be ctored before
\return - ERR if some errors happened, else - NO_ERR.
*/
int List_Resize  (List* list);

/*!
Function that checkы struct list
param [in,out] list - pointer on struct, that must be ctored before
\return - ERR if something fields is wrong, else - NO_ERR.
*/
int List_Ok      (List* list);

int Delay        (float sec);

/*!
Function that linerizes massives: make logical number equal to physical.
param [in,out] list - pointer on struct, that must be ctored before
\return - ERR if some errors happened, else - NO_ERR.
*/
int Linear       (List* list);

/*!
Function that generate dot file logs/graph_dump.dot in dot syntax to make visualized dump in logs/Graph_Dump.png
param [in,out] list - pointer on struct, that must be ctored before
\return - ERR if some errors happened, else - NO_ERR.
*/
int Graph_Dump   (List* list);

/*!
Function that return physical number by logical number.
param [in,out] list - pointer on struct, that must be ctored before
\return - physical number.
*/
long long Logic_To_Phys (List* list, long long lognum);

/*!
Function that return logical number by physical number.
param [in,out] list - pointer on struct, that must be ctored before
\return - logical number.
*/
long long Phys_To_Logic (List* list, long long physnum);

#endif //LISTH