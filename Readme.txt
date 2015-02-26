**************************************************************************************************
*INFLEX software based on the paper: Online Topic-aware Influence Maximization Queries. 
*Contact author: aslayci@acm.org
*Redistribution and use in source and binary forms, with or without modifications, are permitted for 
*academic purposes, provided that the proper acknowledgements are done. 
**************************************************************************************************

=== Prerequisites ===
In order to use INFLEX for answering topic-aware influence maximization queries, first a database of topic distributions that will be used as index points should be created, and the seed sets that will be used as index lists should be precomputed by a standard influence maximization algorithm (i.e., CELF/CELF++). Quality of the results returned by INFLEX heavily depends on the selection of index points - better the simplex coverage, closer the  nearest neighbors retrieved, more similar the seed sets. (section 3.1. contains information)


=== Basic Compilation Info === 
(1) INFLEX tree construction: constructs bb-tree for a given database of topic distributions
To compile: make -f Makefile_BBTreeBuild

(2) INFLEX query processing: returns a seed set to a given topic-aware influence maximization query 
To compile: make -f Makefile_QueryProcess


=== Usage Info ===
(1) INFLEX tree construction:
- To run: ./INFLEX_TreeBuilding -f dataFile(file containing index points) -n numPoints(nr of index points) -d dimPoints(nr of topics)
- Example: ./INFLEX_TreeBuilding -f indexPoints.txt -n 30 -d 10
- Saves the tree in BBTreeIndex.txt 

(2) INFLEX query processing
- To run: ./INFLEX_QueryProcessing -f dataFile(file containing index points) -t treeFile(file containing the tree created in the first step) -q queryFile(file containing the query points) -n numPoints(nr of index points) -m numQueries(nr of query points) -d dim(nr of topics) -k seedSetSize(seed set size queried) -s indexListSize(nr of seeds saved in any index list) [-x rangeMaxLimit(optional upper bound on number of nearest neighbors to select)]

- Example: ./INFLEX_QueryProcessing -f indexPoints.txt -t BBTreeIndex.txt -q queryPoints.txt -n 30 -m 10 -d 10 -k 20 -s 50

- Default folder for reading index lists and for writing the query result lists are defined in utils.c via the macro definitions like below, any change to the name of the input index lists or the number of columns in the index lists should be done here: 
#define indexListLocation "./lists/index/iList_"
#define resultListLocation "./lists/result/resultSeeds_"
#define nrCols 10

