#ifndef DIVERGENCE_C
#define DIVERGENCE_C
#define KLDIVSpecial

#define BOOL int

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include "divergence.h"

/*  kl-divergence specialized to handle zero probs */

#ifdef KLDIVSpecial


/* computes the Kullback-Leibler divergence: KL(x||y) */
double divergence(double *x, double *y, int d) {
  int i;
  double ans=0;

  /*eger basta datayi alirken handle edersen gene de burada da handle etmen lazim geodesic walkda cunku datada olmayan pointlerle deneniyor*/
   handleZeroKL(x,d);
   handleZeroKL(y,d);

  for(i = 0; i < d; i++)
    ans += x[i] * log(x[i]/y[i]);
  
  return ans;	    
}

/*computes the gradient of the KL generator function at point x :	grad F_KL (x)*/
void grad(double *gf, double *x, int d){
	int i;
	//handle zero
	handleZeroKL(x, d);
	for(i=0;i<d;i++)
		gf[i]=log(x[i]);
} 

/* computes the inverse gradient of the KL generator function at point x:	inv grad F_KL (x)*/
void invGrad(double *gfs,double *x, int d){
	int i;
	//handle zero
	handleZeroKL(x, d);
	for(i=0;i<d;i++)
		gfs[i] = exp(x[i]);
}

/* compute the matrix of KL divergence between each point of the set x and each point of the set y:		M(i,j) = KL( x(i)) || y(j) )*/
void divMat(double **dmat, double **x, double **y, int n, int m, int d){
	int i,j,k;
	for (i=0;i<n;i++){
		for(j=0;j<m;j++){
			dmat[i][j] = 0.0;
			for(k=0;k<d;k++){
				if( x[i][k] > ERRORTOL )
					dmat[i][j] += x[i][k]*log( x[i][k]/y[j][k] ) + y[j][k] - x[i][k];
			}
		}
	}
}

/* compute the hessian of the KL generator function at point x*/
void hessian(double *hf, double *x, int d){
	int i;
	for( i=0;i<d;i++ )
		hf[i] = 1.0/x[i];
}

/* compute the centroid with respect to KL divergence (usual mean)*/
void findCentroid( double *centroid, double **data, int* inds, int n, int d ){
	int i;
	double **actualData;	
	actualData = calloc(n,sizeof(double*));

	for( i=0;i<n;i++ )
		actualData[i] = data[inds[i]];
	rightCentroid( centroid,actualData,n,d );
	free(actualData);
}

#endif

/* **************************************** */
/* GENERALIZED KL-DIVERGENCE */
/* **************************************** */

#ifdef KLDIV

/* computes the Kullback-Leibler divergence:	KL(x||q)*/
double divergence(double *x, double *y, int d){
	int i;
	double ans=0;
	for(i=0;i<d;i++){
	    if(x[i] > ERRORTOL )
			ans+= x[i]*log(x[i]/y[i])+y[i]-x[i];
		}
	//printf("compute the KL divergence\n");
	return ans;
}

/*computes the gradient of the KL generator function at point x :	grad F_KL (x)*/
void grad(double *gf, double *x, int d){
	int i;
	for(i=0;i<d;i++)
		gf[i]=log(x[i]);
} 

/* computes the inverse gradient of the KL generator function at point x:	inv grad F_KL (x)*/
void invGrad(double *gfs,double *x, int d){
	int i;
	for(i=0;i<d;i++)
		gfs[i] = exp(x[i]);
}

/* compute the matrix of KL divergence between each point of the set x and each point of the set y:		M(i,j) = KL( x(i)) || y(j) )*/
void divMat(double **dmat, double **x, double **y, int n, int m, int d){
	int i,j,k;
	for (i=0;i<n;i++){
		for(j=0;j<m;j++){
			dmat[i][j] = 0.0;
			for(k=0;k<d;k++){
				if( x[i][k] > ERRORTOL )
					dmat[i][j] += x[i][k]*log( x[i][k]/y[j][k] ) + y[j][k] - x[i][k];
			}
		}
	}
}

/* compute the hessian of the KL generator function at point x*/
void hessian(double *hf, double *x, int d){
	int i;
	for( i=0;i<d;i++ )
		hf[i] = 1.0/x[i];
}

/* compute the centroid with respect to KL divergence (usual mean)*/
void findCentroid( double *centroid, double **data, int* inds, int n, int d ){
	int i;
	double **actualData;	
	actualData = calloc(n,sizeof(double*));

	for( i=0;i<n;i++ )
		actualData[i] = data[inds[i]];
	rightCentroid( centroid,actualData,n,d );
	free(actualData);
}

#endif

/* **************************************** */
/* SYMMETRIZED KL-DIVERGENCE */
/* **************************************** */

#ifdef JDIV
#include "symmetrizedBregmanCentroid.h"

/* computes the Symmetrized Kullback-Leibler divergence (J-divergence):	SKL(x||q)*/
double divergence(double *x, double *y, int d){
	int i;
	double ans=0;
	for(i=0;i<d;i++){
	    if(x[i] > ERRORTOL )
			ans+= (x[i]-y[i])*log(x[i]/y[i]);
		}
	//printf("compute the KL divergence\n");
	return 0.5*ans;
}

/*computes the gradient of the KL generator function at point x :	grad F_KL (x)*/
void grad(double *gf, double *x, int d){
	int i;
	for(i=0;i<d;i++)
		gf[i]=log(x[i]);
} 

/* computes the inverse gradient of the KL generator function at point x:	inv grad F_KL (x)*/
void invGrad(double *gfs,double *x, int d){
	int i;
	for(i=0;i<d;i++)
		gfs[i] = exp(x[i]);
}

/* compute the hessian of the KL generator function at point x*/
void hessian(double *hf, double *x, int d){
	int i;
	for( i=0;i<d;i++ )
		hf[i] = 1.0/x[i];
}

/* compute the centroid with respect to J-divergence (symmetrized Bregman centroid)*/
void findCentroid( double *centroid, double **data, int* inds, int n, int d ){
	int i;
	double **actualData;	//select data by indices
	actualData = calloc(n,sizeof(double*));

	for( i=0;i<n;i++ )
		actualData[i] = data[inds[i]];
	//printf("actualData[0] = %f\n",actualData[0][0]);
	symmetrizedBregmanCentroidGeodesicWalk( centroid,actualData,n,d );
	//printf("centroid[0] = %f\n",centroid[0]);
	free(actualData);
}

#endif

/* **************************************** */
/* multinomial density functions */
/* **************************************** */ 

double multinomialF( double *theta, int d ){
	// d is the dimension of the point theta
	double sum = 0.0;
	int i;
	for( i=0;i<d;i++ )
		sum += exp( theta[i] );
	
	return log(1.0+sum);
}

void multinomialGradF( double *p, double *theta, int d ){
	double sum = 0.0;
	int i;
	
	for( i=0;i<d;i++ )
		sum = sum + exp( theta[i] );
	for( i=0;i<d;i++ )
		p[i] = exp( theta[i] )/( 1.0+sum );

}

/* d is the effective dimension of parameters of multinomial distribution*/
double multinomialDivergence( double *p, double *q, int d ){
	int i;
	double *pq, *gradQ, r;
	double pScalGradQ = 0.0;
	pq = calloc( d,sizeof(double) );
	gradQ = calloc( d,sizeof(double) );

	multinomialGradF( gradQ,q,d );
	for( i=0;i<d;i++ )
		pq[i] = p[i] - q[i];
	for( i=0;i<d;i++ )
		pScalGradQ += pq[i]*gradQ[i];
	r = multinomialF(p,d) - multinomialF(q,d) - pScalGradQ;
	
	free(pq);
	free(gradQ);
	return r;
}

void multinomialInvGradF( double *p, double *heta, int d ){
	double sum = 0.0;
	int i;
		
	for( i=0;i<d;i++ )
		sum = sum + heta[i];
	for( i=0;i<d;i++ )
		p[i] = log( heta[i]/( 1.0-sum ) );

}

/* compute the right-type sided centroid (usual mean)*/
void rightCentroid( double *mu, double **data, int n, int d ){
	int i,j;
  
	for(i=0;i<d;i++){
		for(j=0;j<n;j++)
			mu[i] += data[j][i];
		mu[i] /= ( (double) n);
	}
}

/* compute the left-type sided centroid (inverse gradient of the gradient mean)*/
void multinomialLeftCentroid( double *mu, double **data, int n, int d ){
	int i,j;
	double *addP, *gradMu;
	addP = calloc( d,sizeof(double) );
	gradMu = calloc( d,sizeof(double) );
	for( j=0;j<d;j++ )
		gradMu[j] = 0.0;
	
	for( i=0;i<n;i++ ){
		multinomialGradF( addP,data[i],d );
		// Centroid: uniform weight vector w (barycenters allow arbitrary unit vectors)
		for( j=0;j<d;j++ )
			gradMu[j] = gradMu[j] + addP[j]/( (double)n );
	}
	multinomialInvGradF( mu,gradMu,d );
	free(gradMu);
	free(addP);
}

/* compute a point lying on the geodesic between p and q*/
/* such that gradF(r) = (1-l)*gradF(p) + l*gradF(q)*/
void multinomialGeodesicPoint( double *r, double *p, double *q, int d, double l ){
	int i;
	double *gradR, *gradP, *gradQ;
	gradR = calloc( d,sizeof(double ));
	gradP = calloc( d,sizeof(double ));
	gradQ = calloc( d,sizeof(double ));
	
	multinomialGradF( gradP,p,d );
	multinomialGradF( gradQ,q,d );
	for( i=0;i<d;i++ )
		gradR[i] = (1.0-l)*gradP[i] + l*gradQ[i];
	multinomialInvGradF( r,gradR,d );
	
	free(gradR);
	free(gradP);
	free(gradQ);
}


/* **************************************** */
/* invariant functions for all divergences */
/* **************************************** */ 

/* approximates the KL divergence by hessian*/
double approximateDiv( double *x, double *y, int d ){
	int i;
	double ans = 0;
	double *hesP;
	hesP = calloc( d,sizeof(double) );
	
	hessian( hesP,x,d );
	for( i=0;i<d;i++ )
		ans += (x[i]-y[i])*hesP[i]*(x[i]-y[i]);
	free(hesP);
	return 0.5*ans;

}

//cayton paperda yazdigi gibi hesapla bunu - theta mu ile (1-theta) q ile
//KL histogram icin normalization dene
void geodesicPoint( double *r, double *p, double *q, int d, double l ){
	int i;
	double *gradR, *gradP, *gradQ, sum;
	gradR = calloc( d,sizeof(double ));
	gradP = calloc( d,sizeof(double ));
	gradQ = calloc( d,sizeof(double ));
	
	grad( gradP,p,d );
	grad( gradQ,q,d );
	
	sum = 0;
	for(i=0; i < d; i++) {		
		gradR[i] = pow(gradP[i], l) + pow(gradQ[i], (1 - l)); //bunu thetali versiyonuna gore degistirdim
		sum += gradR[i];
	}
	for(i=0; i < d; i++)
		gradR[i] /= sum;
		
		//gradR[i] = l * gradP[i] + (1 - l) * gradQ[i]; //bunu thetali versiyonuna gore degistirdim
		//gradR[i] = (1.0-l)*gradP[i] + l*gradQ[i]; --eskisi 
	invGrad( r,gradR,d );
	
	free(gradR);
	free(gradP);
	free(gradQ);
} 
//calisani
// void geodesicPoint( double *r, double *p, double *q, int d, double l ){
// 	int i;
// 	double *gradR, *gradP, *gradQ;
// 	gradR = calloc( d,sizeof(double ));
// 	gradP = calloc( d,sizeof(double ));
// 	gradQ = calloc( d,sizeof(double ));
// 	
// 	grad( gradP,p,d );
// 	grad( gradQ,q,d );
// 	for( i=0;i<d;i++ )
// 		//gradR[i] = exp()		
// 		gradR[i] = l * gradP[i] + (1 - l) * gradQ[i]; //bunu thetali versiyonuna gore degistirdim
// 		//gradR[i] = (1.0-l)*gradP[i] + l*gradQ[i]; --eskisi 
// 	invGrad( r,gradR,d );
// 	
// 	free(gradR);
// 	free(gradP);
// 	free(gradQ);
// } 
/* compute a point lying on the geodesic between p and q*/
/* such that gradF(r) = (1-l)*gradF(p) + l*gradF(q)*/
// void geodesicPoint( double *r, double *p, double *q, int d, double l ){
// 	int i;
// 	double *gradR, *gradP, *gradQ;
// 	gradR = calloc( d,sizeof(double ));
// 	gradP = calloc( d,sizeof(double ));
// 	gradQ = calloc( d,sizeof(double ));
// 	
// 	grad( gradP,p,d );
// 	grad( gradQ,q,d );
// 	for( i=0;i<d;i++ )
// 		gradR[i] = (1.0-l)*gradP[i] + l*gradQ[i];
// 	invGrad( r,gradR,d );
// 	
// 	free(gradR);
// 	free(gradP);
// 	free(gradQ);
// } 
/*void grad(double *gf, double *x, int d){
	int i;
	for(i=0;i<d;i++)
		gf[i]=log(x[i]);
} 
void invGrad(double *gfs,double *x, int d){
	int i;
	for(i=0;i<d;i++)
		gfs[i] = exp(x[i]);
}*/
void mean(double* mu, double **data, int *inds, int n, int d){

	int i,j;

	for(i = 0; i < d; i++){
		for(j = 0;j < n; j++){
			mu[i] += data[inds[j]][i];
		}
		mu[i] /= ( ( double ) n);
	}
}

void handleZeroKL(double *x, int d) {
  
  int i; 
  BOOL containsZero = 0;
  
  double sum = 0;
  
  for(i = 0; i < d; i++) {
	if((x[i] - 0.0) < EPSILON) {
      x[i] = EPSILON;
      containsZero = 1;
    }
    sum += x[i];
  }
    
    if(containsZero) 
      for(i = 0; i < d; i++)
		x[i] /= sum;
} 

// // cide for sample variance calc among nodes
// void variance(double *variance, double **data, int *inds, int n, int d) {
// 	int i, j;
// 	
// 	
// 	
// }

#endif