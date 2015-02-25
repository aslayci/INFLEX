#ifndef HEAPSORT_H
#define HEAPSORT_H

int heapParent( int );
int heapLeftChild( int );
int heapRightChild( int );
void heapArrayDecreaseKey( double*,int*,int,double );
void heapArrayIncreaseKey( double*,int*,int,double );
void minArrayHeapify( double*,int*,int,int );
void maxArrayHeapify( double*,int*,int,int );
void minArrayHeapSort( double *,int*,int );
void maxArrayHeapSort( double*,int*,int );
void buildMinArrayHeap( double*,int*,int );
void buildMaxArrayHeap( double*,int*,int );

#endif