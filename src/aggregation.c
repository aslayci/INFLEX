#define _AGGREGATENNLISTS_C

#include "RAUtils.h"
#include "arrayUtils.h"
#include "InsertionSort.h"
#include "utils.h"
#include "aggregation.h"

//#include "CopelandAggregation.h"
//#include "BordaAggregation.h"
//#include "KendallsTauTopK.h"
//#include "evaluationMeasures.h"
//#define EPSILONN pow(2.0, -52.0)

int *copelandScoreCalculation(int *RNNs, double *dToRNNs, double *weights, int nrRNNs, int nRows) {
    
    int i, j, z, pos, unionSize, *nodeList, **lists, *localKemenizedScores, cnter;
    double *copelandScores, sum, **majorityMatrix, cnt;
//    char *sbt1 = ".txt";
    List *root, *conductor, *sortedFinalScores;
    int *resultSet;
    
    resultSet = calloc(nRows, sizeof(int));
    
    //allocate memory for the index lists
    lists = calloc(nrRNNs, sizeof(int *));
    for(i = 0; i < nrRNNs; i++)
        lists[i] = calloc(nRows, sizeof(int));
    
    //import the index lists from files
    for(i = 0; i < nrRNNs; i++) {
        readIndexSeedSet(RNNs[i], lists[i], nRows);
    }
    
    //allocate memory for node list -- for the worst case scenario i.e. no intersection between the input lists - later realloc if needed
    nodeList = calloc(nRows * nrRNNs, sizeof(int));
    unionSize = 0;
    for(i = 0; i < nrRNNs; i++) {
        for(j = 0; j < nRows; j++) {
            pos = indexOfElement(nodeList, lists[i][j], unionSize);
            if(pos == -1) {
                nodeList[unionSize] = lists[i][j];
                unionSize++;
            }
        }
    }
    
    /* allocate memory for the majority matrix */
    majorityMatrix = calloc(unionSize, sizeof(double*));
    for(i = 0; i < unionSize; i++)
    majorityMatrix[i] = calloc(unionSize, sizeof(double));
    
    //	if(!strcmp(aggregationType, "u")) //unweighted copeland score aggregation
    //		createMajorityMatrix(majorityMatrix, lists, nodeList, nrRNNs, unionSize, nRows);
    //
    //	else if(!strcmp(aggregationType, "w")) //weighted copeland score aggregation
    //		createWeightedMajorityMatrix(majorityMatrix, lists, nodeList, weights, nrRNNs, unionSize, nRows);
    //
    //	else {
    //		fprintf(stderr, "aggregation tipini vermedin mal! \n");
    //		exit(1);
    //	}
    
    createWeightedMajorityMatrix(majorityMatrix, lists, nodeList, weights, nrRNNs, unionSize, nRows);
		 	
    /* allocate memory for the copeland score matrix */
    copelandScores = calloc(unionSize, sizeof(double));
    
    /* calculate copeland score from the matrix */
    for(i = 0; i < unionSize; i++) {
        cnt = 0; //nr of lists where majority ranked i before j
        for(j = 0; j < unionSize; j++) {
            if(majorityMatrix[i][j] > majorityMatrix[j][i])
            cnt += 1.0;
        }
        //printf("\n");
        copelandScores[i] = cnt;
    }
    
    /* insertion sort copeland scores */
    root = malloc(sizeof(List));
    root->iValue = copelandScores[0];
    root->pNext = 0;
    root->index = 0;
    conductor = root;
    
    for(i = 1; i < unionSize; i++) {
        while (conductor->pNext != 0)
        conductor = conductor->pNext;
        
        conductor->pNext = malloc(sizeof(List));
        conductor = conductor->pNext;
        
        conductor->pNext = 0;
        conductor->iValue = copelandScores[i];
        conductor->index = i;	//corresponds to index id in the nodeList and majority matrix
    }
    /* sort wrt copeland scores */
    sortedFinalScores = SortList(root);
    
    // no more input for delivery version, so hardcoding here - ugly change later
    int kemenize = 1;
    char *aggregationType = "w";
    
    if(kemenize == 0) { // no local kemenization
        //printf("no kemenization mf.. \n");
        conductor = sortedFinalScores;
        //printf("printing final sorted scores for regular copeland aggregation\n");
        
        cnter = 0;
        while(conductor->pNext != 0 && cnter < nRows) {
            //printf("%d - %f \n", nodeList[conductor->index], conductor->iValue);
            resultSet[cnter++] = nodeList[conductor->index];
            conductor = conductor->pNext;
        }
        if(nRows == unionSize)
        resultSet[cnter] = nodeList[conductor->index]; // do it for the last item in the list as well if k = unionSize
        //printf("%d - %f \n", nodeList[conductor->index], conductor->iValue);
        
    }
    
    else {
        if(kemenize == 1) { //weighted local kemenization
            localKemenizedScores = calloc(unionSize, sizeof(int));
            
            if(!strcmp(aggregationType, "w")) //use the weighted majorityMatrix that has been calculated
            localKemenization(localKemenizedScores, sortedFinalScores, majorityMatrix, nodeList, unionSize);
            
            else if(!strcmp(aggregationType, "u")) { //create unweighted majority matrix
                
                //destroy previous unweighted matrix
                for(i = 0; i < unionSize; i++)
                free(majorityMatrix[i]);
                free(majorityMatrix);
                
                //reallocate for weighted matrix
                majorityMatrix = calloc(unionSize, sizeof(double*));
                for(i = 0; i < unionSize; i++)
                majorityMatrix[i] = calloc(unionSize, sizeof(double));
                
                createWeightedMajorityMatrix(majorityMatrix, lists, nodeList, weights, nrRNNs, unionSize, nRows);
                localKemenization(localKemenizedScores, sortedFinalScores, majorityMatrix, nodeList, unionSize);
                
            }
        }
        
        else if(kemenize == 2) { //unweighted local kemenization
            localKemenizedScores = calloc(unionSize, sizeof(int));
            
            if(!strcmp(aggregationType, "w")) { //create unweighted majority matrix
                //destroy previous weighted matrix
                for(i = 0; i < unionSize; i++)
                free(majorityMatrix[i]);
                free(majorityMatrix);
                
                //reallocate for unweighted matrix
                majorityMatrix = calloc(unionSize, sizeof(double*));
                for(i = 0; i < unionSize; i++)
                majorityMatrix[i] = calloc(unionSize, sizeof(double));
                
                createMajorityMatrix(majorityMatrix, lists, nodeList, nrRNNs, unionSize, nRows);				
                localKemenization(localKemenizedScores, sortedFinalScores, majorityMatrix, nodeList, unionSize);
                
            }
            
            else if(!strcmp(aggregationType, "u")) //use the unweighted majority matrix that has been already calculated
            localKemenization(localKemenizedScores, sortedFinalScores, majorityMatrix, nodeList, unionSize);			
        }
        
        for(i = 0; i < nRows; i++) 				
        resultSet[i] = nodeList[localKemenizedScores[i]];			
        
        free(localKemenizedScores);		
        
    }		
    
    for(i = 0; i < nrRNNs; i++)
    free(lists[i]);		
    free(lists);
    
    for(i = 0; i < unionSize; i++)
    free(majorityMatrix[i]);
    free(majorityMatrix);
    
    free(nodeList);
    //free(weights);
    free(copelandScores);	
    
    //free(conductor);	
    //free(root);
    return resultSet;
    
}

void calculateImportanceWeights(double *weights, double *dToRNNs, int nrRNNs, double maxDistance) {
	//normalization tru non-linear transformation
	int i;
    
    for(i = 0; i < nrRNNs; i++) {
		weights[i] = (exp(maxDistance) - exp(dToRNNs[i])) / exp(dToRNNs[i] + maxDistance);
		
    }

}

/* do top-l aggregation then return top-k for result */
int *aggregateNNLists(int qID, int *RNNs, double *dToRNNs, double *weights, int nrSolutionsFound, int seedSetSize, int nRows, double maxDistance) {
    
    int *raSeedSet;
    raSeedSet = calloc(seedSetSize, sizeof(int));
    raSeedSet = copelandScoreCalculation(RNNs, dToRNNs, weights, nrSolutionsFound, nRows);
    return raSeedSet;

}

void determineNrofNNs(double *weights, int nrRNNs, int *determinedNr) {
    
    int j;
    int nrToOperareOn;
    double sum = 0.0;
    double lastNormedWeight = 1.0;
    double uniformWeight = 1.0;
    nrToOperareOn = 2;
    double criteria = 0.005;
    
    while((((uniformWeight - lastNormedWeight) <= criteria) ) && (nrToOperareOn-1 < 10)) {
        
        sum = 0.0;
        for(j = 0; j < nrToOperareOn; j++)
        sum += weights[j];
        
        lastNormedWeight = weights[nrToOperareOn - 1] / sum;
        uniformWeight = 1.0 / nrToOperareOn;
        
        nrToOperareOn++;
    }
    
    *determinedNr = nrToOperareOn -1;
}
