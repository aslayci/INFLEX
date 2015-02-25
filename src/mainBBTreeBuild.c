//
//
//  INFLEX BB-Tree Construction
//  
//
//

#include "utils.h"
#include "tree.h"
#include "divergence.h"
#include "arrayUtils.h"

char *datafile, *treefile = "BBTreeIndex.txt", *treestatsfile = "BBTreeStats.txt";
int n = -1, d = -1, bucketSize = 30;

void processArgs(int argc, char**argv){
    int i=1;
    if(argc <= 1){
        printf("usage:\n ./INFLEX_BuildTree -f dataFile -n numPoints -d dimPoints \n");
        exit(0);
    }
    
    while(i<argc){
        if(!strcmp(argv[i], "-d"))
            d = atoi(argv[++i]);
        else if(!strcmp(argv[i], "-n"))
            n=atoi(argv[++i]);
        else if(!strcmp(argv[i], "-f"))
            datafile = argv[++i];
        else if(!strcmp(argv[i], "-s"))
            bucketSize=atoi(argv[++i]);
        else{
            fprintf(stderr,"unrecognized option.. exiting \n");
            exit(1);
        }
        i++;
    }
    
    if(n==-1 || d==-1 || datafile==NULL){
        fprintf(stderr,"more arguments needed.. exiting \n");
        exit(1);
    }
}  


int main(int argc, char** argv){
  
	int i, j;
	double **x;
	TREENODE *root;
	struct timeval startTime,endTime;

	printf("**** Processing input arguments **** \n");
	processArgs(argc,argv);

    printf("**** Loading database points **** \n");
  	x = calloc(n, sizeof(double*));
	for(i = 0; i < n; i++)
		x[i]=calloc( d,sizeof(double) );	

	readData(x, n, d, datafile);
	
	for(i=0; i < n; i++)
		handleZeroKL(x[i],d);
		  
	printf("**** Building INFLEX-BBTree **** \n");

	gettimeofday(&startTime,NULL);
	root = buildTree(x, n, d, bucketSize);
    
    gettimeofday(&endTime,NULL);
	printf("Tree building completed in %6.6f seconds \n", timediff(startTime, endTime));
    
    writeTree(root,d,treefile);
    printf("Tree index saved in BBTreeIndex.txt \n");
    
    
    writeBuildResults(treestatsfile, bucketSize, maxDepth, numNonLeafNodes, numLeafNodes, timediff(startTime, endTime));
    
    printf("Tree stats saved in BBTreeStats.txt \n");
    
    for(i = 0; i < n; i++)
        free(x[i]);
    free(x);
	  
    deleteTree(root);
    return 0;
}

