#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "array.h"
#include "data.h"

struct array {
	dataset_t **A;      // Use as array of dataset_t *, ie. dataset_t *A[]
	int size;           // Current array size
	int n;              // Current number of elements used
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

// The following function was adapted from W3.5 of Workshop Week 3
// Creates & returns an empty array.
array_t *arrayCreate() {
	array_t *arr = malloc(sizeof(*arr));
	assert(arr);
	int size = INIT_SIZE;      // "size" used for convenience
	arr->size = size;
	arr->A = malloc(size * sizeof(*(arr->A)));
	assert(arr->A);
	arr->n = 0;
	return arr;
}

// The following function was adapted from W3.8 of Workshop Week 3
// Free memory used by array "arr"
void arrayFree(array_t *arr) {
	for (int i = 0; i < arr->n; i++) {
		datasetFree(arr->A[i]);
	}
	free(arr->A);
	free(arr);
}

// The following function was adapted from W3.8 of Workshop Week 3
// Shrinks the array, to reduce array size to the same 
//    as the number of elements used
void arrayShrink(array_t *arr) {
	if (arr->size != arr->n) {
		// The new size should be at least INIT_SIZE
		arr->size = (arr->n > INIT_SIZE) ? arr->n : INIT_SIZE;
		arr->A = realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

// The following function was adapted from W3.8 of Workshop Week 3
void arrayEnableInsert(array_t *arr) {
	if (arr->n == arr->size) {
		arr->size <<= 1;       // same as arr->size *= 2;
		arr->A= realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

// The following function was adapted from W3.8 of Workshop Week 3
// Inserts data "s" into sorted array "arr", ensuring "arr" remains sorted 
void sortedArrayInsert(array_t *arr, dataset_t *s) {
	// FILL IN CODE HERE 
	int i;
	arrayEnableInsert(arr);
	// starting from the end, shift all elements > s one position to the right
	for (i = arr->n - 1; i >= 0 && strcmp(s->trading_name, arr->A[i]->trading_name) < 0; i-- ) {
		arr->A[i + 1] = arr->A[i];
	}
	// now "s" should be in A[i+1]
	arr->A[i + 1] = s;
	arr->n++;
}

void arrayPrint(array_t* arr){
	int i;
	for(i=0; i<(arr->n); i++){
		printf("%s\n", arr->A[i]->trading_name);
	}
}

array_t* buildarray(FILE *f){
	array_t* dataset_array = arrayCreate();
	dataset_t* s;

	char* dataset = NULL;
	size_t lineBufferLength = MAXCHAR+1;

	datasetSkipHeaderLine(f);
	//read a line and store into a char*
	while(getline(&dataset, &lineBufferLength, f) != EOF){
		assert(dataset);
		s = readdataset_(dataset);
		sortedArrayInsert(dataset_array, s);
	}
	free(dataset);
	fclose(f);
	return dataset_array;
}

void arraySearch(array_t* dataset_array, FILE* outf){
	char* query = NULL;
	size_t lineBufferLength = 0;
	//int totalcmp = 0;
	//use getline to read a line and store into a char*
	while (getline(&query, &lineBufferLength, stdin) != EOF){
		assert(query);
		//remove the new line character at the end
		query[strlen(query)-1] = '\0';

		//int location;
		int stringcmp = 0;
		int charcmp = 0;
		fprintf(outf, "%s\n", query);
		//int totalcmp = 0;
		int result = binary_search(dataset_array, query, &stringcmp, &charcmp, &dataset_array->n);
		if(result != -1){
			//printdata_array(dataset_array->A[result], outf);
			linear_search(dataset_array, query, result, &stringcmp, &charcmp, outf);
		}
		//totalcmp += charcmp;
		printf("%s --> b%d c%d s%d\n", query, 8 * charcmp, charcmp, stringcmp);
	}
	//printf("%d", totalcmp);
	free(query);
	fclose(outf);
}

/*
int binary_search(dataset_t** dataset, int lo, int hi, 
    	char* key, int* location, int* stringcmp, int* charcmp) {
    int mid, outcome;
    
    if (lo>=hi) {
        return 0;
    }

    mid = (lo+hi)/2;

    *location = mid;
	outcome = datacmp(key, dataset[mid]->trading_name, stringcmp, charcmp);
    if ((outcome < 0)) {
        return binary_search(dataset, lo, mid-1, key, location, stringcmp, charcmp);
    } else if (outcome > 0) {
        return binary_search(dataset, mid+1, hi, key, location, stringcmp, charcmp);
    } else {
        return 1;
    }
} */

// The following function was adapted from W3.8 of Workshop Week 3
int binary_search(array_t *arr, char* key, int* stringcmp, int* charcmp, int* arraySize) {
	
	int mid, lo = 0, hi = arr->n - 1;
	while (lo <= hi) {
		mid = (lo + hi) / 2;
		
		int cmp = datacmp(key, arr->A[mid]->trading_name, stringcmp, charcmp);
		if (cmp == 0) return mid;
		if (cmp < 0) { 
			hi = mid - 1;
		} else {
			lo = mid + 1;
		}
	}
	return -1;
}


void linear_search(array_t* dataset_array, char* key, int location, int* stringcmp, int* charcmp, FILE* f){
	int i = location;
	int j = location;
	while(j-1 >= 0 && datacmp(key, dataset_array->A[j-1]->trading_name, stringcmp, charcmp) == 0){
		//printdata_array(dataset_array->A[j-1], f);
		j--;
	}
	for(; j<location; j++){
		printdata_array(dataset_array->A[j], f);
	}
	printdata_array(dataset_array->A[location], f);
	while(i+1 < dataset_array->n && datacmp(key, dataset_array->A[i+1]->trading_name, stringcmp, charcmp) == 0){
		printdata_array(dataset_array->A[i+1], f);
		i++;
	}

} 














