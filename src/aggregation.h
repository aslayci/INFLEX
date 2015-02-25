#ifndef _AGGREGATENNLISTS_H
#define _AGGREGATENNLISTS_H

void calculateImportanceWeights(double*, double*, int, double);
int *aggregateNNLists(int qID, int *RNNs, double *dToRNNs, double *weights, int nrSolutionsFound, int seedSetSize, int nRows, double maxDistance);
int *copelandScoreCalculation(int *RNNs, double *dToRNNs, double *weights, int nrRNNs, int nRows);
//int *aggregateNNLists(int, int*, double*, double*, int, int, int, double*, double*, int*, int*, char*, int, char*, int);
void determineNrofNNs(double*, int, int*);
//void calculateAvgKendallAmongNNs(int, int*, double*, double*, int, int, int, char*, double*, double*, int*, int*, char*, int, char*);

#endif