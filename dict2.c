#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

#include "array.h"

int main(int argc, char* argv[]){
	// stage 2
	if(strcmp(argv[1], "2") == 0){
		FILE *inFile = fopen(argv[2], "r");
		assert(inFile);
		FILE *outFile = fopen(argv[3], "w");
		assert(outFile); 
		//list_t* dataset_list = builddatalist(inFile);
		//listsearch(dataset_list, outFile);
		//listFree(dataset_list);
		array_t* dataset_array = buildarray(inFile);
		arrayShrink(dataset_array);
		//arrayPrint(dataset_array);
		arraySearch(dataset_array, outFile);
		arrayFree(dataset_array);
        
	}
}
