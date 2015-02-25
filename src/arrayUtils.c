#define _ARRAY_UTILS_C

#include <stdlib.h>
#include <stdio.h>
#include "arrayUtils.h"

void insertionSort(int *array, int n) {
	int c,d,t;
	for (c = 1 ; c <= n - 1; c++) {
		d = c;
 
		while ( d > 0 && array[d] < array[d-1]) {
			t = array[d];
			array[d]   = array[d-1];
			array[d-1] = t; 
			d--;
		}
	}
}

/* checks if array contains the element */
int containsElement(int *list, int elem, int size) {	
	int i;
	for(i = 0; i < size; i++)
		if(list[i] == elem)
			return 1;

	return 0;
}

/* returns the index of the element in the specified array*/
/* if element is not in the array, returns -1*/ 
/* if there are multiple occurance of the element, returns first seen index  */
int indexOfElement(int *list, int elem, int size) {
	int i;
	
	for(i = 0; i < size; i++)
		if(list[i] == elem)
			return i;			
	
	return -1;
	
}

/* read item data matrix */
void readItemMatrix(double **x, int n, int d, char* file){
	int i,j;
	float t;
	FILE *fp = fopen(file,"r");
  
	if(fp==NULL){
		fprintf(stderr,"error opening file.. exiting\n");
		exit(1);
	}

	for(i=0;i<n;i++){
		for(j=0;j<d;j++){
			if(fscanf(fp,"%f ", &t)==EOF){
				fprintf(stderr,"error reading file.. exiting \n");
				exit(1);
			}
			x[i][j]=(double)t;
		}
	}
	fclose(fp);
}


//read list data matrix
void readListMatrix(int **x, int n, int d, char* file){
	int i,j;
	int t;
	FILE *fp = fopen(file,"r");
  
	if(fp==NULL){
		fprintf(stderr,"error opening file.. exiting\n");
		exit(1);
	}

	for(i=0;i<n;i++){
		for(j=0;j<d;j++){
			if(fscanf(fp,"%d ", &t)==EOF){
				fprintf(stderr,"error reading file.. exiting \n");
				exit(1);
			}
			x[i][j]=(int)t;
		}
	}
	fclose(fp);
}


void readIndexList(char *file, int *list, int nRows, int nCols) {
	int i, j;	
	FILE *fp = fopen(file, "r");	
	float val;	
	if(fp == NULL) {
		fprintf(stderr, "error opening file.. exiting \n");
		exit(1);
	}		
	for(i = 0; i < nRows; i++) {		
		for(j = 0; j < nCols; j++) {
			if(fscanf(fp, "%f ", &val) == EOF) {
				fprintf(stderr, "file %s could not be read mf.. exiting \n", file);
				exit(1);
			}
			if(j == 0)
				list[i] = (int) val;				
		}		
	}
	fclose(fp);	
}

void writeList(char *file, int *list, int listSize) {
	int i, j;
	FILE *fp = fopen(file, "w");
	
	if(fp == NULL) {
		fprintf(stderr, "error opening file to write mf... exiting \n");
		exit(1);
	}
	
	for(i = 0; i < listSize; i++) 
		fprintf(fp, "%d \n", list[i]);
	
	fclose(fp);
	
}

void swapi(int *p1, int *p2) {
	int temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void swapd(double *p1, double *p2) {
	double temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

