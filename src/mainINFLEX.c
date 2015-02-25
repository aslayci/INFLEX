//
//
//  INFLEX Similarity Search
//
//
//

#include "utils.h"
#include "simSearch.h"
#include "tree.h"
#include "divergence.h"
#include "heapsort.h"
#include "arrayUtils.h"

void processArgs(int, char**);

char *datafile = "./indexPoints.txt", *queryfile = "./queryPoints.txt", *treefile = "./BBTreeIndex.txt";
int n = -1, m = -1, d = -1, seedSetSize = -1, nRows = -1;
int l = 5, rangeMaxLimit = 10, nrSolutionsFound = 0;

int main(int argc, char **argv) {
    
    int i, j, *RNNs, *leafRNNs;
    double *dToRNNs, **x, **q;
    
    TREENODE *root;
    processArgs(argc, argv);
    
    x = calloc(n, sizeof(double*));
    q = calloc(m, sizeof(double*));
    
    for(i = 0; i < n; i++)
    x[i] = calloc(d, sizeof(double));
    
    for(i = 0; i < m; i++)
    q[i] = calloc(d, sizeof(double));
    
    printf("**** Loading index points **** \n");
    readData(x,n,d,datafile);
    
    printf("**** Loading query points **** \n");
    readData(q,m,d,queryfile);
    
    RNNs = calloc(rangeMaxLimit, sizeof(int));
    dToRNNs = calloc(rangeMaxLimit, sizeof(double));
    leafRNNs = calloc(rangeMaxLimit, sizeof(int));
    
    for(i = 0; i < n; i++)
        handleZeroKL(x[i],d);
    
    for(i = 0; i < m; i++)
        handleZeroKL(q[i],d);
    
    printf("**** Loading INFLEX-BBTree **** \n");
    root = readTree(treefile);
    
    printf("**** INFLEX query processing started **** \n");
    
    multiQueryProcess(root, q, x, n, d, RNNs, dToRNNs, l, m, rangeMaxLimit, seedSetSize, leafRNNs, nRows);
    
    /* free memory */
    for(i = 0; i < n; i++)
        free(x[i]);
    free(x);
    
    for(i = 0; i < m; i++)
        free(q[i]);
    free(q);
    
    free(RNNs);
    free(dToRNNs);
    free(leafRNNs);
    
}

void processArgs(int argc, char **argv){
    int i = 1;
    
    if(argc <= 1){
        printf("usage:\n ./INFLEX_QueryProcessing -f dataFile -t treeFile -q queryFile -n numPoints -m numQueries -d dim -k seedSetSize -s indexListSize [-x rangeMaxLimit] \n");
        
        exit(0);
    }
    
    while(i < argc) {
        
        if(!strcmp(argv[i], "-d"))
            d = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-n"))
            n = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-m"))
            m = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-f"))
            datafile = argv[++i];
        else if(!strcmp(argv[i], "-t"))
            treefile = argv[++i];
        else if(!strcmp(argv[i], "-q"))
            queryfile = argv[++i];
        
        else if(!strcmp(argv[i], "-k"))
            seedSetSize = atoi(argv[++i]);
        
        else if(!strcmp(argv[i], "-s"))
            nRows = atoi(argv[++i]);
        
        else if(!strcmp(argv[i], "-x"))
            rangeMaxLimit = atoi(argv[++i]);
        
        else if(!strcmp(argv[i], "-l"))
            l = atoi(argv[++i]);
        
        // removing knn and aggregation evaluators etc for delivery
        //        else if(!strcmp(argv[i], "-h")) //for knn check
        //            truthFile = argv[++i];
        //        else if(!strcmp(argv[i], "-o")) {
        //            searchResultsFile = argv[++i]; //for query results
        //        }
        //        else if(!strcmp(argv[i], "-s")) {
        //            searchStatsFile = argv[++i];
        //        }
        //        else if(!strcmp(argv[i], "-r"))
        //        range = atof(argv[++i]);
        //        aggregationMethod = argv[++i];
        //		else if(!strcmp(argv[i], "-fp")) //for plot creation
        //			forPlots = argv[++i];
        //		else if(!strcmp(argv[i], "-autoK")) //automatic k determination
        //			autoK = atoi(argv[++i]);
        //		else if(!strcmp(argv[i], "-andersDTest"))
        //			andersDTest = atoi(argv[++i]);
        //        else if(!strcmp(argv[i], "-at"))
        //        aggregationType = argv[++i];
        //        else if(!strcmp(argv[i], "-lk"))
        //        kemenize = atoi(argv[++i]);
        //        else if(!strcmp(argv[i], "-am"))
        
        else {
            fprintf(stderr, "unrecognized option.. exiting\n");
            exit(1);
        }
        i++;
    }
    
    if(d == -1 || n == -1 || m == -1 || seedSetSize == -1 || nRows == -1) {
        fprintf(stderr,"more arguments needed.. exiting \n");
        exit(1);
    }
    
}


