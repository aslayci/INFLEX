#ifndef MIN_PRIORITY_QUEUE_H
#define MIN_PRIORITY_QUEUE_H

#include "tree.h"

struct heapTreeNode{
	TREENODE *node;
	double key;	
};

typedef struct heapTreeNode HEAPTREENODE;

//int heapParent( int );
//int heapLeftChild( int );
//int heapRightChild( int );

void minHeapify( HEAPTREENODE*,int,int );
void heapNodeExchange( HEAPTREENODE*,int,int );
TREENODE *heapExtractMin( HEAPTREENODE*,int* );
void heapDecreaseKey( HEAPTREENODE*,int,double );
void minHeapInsert( HEAPTREENODE*,int*,TREENODE*,double );


#endif