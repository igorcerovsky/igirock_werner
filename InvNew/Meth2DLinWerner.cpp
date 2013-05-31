// Meth2DLinWerner.cpp: implementation of the CMeth2DLinWerner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "Meth2DLinWerner.h"
#include "MathFunc.h"
#include <fstream>
#include "MainFrm.h"
#include "InvDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#include "InversionView.h"
//#include "DlgMethodInfo.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//****************** werner thread ****************************************************************************
int ThreadWerner(LPVOID pParam)
{
	ComputeStrct* pStrct;

	pStrct = (ComputeStrct*) pParam;
	CMeth2DLinWerner* pWer = (CMeth2DLinWerner*) pStrct->pMethod;
	pWer->Compute();

	pStrct->pDoc->ThreadFinished();
	delete pStrct;
	return 0;
}
//****************** end werner thread ************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern double g_comp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMeth2DLinWerner, CMeth2DLin, 0)

CMeth2DLinWerner::CMeth2DLinWerner()
{
	 m_nMethod = METH_WERNER;
}

CMeth2DLinWerner::CMeth2DLinWerner(const char* strID, const char* strName, MMSI ip, MMSI ww, 
								   MINMAXD depth, MMSD prof, bool bVisible) :	CMeth2DLin(strID, strName, ip, ww, depth, prof, bVisible)
{
	 m_nMethod = METH_WERNER;
}

CMeth2DLinWerner::~CMeth2DLinWerner()
{
	for( int i=0; i<m_results.GetSize(); i++ )
	{
		delete m_results[i];
	}
	m_results.RemoveAll();
}

void CMeth2DLinWerner::Serialize(CArchive &ar)
{
	TRACE("Entering CMeth2DLinWerner::Serialize\n");

	CMeth2DLin::Serialize(ar);
	if( ar.IsStoring() ) {
		ar << m_nWrType << m_dElevShift;
	}
    else {
		ar >> m_nWrType >> m_dElevShift;
	}
	m_InducedField.Serialize(ar);
}

bool CMeth2DLinWerner::Compute()
{
	if(m_pElev != NULL)
		ComputeWernerVarAlt();
	else
		ComputeWerner();


	return TRUE;
}


//*************************** SVD fit *************************************************************************
#define TOL 1.0e-16 //****************** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CMeth2DLinWerner::SvdFit(double x[], double y[], double z[], double sig[], int ndata, double a[], int ma,
	double **u, double **v, double w[], double *chisq, double *b, double *afunc)
{
	// !!! if channel z is not given put pointer {double* z = NULL}
	int j, i;
	double wmax, tmp, thresh, sum;

	for (i=1; i<=ndata; i++) {
		if(z != NULL)
			BaseFuncs(x[i], y[i], z[i], b[i], afunc, ma);
		else
			BaseFuncs(x[i], y[i], b[i], afunc, ma);
		tmp = 1.0/sig[i];
		for (j=1; j<=ma; j++) {
			u[i][j] = afunc[j]*tmp;
		}
		b[i] *= tmp;
	}
	SvdCmp(u, ndata, ma, w, v);
	wmax = 0.0;
	for (j=1; j<=ma; j++)
		if (w[j] > wmax) wmax=w[j];
	thresh = TOL*wmax;
	for (j=1; j<=ma; j++)
		if (w[j] < thresh) w[j] = 0.0;
	SvdBkSb(u, w, v, ndata, ma, b, a);
	*chisq = 0.0;
	for (i=1; i<=ndata; i++) {
		if(z != NULL)
			BaseFuncs(x[i], y[i], z[i], b[i], afunc, ma);
		else
			BaseFuncs(x[i], y[i], b[i], afunc, ma);
		for (sum=0.0, j=1; j<=ma; j++) sum += a[j]*afunc[j];
		*chisq += (tmp=(y[i]-sum)/sig[i], tmp*tmp);
	}
}

#undef TOL
//*************************** end SVD fit *********************************************************************



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**************** Werner deconvolution for variable altitude data ********************************************
void CMeth2DLinWerner::BaseFuncs(double x, double y, double z, double &b, double p[], int np)
{
	//TRACE("Entering CMeth2DLinWerner::BaseFuncs\n");
	if( ( np == 6) || ( np == 7) || ( np == 10) || ( np == 13) || ( np == 16) ) {
		z += m_dElevShift;
		b = y*(x*x + z*z);
		switch(np) {
		case 6:
			p[1] = y;
			p[2] = y*x;
			p[3] = y*z;
			p[4] = 1;
			p[5] = x;
			p[6] = z;
			break;
		case 7:
			p[1] = y;
			p[2] = y*x;
			p[3] = y*z;
			p[4] = 1;
			p[5] = x;
			p[6] = z;
			p[7] = x*x + z*z;
			break;
		case 10:
			p[1] = y;
			p[2] = y*x;
			p[3] = y*z;
			p[4] = 1;
			p[5] = x;
			p[6] = x*x;
			p[7] = z;
			p[8] = z*z;
			p[9] = x*z;
			p[10] = x*x*x + x*z*z;
			break;
		case 13:
			p[1] = y;
			p[2] = y*x;
			p[3] = y*z;
			p[4] = 1;
			p[5] = x;
			p[6] = x*x;
			p[7] = x*x*x;
			p[8] = z;
			p[9] = z*z;
			p[10] = x*z;
			p[11] = x*z*z;
			p[12] = x*x*z;
			p[13] = x*x*x*x + x*x*z*z;
			break;
		case 16:
			p[1] = y;
			p[2] = y*x;
			p[3] = y*z;
			p[4] = 1;
			p[5] = x;
			p[6] = x*x;
			p[7] = x*x*x;
			p[8] = x*x*x*x;
			p[9] = z;
			p[10] = z*z;
			p[10] = x*z;
			p[10] = x*z*z;
			p[10] = x*x*z;
			p[10] = x*x*z*z;
			p[10] = x*x*x*z;
			p[10] = x*x*x*x*x + x*x*x*z*z;
			break;
		}
	}
	else 
		AfxMessageBox("Werner variable altitude np problem");
}

void CMeth2DLinWerner::ComputeWernerRusultsVarAlt(double a[], double w[], int nu, double dChiSquare, 
		BYTE ip, int ww, int ind)		// nu is number of uknowns
{
	// see Ostrowski at. al. Werner Deconvolution for variable altitude data
	// TRACE("Entering CMeth2DLinWerner::ComputeWernerRusultsVarAlt\n");
	double wmax, wmin;
	double dWDip, dWCondNum;
	double x0, z0, A = 0, B = 0, C0, C1, C2, C3;

	wmax = w[1];
	wmin = w[1];
	for(int i=1; i<=nu; i++) {
		if(w[i]<wmin) wmin=w[i];
		if(w[i]>wmax) wmax=w[i];
	}

	if (wmin != 0)  {
		dWCondNum = wmax/wmin;
	}
	else {
		dWCondNum = 0;
	}
	if(dWCondNum < 1.0e100) {	//!!! optoinal should by added
		if( (-4*a[1]-a[2]*a[2]) > 0 ) {
			x0 = a[2]/2;
			z0 = sqrt(-a[1]-x0*x0);
			switch(nu) {
			case 6:
				A = a[5];
				B = (a[4] + A*x0)/z0;
				break;
			case 7:
				C0 = a[7];
				A = a[5] + 2*C0*x0;
				B = (a[4] + A*x0 - C0*(z0*z0 + x0*x0))/z0;
				break;
			case 10:
				C1 = a[10];
				C0 = a[6] + 2*C1*x0;
				A = a[5] + 2*C0*x0 - C1*(z0*z0 + x0*x0);
				B = (a[4] + A*x0 - C0*(z0*z0 + x0*x0))/z0;
				break;
			case 13:
				C2 = a[13];
				C1 = a[7] + 2*C2*x0;
				C0 = a[6] + 2*C1*x0 - C2*(z0*z0 + x0*x0);
				A = a[5] + 2*C0*x0 - C1*(z0*z0 + x0*x0);
				B = (a[4] + A*x0 - C0*(z0*z0 + x0*x0))/z0;
				break;
			case 16:
				C3 = a[16];
				C2 = a[8] + 2*C3*x0;
				C1 = a[6] + 2*C2*x0 - C3*(z0*z0 + x0*x0);
				C0 = a[6] + 2*C1*x0 - C2*(z0*z0 + x0*x0);
				A = a[5] + 2*C0*x0 - C1*(z0*z0 + x0*x0);
				B = (a[4] + A*x0 - C0*(z0*z0 + x0*x0))/z0;
				break;
			}
			z0 = -z0 - m_dElevShift;
			CResultWerner* pWR = new CResultWerner(	x0, z0, 0.0, dWDip = 0, dWCondNum, 
													dChiSquare, A, B, ind, ww, ip, true); 
			m_results.Add(pWR);
		}
			
	}
}

void CMeth2DLinWerner::ComputeWernerVarAlt()
{
	TRACE("Entering CMeth2DLinWerner::ComputeWerner Variable Altitude\n");
	// 'b' and 'afunc' are here not to be realocated by each call of SvdFit
	double chisq,*x,*y, *z, *sig,*a,*w,**cvm,**u,**v, *b, *afunc;
	int nNPt, nNPtMax;			// Number of poits = number of equations
	int nUn,  nUnMax;			// Number of unknown parameters to be solved for
	int n;
	UINT compMax=0, comp = 0;
	CMainFrame* pMainFrm = (CMainFrame*) AfxGetApp()->GetMainWnd();
	COLORREF color=RGB(170,0,0);

	
	// shift coordinate system
	m_dElevShift = 0 - m_depth.max;

	n = (int) m_nPts;
	// allocate arrays for maximum length needed
	nNPtMax = m_ww.max;
	nUnMax = 16;
	sig = dVector(1,nNPtMax);
	a = dVector(1,nUnMax);
	w = dVector(1,nUnMax);
	cvm = dMatrix(1,nUnMax,1,nUnMax);
	u = dMatrix(1,nNPtMax,1,nUnMax);
	v = dMatrix(1,nUnMax,1,nUnMax);
	b = dVector(1,nNPtMax);				
	afunc = dVector(1,nUnMax);

	// create progress bar
	if(m_ip.max > 4)
		m_ip.max = 4; // maximum allowed

	for(int i = m_ip.min; i <= m_ip.max; i++)	{ //
		switch(i) {
		case 0:		// without interference polynomial
			nUn = 6;
			break;
		case 1:		// interference polynomial 0
			nUn = 7;
			break;
		case 2:		// interference polynomial 1
			nUn = 10;
			break;
		case 3:		// interference polynomial 2
			nUn = 13;
			break;
		case 4:
			nUn = 16;
			break;
		}
		compMax += (m_ww.max-nUn)/m_ww.step;
	}

	for(int i = m_ip.min; i <= m_ip.max; i++)	{ //
		switch(i) {
		case 0:		// without interference polynomial
			nUn = 6;
			break;
		case 1:		// interference polynomial 0
			nUn = 7;
			break;
		case 2:		// interference polynomial 1
			nUn = 10;
			break;
		case 3:		// interference polynomial 2
			nUn = 13;
			break;
		case 4:
			nUn = 16;
			break;
		}
		//for(int j = m_ww.min; j <= m_ww.max; j += m_ww.step)	{
		for(int j = nUn; j <= m_ww.max; j += m_ww.step)	{
			nNPt = j;		// number_of_points(equations) = window_width, ! not zero indexed
			
			for(int k = 0; (k+nNPt) < n; k++)	{	// window is moovin along profile; !!! k is array index -> indexed from !!!( 0 )!!!! 			//!!! nPt is not zero indexed !!!!
				for(int l = 0; l < nNPt; l++)	{	// the mooving window
					sig[l+1] = 1;				}
				// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				x = m_pPosX + k - 1;	
				y = m_pFld + k - 1;
				z = m_pElev + k - 1;	// elev shift is in BaseFuncs
				SvdFit(x, y, z, sig, nNPt, a, nUn, u, v, w, &chisq, b, afunc);
				ComputeWernerRusultsVarAlt(a, w, nUn, chisq, i, nNPt, k);
				//SvdVar(v,nUn,w,cvm);	//!!!!!!!! add to werner results ?????????????????
			}
			comp++;
			g_comp = (double)(comp)/compMax;
			// update progress bar
			pMainFrm->DrawProgress( g_comp, color );
		}
	}

	g_comp = 100.0;
	// Destroy progress bar
	Free_dMatrix(v, 1, nUnMax, 1, nUnMax);
	Free_dMatrix(u, 1, nNPtMax, 1, nUnMax);
	Free_dMatrix(cvm, 1, nUnMax, 1, nUnMax);
	Free_dVector(w, 1, nUnMax);
	Free_dVector(a, 1, nUnMax);
	Free_dVector(sig, 1, nNPtMax);
	Free_dVector(b, 1, nNPtMax);
	Free_dVector(afunc, 1, nUnMax);

	//Clustering((m_depth.max - m_depth.min)/30);
	SetClusteringPram( CLST_NORMAL, (m_depth.max - m_depth.min)/30 );
	Clustering();
	//AfxBeginThread( (AFX_THREADPROC)ThreadClustering, this, THREAD_PRIORITY_NORMAL );
}
//**************** end Werner deconvolution for variable altitude data ****************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**************** normal Werner deconvolution ****************************************************************
void CMeth2DLinWerner::ComputeWerner()
// !!!!!!!!!!!!!! attention x, y, sig, a, w, cvm, u, u are pointers !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! like p_x ...
{
	TRACE("Entering CMeth2DLinWerner::ComputeWerner normal werner\n");
	double chisq,*x,*y,*sig,*a,*w,**cvm,**u,**v, *b, *afunc;
	int nNPt, nNPtMax;			// Number of poits = number of equations
	int nUn,  nUnMax;			// Number of unknown parameters to be solved for
	int n;
	CMainFrame* pMainFrm = (CMainFrame*) AfxGetApp()->GetMainWnd();
	COLORREF color=RGB(0,0,180);
	UINT comp=0, compMax=0;

	m_dElevShift = m_depth.max;

	n = (int) m_nPts;
	// allocate arrays for maximum length needed
	nNPtMax = m_ww.max;
	nUnMax = 4 + m_ip.max;
	sig = dVector(1,nNPtMax);
	a = dVector(1,nUnMax);
	w = dVector(1,nUnMax);
	cvm = dMatrix(1,nUnMax,1,nUnMax);
	u = dMatrix(1,nNPtMax,1,nUnMax);
	v =	dMatrix(1,nUnMax,1,nUnMax);
	b = dVector(1,nNPtMax);				
	afunc = dVector(1,nUnMax);

//	if( m_nWrType == WERNER_NORMAL ) {
	// create progress bar
	pMainFrm->DrawProgress( 0.0, color );
	g_comp = 0;
	for(int i = m_ip.min; i <= m_ip.max; i++)	{ //
		nUn = 4 + i;	// number_of_unknowns = 4 + interference_polynomial ! not zero indexed
		compMax += (m_ww.max-nUn)/m_ww.step;
	}
	for(int i = m_ip.min; i <= m_ip.max; i++)	{ //
		nUn = 4 + i;	// number_of_unknowns = 4 + interference_polynomial ! not zero indexed
		if(m_ww.min < nUn) {
			m_ww.min = nUn;
		}
		for(int j = m_ww.min; j <= m_ww.max; j += m_ww.step)	{
			nNPt = j;		// number_of_points(equations) = window_width, ! not zero indexed
			for(int k = 0; (k+nNPt) < n; k++)	{	// window is moovin along profile; !!! k is array index -> indexed from !!!( 0 )!!!! 			//!!! nPt is not zero indexed !!!!
				for(int l = 0; l < nNPt; l++)	{				// filling the mooving window
					sig[l+1] = 1;				}
				// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				x = m_pPosX + k - 1;		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
				y = m_pFld + k - 1;			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				SvdFit(x, y, NULL, sig, nNPt, a, nUn, u, v, w, &chisq, b, afunc);
				ComputeWernerRusults(a, w, nUn, chisq, i, nNPt, k);
				//SvdVar(v,nUn,w,cvm);		//!!!!!!!! add to werner results ?????????????????
			}
			// update progress bar
			comp++;
			g_comp = (double)(comp)/compMax;
			pMainFrm->DrawProgress( g_comp, color );
		}
	}
	g_comp = 1;
//	}

	// Destroy progress bar
	pMainFrm->DrawProgress( 1.0, color );
	// free memory
	Free_dMatrix(v, 1, nUnMax, 1, nUnMax);
	Free_dMatrix(u, 1, nNPtMax, 1, nUnMax);
	Free_dMatrix(cvm, 1, nUnMax, 1, nUnMax);
	Free_dVector(w, 1, nUnMax);
	Free_dVector(a, 1, nUnMax);
	Free_dVector(sig, 1, nNPtMax);
	Free_dVector(b, 1, nNPtMax);
	Free_dVector(afunc, 1, nUnMax);

	//Clustering((m_depth.max - m_depth.min)/30);
	SetClusteringPram( CLST_NORMAL, (m_depth.max - m_depth.min)/30 );
	Clustering();
	//AfxBeginThread( (AFX_THREADPROC)ThreadClustering, this, THREAD_PRIORITY_NORMAL );

}

void CMeth2DLinWerner::BaseFuncs(double x, double y, double &b, double p[], int np)
{
	//TRACE("Entering CMeth2DLinWerner::BaseFuncs1 \n");
	b = y*x*x;
	p[1]=y;
    p[2]=y*x;
    for(int i=0; i<=(np-3); i++) {
		p[i+3]=pow(x, i);
	}
}

void CMeth2DLinWerner::ComputeWernerRusults(double a[], double w[], int nu, double dChiSquare, BYTE ip, int ww, int ind)		// nu is number of uknowns
{
	//TRACE("Entering CMeth2DLinWerner::ComputeWernerRusults\n");

	double wmax, wmin;
	double dWSusc, dWDip, dWCondNum;
	double x0, z0, A, B, C0, C1, C2, C3;

	wmax = w[1];
	wmin = w[1];
	for(int i=1; i<=nu; i++) {
		if(w[i]<wmin) wmin=w[i];
		if(w[i]>wmax) wmax=w[i];
	}

	if (wmin != 0)  {
		dWCondNum = wmax/wmin;
	}
	else {
		dWCondNum = 0;
	}
	if(dWCondNum < 1.0e100) {	//!!! optoinal should by added
		if( (-4*a[1]-a[2]*a[2]) > 0 ) {
			x0 = a[2]/2;
			z0 = sqrt(-a[1]-x0*x0);
			switch(nu) {
				case 4: 
					A=a[4];
					B=(a[3]+A*x0)/z0;
					break;
				case 5:
					C0=a[5];
					A=a[4]+2*x0*C0;
					B=(a[3]+A*x0 - C0*pow(x0, 2) - C0*pow(z0, 2))/z0;
					break;
				case 6:
					C1=a[6];
					C0=a[5]+2*x0*C1;
					A=a[4]+2*x0*C0 - C1*pow(x0, 2) - C1*pow(z0, 2);
					B=(a[3]+A*x0 - C0*pow(x0, 2) - C0*pow(z0, 2))/z0;
					break;
				case 7:
					C2=a[7];
					C1=a[6]+2*C2*x0;
					C0=a[5]+2*C1*x0 - C2*pow(x0, 2) - C2*pow(z0, 2);
					A=a[4]+2*x0*C0 - C1*pow(x0, 2) - C1*pow(z0, 2);
					B=(a[3]+A*x0 - C0*pow(x0, 2) - C0*pow(z0, 2))/z0;
					break;
				case 8:
					C3=a[8];
					C2=a[7]+2*C3*x0;
					C1=a[6]+2*C2*x0 - C3*pow(x0, 2) - C3*pow(z0, 2);
					C0=a[5]+2*C1*x0 - C2*pow(x0, 2) - C2*pow(z0, 2);
					A =a[4]+2*x0*C0 - C1*pow(x0, 2) - C1*pow(z0, 2);
					B =(a[3]+A*x0 - C0*pow(x0, 2) - C0*pow(z0, 2))/z0;
					break;
			}
			//TRACE("x0 je %6.1f, z0 je %6.1f \n", x0, z0);
			//*********!!!!!!!!!!!!!! add copmputations for dip and susc **********************!!!!!!!!!!!!!!!!!!!!
			dWSusc	= 0;
			//*********!!!!!!!!!!!!!! don't forget *********************************************!!!!!!!!!!!!!!!!!!!
			z0 = -z0 + m_dElevShift;
			CResultWerner* pWR = new CResultWerner(	x0, z0, dWSusc, dWDip = 0, dWCondNum, dChiSquare, 
													A, B, ind, ww, ip, true); 
			m_results.Add(pWR);
		}
	}
}

//**************** end normal Werner deconvolution ************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool CMeth2DLinWerner::IsCaptured(CPoint pt)
{
	return false;
}


void CMeth2DLinWerner::Properties(CDocument* pDoc)
{
}

bool CMeth2DLinWerner::RemoveSolution(int nIndex)
{
	if( !IsEmpty() ) {
		if(nIndex == -1)
			nIndex = m_nCurSol;
		m_results.RemoveAt( nIndex );
		return true;
	}

	return false;
}

bool CMeth2DLinWerner::ShowSolution(int nIndex, bool bShow)
{
	if( !IsEmpty() ) {
		if(nIndex == -1) 
			nIndex = m_nCurSol;
		m_results[nIndex]->m_bShow = bShow;
		return true;
	}

	return false;
}


bool CMeth2DLinWerner::ShowAllSolutions()
{
	if( !IsEmpty() ) {
		for( int i=0; i< m_results.GetSize(); i++)
			m_results[i]->m_bShow = true;
		return TRUE;
	}

	return FALSE;
}

void CMeth2DLinWerner::ComputeDip(double bb, double strike)
{	
	TRACE("Entering CMeth2DLinWerner::ComputeDip\n");

	double pi= 3.1415926535;	
	double pii = pi/180;
	double Ml, Mt, mm, nx, nz, A, B;

	nx = cos(m_InducedField.m_dInclination*pii)*sin(strike*pii);
	nz = sin(m_InducedField.m_dInclination*pii);
	mm = 2*bb*(nx*nx + nz*nz);	
	int n = (int) m_results.GetSize();
	for( int i =0; i < n; i++) {
		CResultWerner* pWr = (CResultWerner*) m_results[i];
		A = pWr->m_dA;
		B = pWr->m_dB;
		Ml =(-A*nx + B*nz)/mm;
		Mt =(-B*nx + A*nz)/mm;
		double dip = atan(Ml/Mt)*180/pi + m_InducedField.m_dInclination;
		pWr->m_dDip = dip;
	}
}

void CMeth2DLinWerner::FilterResults(FILTSTRCT filtRes)
{
	TRACE("Entering CMeth2DLinWerner::FilterResults\n");

	int n = (int)m_results.GetSize();
	for( int i =0; i < n; i++) {
		CResultWerner* pWr = (CResultWerner*) m_results[i];
		if( (pWr->m_dCondNum < filtRes.dCondNum) && (pWr->m_dChiSq < filtRes.dChiSq) &&
			(pWr->m_dZ > filtRes.dAltMin) && (pWr->m_dZ < filtRes.dAltMax) &&
			(pWr->m_nIP >= filtRes.nIpStart) && (pWr->m_nIP <= filtRes.nIpEnd) &&
			//(pWr->m_nWndIndex == filtRes.m_nWndIndex) &&
			(pWr->m_nWndWdth >= filtRes.nWwStart) && (pWr->m_nWndWdth <= filtRes.nWwEnd)) {
			if(filtRes.whatFilter == WF_ALL) {
				pWr->m_bShow = TRUE;
			}
			if( (filtRes.whatFilter == WF_SELECTED) && (pWr->m_bShow) ) {
				pWr->m_bShow = TRUE;
			}
		}
		else {
			pWr->m_bShow = FALSE;
		}
	}
}


void CMeth2DLinWerner::Export()
{
	TRACE("Entering CMeth2DLinWerner::Export\n");

	CFileDialog dlg(FALSE, "*.dat", "out.dat", OFN_OVERWRITEPROMPT, "Data Files (*.dat)|*.dat||");
	dlg.m_ofn.lpstrTitle = "Export Method Results";
	if(dlg.DoModal() == IDOK) {
		std::ofstream file;
		file.open((LPCSTR) dlg.GetPathName());

		file << "x" << " "				// 
			<< "z" << " "				// 	
			<< "IP" << " "				// 
			<< "PtInClusA" << " "		// 
			<< "PtInClusW" << " " 		// 
			<< "dClusN" << " "			// 
			<< "dClusP" << " " 			// 
			<< "WndWdth" << " "			// 
			<< "WndIndex" << " "		// 
			<< "CondNum" << " "			// 
			<< "ChiSq" << " "			// 
			<< "Dip" << " "				// 
			<< "Susc" << " "			// 
			<< "A" << " " 
			<< "B" << std::endl;

		int n = (int)m_results.GetSize();
		for( int i =0; i < n; i++) {
			CResultWerner* pWr = (CResultWerner*) m_results[i];
			if( pWr->m_bShow ) {
				file << pWr->m_dX << " "		// 
					<< pWr->m_dZ << " "			// 
					<< pWr->m_nIP << " "		// 
					<< pWr->m_nPtClsA << " "	// 
					<< pWr->m_nPtClsW << " "	// 
					<< pWr->m_dClsN << " "		// 
					<< pWr->m_dClsP << " "		// 
					<< pWr->m_nWndWdth << " "	// 
					<< pWr->m_nWndIndex << " "	// 
					<< pWr->m_dCondNum << " "	// 
					<< pWr->m_dChiSq << " " 
					<< pWr->m_dDip << " " 
					<< pWr->m_dSusc << " " 
					<< pWr->m_dA << " " 
					<< pWr->m_dB << " " << std::endl;
			}
		}
	}
}


int CMeth2DLinWerner::GetSolCount()
{
	if( !IsEmpty() ) {
		return (int)m_results.GetSize();
	}

	return 0;
}

void CMeth2DLinWerner::Info()
{
/*	CDlgMethodInfo dlg;

	dlg.m_strMethod = "Werner Deconvolution";
	dlg.m_strMethID = m_strID;
	dlg.m_strMethName = m_strName;
	dlg.m_strProf = m_strProfName;
	if( m_pChanZ == NULL )
		dlg.m_strChanElev = "None";
	else
		dlg.m_strChanElev = m_pChanZ->GetChannelName();
	dlg.m_strChanField = m_pChanY->GetChannelName();
	dlg.m_strChanPos = m_pChanX->GetChannelName();

	if(dlg.DoModal() == IDOK) {
		m_strName = dlg.m_strMethName;
	}
*/
}


bool CMeth2DLinWerner::IsEmpty()
{
	if( m_results.GetSize() > 0 )
		return true;

	return false;
}

