#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
#include "list.h"

// Constant definitions
#define MAXCHAR 128
#define NUMDATA 14

// Data definitions
typedef struct dataset dataset_t;

/*------------ Function definitions -----------------------------*/

// Skip the header line of .csv file "f"
void datasetSkipHeaderLine(FILE *f);

// Free the memory of the dataset
void datasetFree(dataset_t *s);

// Loade data to the structure of dataset_t
dataset_t* readdataset_(char* dataset);
int datacmp(char* key, char* target, int* stringcmp, int* charcmp);

void printdata_array(dataset_t* dataset, FILE* f);

#endif


