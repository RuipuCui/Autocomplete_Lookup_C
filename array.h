#ifndef _ARRAY_H_
#define _ARRAY_H_
#include "data.h"

#define INIT_SIZE 2     // Initial size of arrays 

typedef struct array array_t;
typedef struct dataset dataset_t;

// Creates & returns an empty array
array_t *arrayCreate();

// Free memory used by array "arr"
void arrayFree(array_t *arr);

// Inserts data "s" into array "arr", ensuring "arr" is sorted 
void sortedArrayInsert(array_t *arr, dataset_t *s);


// Shrinks the array, to reduce array size to the same 
//    as the number of elements used
void arrayShrink(array_t *arr);

void arrayEnableInsert(array_t *arr);

void arrayPrint(array_t* arr);

array_t* buildarray(FILE *f);

void arraySearch(array_t* dataset_array, FILE* outf);

//int binary_search(dataset_t** dataset, int lo, int hi, char* key, int* location, int* stringcmp, int* charcmp);
int binary_search(array_t *arr, char* key, int* stringcmp, int* charcmp, int* arraySize);

void linear_search(array_t* dataset_array, char* key, int location, int* stringcmp, int* charcmp, FILE* f);

#endif
