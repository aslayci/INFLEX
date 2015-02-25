#ifndef TREE_H
#define TREE_H

#include "utils.h"
#include "divergence.h"
#include "listUtils.h"
#include "anderson.h"
#include "newTypes.h"

int maxDepth;
double sumLeafDepth;
double NumChildrenOfNonLeaves;
int numNonLeafNodes;
int numLeafNodes;

TREENODE* buildTree(double**,int,int,int );
TREENODE* recSubtreeBuild( double**,int*,int,int,int,int,double );
TREENODE* setupNode( int,int );

// for gaussian
void GmeansPlusPlus( CLUSTER_LIST_PT*,int*,double**,int*,int,int,double );
void initTwoMeansPlusPlus( int*,int*,int*,int*,double**,int*,int,int );
void deleteTree( TREENODE* );
int genRand(int);
double unifRand(double);

// for tree IO
void writeTree(TREENODE*,int,char*);
void writeNode(TREENODE*,int,FILE*);
TREENODE* readTree(char*);
TREENODE* readNode(int,FILE*);


#endif
