#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>

#define CLOSEENOUGH 1e-3

typedef int (*compfn)(const void*, const void*);

void readData(double**,int,int,char*);
void writeBuildResults(char*, int, int, int, int, double);
double timediff(struct timeval,struct timeval);
void insert(int*,double*,int,double,int);
void nnInsert(int*, double*, int, int, double, int*, int);
int file_exists(char*);

void readIndexSeedSet(int iID, int *list, int nRows);
void writeResultSeedSet(int qID, int *seedSet, int seedSetSize);





#endif
