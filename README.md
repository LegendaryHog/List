# List on massives
    It is project for Ilab (Intel course for DREC MIPT). Doubly linked list realized on massives. Instead of Nodes with data, pointer on next and previous Nodes we have three massives: massive of data, massive of next Nodes, massive of previous Nodes. Node is index in massive, on this index elment of data equal data of this Node, element of next equal index of next Node, element of previous equal index of previous Node.

## Index
1. [Doxygen documentation.](#1-doxygen-documentation)

2. [Features of work with list on massives.](#2-features-of-work-with-list-on-massives)
3. [Graphviz dump.](#3-graphviz-dump)
4. [Example of main for debugging.](#4-example-of-main-for-debugging)

## 1. Doxygen documentation.
    Documentation is there - not made yet.

## 2. Features of work with list on massives.
The biggest trouble in work with this realization of list is massives. I show you examples.
Next:

In ordinary list on pointer:

```c    
node = node->next; //go forward on next link

int num = node->next->data; //num is equal to data of next Node of node 
```

In list on massives:
```c
index = list->next[index]; //go forward on next link

int num = list->data[list->next[index]]; // num is equal to data of next Node of index
```