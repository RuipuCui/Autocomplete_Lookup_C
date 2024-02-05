#ifndef _LIST_H_
#define _LIST_H_

#include "data.h"

// Type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct list list_t;
typedef struct dataset dataset_t;

// Node definitions
typedef struct node node_t;

// Generous functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Creates & returns an empty linked list
list_t *listCreate();

// Free the list, with the help of function "freeData" for freeing the data component
void listFree(list_t *list);

// load data to node and connect all data nodes 
list_t* builddatalist(FILE* f);

// Inserts a new node with value "data" to the end of "list" 
void listappend(list_t *list, dataset_t *data);

// Performs linear search in "list", returns found data or NULL 
// Here we assume that key is an int (a limited assumption for sake of simplicity!)
void listsearch(list_t* list, FILE* outf);

// print single node's dataset to the outFile
void printdata(node_t* dataset, FILE* f);

#endif
