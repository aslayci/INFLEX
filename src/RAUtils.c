#define _RA_UTILS_C

#include "RAUtils.h"

void createWeightedMajorityMatrix(double **majorityMatrix, int **lists, int *nodeList, double *weights, int nrRNNs, int unionSize, int nRows){
	
	int i, j, z, index1, index2;
	double comp12, comp21;

	/* initialize its entries to zero */
	for(i = 0; i < unionSize; i++) {
		for(j = i; j < unionSize; j++) {
			comp12 = 0.0;
			comp21 = 0.0;
			if(i == j) {
				majorityMatrix[i][j] = 0.0;
				continue;
			}
			for(z = 0; z < nrRNNs; z++) {				
				index1 = indexOfElement(lists[z], nodeList[i], nRows);
				index2 = indexOfElement(lists[z], nodeList[j], nRows);
				if(index1 == -1 && index2 == -1) //when none of the items exist in the list
					continue;				
				else if(index1 == -1) //only second item exists in the list so default ranked ahead
					comp21 += weights[z];
				else if(index2 == -1)
					comp12 += weights[z];
				else if(index1 < index2) //both items exist in the list and first item is ranked ahead
					comp12 += weights[z];
				else if(index2 < index1) //both items exist in the list and second item is ranked ahead
					comp21 += weights[z];				
			}
			majorityMatrix[i][j] = comp12;
			majorityMatrix[j][i] = comp21;
		}
	}
		
}


void createMajorityMatrix(double **majorityMatrix, int **lists, int *nodeList, int nrRNNs, int unionSize, int nRows){
	
	int i, j, z, index1, index2;
	double comp12, comp21;

	/* initialize its entries to zero */
	for(i = 0; i < unionSize; i++) {
		for(j = i; j < unionSize; j++) {
			comp12 = 0.0;
			comp21 = 0.0;
			if(i == j) {
				majorityMatrix[i][j] = 0.0;
				continue;
			}
			for(z = 0; z < nrRNNs; z++) {				
				index1 = indexOfElement(lists[z], nodeList[i], nRows);
				index2 = indexOfElement(lists[z], nodeList[j], nRows);
				if(index1 == -1 && index2 == -1) //when none of the items exist in the list
					continue;				
				else if(index1 == -1) //only second item exists in the list so default ranked ahead
					comp21 += 1.0;
				else if(index2 == -1)
					comp12 += 1.0;
				else if(index1 < index2) //both items exist in the list and first item is ranked ahead
					comp12 += 1.0;
				else if(index2 < index1) //both items exist in the list and second item is ranked ahead
					comp21 += 1.0;				
			}
			majorityMatrix[i][j] = comp12;
			majorityMatrix[j][i] = comp21;
		}
	}	
}

/* weighted majority matrix -- weighted local kemenization */
/* unweighted majority matrix -- unweighted local kemenization */
void localKemenization(int *localKemenizedScores, List *sortedFinalScores, double **majorityMatrix, int *nodeList, int unionSize) {
	
	int i, pos, pos2;
	int cnter;
	List *conductor;
	int *toBeKemenized;
	
	toBeKemenized = calloc(unionSize, sizeof(int));	
			
	conductor = sortedFinalScores;	
	cnter = 0;
	while(conductor->pNext != 0) {
		toBeKemenized[cnter++] = conductor->index;		
			conductor = conductor->pNext;		
		}		
	toBeKemenized[cnter] = conductor->index;	
		
	localKemenizedScores[0] = toBeKemenized[0];	
	
	for(i = 1; i < unionSize; i++) {
		pos = i;		
		if(majorityMatrix[toBeKemenized[pos]][localKemenizedScores[pos - 1]] <= majorityMatrix[localKemenizedScores[pos - 1]][toBeKemenized[pos]])
			localKemenizedScores[pos] = toBeKemenized[pos];
		else{			
			while(pos > 0 && majorityMatrix[localKemenizedScores[pos - 1]][toBeKemenized[i]] < majorityMatrix[toBeKemenized[i]][localKemenizedScores[pos - 1]]){
				localKemenizedScores[pos] = localKemenizedScores[pos - 1];
				pos--;
			}
			localKemenizedScores[pos] = toBeKemenized[i];
		}		
	}	

}


/* bunda bir sorun vah digerini kullaniyorum o yuzden */
// void localKemenization(int *localKemenizedScores, List *sortedFinalScores, int **majorityMatrix, int *nodeList, int unionSize) {
// 	
// 	int pos, pos2;
// 	List *conductor;
// 	
// 	localKemenizedScores[0] = sortedFinalScores->index;	
// 	pos = 1;
// 	conductor = sortedFinalScores->pNext;	
// 	
// 	while(conductor->pNext != 0) {
// 		//printf("now this %d \n", nodeList[conductor->index]);
// 		localKemenizedScores[++pos] = conductor->pNext->index;				
// 		if(majorityMatrix[localKemenizedScores[pos - 1]][localKemenizedScores[pos]] >= majorityMatrix[localKemenizedScores[pos]][localKemenizedScores[pos - 1]]) {
// 			conductor = conductor->pNext;
// 			//continue;
// 		}
// 		else {
// 			pos2 = pos;
// 			while(pos2 > 0 && majorityMatrix[localKemenizedScores[pos2 - 1]][localKemenizedScores[pos2]] < majorityMatrix[localKemenizedScores[pos2]][localKemenizedScores[pos2 - 1]]) {
// 				swapi((int*) localKemenizedScores + pos2, (int*)localKemenizedScores + pos2 - 1);
// 				pos2--;
// 			}
// 			conductor = conductor->pNext;
// 		}
// 	
// 	}
// 
// }