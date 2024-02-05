#ifndef _ARRAY_H_
#define _ARRAY_H_
#include "data.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define CHAR_BIT_NUM 8

typedef struct dataset dataset_t;

typedef struct tree tree_t;

int getBit(char key, int n);

int data_bit_cmp(char* trading_name, char* prefix, int* common_bits, int max_cmp, int prefix_passed);

int data_bit_cmp2(char* trading_name, char* prefix, int* common_bits, int max_cmp, int prefix_passed, int* charcmp, int* bitcmp);

void create_node(tree_t** tree, int prefixBit, char* prefix, tree_t* branchA, tree_t* branchB);

void update_node(tree_t** tree, int prefixBit, char* prefix, tree_t* branchA, tree_t* branchB);

tree_t* radixTreeInsert(tree_t* tree, dataset_t* dataset, int prefix_passed);

void Search(tree_t* tree, FILE* outf);

void treeSearch(tree_t* tree, char* query, int prefix_passed, int* stringcmp, int* charcmp, int* bitcmp, FILE* outf);

tree_t* builddataTree(FILE *f);

void printTree(tree_t* tree, FILE* outf);

void freeTree(tree_t* tree);

void printdataset(dataset_t* dataset, FILE* f);


#endif
