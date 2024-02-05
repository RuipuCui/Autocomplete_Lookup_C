#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"
#include "tree.h"

int main(int argc, char* argv[]){
	// stage 3
	if(strcmp(argv[1], "3") == 0){
		FILE *inFile = fopen(argv[2], "r");
		assert(inFile);
		FILE *outFile = fopen(argv[3], "w");
		assert(outFile); 
		tree_t* dataset_tree = builddataTree(inFile);
		Search(dataset_tree, outFile);
		freeTree(dataset_tree);       
	}
}
