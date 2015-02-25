#ifndef _INSERTIONSORT_C
#define _INSERTIONSORT_C

#include <stdlib.h>
#include "InsertionSort.h"

/* Regular Insertion Sort */ 

List *SortList(List *pList) {	
	
	List *pSorted = NULL;	

	while(pList != NULL) {
		
		/* head of the list */
		List *pHead = pList; 		
		/* trailing pointer for efficient splice */
		List **ppTrail = &pSorted; 
		
		/* pop head of the list */
		pList = pList->pNext;
		
		/* splice head into sorted list at a proper place */
		while(1) {
			if(*ppTrail == NULL || pHead->iValue > (*ppTrail)->iValue) {
				pHead->pNext = *ppTrail; 
				*ppTrail = pHead;
				break;
			}
			else
				ppTrail = & (*ppTrail)->pNext;
		}
	}
		
	return pSorted;	
} 

#endif

