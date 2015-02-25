#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "anderson.h"
#define CRIT_TABLE_LENGTH 15

const double alphaTable[CRIT_TABLE_LENGTH] = { 0.5, 0.6, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 0.975, 0.98, 0.99, 0.995, 0.999, 0.9995, 0.9999};

const double critADTable[CRIT_TABLE_LENGTH] = { 0.3416, 0.3843, 0.4373, 0.4706, 0.5110, 0.5632, 0.6329, 0.7544, 0.8746, 0.9163, 1.0412, 1.1625, 1.4607, 1.5943, 1.8195};

typedef int (*compfn)(const void*, const void*);

double errorFunction(double x){
	//returns the error function as 1 - erfc, where erfc is the complementary error function with fractional error everywhere less than 1.2x10^-7
	double t,z,ans;
	
	z = fabs(x);
	t = 1.0/( 1.0+0.5*z );
	ans = 1.0 - t*exp( -z*z-1.26551223 + t*( 1.00002368 + t*( 0.37409196 + t*( 0.09678418 + 
			t*( -0.18628806 + t*( 0.27886807 + t*( -1.13520398 + t*( 1.48851587 + 
			t*( -0.82215223 + t*0.17087277)	) ) ) ) ) ) ) );
	
	if( z>=0 )
		return ans;
	return (-ans);
}
	
double getADCriticalValue( double alpha ){
	/* returns the critical value for the test of Anderson-Darling, that is the maximum value of the test for the normality hypthesis to be accepted
	with a significance alpha (e.g 0.7544 for alpha=0.95)*/
	int i;
    int bestMatchIdx = 0;
    double bestMatchDist = -1.0;       // difference from alpha to the best value we have 
	double absDist;

    // search for the nearest tabulated value of alpha /
    for( i=0;i<CRIT_TABLE_LENGTH;i++ ){
		absDist = fabs( alphaTable[i]-alpha ); 
		/*printf("absDist %6.3f\n",absDist);
		printf("bestMatchDist %6.3f\n",bestMatchDist);
		printf("bastMatchIdx %d\n",bestMatchIdx);*/
        if( ( bestMatchDist==-1.0) || ( absDist<bestMatchDist ) ){
            /*printf("Entered if\n");*/
			bestMatchIdx = i;
            bestMatchDist = absDist;
            /*printf("bestMatchDist %6.3f\n",bestMatchDist);
            printf("bastMatchIdx %d\n",bestMatchIdx);
            printf("End if\n");*/
        }
    }
    return critADTable[bestMatchIdx];
} 

/* returns the value of the normal cumulative distribution function at x>=0*/
double normalCdfPos(double x){
    double res;
	res = 0.5*(1 + errorFunction(x/sqrt(2.0)));
	if(res==1.0)
		res = 0.999999999999999;
	return res;
}

/* returns the value of the normal cdf at x*/
double normalCdf(double x){
    double res;
	if(x<0.0){
		res = 1.0 - normalCdfPos(-x);
		return res;
    }
	res = normalCdfPos(x);
	return res;
}

/* compare function for performing qsort*/
int compareDoubles( double* A, double *B ){
	if( *A<*B )
	    return -1;
    else if( *A==*B )
        return 0;
    else
        return 1;
}

/*performs the test of Anderson-Darling for a set of scalar data*/
int andersonTest(double *scalarData, int n, double alpha){
	
	double mu, var, std;
    double aSquared, critical;
	double *z, *vals;
	int i;
	z = calloc(n,sizeof(double));
	vals = calloc(n,sizeof(double));
	critical = getADCriticalValue(alpha);	
	
	//compute the mean of the data
	mu = 0.0;
	for(i=0;i<n;i++){
		mu+=scalarData[i];
    }
    mu/=(double)n;  
	
	//compute the variance
	var = 0.0;
	std = 0.0;
	for(i=0;i<n;i++)
		var+=pow((scalarData[i]-mu),2);
    var/=(double)(n-1);	
	std = sqrt(var); 
	
	// normalize scalarData
	for(i=0;i<n;i++)
		vals[i] = (scalarData[i]-mu)/std;
	// sort the vals and compute their normal cdf	
	qsort( vals,n,sizeof(double),(compfn)compareDoubles );
	for(i=0;i<n;i++)
		z[i] = normalCdf(vals[i]);
	
	// apply the test of Anderson-Darling
	aSquared = 0.0;
	for(i=0;i<n;i++){
		aSquared+=( ( 2*i+1 )*( log( z[i] ) + log( 1-z[n-1-i] ) ) );
	}
	aSquared/=-(double)n;
	aSquared-=(double)n;
	/* apply the approximae adjustment*/
	aSquared*=( 1.0 + 4.0/( (double)n ) - 25.0/pow( (double)n,2.0 ) );
	//printf("asquared %f \n", aSquared);
	//printf("asquared %f \n", critical);
	free(z);
	free(vals);
	// verify the result with respect to the critical value
	if(aSquared<critical)	//accept the null hypothesis (no split)
		return 1;
	// else reject the null hypothesis (split the actual ball)
	return 0;
	
}

/* performs the Anderson-Darling test for a subset of n d-dimensional data (subset is selected by indices inds)*/
//anders = vecAndersonTest( data,indsActual,nActual,d,lCenter,rCenter,alpha );	/* performs the Anderson-Darling normality test*/
int vecAndersonTest( double **data, int* inds, int n, int d, double *lCenter, double *rCenter, double alpha ){
	double *scalarData;
	int res;
	scalarData = calloc(n,sizeof(double));
	
	/* projects the points on the line passing through the two centers*/
	projectPointSet( scalarData,data,inds,n,d,lCenter,rCenter );
	/* performs the Anderson test on the scalar representations of data: returns 1 if the null hypothesis is accepted (do not split), 0 otherwise (split) */
	res = andersonTest(scalarData,n,alpha);
	free( scalarData );
	return res;
}

/* projects a set of points onto the line linking the centers*/
void projectPointSet(double *scalarData, double **data, int *inds, int n, int d, double *lCenter, double *rCenter){
	int i,j;
	double *v;
	double l2v=0.0;
	v = calloc(d,sizeof(double));
	
	for(j=0;j<d;j++){
		v[j]=lCenter[j]-rCenter[j];
		l2v+=pow(v[j],2.0);
	}
	// compute the projection of each point
	for(i=0;i<n;i++){
		for(j=0;j<d;j++){
			scalarData[i]+=data[inds[i]][j]*v[j];
		}
		scalarData[i]/=l2v;
	}
	free(v);
	return;
}

