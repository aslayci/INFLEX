#define RANGE_SEARCH_C
#define LAMBDA_PRECISION 0.00001

#include "simSearch.h"

long distCount; //number of computed distances
int visitedLeaves; //number of visited leaves
int nrIntersectingInternalNodes; //number of intersecting internal nodes
int internalNodeCounter; //number of visited internal nodes
double alpha = 0.95;
int determinedNr;
double *x;

int containnsElement(int*, int, int);

void multiQueryProcess(TREENODE *root, double **Q, double **data, int n, int d, int *RNNs, double *dToRNNs, int maxLeaves, int m, int rangeMaxLimit, int seedSetSize, int *leafRNNs, int nRows) {
  		
    double nextNNDist, runningTime, *aggWeights, kendallDistance, jaccardDistance;
    int i,j, tmpLimit, nrSolutionsFound, exact = 0, anders = 0;;
    struct timeval startTime, endTime;
    
    
    int *raSeedSet;
    
    x = calloc(d,sizeof(double));
    raSeedSet = calloc(nRows, sizeof(int));
    aggWeights = calloc(rangeMaxLimit, sizeof(double));
    
    // max distance calculate
    double *t1,*t2;
    t1 = calloc(d, sizeof(double));
    t2 = calloc(d, sizeof(double));
    t1[0] = 1.0;
    t2[1] = 1.0;
    
    double maxDistance = divergence(t1,t2,d);
    
    /*** similarity search + rank aggregation for each query specified in the input file ***/
    for(i = 0; i < m; i++) {
        
        printf("processing query %d\n", i);
    
        distCount = 0;
        visitedLeaves = 0;
        nrIntersectingInternalNodes = 0;
        internalNodeCounter = 0;
        nrSolutionsFound = 0;
        tmpLimit = rangeMaxLimit;
        
        for(j = 0; j < rangeMaxLimit; j++){
            dToRNNs[j] = HUGE_VAL;
            RNNs[j] = -1;
            leafRNNs[j] = -1;
        }
        
        for(j = 0; j < nRows; j++)
            raSeedSet[j] = -1;
        
        //start timer
        gettimeofday(&startTime,NULL);
        
        inflexSimSearch(root, Q[i], data, d, maxLeaves, RNNs, dToRNNs, &tmpLimit, &nextNNDist, leafRNNs, &exact);
       
        for(j = 0; j < tmpLimit && RNNs[j] != -1; j++)
            nrSolutionsFound++;
        
        for(j = 0; j < rangeMaxLimit; j++)
            aggWeights[j] = 0.0;
    
        //for(j = 0; j < rangeMaxLimit; j++)
//        aggWeights[j] = 0.0;
//        calculateImportanceWeights(aggWeights, dToRNNs, nrSolutionsFound, maxDistance);
//        determinedNr = 0;
//        //printf("determining nr of nns \n****************\n");
//        determineNrofNNs(aggWeights, rangeMaxLimit, &determinedNr);
//        nrSolutionsFound = determinedNr;
//        for(j = 0; j < rangeMaxLimit; j++)
//        aggWeights[j] = 0.0;
//        calculateImportanceWeights(aggWeights, dToRNNs, nrSolutionsFound, maxDistance);
        
        calculateImportanceWeights(aggWeights, dToRNNs, nrSolutionsFound, maxDistance);
        raSeedSet = aggregateNNLists(i, RNNs, dToRNNs, aggWeights, nrSolutionsFound, seedSetSize, nRows, maxDistance);
        
        // stop timer
        gettimeofday(&endTime,NULL);
        runningTime = timediff(startTime,endTime);
        printf("query %d processed in %6.3f milliseconds \n", i, (runningTime * 1000));
        printf("KL-div with closest nearest-neighbor = %f, with furthest nearest-neighbor = %f \n", dToRNNs[0], dToRNNs[nrSolutionsFound - 1]);
        
        // write the seed set returned to query
        writeResultSeedSet(i, raSeedSet, seedSetSize);
        printf("seed set for query %d saved in ./lists/result/resultSeeds_%d.txt  \n", i, i);
        
    }
}

//  cayton style
int needToExplore( TREENODE *node, double *q, int d, double qToActualKnnDist ){
    int nIter;
    double lambda, lambdaMin, lambdaMax, xToMu, xToQ, lbnd;
    
    lambdaMin = 0.0;
    lambdaMax = 1.0;
    nIter = 0;
    
    if( node->divQToMu < node->R || node->divQToMu < qToActualKnnDist)
    return 1;
    
    while(1){
        /*explores the node if the query is close enough to the ball projection*/
        if( (lambdaMax-lambdaMin) < LAMBDA_PRECISION )
        return 1;
        lambda = 0.5*(lambdaMin+lambdaMax);		// halve the interval
        
        // if the actual x is not close enough to the ball projection
        geodesicPoint( x,node->mu,q,d,lambda );	// finds the corresponding point on the geodesic
        xToMu = divergence( x,node->mu,d );	// for an asymmetric divergence D(x||mu) is coherent with how the radius is computed
        xToQ = divergence( x,q,d );
        //distCount+=2;
        lbnd = xToQ + ( 1.0/lambda -1.0 )*( xToMu - node->R );
        
        /*prunes the node if the boundary of the ball can't be closer to the query than the actual kNN candidate (lower bound)*/
        if( qToActualKnnDist < lbnd ){
            return 0;
        }
        
        /*if the actual estimation of x is outside the ball*/
        if( xToMu > node->R ){
            lambdaMax = lambda;
        }
        /*if the actual estimation of x is inside the ball*/
        else if( xToQ < qToActualKnnDist ){
            return 1;
        }
        else{
            lambdaMin = lambda;
        }
        nIter++;
    }
    
}

void inflexSimSearch(TREENODE *root, double *q, double **data, int d, int maxLeaves, int *RNNs, double *dToRNNs, int *rangeMaxLimitpt, double *nextNNDist, int *leafRNNs, int *isExact) {
    
    int i,j, h, anders;
    double divTemp, diff;
 
    TREENODE *node;
    HEAPTREENODE *queue;
    int heapSize = 0;
    int nnSize = 0;
    int stop = 0;
    int stop2 = 0;
    double dTomu = 0.0;
    double criter = 0.0;
    int isToBeVisited = 0;
    
    /* changed for multi */
    int subQueueSize;
    HEAPTREENODE *subQueue;
    
    //right-left child
    LIST_NODE *nodeChild;
    nodeChild = calloc(1, sizeof(LIST_NODE));
    
    queue = (HEAPTREENODE*)calloc(root->n, sizeof(HEAPTREENODE));	// allocate memory for the heap
    
    if(queue==NULL)
    printf("impossibru to allocate memory for the queue\n");
    
    minHeapInsert(queue, &heapSize, root,-1.0 ); //add the root node to the heap
    
    while(!stop && !stop2 && heapSize){
        
        node = heapExtractMin(queue, &heapSize);
        //printf("visited leaves so far: %d\n", visitedLeaves);
        isToBeVisited = needToExplore(node,q,d,dToRNNs[(*rangeMaxLimitpt)-1]);
        while(!node->isLeaf && isToBeVisited) { //if the node is not leaf
            internalNodeCounter++;
            nrIntersectingInternalNodes++;
            subQueue = &(queue[heapSize]);
            subQueueSize = 0;
            treeNodeListHeadCopy(node->children,nodeChild);		// retrieves the first child node
            for(i=0; i < node->nChildren; i++ ){
                nodeChild->treeNodePt->divQToMu = divergence(nodeChild->treeNodePt->mu, q, d);
                distCount++;
                minHeapInsert( subQueue,&subQueueSize,nodeChild->treeNodePt,nodeChild->treeNodePt->divQToMu);	//push each child into the subqueue and retrieve the first after reordering (this is the subtree that has to be explored)
                treeNodeListHeadCopy( nodeChild->next,nodeChild );		// retrieves the following child node
            }
            heapSize += node->nChildren-1;						// update the size of the overall queue
            node = heapExtractMin(subQueue, &subQueueSize);
            //}
            
        }
        
        if(node->isLeaf && isToBeVisited) {
            
            distCount += node->n;
            visitedLeaves++;
            dTomu = divergence(q, node->mu, d);
            criter = dTomu / node->R;
            
            for(i = 0; i < node->n; i++) {
                divTemp = divergence(data[node->inds[i]], q, d);
                if(RNNs[(*rangeMaxLimitpt) - 1] == -1 || divTemp < dToRNNs[(*rangeMaxLimitpt) - 1])
                nnInsert(RNNs, dToRNNs, (*rangeMaxLimitpt), node->inds[i], divTemp, leafRNNs, visitedLeaves);
            }
            
            if(anders == 1) {
                if(vecAndersonTest(data, node->inds, node->n, d, node->mu, q, alpha) == 1) {
                    stop2 = 1;
                    break;
                }
            }
            
        }
        
        stop = checkStopCondition(visitedLeaves,maxLeaves);	
    }
    
    free(queue);
    
}

int checkStopCondition( int numVisitedLeaves, int maxLeaves ){
    if( numVisitedLeaves>=maxLeaves )
        return 1;
    else
        return 0;
}


int containnsElement(int *list, int elem, int size) {	
    int i;
    for(i = 0; i < size; i++)
    if(list[i] == elem)
    return 1;
    
    return 0;
}
