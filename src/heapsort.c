#define HEAPSORT_C

#include <math.h>
#include <stdio.h>
#include "heapsort.h"

// returns the index of the parent of the heap element ind
int heapParent( int ind ){
	return (int)floor( (ind-1)/2 );
}

// returns the index of the left child of the heap element ind
int heapLeftChild( int ind ){
	return 2*ind+1;
}

// returns the index of the right child of the heap element ind
int heapRightChild( int ind ){
	return 2*ind+2;
}

void heapArrayDecreaseKey( double *keys, int *inds, int h, double key ){
	int parent, tempInd;
	double tempKey;
	if( key > keys[h] )
		printf("error: new key is larger than the actual key\n");
	keys[h] = key;
	parent = heapParent(h);
	while( h > 0 && keys[parent] > keys[h] ){
		// exchange the two elements
		tempKey = keys[parent];
		keys[parent] = keys[h];
		keys[h] = tempKey;
		tempInd = inds[parent];
		inds[parent] = inds[h];
		inds[h] = tempInd;
		h = parent;
		parent = heapParent(h);
	}
}

void heapArrayIncreaseKey( double *keys, int *inds, int h, double key ){
	int parent, tempInd;
	double tempKey;
	if( key < keys[h] )
		printf("error: new key is smaller than the current key\n");
	keys[h] = key;
	parent = heapParent(h);
	while( h > 0 && keys[parent] < keys[h] ){
		// exchange the two elements
		tempKey = keys[parent];
		keys[parent] = keys[h];
		keys[h] = tempKey;
		tempInd = inds[parent];
		inds[parent] = inds[h];
		inds[h] = tempInd;
		h = parent;
		parent = heapParent(h);
	}
}

// lets the value at index h float down in the min-heap so that the subtree rooted at index ind becomes a min-heap
// (it assumes that both LEFT(ind) and RIGHT(ind) are min-heaps)
void minArrayHeapify( double *keys, int *inds, int heapSize, int h ){
	int l,r,smallest,tempInd;
	double tempKey;
	
	l = heapLeftChild(h);
	r = heapRightChild(h);
	if( l < heapSize && keys[l] < keys[h] )
		smallest = l;
	else
		smallest = h;
	if( r < heapSize && keys[r] < keys[smallest] )
		smallest = r;
	if( smallest!=h ){
		// exchange the two elements
		tempKey = keys[smallest];
		keys[smallest] = keys[h];
		keys[h] = tempKey;
		tempInd = inds[smallest];
		inds[smallest] = inds[h];
		inds[h] = tempInd;
		minArrayHeapify( keys,inds,heapSize,smallest );
	}
}

// lets the value at index h float down in the max-heap so that the subtree rooted at index ind becomes a max-heap
// (it assumes that both LEFT(ind) and RIGHT(ind) are max-heaps)
void maxArrayHeapify( double *keys, int *inds, int heapSize, int h ){
	int l,r,largest,tempInd;
	double tempKey;
	
	l = heapLeftChild(h);
	r = heapRightChild(h);
	if( l < heapSize && keys[l] > keys[h] )
		largest = l;
	else
		largest = h;
	if( r < heapSize && keys[r] > keys[largest] )
		largest = r;
	if( largest!=h ){
		// exchange the two elements
		tempKey = keys[largest];
		keys[largest] = keys[h];
		keys[h] = tempKey;
		tempInd = inds[largest];
		inds[largest] = inds[h];
		inds[h] = tempInd;
		maxArrayHeapify( keys,inds,heapSize,largest );
	}
}

void minArrayHeapSort( double *keys, int *inds, int heapSize ){
	int i, tmpInd;
	double tmpKey;
	buildMinArrayHeap( keys,inds,heapSize );
	for( i=heapSize-1;i>0;i--){
		// exchange elements 0 and i
		tmpKey = keys[0];
		tmpInd = inds[0];
		keys[0] = keys[i];
		inds[0] = inds[i];
		keys[i] = tmpKey;
		inds[i] = tmpInd;
		heapSize--;
		minArrayHeapify( keys,inds,heapSize,0 );
	}	
}

void maxArrayHeapSort( double *keys, int *inds, int heapSize ){
	int i, tmpInd;
	double tmpKey;
	buildMaxArrayHeap( keys,inds,heapSize );
	for( i=heapSize-1;i>0;i--){
		// exchange elements 0 and i
		tmpKey = keys[0];
		tmpInd = inds[0];
		keys[0] = keys[i];
		inds[0] = inds[i];
		keys[i] = tmpKey;
		inds[i] = tmpInd;
		heapSize--;
		maxArrayHeapify( keys,inds,heapSize,0 );
	}	
}

void buildMinArrayHeap( double *keys, int *inds, int length ){
	int i;
	for( i = (int)(floor((double)(length/2))-1.0); i>=0; i-- )
		minArrayHeapify( keys,inds,length,i );
}

void buildMaxArrayHeap( double *keys, int *inds, int length ){
	int i;
	for( i = (int)(floor((double)(length/2))-1.0); i>=0; i-- )
		maxArrayHeapify( keys,inds,length,i );
}