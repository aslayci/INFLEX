#ifndef ANDERSON_H
#define ANDERSON_H

double errorFunction( double );
double getADCriticalValue( double );
double normalCdfPos( double );
double normalCdf( double );
int compareDoubles( double*,double* );
int andersonTest( double*,int,double );
int vecAndersonTest( double**,int*,int,int,double*,double*,double );
void projectPointSet( double*,double**,int*,int,int,double*,double* );

#endif
