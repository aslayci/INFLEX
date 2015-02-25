#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include "newTypes.h"

/* ****************************************************************************************** */
/************************* LIST OF TREENODE POINTERS ***************************************/

void treeNodeListHeadCopy( TREENODE_LIST_PT,LIST_NODE* );
void treeNodeListPop( TREENODE_LIST_PT,TREENODE* );		// not debugged	(not used in bbt building)
void treeNodeListPush( TREENODE_LIST_PT*,TREENODE* );	// debugged
void treeNodeListElementDelete( TREENODE_LIST_PT,TREENODE* );	// not debugged (not used in bbt building)
void treeNodeListFirstElemDelete( TREENODE_LIST_PT* );	// debugged
void treeNodeStartList( TREENODE_LIST_PT* );			// debugged	
int isEmptyTreeNodeList( TREENODE_LIST_PT );			// not debugged	(not used in bbt building)
int isInTreeNodeList( TREENODE_LIST_PT,TREENODE* );		// not debugged	(not used in bbt building)

/* ****************************************************************************************** */
/************************QUEUE OF TREENODE POINTERS **************************************/

void treeNodeStartQueue( TREENODE_QUEUE* );				// debugged
int isEmptyTreeNodeQueue( TREENODE_QUEUE );				// not debugged	(not used in bbt building)
void treeNodeEnQueue( TREENODE_QUEUE*,TREENODE* );		// (used only in readNode function)
void treeNodeDeQueue( TREENODE_QUEUE*,TREENODE** );		// debugged

/* ****************************************************************************************** */
/*********************************** LIST OF CLUSTERS ***************************************/

int *clusterPop( CLUSTER_LIST_PT,int* );			// not debugged (not used in bbtree building)
void clustPush( CLUSTER_LIST_PT*,int*,int );		// debugged
void clustListFirstElemDelete( CLUSTER_LIST_PT* );	// debugged
void startClustList( CLUSTER_LIST_PT* );			// debugged
int isEmptyClustList( CLUSTER_LIST_PT );			// debugged

/* ****************************************************************************************** */
/********************************* QUEUE OF CLUSTERS ***************************************/

void startClustQueue( CLUSTER_QUEUE* );				// debugged
int isEmptyClustQueue( CLUSTER_QUEUE );				// not debugged (not used in bbtree building)
void clustEnQueue( CLUSTER_QUEUE*,int*,int);		// debugged
int *clustDeQueue( CLUSTER_QUEUE*,int* );			// debugged

#endif
