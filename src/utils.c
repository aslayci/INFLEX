#include "utils.h"
#include "listUtils.h"

#define indexListLocation "./lists/index/iList_"
#define resultListLocation "./lists/result/resultSeeds_"
#define nrCols 10


void readIndexSeedSet(int iID, int *list, int nRows) {
    
    int nCols = nrCols;
    char file[100] = indexListLocation;
    char tmp[10];
    char *sbt1 = ".txt";
    sprintf(tmp, "%d", iID);
    strcat(file, tmp);
    strcat(file, sbt1);
    
    int i, j;
    FILE *fp = fopen(file, "r");
    float val;
    if(fp == NULL) {
        fprintf(stderr, "error opening file.. exiting \n");
        exit(1);
    }
    for(i = 0; i < nRows; i++) {
        for(j = 0; j < nCols; j++) {
            if(fscanf(fp, "%f ", &val) == EOF) {
                fprintf(stderr, "file %s could not be read.. exiting \n", file);
                exit(1);
            }
            if(j == 0)
            list[i] = (int) val;
        }
    }
    fclose(fp);
}

void writeResultSeedSet(int qID, int *seedSet, int seedSetSize){
    
    FILE *fp2;
    char file[100] = resultListLocation;
    char *sbt1 = ".txt";
    char tmp[10];
    sprintf(tmp, "%d", qID);
    strcat(file, tmp);
    strcat(file, sbt1);
    int j;
    
    fp2 = fopen(file, "w");
    for(j = 0; j < seedSetSize; j++)
    fprintf(fp2, "%d\n", seedSet[j]);
    
    fclose(fp2);
    
}

// read vector of topic distributions (matrix of doubles)
void readData(double **x, int n, int d, char* file){
    int i,j;
    float t;
    FILE *fp = fopen(file,"r");
    
    if(fp==NULL){
        fprintf(stderr,"error opening file.. exiting\n");
        exit(1);
    }
    
    for(i=0;i<n;i++){
        for(j=0;j<d;j++){
            if(fscanf(fp,"%f ", &t)==EOF){
                fprintf(stderr,"error reading file.. exiting \n");
                exit(1);
            }
            x[i][j]=(double)t;
        }
    }
    fclose(fp);
}

void writeBuildResults(char *file, int bucketSize, int maxDepth, int numNonLeafNodes, int numLeafNodes, double timeDif) {
    FILE *fp = fopen(file, "w");
    
    if(fp == NULL){
        fprintf(stderr,"unable to open input results file... exiting \n");
        exit(1);
    }
    
    fprintf(fp, "Bucketsize: %d \n maxDepth: %d \n Nr Non-Leaf Nodes: %d \n Nr Leaf Nodes: %d \n Build Time: %f \n", bucketSize, maxDepth, numNonLeafNodes, numLeafNodes, timeDif);
    fclose(fp);
}

double timediff(struct timeval start, struct timeval end){
    return (double)(end.tv_sec+end.tv_usec/1e6 - start.tv_sec - start.tv_usec/1e6); // in seconds
}

void insert(int* NNs, double* dtoNNs, int newNN, double dNNnew, int k){
    
    int i=0;
    while (i < (k-1) && dtoNNs[i+1]>dNNnew){
        NNs[i]=NNs[i+1];
        dtoNNs[i]=dtoNNs[i+1];
        i++;
    }
    
    if ( dtoNNs[i]>dNNnew ) {
        NNs[i]=newNN;
        dtoNNs[i]=dNNnew;
    }
    
}

void nnInsert(int *RNNs, double *dtoRNNs, int rangeMaxLimit, int newRNN, double dRNNNew, int *leafRNNs, int leafNr) {
    
    int i = rangeMaxLimit - 1;
    
    while(i > 0 && dRNNNew < dtoRNNs[i - 1]) {
        RNNs[i] = RNNs[i - 1];
        dtoRNNs[i] = dtoRNNs[i - 1];
        leafRNNs[i] = leafRNNs[i - 1];
        i--;
    }
    if(dRNNNew < dtoRNNs[i]) {
        RNNs[i] = newRNN;
        dtoRNNs[i] = dRNNNew;
        leafRNNs[i] = leafNr;
    }

}

/* check if a file exists */
int file_exists(char *file) {
    FILE *fp = fopen(file, "r");
    
    if(fp != NULL){
        fclose(fp);
        return 1;
    }	
    return 0;	
}
