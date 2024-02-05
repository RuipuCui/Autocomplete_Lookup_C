#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"

int main(int argc, char* argv[]){
	// stage 1
	if(strcmp(argv[1], "1") == 0){
		FILE *inFile = fopen(argv[2], "r");
		assert(inFile);
		FILE *outFile = fopen(argv[3], "w");
		assert(outFile); 
		list_t* dataset_list = builddatalist(inFile);
		listsearch(dataset_list, outFile);
		listFree(dataset_list);
	}
}

