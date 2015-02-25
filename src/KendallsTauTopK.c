#define KENDALLS_TAU_TOPK_C

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "arrayUtils.h"
#include "KendallsTauTopK.h"

/* Normalized Kendalls Tau distance calculation for comparing two top-k lists
	Gives distance between 0 and 1: smaller the score, closer matches between lists
	Normalization is done with the maximum number of disagreements
	Fagin defined the maximum only for Kendall_min case as k * k where case 4's penalty is equal to 0
	I define it by including case 4 worst case scenario 
*/

double KendallsTauTopK(int *list1, int *list2, int k, double penalty) {
	//printf("cagrildi! \n");
	int i, j, unionSize, index; 
	int *unionNodes; //union of all nodes of the input lists
	int **position; // the position/rank of the union nodes in list 1 and list2 [rank(list1), rank(list2)]
	double maxPossibleInvCnt = 0.0;
	double inversionCnt = 0.0;	
	
	/* will keep positions in separate arrays*/
	int *pos1, *pos2;
	/* allocate memory for the worst case (no intersection) */
	pos1 = calloc(2 * k, sizeof(int));
	pos2 = calloc(2 * k, sizeof(int));
	
	/*initialize positions */
	for(i = 0; i < 2 * k; i++){
		pos1[i] = k + 1; 
		pos2[i] = k + 1;
	}
	
	unionNodes = calloc(2 * k, sizeof(int)); //allocate memory for worst case (no intersection)
	
	for(i = 0; i < k; i++) { //get the elements from list 1 and their positions in pos1
		unionNodes[i] = list1[i];
		pos1[i] = i;
	}
		
	for(j = 0; j < k; j++) { //get the elements from list 2 and update their positions in pos2
		if(!containsElement(list1, list2[j], k)) { //element is only in list2
			//no need to update in pos1 as it was already initialized to k + 1
			unionNodes[i] = list2[j]; //insert it into unionNodes with index i
			pos2[i++]	= j; //insert its rank in the corresponding pos2 for index i			
		}
		else { //elemens is in both lists 
			//no need to update unionNodes as it already contains the element
			index = indexOfElement(list1, list2[j], k);		
			pos2[index] = j;
		}			
	}
		
	unionSize = i;
	
// 	printf("***kontrol***\n");
// 	printf("union size: %d\n", i);
// 	for(i = 0; i < unionSize; i++) 
// 		printf("eleman %d: pos1: %d pos2: %d \n", unionNodes[i], pos1[i], pos2[i]);
	
	for(i = 0; i < unionSize - 1; i++) { //bubble sort to compare ranks
		for(j = i + 1; j < unionSize; j++) {			
			// Fagin Case 4 -- i and j both appear in list 1 but none appear in list 2 or vice versa
			if(pos1[i] + pos1[j] > (2 * k) || pos2[i] + pos2[j] > (2 * k))
				inversionCnt += penalty;
			//Fagin Case 1-2-3
			else if((pos1[i] > pos1[j] && pos2[i] < pos2[j]) || (pos1[i] < pos1[j] && pos2[i] > pos2[j]))
				inversionCnt += 1.0;
		}		
	}		
	
	maxPossibleInvCnt = k * k + k * (k - 1) * penalty;
	return inversionCnt / maxPossibleInvCnt;
	
}