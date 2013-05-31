// Rglrz.h: interface for the CRglrz class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <afxtempl.h>

typedef enum{
	FFT_FRW = 0,
	FFT_INV = 1
} FFTTRANSF;

typedef enum{
	DRV_Z = 0,
	DRV_X = 1,
	FLDCONT = 2
} DERIVS;

typedef enum{
	RP_NEW = 0,
	RP_NEXT = 1,
	RP_PREV = -1
} RPFIND;

typedef CArray<double, double> DArray;

class CRglrz : public CObject  
{
public:
	CRglrz();
	virtual ~CRglrz();

public:
	void GetSpec(DArray &ar);
	int GetRegParN()		{return m_nRegPar;}
	double GetRegPar()		{return m_dRegPar;}
	void SetRegPar(double min, double max, double q);
	double FindRegPar(int nFlag = 0);
	void GetRglrzNorm(DArray &ar);
	void Compute(int tag, double z);
	void Compute_Reg(int tag, double regParam, double z);
	void Compute_Reg(int tag, double z);
	BOOL SetData(double *pData, int n, double delta);

public:
	BOOL	m_bInit;
	int		m_nRegPar;			// regularization parameter index in array
	double	m_dRegPar;			// regularization parameter
	double	m_dRegParStart;		// start
	double	m_dRegParEnd;		// ens
	double	m_dRegParStep;		// step

	DArray	m_data;			// data array
	DArray  m_reg;				// result array
	DArray  m_noreg;			// result array
	double	m_dDelta;			// data spacing
	long	m_nData;			// number of data
	double	m_dNorm;			// norm

	double m_dDatMin;			// data minimum
	double m_dDatMax;			// data maximum

	double m_rpMax, m_rpMin, m_rpQ;						// regularization parameter

	DArray m_norm;
	DArray m_spec;

protected:
	void Spectrum(double *rdata, double *idata, int n);
	double NormC(double *pData1, double *pData2, int n);
	double NormL(double *pData, double *pData2, int n);
};

double* dVector(long n);
double* dVector(long nl, long nh);
void DerivX(double *pData, int n, double delta, double a, double z=0.0);
void DerivX(double *pData, int n, double delta, double z=0.0);
void DerivZ(double *pData, int n, double delta, double a, double z=0.0);
void DerivZ(double *pData, int n, double delta, double z=0.0);
void FldCont(double *pData, int n, double delta, double a, double z=0.0);
void FldCont(double *pData, int n, double delta, double z=0.0);

