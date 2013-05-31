// Rglrz.cpp: implementation of the CRglrz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "Rglrz.h"
#include <math.h>
#include <malloc.h>
#include <stdlib.h> 
#include <fstream>
#include "fourier.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795
#define PI2 6.283185307179586476925286766559

double* dVector(long n)
{
	double *v;

	v=(double *) malloc( (n)*sizeof(double) );
	return v;
}

template <class T> void Swap( T& a, T& b )
{
    T c( a );
    a = b; b = c;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//********** derivatives ************************************************************************************
void DerivX(double *rdata, double *idata, int n, double delta, double z=0)
{
	// derivative x
	double k, temp;							// wawenumber
	delta /= PI2;

	// zero wawenumber component
	rdata[0] = 0.0;
	idata[0] = 0.0;
	// nonzero wawenumber component
	int i = 1;
	for( ; i < n/2; i++) {
		k = (double)i/(n*delta);
		// positive frequences
		temp = rdata[i];
		rdata[i] = -k*idata[i];		// real
		idata[i] = k*temp;			// imaginary

		// negative frequences
		k = -k;
		temp = rdata[n-i];
		rdata[n-i] = -k*idata[n-i];	// real
		idata[n-i] = k*temp;		// imaginary
	}
	// nyquist
	temp = rdata[n/2];
	k = 1.0/(2*delta);
	rdata[n/2] = -k*idata[i];		// real
	idata[n/2] = k*temp;			// imaginary
}

void DerivX(double *rdata, double *idata, double *rout, double *iout, int n, double delta, double a, double z)
{
	// derivative x
	double k, thk;							// wawenumber
	delta /= PI2;

	// zero wawenumber component
	rout[0] = 0;			// real
	iout[0] = 0;			// imaginary
	// nonzero wawenumber component
	for( int i = 1; i < n/2; i++) {
		k = (double)i/(n*delta);
		thk = tanh(k);
		k = k/(1 + a*k*k*thk*thk);		// regularized
		// positive frequences
		rout[i] = -k*idata[i];			// real
		iout[i] = k*rdata[i];			// imaginary

		// negative frequences
		k = -k;
		rout[n-i] = -k*idata[n-i];		// real
		iout[n-i] = k*rdata[n-i];		// imaginary
	}
	// nyquist
	k = 1.0/(2*delta);
	thk = tanh(k);
	k = k/(1 + a*k*k*thk*thk);		// regularized
	rout[n/2] = -k*idata[n/2];		// real
	iout[n/2] = k*rdata[n/2];		// imaginary
}

void DerivZ(double *rdata, double *idata, int n, double delta, double z)
{
	// derivative z
	double k;							// wawenumber
	delta /= PI2;

	// zero wawenumber component
	idata[0] = 0.0;
	rdata[0] = 0.0;
	// nonzero wawenumber component
	for( int i = 1; i < n/2; i++) {
		k = (double)i/(n*delta);
		// positive frequences
		rdata[i] *= k;		// real
		idata[i] *= k;		// imaginary

		// negative frequences
		k -=k;
		rdata[n-i] *= k;	// real
		idata[n-i] *= k;	// imaginary
	}
	// nyquist
	k = 1.0/(2*delta);
	rdata[n/2] *= k;	// real
	idata[n/2] *= k;	// imaginary
}

void DerivZ(double *rdata, double *idata, double *rout, double *iout, int n, double delta, double a, double z)
{
	// derivative z
	double k, thk;							// wawenumber
	delta /= PI2;

	// zero wawenumber component
	//rout[0] = rdata[0];			// real
	//iout[0] = idata[0];			// imaginary
	rout[0] = 0;			// real
	iout[0] = 0;			// imaginary
	// nonzero wawenumber component
	for( int i = 1; i < n/2; i++) {
		k = (double)i/(n*delta);
		thk = tanh(k);
		k = k/(1 + a*k*k*thk*thk);		// regularized
		// positive frequences
		rout[i] = k*rdata[i];		// real
		iout[i] = k*idata[i];		// imaginary

		// negative frequences
		rout[n-i] = k*rdata[n-i];	// real
		iout[n-i] = k*idata[n-i];	// imaginary
	}
	// nyquist
	k = 1.0/(2*delta);
	k = k/(1 + a*k*k*thk*thk);			// regularized
	rout[n/2] = k*rdata[n/2];		// real
	iout[n/2] = k*idata[n/2];		// imaginary
}

void FldCont(double *rdata, double *idata, int n, double delta, double z)
{
	// downward field continuation
	double k;							// wawenumber
	delta /= PI2;

	// zero wawenumber component
	idata[0] = 0.0;
	rdata[0] = 0.0;
	// nonzero wawenumber component
	for( int i = 1; i < n/2; i++) {
		k = (double)i/(n*delta);
		k = exp(fabs(k*z));
		// positive frequences
		rdata[i] *= k;		// real
		idata[i] *= k;		// imaginary

		// negative frequences
		rdata[n-i] *= k;	// real
		idata[n-i] *= k;	// imaginary
	}
	// nyquist
	k = 1.0/(2*delta);		// nyquist critical wawenumber (frequency)
	k = exp(fabs(k*z));
	rdata[n/2] *= k;	// real
	idata[n/2] *= k;	// imaginary
}

void FldCont(double *rdata, double *idata, double *rout, double *iout, int n, double delta, double a, double z)
{
	// downward field continuation
	double k;							// wawenumber
	delta /= PI2;

	// zero wawenumber component
	rout[0] = 0;			// real
	iout[0] = 0;			// imaginary
	// rout[0] = idata[0];			// real
	// iout[0] = rdata[0];			// imaginary
	// nonzero wawenumber component
	for( int i = 1; i < n/2; i++) {
		if( i <= n/2)
			k = (double)i/(n*delta);
		k = exp(fabs(k*z)) / ( 1+a*k*k*exp(fabs(k*z)) );		// regularized
		// positive frequences
		rout[i] = k*rdata[i];		// real
		iout[i] = k*idata[i];		// imaginary

		// negative frequences
		rout[n-i] = k*rdata[n-i];	// real
		iout[n-i] = k*idata[n-i];	// imaginary
	}
	// nyquist
	k = 1.0/(2*delta);		// nyquist critical wawenumber (frequency)
	k = exp(fabs(k*z)) / ( 1+a*k*k*exp(fabs(k*z)) );		// regularized		
	rout[n/2] = k*rdata[n/2];		// real
	iout[n/2] = k*idata[n/2];		// imaginary
}


//********** derivatives ************************************************************************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRglrz::CRglrz()
{
	//m_data = NULL;
	//m_reg = NULL;
	//m_noreg = NULL;
	//m_bInit = FALSE;
}

CRglrz::~CRglrz()
{
	//if(m_bInit) {
	//	free( m_data);
	//	free( m_reg);
	//	free( m_noreg);
	//}
}



BOOL CRglrz::SetData(double *pData, int n, double delta)
{
	//if(m_bInit) {
	//	free( m_data);
	//	free( m_reg);
	//	free( m_noreg);
	//}
	m_dDelta = delta;
	int i;	
	i = 2;
	while(n > pow(2.0, i)) {
		i++;
	}
	m_nData = (long) (pow(2.0, i));		// m-> power of 2, number of m_data points for FFT

	// allocate data and result arrays
	//m_data	= dVector(m_nData);
	//m_reg	= dVector(m_nData);
	//m_noreg = dVector(m_nData);
	m_data.SetSize( m_nData );
	m_reg.SetSize( m_nData );
	m_noreg.SetSize( m_nData );
	for(i = 0; i < m_nData; i++) {
		if(i < n)
			m_data[i] = pData[i];
		else
			m_data[i] = 0.0;
	}

	return TRUE;
}

void CRglrz::Compute(int tag, double z)
{
	void (*pDrvFnc)(double *rdata, double *idata, int n, double delta, double z);

	switch(tag) {
	case DRV_X:
		pDrvFnc = DerivX;
		break;
	case DRV_Z:
		pDrvFnc = DerivZ;
		break;
	case FLDCONT:
		pDrvFnc = FldCont;
		break;
	}


	double *rdata, *idata, *rout, *iout;
	int n = m_nData;
	int i;

	idata = dVector(n);
	rdata = dVector(n);
	iout = dVector(n);
	rout = dVector(n);
	for(i=0; i < n; i++) {
		rdata[i] = m_data[i];
		idata[i] = 0;
	}

	fft_double(m_nData, FFT_FRW, rdata, idata, rout, iout);
	pDrvFnc(rout, iout, m_nData, m_dDelta, z);
	fft_double(m_nData, FFT_INV, rout, iout, m_noreg.GetData(), idata);

	free( rdata );
	free( idata );
	free( iout );
	free( rout );
}

void CRglrz::Compute_Reg(int tag, double z)
{
	void (*pDrvFnc)(double *rdata, double *idata, double *rout, double *iout, int n, double delta, double a, double z);

	switch(tag) {
	case DRV_X:
		pDrvFnc = DerivX;
		break;
	case DRV_Z:
		pDrvFnc = DerivZ;
		break;
	case FLDCONT:
		pDrvFnc = FldCont;
		break;
	}

	double *rdata, *idata, *rft, *ift, *ptmp, *iprv, *rprv, *rout, *iout;
	int n = m_nData;
	int i;

	idata = dVector(n);
	rdata = dVector(n);
	ift = dVector(n);
	rft = dVector(n);
	iout = dVector(n);
	rout = dVector(n);
	iprv = dVector(n);
	rprv = dVector(n);
	ptmp = dVector(n);
	for(i=0; i < n; i++) {
		rdata[i] = m_data[i];
		idata[i] = 0;
	}

	double a = m_rpMin;
	fft_double(m_nData, FFT_FRW, rdata, idata, rft, ift);
	pDrvFnc(rft, ift, rout, iout, m_nData, m_dDelta, a, z);
	fft_double(m_nData, FFT_INV, rout, iout, rdata, idata);

	m_norm.RemoveAll();
	a *= m_rpQ;
	while(a < m_rpMax) {
		// change pointers
		Swap(rout, rprv);
		Swap(iout, iprv);
		pDrvFnc(rft, ift, rout, iout, m_nData, m_dDelta, a, z);
		fft_double(m_nData, FFT_INV, rout, iout, rdata, idata);
		double norm = NormC(rout, rprv, m_nData);
		m_norm.Add( norm );
		//TRACE( " a= %f norm =%f \n", (float) a, (float) norm);
		a *= m_rpQ;
	}

	// find regularization parameter and conpute regularization
	a = FindRegPar(0);
	pDrvFnc(rft, ift, rout, iout, m_nData, m_dDelta, a, z);
	Spectrum(rout, iout, n);
	fft_double(m_nData, FFT_INV, rout, iout, m_reg.GetData(), idata);


	free( rdata );
	free( idata );
	free( ift );
	free( rft );
	free( iout );
	free( rout );
	free( iprv );
	free( rprv );
	free( ptmp );
}

void CRglrz::Compute_Reg(int tag, double regParam, double z)
{
	void (*pDrvFnc)(double *rdata, double *idata, double *rout, double *iout, int n, double delta, double a, double z);

	switch(tag) {
	case DRV_X:
		pDrvFnc = DerivX;
		break;
	case DRV_Z:
		pDrvFnc = DerivZ;
		break;
	case FLDCONT:
		pDrvFnc = FldCont;
		break;
	}

	double *rdata, *idata, *rft, *ift, *rout, *iout;
	int n = m_nData;
	int i;
	m_dRegPar = regParam;

	idata = dVector(n);
	rdata = dVector(n);
	ift = dVector(n);
	rft = dVector(n);
	iout = dVector(n);
	rout = dVector(n);
	for(i=0; i < n; i++) {
		rdata[i] = m_data[i];
		idata[i] = 0;
	}

	fft_double(m_nData, FFT_FRW, rdata, idata, rft, ift);
	pDrvFnc(rft, ift, rout, iout, m_nData, m_dDelta, regParam, z);
	Spectrum(rout, iout, n);
	fft_double(m_nData, FFT_INV, rout, iout, m_reg.GetData(), idata);


	free( rdata );
	free( idata );
	free( ift );
	free( rft );
	free( iout );
	free( rout );
}

double CRglrz::NormC(double *pData1, double *pData2, int n)
{

	double norm;

	norm = fabs(pData1[0]-pData2[0]);
	for(int i=1; i < n; i++) {		// exclude fist and last values 
		norm  = max( norm, fabs(pData1[i]-pData2[i]) );
		//norm  += fabs(pData1[i]-pData2[i]) ;
	}

	return norm;

}

double CRglrz::NormL(double *pData, double *pData2, int n)
{
	double norm = 0.0;

	for(int j=1; j < n-2; j +=2) {		// exclude fist and last values 
		norm  += pow(pData[j]-pData[j+2], 2);
	}

	return sqrt( norm );
}


void CRglrz::GetRglrzNorm(DArray &ar)
{
	ar.RemoveAll();
	for(int i=0; i < m_norm.GetSize(); i++) {
		ar.Add(m_norm[i]);
	}
}

double CRglrz::FindRegPar(int nFlag)
{
	int startIndex, incr;
	switch(nFlag) {
	case 0: 
		startIndex = 1;
		incr = 1;
		break;
	case 1: 
		startIndex = m_nRegPar;
		incr = 1;
		break;
	case -1: 
		startIndex = m_nRegPar-2;
		incr = -1;
		break;
	}
	int n = (int)m_norm.GetSize();
	BOOL bStop = FALSE;
	double rp;

	int i = startIndex+1;
	while( !bStop && i<(n-1) && i>1 ) {
		if( m_norm[i-1] > m_norm[i] && m_norm[i] < m_norm[i+1] ) {
			bStop = TRUE;
			rp = m_rpMin*pow(m_rpQ, i-1);
			m_dRegPar = rp;
			m_nRegPar = i;
		}
		i += incr;
	}

	if(!bStop) {
		AfxMessageBox("Regularization parameter can not be find.", MB_ICONINFORMATION);
		rp = 1.0;
		m_dRegPar = rp;
		m_nRegPar = 1;
	}
	return rp;
}

void CRglrz::SetRegPar(double min, double max, double q)
{
	m_rpMin = min;
	m_rpMax = max;
	m_rpQ   = q;
}

void CRglrz::Spectrum(double *rdata, double *idata, int n)
{
	double nn = 1.0/(n*n), s;
	// zero wawenumber component
	m_spec.RemoveAll();
	// nonzero wawenumber component
	for(int i = 1; i <= n/2; i++) {
		s = nn*(sqrt( pow(rdata[i], 2) + pow(idata[i],2) ) + sqrt( pow(rdata[n-i], 2) + pow(idata[n-i],2) ) );
		m_spec.Add(s);
	}
}

void CRglrz::GetSpec(DArray &ar)
{
	ar.RemoveAll();
	for(int i=0; i < m_spec.GetSize(); i++) {
		ar.Add(m_spec[i]);
	}
}
