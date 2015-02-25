#ifndef DIVERGENCE_H
#define DIVERGENCE_H

#define ERRORTOL 1e-10
#define EPSILON pow(2.0, -52.0)
#define CLOSEENOUGH 1e-3

double divergence( double*,double*,int );
void grad( double*,double*,int );
void invGrad( double*,double*,int );
void divMat( double**,double**,double**,int,int,int );
void hessian(double*,double*,int );
void findCentroid( double*,double**,int*,int,int );

double multinomialF( double*,int );
void multinomialGradF( double*,double*,int );
double multinomialDivergence( double*,double*,int );
void multinomialInvGradF( double*,double*,int );
void rightCentroid( double*,double**,int,int );
void multinomialLeftCentroid( double*,double**,int,int );
double approximateDiv( double*,double*,int );
void multinomialGeodesicPoint( double*,double*,double*,int,double );
void geodesicPoint( double*,double*,double*,int, double );
void mean(double*,double**,int*,int,int);
//void variance()


/* C: to handle zero probabilities */
void handleZeroKL(double*, int); //if the distribution contains zero values, add epsilon and normalize the distribution vector

#endif
