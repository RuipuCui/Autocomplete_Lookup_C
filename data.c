#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "list.h"

// The details of "struct dataset" 
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

//store a single set of data 
dataset_t* readdataset_(char* dataset) {	
	char* c = dataset;
	char** data = malloc(sizeof(char*) * NUMDATA);
	assert(data);
	char* onedata = malloc(sizeof(char) * (MAXCHAR+1));
	assert(onedata);

	int num = 0;
	int index = 0;
	int quote = 0;

	//do not move to the next when read a comma inside of ""
	while(*c != EOF && *c != '\0' && *c != '\n'){
		if(*c == ',' && quote == 0){
			onedata[index] = '\0';
			data[num] = malloc(sizeof(char) * (MAXCHAR+1));
			assert(data[num]);
			strcpy(data[num], onedata);			
			num ++;
			index = 0;
		}else if(*c == '"' && quote == 0){
			quote = 1;
		}else if(*c == '"' && quote == 1){
			quote = 0;
		}else{
			onedata[index] = *c;
			index++;
		}
		c++;
	}

	onedata[index] = '\0';
	data[num] = malloc(sizeof(char) * (MAXCHAR+1));
	assert(data[num]);
	strcpy(data[num], onedata);
	num++;

	dataset_t* s = malloc(sizeof(*s));
	assert(s);
	s->census_year = atoi(data[0]);
	s->block_id = atoi(data[1]);     
	s->property_id = atoi(data[2]);
	s->base_property_id = atoi(data[3]);
	s->building_address = strdup(data[4]);
	s->clue_small_area = strdup(data[5]);
	s->business_address = strdup(data[6]);
	s->trading_name = strdup(data[7]);
	s->industry_code = atoi(data[8]);
	s->industry_description = strdup(data[9]);
	s->seating_type = strdup(data[10]);
	s->number_of_seats = atoi(data[11]);
	char* double_1;
	s->longitude = strtod(data[12], &double_1);
	char* double_2;
	s->latitude = strtod(data[13], &double_2);

	int i;
	for(i = 0; i < num; i++){
		free(data[i]);
	}
	free(data);
	free(onedata);
	//free(dataset);
	return s; 
}

//free a struct
void datasetFree(dataset_t *s) {
	free(s->building_address);
	free(s->clue_small_area);
	free(s->business_address);
	free(s->trading_name);
	free(s->industry_description);
	free(s->seating_type);
	free(s);
}


//The following function was adapted from W3.8 of Workshop Week 3
//skip the first line
void datasetSkipHeaderLine(FILE *f) {
	while (fgetc(f) != '\n');
}
int datacmp(char* key, char* target, int* stringcmp, int* charcmp){
	//int i;
	*stringcmp += 1;
	while(*key){
		if(*target != *key){
			*charcmp += 1;
			if(*target > *key){
				return -1;
			}else if(*target < *key){
				return 1;
			}
		}
		*charcmp += 1;
		target++;
		key++;
	}
	*charcmp += 1;
	return 0;
}

void printdata_array(dataset_t* dataset, FILE* f){
	fprintf(f, "--> census_year: %d || block_id: %d || property_id: %d || base_property_id: %d || building_address: %s || clue_small_area: %s || business_address: %s || trading_name: %s || industry_code: %d || industry_description: %s || seating_type: %s || number_of_seats: %d || longitude: %.5lf || latitude: %.5lf || \n", 
	dataset->census_year, dataset->block_id, dataset->property_id, 
	dataset->base_property_id, dataset->building_address, dataset->clue_small_area, dataset->business_address, 
	dataset->trading_name, dataset->industry_code, dataset->industry_description, dataset->seating_type, 
	dataset->number_of_seats, dataset->longitude, dataset->latitude);
}























