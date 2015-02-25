#ifndef NEW_TYPES_H
#define NEW_TYPES_H

typedef struct listNode *TREENODE_LIST_PT;  // the type TREENODE_LIST_PT is a pointer to a list

typedef struct treenode{
  double* mu;			// center
  double* gradMu;		// gradient of the generator function evaluated at the center
  double divQToMu; 		//used idfn search procedure
  double R;				// radius of the ball
  int* inds; 			/* indices of the data stored in this node */
  int n;				// number of data stored in this node
  int isLeaf;			// 1 if this node is a leaf, 0 otherwise
  int depth;			// depth of this node (is 0 for a subtree containing only one node, so that the root has depth = 0)
  int nChildren;		// number of child nodes of this node (empty if the actual node is a leaf)
  TREENODE_LIST_PT children;	//list of the child nodes
  struct treenode *leftChild;	// pointers to children
  struct treenode *rightChild;	// *	*	*	*
  //double variance; //leaf node larda kullanmak icin  
} TREENODE;

/* defines data structure for the list of child nodes*/
typedef struct listNode{
	TREENODE *treeNodePt; 	//eventually define a type for *treenode
	struct listNode *next;	
} LIST_NODE;

/* defines data structure for the queue of child nodes*/
typedef struct queueNode{
	TREENODE_LIST_PT first;
	TREENODE_LIST_PT last;
} TREENODE_QUEUE;

/* defines data structure for the list of clusters of a pointset*/
typedef struct cluster{
	int *clustPtsInds; 	//array of indexes of points stored in this cluster
	int nClustPts;			// number of points stored in this cluster
	struct cluster *next;	//link to the following cluster
} CLUSTER;			

typedef CLUSTER *CLUSTER_LIST_PT;	// the type CLUSTER_LIST_PT is a pointer to the head of a list

/* defines data structure for the queue of clusters of a pointset*/
typedef struct clustQueue{
	CLUSTER_LIST_PT first;
	CLUSTER_LIST_PT last;
} CLUSTER_QUEUE;


#endif
