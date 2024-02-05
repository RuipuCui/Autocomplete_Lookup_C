#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"
#include "data.h"
#include "list.h"

struct list {
	node_t *head;  // Points to the first node of the list
	node_t *tail;  // Points to the last node of the list               // Number of elements in the list
};

// Create a new node for the data and link
struct node {
	dataset_t *data;              // Points to the data element of the node
	node_t *next;            // Points to the next node in the list
};

struct dataset{
	int census_year;         // - The year the information was recorded for (integer)
	int block_id;             //- The city block ID. (integer)
	int property_id;          //- The ID of the property. (integer)
	int base_property_id;     //- The ID of the building the business is in. (integer)
	char* building_address;     //- The address of the building. (string)
	char* clue_small_area;     // - The CLUE area of Melbourne that the building is in. (string)
	char* business_address;     //- The address of the business itself. (string)
	char* trading_name;         //- The name of the business. (string)
	int industry_code;        //- The ID for the category of the business. (integer)
	char* industry_description; //- The description of the category of the business. (string)
	char* seating_type;         //- The type of seating the record describes. (string)
	int number_of_seats;     // - The number of seats provided of this type. (integer)
	double longitude;           // - The longitude (x) of the seating location. (double)
	double latitude; 
};

struct tree{
	int prefixBit;
	char* prefix;
	tree_t* branchA;
	tree_t* branchB;
	list_t *dataset_list;
};

int getBit(char key, int n){
	return key>>(7-n) & 1;
}

//compare data in bit level 
int data_bit_cmp(char* trading_name, char* prefix, int* common_bits, int max_cmp, int prefix_passed){
	int i;
	for(i=0; i<max_cmp; i++){
		int bit_passed = prefix_passed % CHAR_BIT_NUM;		
		int charbit_passed = i % CHAR_BIT_NUM;				
		int trading_key = trading_name[(i + prefix_passed) / CHAR_BIT_NUM];
		int prefix_key = prefix[(i + bit_passed) / CHAR_BIT_NUM];
		int bit_trading_name = getBit(trading_key, (prefix_passed + charbit_passed) % CHAR_BIT_NUM);
		int bit_prefix = getBit(prefix_key, (bit_passed + charbit_passed) % CHAR_BIT_NUM);

		//bit_trading_name is 1 and bit_prefix is 0
		if(bit_trading_name > bit_prefix){
			*common_bits = i;
			return 1;
		}

		//bit_trading_name is 0 and bit_prefix is 1
		if(bit_trading_name < bit_prefix){
			*common_bits = i;
			return -1;
		}

	}
	
	*common_bits = i;
	return 0;
}

//this compare function used in search function as the number of comparisons needed to be recorded
int data_bit_cmp2(char* trading_name, char* prefix, int* common_bits, int max_cmp, int prefix_passed, int* charcmp, int* bitcmp){
	int i;
	for(i=0; i<max_cmp; i++){
		*bitcmp += 1;
		int bit_passed = prefix_passed % CHAR_BIT_NUM;		
		int charbit_passed = i % CHAR_BIT_NUM;				
		int trading_key = trading_name[(i + prefix_passed) / CHAR_BIT_NUM];
		int prefix_key = prefix[(i + bit_passed) / CHAR_BIT_NUM];
		int bit_trading_name = getBit(trading_key, (prefix_passed + charbit_passed) % CHAR_BIT_NUM);
		int bit_prefix = getBit(prefix_key, (bit_passed + charbit_passed) % CHAR_BIT_NUM);
		if (charbit_passed == 0) {
			*charcmp += 1;
		}

		//bit_trading_name is 1 and bit_prefix is 0
		if(bit_trading_name > bit_prefix){
			*common_bits = i;
			return 1;
		}

		//bit_trading_name is 0 and bit_prefix is 1
		if(bit_trading_name < bit_prefix){
			*common_bits = i;
			return -1;
		}

	}
	*common_bits = i;
	return 0;
}

//create a tree node
void create_node(tree_t** tree, int prefixBit, char* prefix, tree_t* branchA, tree_t* branchB){
	(*tree)->prefixBit = prefixBit;
	(*tree)->prefix = strdup(prefix);
	(*tree)->branchA = branchA;
	(*tree)->branchB = branchB;
}

//store values into the tree node
void update_node(tree_t** tree, int prefixBit, char* prefix, tree_t* branchA, tree_t* branchB){
	(*tree)->prefixBit = prefixBit;
	char* old_prefix = (*tree)->prefix;
	//the maximum num of char of the prefix is (prefixBit + CHAR_BIT_NUM * 2) / CHAR_BIT_NUM)
	(*tree)->prefix = strndup(prefix, (prefixBit + CHAR_BIT_NUM * 2) / CHAR_BIT_NUM);
	free(old_prefix);
	(*tree)->branchA = branchA;
	(*tree)->branchB = branchB;
}

//insert data into the tree
tree_t* radixTreeInsert(tree_t* tree, dataset_t* dataset, int prefix_passed){

	int trading_size = strlen(dataset->trading_name)+1;

	//if the tree is null, create a new tree
	if(!tree){
		tree = malloc(sizeof(tree_t));
		assert(tree);
		list_t* list = listCreate();
		listappend(list, dataset);
		char* prefix_behind = dataset->trading_name + (prefix_passed) / CHAR_BIT_NUM;
		create_node(&tree, trading_size * CHAR_BIT_NUM - prefix_passed, prefix_behind, NULL, NULL);
		tree->dataset_list = list;
		return tree;
	}

	int common_bits = 0;
	int max_cmp = min(trading_size * CHAR_BIT_NUM - prefix_passed, tree->prefixBit);
	int result = data_bit_cmp(dataset->trading_name, tree->prefix, &common_bits, max_cmp, prefix_passed);
	int bit_passed = prefix_passed % CHAR_BIT_NUM;		
	char* prefix_behind = dataset->trading_name + (common_bits + prefix_passed) / CHAR_BIT_NUM;

	if(result == 0){
		if(common_bits == trading_size * CHAR_BIT_NUM - prefix_passed && common_bits == tree->prefixBit){
			//excatly match and no need to visit its banches
			listappend(tree->dataset_list, dataset);
		}else if(common_bits == trading_size * CHAR_BIT_NUM - prefix_passed && common_bits != tree->prefixBit){
			/*create a new tree node that store the common bits of data that need to be inserted
			and the current tree node */
			tree_t* branch = malloc(sizeof(tree_t));
			assert(branch);
			list_t* list = listCreate();
			listappend(list, dataset);
			create_node(&branch, tree->prefixBit - common_bits, tree->prefix + (common_bits + bit_passed) / CHAR_BIT_NUM, 
								 tree->branchA, tree->branchB);
			int next_prefix_bit = getBit(tree->prefix[(common_bits + bit_passed) / CHAR_BIT_NUM], (common_bits + bit_passed) % CHAR_BIT_NUM);
			branch->dataset_list = tree->dataset_list;
			if(next_prefix_bit == 1){
				update_node(&tree, common_bits, tree->prefix,
						    NULL, branch);
			}else if(next_prefix_bit == 0){
				update_node(&tree, common_bits, tree->prefix,
						    branch, NULL);
			}
			tree->dataset_list = list;
		}else if(common_bits != trading_size * CHAR_BIT_NUM - prefix_passed && common_bits == tree->prefixBit){
			//insert to its branch
			int next_data_bit = getBit(dataset->trading_name[(prefix_passed + common_bits) / CHAR_BIT_NUM], (prefix_passed + common_bits) % CHAR_BIT_NUM);
			if(next_data_bit == 1){
				tree->branchB = radixTreeInsert(tree->branchB, dataset, prefix_passed + common_bits);
			}else if(next_data_bit == 0){
				tree->branchA = radixTreeInsert(tree->branchA, dataset, prefix_passed + common_bits);
			}

		}

	} else {
		/*create a new tree node that store the common bits of data that need to be inserted
			and the current tree node */
		tree_t* childA = malloc(sizeof(tree_t));
		assert(childA);
		tree_t* childB = malloc(sizeof(tree_t));
		assert(childB);
		list_t* list = listCreate();
		listappend(list, dataset);
		create_node(&childA, tree->prefixBit - common_bits, tree->prefix + (common_bits + bit_passed) / 8, 
					tree->branchA, tree->branchB);
		create_node(&childB, trading_size * CHAR_BIT_NUM - common_bits - prefix_passed, 
					prefix_behind, NULL, NULL);
		childA->dataset_list = tree->dataset_list;
		childB->dataset_list = list;

		if(result == -1){
			update_node(&tree, common_bits, tree->prefix,
						childB, childA);
		}else if(result == 1){
			update_node(&tree, common_bits, tree->prefix,
						childA, childB);
		}
		tree->dataset_list = NULL;
	}
	return tree;

}

tree_t* builddataTree(FILE *f){
	tree_t* dataset_tree = NULL;
	dataset_t* s;

	char* dataset = NULL;
	size_t lineBufferLength = MAXCHAR+1;

	datasetSkipHeaderLine(f);
	//read a line and store into a char*
	while(getline(&dataset, &lineBufferLength, f) != EOF){
		assert(dataset);
		s = readdataset_(dataset);
		dataset_tree = radixTreeInsert(dataset_tree, s, 0);
	}
	free(dataset); 
	fclose(f);
	return dataset_tree;
}


void Search(tree_t* tree, FILE* outf){
	char* query = NULL;
	size_t lineBufferLength = 0;
	//use getline to read a line and store into a char*
	while (getline(&query, &lineBufferLength, stdin) != EOF){
		assert(query);
		//remove the new line character at the end
		query[strlen(query)-1] = '\0';

		//int location;
		int stringcmp = 0;
		int charcmp = 0;
		int bitcmp = 0;
		fprintf(outf, "%s\n", query);
		//printf(" xxx ");
		treeSearch(tree, query, 0, &stringcmp, &charcmp, &bitcmp, outf);
		printf("%s --> b%d c%d s%d\n", query, bitcmp, charcmp, stringcmp);
	}
	free(query);
	fclose(outf);
}

void treeSearch(tree_t* tree, char* query, int prefix_passed, int* stringcmp, int* charcmp, int* bitcmp, FILE* outf){
	int trading_size = strlen(query);

	if(!tree){
		return;
	}

	int common_bits = 0;
	int max_cmp = min(trading_size * CHAR_BIT_NUM - prefix_passed, tree->prefixBit);
	int result = data_bit_cmp2(query, tree->prefix, &common_bits, max_cmp, prefix_passed, charcmp, bitcmp);

	if(result == 0){
		if(common_bits == trading_size * CHAR_BIT_NUM - prefix_passed && common_bits == tree->prefixBit){
			*stringcmp += 1;
			printTree(tree, outf);
			return;
		}else if(common_bits == trading_size * CHAR_BIT_NUM - prefix_passed && common_bits != tree->prefixBit){
			*stringcmp += 1;
			printTree(tree, outf);
			return;
		}else if(common_bits != trading_size * CHAR_BIT_NUM - prefix_passed && common_bits == tree->prefixBit){
			int next_data_bit = getBit(query[(prefix_passed + common_bits) / CHAR_BIT_NUM], (prefix_passed + common_bits) % CHAR_BIT_NUM);
			//search at branch
			if(next_data_bit == 1){
				treeSearch(tree->branchB, query, prefix_passed + common_bits, stringcmp, charcmp, bitcmp, outf);
			}else if(next_data_bit == 0){
				treeSearch(tree->branchA, query, prefix_passed + common_bits, stringcmp, charcmp, bitcmp, outf);
			}

		}

	}

}

void printTree(tree_t* tree, FILE* outf){
	if(!tree){
		return;
	}

	if (tree->dataset_list) {
		node_t* curr = tree->dataset_list->head;
		while(curr != NULL){
			printdata(curr, outf);
			curr = curr->next;
		}
	}
	printTree(tree->branchA, outf);
	printTree(tree->branchB, outf);
}


void freeTree(tree_t* tree){
	if(!tree){
		return;
	} else {
		if(tree->dataset_list){
			listFree(tree->dataset_list);
		}
		freeTree(tree->branchA);
		freeTree(tree->branchB);
		free(tree->prefix);
		free(tree);
	}
}









