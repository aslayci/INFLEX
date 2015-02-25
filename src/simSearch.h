#ifndef RANGE_SEARCH_H
#define RANGE_SEARCH_H

#include "utils.h"
#include "tree.h"
#include "divergence.h"
#include "listUtils.h"
#include "minPriorityQueue.h"
#include "aggregation.h"
#include "anderson.h"

//void multiRangeSearch(TREENODE*, double**, double**, int, int, double, int*, double*, int, int, int, char*, char*, int, int*, char*, int*, int*, char*, char*, int, char*, int, int);

//void multiQueryProcess(TREENODE*, double**, double**, int, int, int*, double*, int, int, int, int, int*, int);
void multiQueryProcess(TREENODE *root, double **Q, double **data, int n, int d, int *RNNs, double *dToRNNs, int maxLeaves, int m, int rangeMaxLimit, int seedSetSize, int *leafRNNs, int nRows);

void inflexSimSearch(TREENODE *root, double *q, double **data, int d, int maxLeaves, int *RNNs, double *dToRNNs, int *rangeMaxLimitpt, double *nextNNDist, int *leafRNNs, int *isExact);

int needToExplore(TREENODE*, double*, int, double);
int checkStopCondition(int, int);
int ballContainment(double*, double*, int, double, double);

int decideToStayAtLeaf(TREENODE*, double**, int, double*, int);

#endif