#define MIN_PRIORITY_QUEUE_C

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "minPriorityQueue.h"
#include "heapsort.h"

void minHeapify( HEAPTREENODE *heap, int heapSize, int ind ){
	int l,r,smallest;
	
	l = heapLeftChild(ind);
	r = heapRightChild(ind);
	if( l <= heapSize && heap[l].key < heap[ind].key )
		smallest = l;
	else
		smallest = ind;
	if( r <= heapSize && heap[r].key < heap[smallest].key )
		smallest = r;
	if( smallest!=ind ){
		heapNodeExchange( heap,ind,smallest );
		minHeapify( heap,heapSize,smallest );
	}
}

void heapNodeExchange( HEAPTREENODE* heap, int ind1, int ind2 ){
	TREENODE *n;	// temporary variables
	double k;
	
	n = heap[ind1].node;
	k = heap[ind1].key;
	heap[ind1].node = heap[ind2].node;
	heap[ind1].key = heap[ind2].key;
	heap[ind2].node = n;
	heap[ind2].key = k;
}

TREENODE *heapExtractMin( HEAPTREENODE* heap, int *heapSize){
	TREENODE *min;	// changed pointer
	
	if(*heapSize<1)
		printf("error: heap underflow!\n");
	min = heap[0].node;
	
	(*heapSize)--;
	heap[0].node = heap[*heapSize].node;
	heap[0].key = heap[*heapSize].key;
	minHeapify( heap,*heapSize,0 );
	return min;
}

TREENODE *heapExtractLastElement( HEAPTREENODE* heap, int *heapSize){
	TREENODE *last;
	
	if(*heapSize<1)
		printf("error: heap underflow!\n");
	(*heapSize)--;
	last = heap[*heapSize-1].node;
	
	return last;
}

void heapDecreaseKey( HEAPTREENODE* heap, int ind, double key ){
	int parent;
	if( key > heap[ind].key )
		printf("error: new key is larger than the actual key\n");
	heap[ind].key = key;
	parent = heapParent(ind);
	while( ind > 0 && heap[parent].key > heap[ind].key ){
		heapNodeExchange( heap,ind,parent );
		ind = parent;
		parent = heapParent(ind);
	}
}

void minHeapInsert( HEAPTREENODE* heap, int *heapSize, TREENODE *node, double key ){
	heap[*heapSize].node = node;
	heap[*heapSize].key = HUGE_VAL;
	(*heapSize)++;
	heapDecreaseKey( heap,(*heapSize)-1,key );
}

