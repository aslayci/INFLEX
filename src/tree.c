//
//
// Adapted for INFLEX from implementations of Nielsen et al. and Cayton
//
//

#ifndef TREE_C
#define TREE_C

#include "tree.h"

/* define the minimum and maximum number of child nodes for each node of the tree*/
#define MAX_N_CHILDREN 4
#define MAX_BALL_RADIUS 0.01
#define ALPHA 0.99	// confidence level for the Anderson-Darling test
#define MAX_N_PTS_CLUSTER 10 // maximum number of points that can be stored in a Gmeans cluster

/* builds the whole tree for a dataset, given a maximum bucketSize */
TREENODE* buildTree(double **data, int n, int d, int bucketSize ){
	int i;
	int* inds;
	int depth = 0;		/* the root depth is zero*/
	TREENODE *root;

	/*seed the random number generator */
	struct timeval tNow;
	gettimeofday(&tNow,NULL);
	srand(tNow.tv_usec);
    
	/* allocates the array for data indexes*/
	inds = calloc(n,sizeof(int));  
	if(inds==NULL)
		printf("error w 0.01ith allocation");
	/* all indexes are stored in the root node*/	
	for(i=0;i<n;i++)
		inds[i]=i;	
	
	/* calls the recursive building function for the root*/
	root = recSubtreeBuild( data,inds,n,d,bucketSize,depth,ALPHA );

	free(inds);
	
	/* statistic on the tree depth (maximum depth and average depth of a node)*/
	printf("tree depth = %d\n",maxDepth);	// tree depth
	printf("mean depth = %6.2f\n",sumLeafDepth/(double)n);	// average tree depth
	printf("number of non-leaf nodes = %d\n",numNonLeafNodes);
	printf("number of leaf nodes = %d\n", numLeafNodes);
	printf("mean number of children per node = %f\n",NumChildrenOfNonLeaves/(double)numNonLeafNodes);

	return root;
}

/* recursively builds a subtree for a data subset, by using bucketSize for the stop condition*/
TREENODE* recSubtreeBuild(double **data, int* inds, int n, int d, int bucketSize, int depth, double alpha){
  
	int i,j;
	double divTemp;	
	CLUSTER_LIST_PT clusters;							/* pointer to a list of clusters*/
	int *childInds = calloc( n,sizeof(int) );
	int childNPts;							
	TREENODE *child;
	int nClusters = 0;
		
	/********************** inizializes a new treenode**************************/
	/**********************************************************************/
	TREENODE* node = setupNode(n,d);
	// inizialization of known node fields
	node->n=n;
	/*for(i=0;i<n;i++)
		node->inds[i]=inds[i];*/
	mean(node->mu,data,inds,n,d);
	//iste bu noktada variance hesapla
	
  	node->R=0.0;
	for(i=0;i<n;i++){
		divTemp = divergence(data[inds[i]],node->mu,d);
		if(divTemp> node->R)
			node->R = divTemp;
	}
	grad(node->gradMu,node->mu,d); 
	
	node->depth = depth;
	if (depth>maxDepth)		// maxDepth is a global variable
		maxDepth = depth;
  
	/* *****************if the actual node it small enough*************************/
	/********************it is set as a leaf*************************************/
	if (node->R < MAX_BALL_RADIUS || n <= bucketSize){
        node->isLeaf = 1;
		numLeafNodes++;
		sumLeafDepth += node->n * node->depth;
		for(i=0;i<n;i++)
			node->inds[i]=inds[i];
		return node;
	}
		
	/**********************else it must be splitted*****************************/
	/**********************************************************************/	
	startClustList( &clusters );							/* inizializes it to the empty list*/
		
	node->isLeaf=0;	
	GmeansPlusPlus( &clusters,&nClusters,data,inds,n,d,alpha );	/* splits the actual node by Gmeans*/
	node->nChildren = nClusters;
	
	// for each cluster in the list creates a new tree node
	numNonLeafNodes++;
	while( clusters!=NULL ){
		childNPts = clusters->nClustPts;
		for( j=0;j<childNPts;j++ )
			childInds[j] = clusters->clustPtsInds[j];
		child = recSubtreeBuild( data,childInds,childNPts,d,bucketSize,depth+1,alpha );
		treeNodeListPush( &(node->children),child );
		clustListFirstElemDelete( &clusters );
		NumChildrenOfNonLeaves++;
	}
	free(childInds);
	return node;
}

TREENODE* setupNode(int n, int d){
  TREENODE *node;
  node = (TREENODE*)calloc(1,sizeof(TREENODE));
  node->mu = calloc(d,sizeof(double));
  node->gradMu = calloc(d,sizeof(double));
  node->inds = calloc(n,sizeof(int));
  treeNodeStartList( &(node->children) );
  
  return node;
}

/* divides a dataset in G clusters following the Gmeans algorithm (input clusters has to be an empty list, *nClusters must be 0)*/
void GmeansPlusPlus( CLUSTER_LIST_PT *clusters, int *nClusters, double **data, int *inds, int n, int d, double alpha ){
	CLUSTER_QUEUE queue;
	int *indsL, *indsR;
	int *indsActual;	/*pointers to values (inds and n) of the actual cluster: do not allocate memory for it (allocation is done by the function DeQueue)*/
	int nActual = 0;
	int lLength, rLength;
	double *lCenter, *rCenter;
	int anders;
	
	/* input clusters must be an empty list*/
	if( isEmptyClustList(*clusters)!=1 || *nClusters!=0){
		printf("Not empty list of clusters: Gmeans cannot start\n");
		return;
	}
	
	indsL = calloc( n,sizeof(int) );
	indsR = calloc( n,sizeof(int) );
	lCenter = calloc( d,sizeof(double) );
	rCenter = calloc( d,sizeof(double) );
	startClustQueue( &queue );	/*inizializes the queue as empty*/
	
	initTwoMeansPlusPlus( indsL,indsR,&lLength,&rLength,data,inds,n,d );	/* splits the node in two clusters by 2-means++*/
	*nClusters = (*nClusters)+2;
	// addds the two clusters to the queue, firstly the more numerous
	// in the case not splitting both is not allowed, the bigger one has greater priority (maybe change the priority as a function of the node radius)
	if( lLength > rLength ){
		clustEnQueue( &queue,indsL,lLength );	/* adds the two clusters to the queue*/
		clustEnQueue( &queue,indsR,rLength );	/*						*/
	}
	else{
		clustEnQueue( &queue,indsR,rLength );
		clustEnQueue( &queue,indsL,lLength );
	}
	
	while( queue.first!=NULL ){
		indsActual = (int*)clustDeQueue( &queue, &nActual );	/* pops the first element of the queue*/
		
		if( *nClusters+1 <= MAX_N_CHILDREN && nActual >= MAX_N_PTS_CLUSTER ){	/*replacing a cluster by two clusters increases the actual num of clusters by 1*/
			initTwoMeansPlusPlus( indsL,indsR,&lLength,&rLength,data,indsActual,nActual,d );	/* splits the actual cluster by 2-means++*/
			mean(lCenter,data,indsL,lLength,d); 	/* Compute the centroids of the two clusters*/
			mean(rCenter,data,indsR,rLength,d);		/*							*/
			anders = vecAndersonTest( data,indsActual,nActual,d,lCenter,rCenter,alpha );	/* performs the Anderson-Darling normality test*/
			if ( anders==0 ){						/* if the node has to be splitted*/
				clustEnQueue( &queue,indsL,lLength );	/* adds the two clusters to the queue*/
				clustEnQueue( &queue,indsR,rLength );	/*						*/
				*nClusters = (*nClusters)+1;
			}
			else								/* if the node is accepted without splitting */
				clustPush( clusters,indsActual,nActual );	/* adds the actual cluster to the list of clusters*/
		}		
		else	/* too few points in the actual cluster*/
			clustPush( clusters,indsActual,nActual );	/* adds the actual cluster to the list of clusters*/
	}
}


/* inizialize two clusters of a dataset based on kmeans++ algorithm */
void initTwoMeansPlusPlus( int *indsL, int *indsR, int *lLength, int *rLength, double **data, int *inds, int n, int d ){
  
	//declarations
	int i, lp, rp;
	double *cumSum;				// the cdf for randomly extracting the second seed
	double wRand, cost;
	int lCount, rCount;	
	double *muL, *muR;				/* the two seeds*/
	double *dL,*dR;	    		// arrays that store divergences to the two seeds

	// memory allocation
	muL = calloc(d,sizeof(double));	/* allocate memory for the two seeds  */
	muR = calloc(d,sizeof(double));	/*						*/
  	dL = calloc(n,sizeof(double));		/* allocate memory for divergence arrays*/
	dR = calloc(n,sizeof(double));		/*							*/
	cumSum = calloc(n,sizeof(double));
  
	/* inizialize the first seed at random */
	lp = genRand(n); 					// generate a random integer between 0 and n-1
	for(i=0;i<d;i++)
		muL[i] = data[inds[lp]][i]; 	// computes coordinates of the first seed 
    
	// compute distances of each point to the first seed
	for(i=0;i<n;i++)
		dL[i] = divergence(data[inds[i]],muL,d);
	// compute the cdf of the distances
	cost = 0.0;	// overall distance of cluster points to the centroid: measures the quality of the clustering with respect to data dispersion around the seeds
	cumSum[0] = dL[0];
	for( i=1;i<n;i++ )
		cumSum[i] = dL[i] + cumSum[i-1];
	
	rp = lp;
	while(lp == rp){
		/* choose the second seed based on the cdf */
		wRand = unifRand( cumSum[n-1] );
		i = 0;
		while ( cumSum[i]<wRand && i<n )
			i++;
		rp = i;
	}
	for( i=0;i<d;i++ )
		muR[i] = data[inds[rp]][i]; 
	
	// compute distances of all the points to the second seed
	for(i=0;i<n;i++)
		dR[i] = divergence(data[inds[i]],muR,d);
	
	// assign each point to one subset based on its distance to the two seeds 
    lCount = 0;
    rCount = 0;
    for( i=0;i<n;i++ ){
		if(dL[i] < dR[i]){
			indsL[lCount++] = inds[i];
			cost += dL[i];
		}
		else{
			indsR[rCount++] = inds[i];
			cost += dR[i];
		}
    }
	  
	(*lLength) = lCount;
	(*rLength) = rCount;
	    
	free(muL);
	free(muR);
	free(dL);
	free(dR);
	free(cumSum);
	
}

/* update the following function*/
void deleteTree(TREENODE* root){
  /* update to use with list of children*/
	if( !( root->isLeaf ) ){
		while( root->children!=NULL ){
			deleteTree( root->children->treeNodePt );
			treeNodeListFirstElemDelete( &(root->children) );
		}
	}
	free(root->mu);
	free(root->gradMu);
	free(root->inds);
	free(root);
}

/* generates a random integer in [0,n-1] */
/* note: assumes that rand has already been seeded */
int genRand(int n){
	unsigned int val;
	/* errno_t err;
	err = rand_s( &val );
	if (err != 0){
            printf_s("The rand_s function failed!\n");
        }
	return (unsigned int) ( (double)val/(double)UINT_MAX*(double)n );*/
	while(n <= ( val = rand() / ( ( ( unsigned ) RAND_MAX + 1 )/n ) ) );
	return val;
}

/* generate a uniformly distributed random double in [0,x) */
/* always assuume that rand has been already seeded */
double unifRand(double x){
	/*unsigned int val;
	errno_t err;
	err = rand_s( &val );
	if (err != 0){
            printf_s("The rand_s function failed!\n");
        }
	return (double) val / (double) UINT_MAX * x;*/

	double val = ( ( double )rand() / ( ( double )( RAND_MAX ) + ( double )( 1 ) ) ) * x;
	return val;
}

/* saves a bbtree++ in a text file*/
void writeTree(TREENODE* root,int d, char* file){
    FILE *fp = fopen( file,"w" );		/* opens the file in only write mode*/
    if( fp==NULL ){
        fprintf(stderr,"unable to open output file \n");
        return;
    }
    fprintf(fp,"%d \n",d);	/* writes the dimension of data*/
    writeNode(root,d,fp);
    fclose(fp);
}

/* writes fields of a single node structure*/
void writeNode(TREENODE* node, int d, FILE* fp){
    int i;
    TREENODE_LIST_PT list;		/* list is used to point to the elements of the children list*/
    treeNodeStartList(&list);	/* it is inizialized to the NULL pointer*/
    
    /* writes the number of points, the radius, isLeaf, the depth of the actual node*/
    fprintf( fp,"%d %f %d %d %d\n", node->n, node->R, node->isLeaf, node->depth, node->nChildren );
    for (i=0;i<d;i++)
    fprintf( fp,"%f ",node->mu[i] );	/* writes the centroid*/
    fprintf(fp,"\n");
    for (i=0;i<d;i++)
    fprintf( fp,"%f ",node->gradMu[i] );	/* writes the gradient at the centroid*/
    fprintf(fp,"\n");
    if(!(node->isLeaf)){
        list = node->children;
        while( list!= NULL ){
            /*NOT : buraya ekle childrenlar rahat gozuksun diye -- sonra sil yoksa readData yaparken sorun olur */
            //fprintf(fp,"*************kontrol****************\n");
            writeNode( list->treeNodePt,d,fp );
            list = list->next;
        }
    }
    for (i=0;i<node->n;i++){
        fprintf(fp,"%d ",node->inds[i]);	/* writes indices of data stored in the actual node*/
    }
    fprintf(fp,"\n");
}


/* read a bbtree++ from a file*/
TREENODE* readTree(char*file){
    int d;
    
    FILE *fp = fopen( file,"r" );		/* opens the file in only read mode*/
    if(fp==NULL){
        fprintf(stderr,"unable to open input file... exiting \n");
        exit(1);
    }
    fscanf(fp,"%d \n",&d);		/* read the dimension of data*/
    return readNode(d,fp);
    fclose(fp);
}

TREENODE* readNode(int d, FILE* fp){
    //declarations
    int i;
    float temp;
    TREENODE_QUEUE queue;			/* queue is used to enqueue the childrens of the actual node (the first read must be the first written) */
    TREENODE *nodept = NULL;		/* it is used as an alias to push a node pointer from the queue to the children list*/
    TREENODE* node;
    
    // allocations
    treeNodeStartQueue(&queue);		/* it is inizialized to the empty queue*/
    node = (TREENODE*)calloc( 1,sizeof(TREENODE) );		/* allocates memory for the node*/
    
    /* reads number of points, radius, isLeaf, depth and number of children of the actual node*/
    fscanf( fp,"%d %f %d %d %d\n", &(node->n), &temp, &(node->isLeaf), &(node->depth), &(node->nChildren) );
    node->mu = calloc(d,sizeof(double));
    node->gradMu = calloc(d,sizeof(double));
    node->R = (double)temp;
    
    for(i=0;i<d;i++){
        fscanf( fp,"%f ",&temp );			/* reads the center*/
        node->mu[i]=(double)temp;
    }
    fscanf(fp,"\n");
    for(i=0;i<d;i++){
        fscanf(fp,"%f ",&temp);				/* reads the gradient at the center*/
        node->gradMu[i]=(double)temp;
    }
    fscanf(fp,"\n");
    
    treeNodeStartList( &(node->children) );		/* the list of children is inizialized to the empty list also for leaf nodes*/
    if (!(node->isLeaf)){
        for( i=0;i<node->nChildren;i++ )
        treeNodeEnQueue( &queue,readNode(d,fp) );	/* adds the i-th child at the end of the queue*/
        while( queue.first!=NULL ){
            treeNodeDeQueue( &queue,&nodept );			/* retrieves the first element of the queue*/
            treeNodeListPush( &(node->children),nodept );	/*and adds it at the head of the children list*/
        }
    }
    node->inds = calloc(node->n,sizeof(int));
    for(i=0;i< node->n;i++){
        fscanf( fp,"%d ",&(node->inds[i]) );	/* reads the indices of stored points*/
    }
    fscanf(fp,"\n");
    return node;
}



#endif

