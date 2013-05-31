// CMeth2DLinEuler.cpp: implementation of the CMeth2DLinEuler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "Meth2DLinEuler.h"
#include "fstream.h"
#include "EulDlgExp.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMeth2DLinEuler, C2DMetLinear, 0)

void CMeth2DLinEuler::Serialize(CArchive &ar)
{
	TRACE("Entering CMeth2DLinEuler::Serialize\n");

	C2DMetLinear::Serialize(ar);
	if( ar.IsStoring() ) {
		ar << m_dClusRadius << m_nMaxPtInClus << m_dSiStart << m_dSiEnd << m_dSiStep;
	}
    else {
		ar >> m_dClusRadius >> m_nMaxPtInClus >> m_dSiStart >> m_dSiEnd >> m_dSiStep;
	}
	m_EulResList.Serialize(ar);
}

void CMeth2DLinEuler::BaseFuncs(double fdx, double fdz, double p[], double si, int np)
{
	p[1] = fdx;
    p[2] = fdz;
    for(int i=0; i<=(np-3); i++) {
		p[i+3]=si*pow(1, i);
	}
}
void CMeth2DLinEuler::ComputeEulerRusults(double a[], double w[], int nu, double dChiSquare, 
										 double spos, double epos, int ip, double dSi, int ww, double **cvm)
{
	double wmax, wmin, dCondNum;

	wmax = w[1];
	wmin = w[1];
	for(int i=1; i<=nu; i++) {
		if(w[i]<wmin) wmin=w[i];
		if(w[i]>wmax) wmax=w[i];
	}

	if (wmin != 0)  {
		dCondNum = wmax/wmin;
	}
	else {
		dCondNum = 0;
	}
	if(dCondNum < 1.0e100) {	//!!! optoinal should by added
		if( ((a[2] > m_dMinDepth) && (a[2] < m_dMaxDepth)) && (a[1] > m_prof.min ) && (a[1] < m_prof.max) ) {
			CResultEuler* pER = new CResultEuler(a[1], a[2], dCondNum, dChiSquare, spos, epos, ip, dSi, ww); 
			m_EulResList.AddTail(pER);
			}
		TRACE("EulRes x0 = %f, z0 = %f\n", a[1], a[2]);
		}
}

void CMeth2DLinEuler::ComputeEuler(/*CList<double, double>* px, CList<double, double>* pz, 
	CList<double, double>* pf, CList<double, double>* pfdx, CList<double, double>* pfdz, double h*/)
{

	double chisq, *sig, *a, *w, **cvm, **u, **v, *b, *afunc;
	double *x;		// x position channel
	double *z;		// z elevation channel
	double *f;		// measured field channel
	double *fdx;	// df/dx
	double *fdz;	// df/dz

	int nNPt, nNPtMax;	// Number of poits = number of equations
	int nUn, nUnMax;	// Number of unknown parameters to be solved for
	int n;				// number of profile points


	n = (int) m_nPts;
	//x = m_pPosX;
	//z = m_pElev;
	//f = m_pFld;
	//fdx = m_pDrvX;
	//fdz = m_pDrvZ;

	// allocate arrays for maximum length needed
	nNPtMax = m_ww.max;
	nUnMax = 3 + m_ip.max;
	sig =	dVector(1, nNPtMax);
	a =		dVector(1, nUnMax);
	w =		dVector(1, nUnMax);
	cvm =	dMatrix(1, nUnMax, 1, nUnMax);
	u =		dMatrix(1, nNPtMax, 1, nUnMax);
	v =		dMatrix(1, nUnMax, 1, nUnMax);
    b =		dVector(1, nNPtMax);
	afunc = dVector(1, nUnMax);

	// compute all
	for(double  dSi = m_dSiStart; dSi <= m_dSiEnd; dSi += m_dSiStep) {
		for(int i = m_ip.min; i <= m_ip.max; i++)	{ //
			nUn = 3 + i;	// number_of_unknowns = !!!3!!! + interference_polynomial ! not zero indexed
			if(m_ww.min < nUn) {
				m_ww.min = nUn;
			}
			for(int j = m_ww.min; j <= m_ww.max; j += m_ww.step)	{
				nNPt = j;		// number_of_points(equations) = window_width, ! not zero indexed
				for(int k = 0; (k+nNPt) < nNProfPoints; k++)	{	// window is moovin along profile; !!! k is array index -> indexed from !!!( 0 )!!!! 			//!!! nPt is not zero indexed !!!!
					// filling the mooving window
					x = m_pPosX	+ k - 1;
					z = m_pElev	+ k - 1;
					f = m_pFld	+ k - 1;
					fdx = m_pDrvX	+ k - 1;
					fdz = m_pDrvZ	+ k - 1;
					for(int l = 1; l <= nNPt; l++)	{			
						//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						if(h == 0)
							z[l+1] = 0.0;
						sig[l+1] = 1;
					}
					SvdFitEul(x, z, f, fdx, fdz, sig, nNPt, a, nUn, u, v, w, &chisq, dSi, this);
					//SvdVar(v,nUn,w,cvm);	//!!!!!!!! add to euler results ?????????????????
					ComputeEulerRusults(a, w, nUn, chisq, x[1], x[l], i, dSi, nNPt, cvm);
				}
			}
		}
	}
	
	// free memory
	Free_dMatrix(v, 1, nUnMax, 1, nUnMax);
	Free_dMatrix(u, 1, nNPtMax, 1, nUnMax);
	Free_dMatrix(cvm, 1, nUnMax, 1, nUnMax);
	Free_dVector(w, 1, nUnMax);
	Free_dVector(a, 1, nUnMax);
	Free_dVector(sig, 1, nNPtMax);
	Free_dVector(afunc, 1, nUnMax);
	Free_dVector(b, 1, nNPtMax);

	// cluster solutions
	Clustering((m_dMaxDepth - m_dMinDepth)/30);
}

void CMeth2DLinEuler::FilterResults(double dCondNum, double dMinDepth, double dMaxDepth, BYTE nIpEnd,
			BYTE nIpStart, int nWwEnd, double dWEndPos, int nWwStart, double dWStartPos, double dSiStart, double dSiEnd)
{
	CResultEuler* pEul;
	POSITION pos = m_EulResList.GetHeadPosition();
	while(pos) {
		pEul = m_EulResList.GetNext(pos);
		if((pEul->m_dCondNum < dCondNum) &&
			(pEul->m_dZ > dMinDepth) && (pEul->m_dZ < dMaxDepth) &&
			(pEul->m_nIP >= nIpStart) && (pEul->m_nIP <= nIpEnd) &&
			(pEul->m_dWStartPos >= dWStartPos) && (pEul->m_dWEndPos <= dWEndPos) &&
			(pEul->m_nWindowWidth >= nWwStart) && (pEul->m_nWindowWidth <= nWwEnd) &&
			(pEul->m_dSi >= dSiStart) && (pEul->m_dSi <= dSiEnd)) {
			pEul->m_bVisible = TRUE;
		}
		else {
			pEul->m_bVisible = FALSE;
		}
	}
}

BOOL CMeth2DLinEuler::Export(CString strFileName, double dMinDepth, double dMaxDepth, int nIpStart, int nIpEnd, double dWStartPos, double dWEndPos, 
							 int nWwStart, int nWwEnd, int nMinPtInClus, int nMaxPtInClus, double dSiStart, double dSiEnd, double dCondNum)
{
	ofstream file;
	file.open((LPCSTR) strFileName);
	file.setmode();
	file << "x" << " " << "z" << " " << "IP" << " " << "SI" << " " << "PtInClus" << " " << 
		"WndWdth" << " " << "WndStartPos" << " " << "WndEndPos" << " " << 
		"CondNum" << " " << "ChiSq" << " " << endl;

	POSITION pos = m_EulResList.GetHeadPosition();
	while(pos) {
		CResultEuler* pER = m_EulResList.GetNext(pos);
		if((pER->m_dCondNum < dCondNum) &&
			(pER->m_dZ > dMinDepth) && (pER->m_dZ < dMaxDepth) &&
			(pER->m_nIP >= nIpStart) && (pER->m_nIP <= nIpEnd) &&
			(pER->m_dWStartPos >= dWStartPos) && (pER->m_dWEndPos <= dWEndPos) &&
			(pER->m_nWindowWidth >= nWwStart) && (pER->m_nWindowWidth <= nWwEnd) &&
			(pER->m_nPtInClus >= nMinPtInClus) && (pER->m_nPtInClus <= nMaxPtInClus &&
			(pER->m_dSi >= dSiStart) && (pER->m_dSi <= dSiEnd) )) {
			file << pER->m_dX << " " << pER->m_dZ << " " << pER->m_nIP << " " << pER->m_dSi << " " << 
				pER->m_nPtInClus << " " << pER->m_nWindowWidth << " " << pER->m_dWStartPos << " " << 
				pER->m_dWEndPos << " " << pER->m_dCondNum << " " << pER->m_dChiSquare << endl;
		}
	}
			
	return TRUE;
}

BOOL CMeth2DLinEuler::Export()
{
	//!!! dorobit
/*	CEulDlgExp exDlg;

	exDlg.m_pMtLst = m_pMtLst;
	if(exDlg.DoModal() == IDOK) {
		CFileDialog fdlg(FALSE, "eul", "EulExport.eul", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Euler deconvolution export file (*.eul)|*.eul|ASCII data file (*.dat)|*.dat||");
		if(fdlg.DoModal() == IDOK) {
			ofstream file;
			file.open((LPCSTR) strFileName);
			file.setmode();
			file << "x" << " " << "z" << " " << "IP" << " " << "SI" << " " << "PtInClus" << " " << 
				"WndWdth" << " " << "WndStartPos" << " " << "WndEndPos" << " " << 
				"CondNum" << " " << "ChiSq" << " " << endl;

			POSITION pos = m_EulResList.GetHeadPosition();
			while(pos) {
				CResultEuler* pER = m_EulResList.GetNext(pos);
				if( (pER->m_dCondNum < exDlg.m_dCondNum) &&
					(pER->m_dZ > exDlg.m_dMinDepth) && (pER->m_dZ < dMaxDepth) &&
					(pER->m_nIP >= nIpStart) && (pER->m_nIP <= nIpEnd) &&
					(pER->m_dWStartPos >= dWStartPos) && (pER->m_dWEndPos <= dWEndPos) &&
					(pER->m_nWindowWidth >= nWwStart) && (pER->m_nWindowWidth <= nWwEnd) &&
					(pER->m_nPtInClus >= nMinPtInClus) && (pER->m_nPtInClus <= nMaxPtInClus &&
					(pER->m_dSi >= dSiStart) && (pER->m_dSi <= dSiEnd) )) {
					file << pER->m_dX << " " << pER->m_dZ << " " << pER->m_nIP << " " << pER->m_dSi << " " << 
						pER->m_nPtInClus << " " << pER->m_nWindowWidth << " " << pER->m_dWStartPos << " " << 
						pER->m_dWEndPos << " " << pER->m_dCondNum << " " << pER->m_dChiSquare << endl;
				}
			}

	}*/
	return TRUE;
}

void CMeth2DLinEuler::Clustering(double ClusterRadius)
{
	double cr = ClusterRadius;
	double x, z, x0, z0, r;
	POSITION pos, cpos, pos1;
	CResultEuler* pEul;
	CResultEuler* pCEul;

	m_nMaxPtInClus = 0;

	pos = m_EulResList.GetHeadPosition();
	while(pos) {
		pos1 = pos;
		pEul = m_EulResList.GetNext(pos);
		// max min CondNum and ChiSq
		if(pEul->m_dCondNum > m_dMaxCond)
			m_dMaxCond = pEul->m_dCondNum;
		if(pEul->m_dCondNum < m_dMinCond)
			m_dMinCond = pEul->m_dCondNum;
		if(pEul->m_dChiSquare > m_dMaxChi)
			m_dMaxChi = pEul->m_dChiSquare;
		if(pEul->m_dChiSquare < m_dMinChi)
			m_dMinChi = pEul->m_dChiSquare;
		////////////////////////////
		pEul->m_nPtInClus = 0;

		x0 = pEul->m_dX;
		z0 = pEul->m_dZ;

		cpos = pos1;
		r = 0;
		if(cpos)
			pCEul = m_EulResList.GetPrev(cpos);
		while(cpos && (r < cr)) {				// searching in previous points
		pCEul = m_EulResList.GetPrev(cpos);
			x = pCEul->m_dX;
			z = pCEul->m_dZ;
			r = sqrt((x-x0)*(x-x0) +  (z-z0)*(z-z0));
			if(r < cr) {
				pEul->m_nPtInClus++;
			}
		}
		cpos = pos;
		r = 0;
		while(cpos && (r < cr)) {				// searching in following points
			pCEul = m_EulResList.GetNext(cpos);
			x = pCEul->m_dX;
			z = pCEul->m_dZ;
			r = sqrt((x-x0)*(x-x0) +  (z-z0)*(z-z0));
			if(r < cr) {
				pEul->m_nPtInClus++;
			}
		}
		if(m_nMaxPtInClus < pEul->m_nPtInClus)
			m_nMaxPtInClus = pEul->m_nPtInClus;
	}
}



void CMeth2DLinEuler::SvdFit(double x[],double z[], double f[], double fdx[], double fdz[], double sig[], int ndata, double a[], int ma,	double **u, double **v, double w[], double *chisq, double si, double *b, double *afunc)
{
	int j,i;
	double wmax,tmp,thresh,sum;

	for(i = 1; i <= ndata; i++) {
		BaseFuncs(fdx[i], fdz[i], afunc, si, ma);
		tmp=1.0/sig[i];
		for (j=1;j<=ma;j++) {
			u[i][j]=afunc[j]*tmp;
		}
		b[i]=x[i]*fdx[i] + z[i]*fdz[i] + f[i]*si;
	}
	SvdCmp(u,ndata,ma,w,v);
	wmax=0.0;
	for (j=1;j<=ma;j++)
		if (w[j] > wmax) wmax=w[j];
	thresh=TOL*wmax;
	for (j=1;j<=ma;j++)
		if (w[j] < thresh) w[j]=0.0;
	SvdBkSb(u,w,v,ndata,ma,b,a);
	*chisq=0.0;
	for (i=1;i<=ndata;i++) {
		pParam->BaseFuncs(fdx[i], fdz[i], afunc, si, ma);
		for (sum=0.0,j=1;j<=ma;j++) sum += a[j]*afunc[j];
		*chisq += (tmp=(z[i]-sum)/sig[i],tmp*tmp);
	}
	//Free_dVector(afunc,1,ma);
	//Free_dVector(b,1,ndata);
}
