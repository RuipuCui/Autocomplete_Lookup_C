#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "data.h"

// A linked list is defined as a couple of pointers
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

// The following function was adapted from W3.8 of Workshop Week 3
// create a empty list
list_t *listCreate() {
	list_t *list = malloc(sizeof(*list));
	assert(list);
	list->head = list->tail = NULL;
	return list;
}

// The following function was adapted from W3.8(writen by myself) of Workshop Week 3
// append a new node at the tail of the list
void listappend(list_t *list, dataset_t *data) {
	assert(list);

	// Creates a new node and set data to provided value
	node_t *new_ = malloc(sizeof(*new_));
	assert(new_);
	new_->data = data;
	
	//if the list is empty
	if(list->head == NULL && list->tail == NULL){
		list->head = new_;
		new_->next = NULL;
		list->tail = new_;
	}else{
		list->tail->next = new_;
		new_->next = NULL;
		list->tail = new_;
	}
}


//build linked list
list_t* builddatalist(FILE *f){
	list_t* dataset_list = listCreate();
	dataset_t* s;

	char* dataset = NULL;
	size_t lineBufferLength = MAXCHAR+1;

	datasetSkipHeaderLine(f);
	//read a line and store into a char*
	while(getline(&dataset, &lineBufferLength, f) != EOF){
		assert(dataset);
		s = readdataset_(dataset);
		listappend(dataset_list, s);
		//free(dataset);
	}
	free(dataset);
	fclose(f);
	return dataset_list;
}


//search in the linked list
void listsearch(list_t* list, FILE* outf){
	char* query = NULL;
	size_t lineBufferLength = 0;
	//use getline to read a line and store into a char*
	while (getline(&query, &lineBufferLength, stdin) != EOF){
		assert(query);
		//remove the new line character at the end
		query[strlen(query)-1] = '\0';
		node_t* curr = list->head;
		int numfound = 0;
		fprintf(outf, "%s\n", query);
		while(curr != NULL){
			if(strcmp(curr->data->trading_name, query) == 0){
				numfound++;
				printdata(curr, outf);
			}
			curr = curr->next;
		}
		if(numfound == 0){
			printf("%s --> NOTFOUND\n", query);
		}else{
			printf("%s --> %d\n", query, numfound);
		}
	}
	free(query);
	fclose(outf);
}


// The following function was adapted from W3.8 of Workshop Week 3
void listFree(list_t *list){
	assert(list != NULL);
	node_t *curr = list->head; // curr points to the first node
	while (curr) {             // While curr not reaching the end of list
		node_t *tmp = curr;
		curr = curr->next;     // Advance curr to the next next node 
		datasetFree(tmp->data);       // Frees the previous node including
		free(tmp);             //    Freeing the space used by data
	}
	free(list);                // Free the list itself
} 

//print the result 
void printdata(node_t* dataset, FILE* f){
	fprintf(f, "--> census_year: %d || block_id: %d || property_id: %d || base_property_id: %d || building_address: %s || clue_small_area: %s || business_address: %s || trading_name: %s || industry_code: %d || industry_description: %s || seating_type: %s || number_of_seats: %d || longitude: %lf || latitude: %lf || \n", 
	dataset->data->census_year, dataset->data->block_id, dataset->data->property_id, 
	dataset->data->base_property_id, dataset->data->building_address, dataset->data->clue_small_area, dataset->data->business_address, 
	dataset->data->trading_name, dataset->data->industry_code, dataset->data->industry_description, dataset->data->seating_type, 
	dataset->data->number_of_seats, dataset->data->longitude, dataset->data->latitude);
}



