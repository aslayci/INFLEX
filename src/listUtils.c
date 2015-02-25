#ifndef LIST_UTILS_C
#define LIST_UTILS_C
#include <stdio.h>
#include <stdlib.h>
#include "newTypes.h"
#include "listUtils.h"

/* ****************************************************************************************** */
/************************* LIST OF TREENODE POINTERS ***************************************/

/* copies the first element of a list to a LIST_NODE struct */
void treeNodeListHeadCopy( TREENODE_LIST_PT list, LIST_NODE *node ){
		
	if( list!=NULL ){
		node->treeNodePt = list->treeNodePt;
		node->next = list->next;
		return;
	}
	node->treeNodePt = NULL;		
	//node->next = NULL;
}

/* deletes the first element of a list and assign it to Tnode*/
void treeNodeListPop( TREENODE_LIST_PT list, TREENODE *Tnode ){
	TREENODE_LIST_PT q;	
	
	if( list!=NULL ){
		Tnode = list->treeNodePt;	//copy the element there is at list head
		q = list;	// create an alias to access to the actual head of the list
		list = list->next;	// delete the actual head
		free(q);
		return;
	}
	printf("Empty list");		
}

/* insert the element Tnode at the head of a list*/
void treeNodeListPush( TREENODE_LIST_PT *list, TREENODE *Tnode ){
	TREENODE_LIST_PT q;	//pointer to a new list field
	
	q = (TREENODE_LIST_PT)calloc( 1,sizeof(LIST_NODE) );	//allocate memory for a new list field
	q->treeNodePt = Tnode;	//store the new element
	q->next = *list;	//link to the rest of the list
	*list = q;	
}

/* delete from the list all the elements equal to Tnode*/
void treeNodeListElementDelete( TREENODE_LIST_PT list, TREENODE *Tnode ){
	if( list!=NULL ){
		treeNodeListElementDelete( list->next,Tnode );
		if( Tnode == list->treeNodePt )
			treeNodeListPop( list,Tnode );
	}
}

/* delete from the list the first element*/
void treeNodeListFirstElemDelete( TREENODE_LIST_PT *list ){
	TREENODE_LIST_PT q;
	if( (*list)!=NULL ){
		q = *list;	// create an alias to access to the actual head of the list
		*list = (*list)->next;
		free(q);
	}
}

/* insertion of an element in order -------- not yet implemented, because not useful for a list of pointers to treenodes*/

/* print of a list ----------- not yet implemented because not useful for a list of pointers to treenodes*/

/* inzialize list to an empty list*/
void treeNodeStartList( TREENODE_LIST_PT *list){
	(*list) = NULL;
}

/* returns 1 if the list is empty*/
int isEmptyTreeNodeList( TREENODE_LIST_PT list){
	if( list==NULL )
		return 1;
	return 0;
}

/* returns 1 if the element Tnode is stored in the list*/
int isInTreeNodeList( TREENODE_LIST_PT list, TREENODE *Tnode ){
	if( list==NULL )
		return 0;
	if( Tnode == list->treeNodePt )
		return 1;
	return isInTreeNodeList( list->next,Tnode );
}

/* ****************************************************************************************** */
/************************QUEUE OF TREENODE POINTERS **************************************/

/* inizialize an empty queue*/
void treeNodeStartQueue( TREENODE_QUEUE *q ){
	(*q).first = NULL;
}

/* returns 1 if the queue is empty, 0 otherwise*/
int isEmptyTreeNodeQueue( TREENODE_QUEUE q ){
	if( q.first==NULL )
		return 1;
	return 0;
}

/* adds an element at the end of the queue*/
void treeNodeEnQueue( TREENODE_QUEUE *q,  TREENODE *Tnode ){
	TREENODE_LIST_PT p;	//pointer to a new list field
	
	p = (TREENODE_LIST_PT)calloc( 1,sizeof(LIST_NODE) );	//allocate memory for a new list field
	// copy the new element to another memory location pointed by treeNodePt
	p->treeNodePt = Tnode;
	if( q->first==NULL )	//if the queue is empty
		q->first = p;	// the new node is the first and the last 
	else				// otherwise
		q->last->next = p;	// it must be pointed by the last node
	p->next = NULL;		
	q->last = p;			// the new node is the last in the queue
}

/* extracts the first element from the queue*/
void treeNodeDeQueue( TREENODE_QUEUE *q, TREENODE **Tnode ){
	TREENODE_LIST_PT p;

	/* points to the node to be extracted*/
	p = (*q).first;
	// copy the first element 
	if( p!=NULL ){
		(*Tnode) = p->treeNodePt;
		(*q).first = p->next;	// extracts the node from the queue
		free(p);
		return;
	}
	printf("Empty queue of treeNodes");		
}

/* ****************************************************************************************** */
/*********************************** LIST OF CLUSTERS ***************************************/

/* pops the first element of a cluster list (the function returns a pointer to indices of the retrieved cluster)*/
int *clusterPop( CLUSTER_LIST_PT list, int *n ){
	int i;
	int *inds;
	CLUSTER_LIST_PT q;	
	// copy the first element 
	if( list!=NULL ){
		*n = list->nClustPts;
		inds = calloc( *n,sizeof(int) );
		for ( i=0;i<*n;i++ )
			inds[i] = list->clustPtsInds[i];	
		q = list;	// create an alias to access to the actual head of the list
		list = list->next;	// delete the actual head
		free(q);
		return inds;
	}
	printf("Empty list of clusters");		
	return NULL;
}

/* insert the element Tnode at the head of a list (remember to free the memory pointed by inds after calling this function)*/
void clustPush( CLUSTER_LIST_PT *list, int *inds, int n ){
	int i;
	CLUSTER_LIST_PT q;	//pointer to a new list field
	
	q = (CLUSTER_LIST_PT)calloc( 1,sizeof(struct cluster) );	//allocate memory for a new list field
	// copy the new element to another memory location pointed by clustPtsInds
	q->clustPtsInds = calloc( n,sizeof(int) );
	for( i=0;i<n;i++ )
		q->clustPtsInds[i] = inds[i];
	q->nClustPts = n;
	q->next = *list;	//link to the rest of the list
	*list = q;	
}

/* delete from the list the first element*/
void clustListFirstElemDelete( CLUSTER_LIST_PT *list ){
	CLUSTER_LIST_PT q;	
	if( (*list)!=NULL ){
		q = *list;	// create an alias to access to the actual head of the list
		*list = (*list)->next;	// delete the actual head
		free(q->clustPtsInds);
		free(q);
		return;
	}
}

/* delete from the list all the elements equal to Tnode (not implemented for cluster list since not useful)*/

/* insertion of an element in order -------- not yet implemented, because not useful for a list of clusters*/

/* print of a list ----------- not yet implemented because not useful for a list of clusters*/

/* inzialize list to an empty list*/
void startClustList( CLUSTER_LIST_PT *list){
	*list = NULL;
}

/* returns 1 if the list is empty*/
int isEmptyClustList( CLUSTER_LIST_PT list){
	if( list==NULL )
		return 1;
	return 0;
}

/* returns 1 if the element Tnode is stored in the list (not implemented for list of clusters as not useful)*/
/*int isInList( TREENODE_LIST_PT list, treenode *Tnode ){
	if( list==NULL )
		return 0;
	if( Tnode==list->treeNodePt )
		return 1;
	return isInList( list->next,Tnode );
}*/

/* ****************************************************************************************** */
/********************************* QUEUE OF CLUSTERS ***************************************/

/* inizialize an empty queue*/
void startClustQueue( CLUSTER_QUEUE *q ){
	(*q).first = NULL;
}

/* returns 1 if the queue is empty, 0 otherwise*/
int isEmptyClustQueue( CLUSTER_QUEUE q ){
	if( q.first==NULL )
		return 1;
	return 0;
}

/* adds an element at the end of the queue*/

void clustEnQueue( CLUSTER_QUEUE *q,  int *inds, int n ){
	int i;
	CLUSTER_LIST_PT p;	//pointer to a new list field
	
	p = (CLUSTER_LIST_PT)calloc( 1,sizeof(struct cluster) );	//allocate memory for a new list field
	// copy the new element to another memory location pointed by clustPtsInds
	p->clustPtsInds = calloc( n,sizeof(int) );
    if(p->clustPtsInds==NULL)
			printf("cannot allocate memory for the fields inds\n");
	for( i=0;i<n;i++ )
		p->clustPtsInds[i] = inds[i];
	p->nClustPts = n;
	if( q->first==NULL )	//if the queue is empty
		q->first = p;	// the new node is the first
	else				// otherwise
		q->last->next = p;	// it must be pointed by the last node
	p->next = NULL;		
	q->last = p;			// the new node is the last in the queue
}

/* extracts the first element from the queue and returns a pointer to it*/
int *clustDeQueue( CLUSTER_QUEUE *q, int *n ){
	int i;
	int *inds;
	CLUSTER_LIST_PT p;

	/* points to the node to be extracted*/
	p = q->first;
	// copy the first element 
	if( p!=NULL ){
		*n = p->nClustPts;
		inds = calloc(*n,sizeof(int));
		for ( i=0;i<*n;i++ )
			inds[i] = p->clustPtsInds[i];	
		q->first = p->next;	// extracts the node from the queue
		free(p);
		return inds;
	}
	
	printf("Empty queue of clusters");		
	return NULL;
}
#endif
