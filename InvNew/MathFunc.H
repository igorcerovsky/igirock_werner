//#include "Method.h"
#include  <stdio.h>
#include  <stdlib.h>
#include <math.h>

#define NR_END 1
#define FREE_ARG char*

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))


void nrerror(char error_text[]);

double* dVector(long nl, long nh);

void Free_dVector(double *v, long nl, long nh);

double **dMatrix(long nrl, long nrh, long ncl, long nch);

void Free_dMatrix(double **m, long nrl, long nrh, long ncl, long nch);

double Pythag(double a, double b);

// -------------------------- SVD ---------------------------------------------------------------------------------------------------
void SvdCmp(double **a, int m, int n, double w[], double **v);
	
void SvdBkSb(double **u, double w[], double **v, int m, int n, double b[], double x[]);

void SvdVar(double **v, int ma, double w[], double **cvm);

// -------------------------- end SVD ------------------------------------------------------------------------------------------------


// -------------------------- FFT 1D -------------------------------------------------------------------------------------------------
void FFT1D(double data[], unsigned long nn, int isign);
// -------------------------- end FFT 1D ---------------------------------------------------------------------------------------------
