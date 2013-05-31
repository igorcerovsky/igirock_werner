// Method2D.cpp: implementation of the CMeth2DLin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "Method2D.h"
#include "MathFunc.h"
#include "DlgMethodProperties.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern double g_comp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMeth2DLin, CObject, 0);

CMeth2DLin::CMeth2DLin()
{
	NullDataPointers();
	m_bElevation = TRUE;
	m_dClsRadP = 0.0;
	m_dClsRadP = 0.0;
	m_bVisible = TRUE;
	m_nClstType = CLST_NORMAL;
	m_dTrashN = 0.7;
	m_dTrashP = 0.7;
	m_dClsExp = 2;
	m_color = RGB(0,0,255);
}

CMeth2DLin::CMeth2DLin(const char* strID, const char* strName, MMSI ip, MMSI ww, MINMAXD depth, MMSD prof, bool bVisible)
{
	NullDataPointers();
	m_strID = strID;
	m_strName = strName;
	m_ip = ip;
	m_ww = ww;
	m_depth = depth;
	m_prof = prof;
	m_bElevation = TRUE;
	m_dClsRadP = 0.0;
	m_dClsRadP = 0.0;
	m_bVisible = bVisible;
	m_nClstType = CLST_NORMAL;
	m_dTrashN = 0.7;
	m_dTrashP = 0.7;
	m_dClsExp = 2;
	m_color = RGB(0,0,255);
}

CMeth2DLin::~CMeth2DLin()
{

}

void CMeth2DLin::Serialize(CArchive &ar)
{
    TRACE("CMeth2DLin::Serialize\n");
	if( ar.IsStoring() ) {
		ar  << m_strID << m_strName << m_strPfofID << 
			m_ww.min << m_ww.max << m_ww.step <<
			m_ip.min << m_ip.max <<m_ip.step << 
			m_depth.min << m_depth.max <<
			m_prof.min << m_prof.max << m_prof.step <<
			m_cond.min << m_cond.max <<
			m_chi.min << m_chi.max <<
			m_clsNum.min << m_clsNum.max <<
			m_ptInCls.min << m_ptInCls.max <<
			m_dClsRadP << m_dClsRadN <<
			m_dTrashN <<
			m_dTrashP <<
			m_dClsExp <<
			m_nCurSol <<
			m_nPts <<
			m_bElevation <<
			m_color;
	}
    else {
		ar  >> m_strID >> m_strName >> m_strPfofID >>
			m_ww.min >> m_ww.max >> m_ww.step >>
			m_ip.min >> m_ip.max >>m_ip.step >> 
			m_depth.min >> m_depth.max >>
			m_prof.min >> m_prof.max >> m_prof.step >>
			m_cond.min >> m_cond.max >>
			m_chi.min >> m_chi.max >>
			m_clsNum.min >> m_clsNum.max >>
			m_ptInCls.min >> m_ptInCls.max >>
			m_dClsRadP >> m_dClsRadN >>
			m_dTrashN >>
			m_dTrashP >>
			m_dClsExp >>
			m_nCurSol >>
			m_nPts >>
			m_bElevation >>
			m_color;
	}
	m_results.Serialize(ar);
}

FILTSTRCT CMeth2DLin::GetFiltInfo()
{
	FILTSTRCT filtRes;

	filtRes.dWwEndPos	= m_ww.max;
/*	filtRes.dWwStartPos = m_ww.min;
	filtRes.nIpEnd		= m_ip.max;
	filtRes.nIpStart	= m_ip.min;
	filtRes.nWwEnd		= m_ww.max;
	filtRes.nWwStart	= m_ww.min;
	filtRes.dCondNum	= m_cond.max;
	filtRes.dChiSq		= m_chi.max;
*/
	return filtRes;
}

void CMeth2DLin::SetDataPointers(double *pPosX, double *pElev, double *pFld, double *pDrvX, double *pDrvZ, int n)
{
	m_pFld  = pFld;
	m_pPosX = pPosX;
	m_pElev = pElev;
	m_pDrvX = pDrvX;
	m_pDrvZ = pDrvZ;
	m_nPts  = n;
}

void CMeth2DLin::NullDataPointers()
{
	m_pFld  = NULL;
	m_pPosX = NULL;
	m_pElev = NULL;
	m_pDrvX = NULL;
	m_pDrvZ = NULL;
	m_pElev = NULL;
	m_nPts  = 0;
}

int CMeth2DLin::Clustering()
{
	// rc cluster radius, MUST NOT be 0
	// exp exponent
	// clustering pow(r, dPow)
	TRACE("Entering CMeth2DLinWerner::Clustering - advanced\n");

	g_comp = 0;
	// default values, sets after depth
	int i;
	double x, z, x0, z0, r, rc, rcl, compMax, comp=0, exp;
	double rMin = 1.0e-6;
	int ww, wwj;	// window width
	int maxPtClusA = 0;
	CResult* pRes;
	CResult* pWrj;
	COLORREF color = RGB(160, 0, 0);

	// initialization
	m_chi.min = 0;
	m_clsNum.max = 0;
	m_chi.min = m_results[0]->m_dChiSq;
	m_chi.max = m_results[0]->m_dChiSq;
	m_cond.min = m_results[0]->m_dCondNum;
	m_cond.max = m_results[0]->m_dCondNum;
	// create progress bar
	CMainFrame* pMainFrm = (CMainFrame*) AfxGetApp()->GetMainWnd();

	int n;
	n = (int) m_results.GetSize();
	//SetClsRadius( rc );
	if( m_nClstType == CLST_NORMAL ) {
		rc = m_dClsRadN;
		rcl = m_dClsRadL;
	}
	if( m_nClstType == CLST_POWER ) {
		rc = m_dClsRadP;
	}
	exp = m_dClsExp;
	compMax = n*(n-1);
	double step=compMax/100, old = compMax;
	pMainFrm->DrawProgress( 0.0, color );
	for( i = 0; i < n; i++ ) {
		pRes =  m_results[i];
		// find max min CondNum and ChiSq
		if(pRes->m_dCondNum > m_cond.max)	m_cond.max = pRes->m_dCondNum;
		if(pRes->m_dCondNum < m_cond.min)	m_cond.min = pRes->m_dCondNum;
		if(pRes->m_dChiSq > m_chi.max)		m_chi.max = pRes->m_dChiSq;
		if(pRes->m_dChiSq < m_chi.min)		m_chi.min = pRes->m_dChiSq;
		////////////////////////////
		pRes->m_dClsP = 0.0;
		pRes->m_dClsN = 0.0;
		pRes->m_nPtClsA = 0;

		x0 = pRes->m_dX;
		z0 = pRes->m_dZ;
		ww = pRes->m_nWndWdth;;
		if(i+1 < n) wwj = m_results[i+1]->m_nWndWdth;
		for(int j = i+1; j < n; j++ ) {
			pWrj =  m_results[j];
			if( pWrj->m_bShow && pRes->m_bShow ) {
				wwj = pWrj->m_nWndWdth;
				x = pWrj->m_dX;
				z = pWrj->m_dZ;
				r = sqrt((x-x0)*(x-x0) +  (z-z0)*(z-z0));
				// all solutions
				if(r < rc) {
					pRes->m_nPtClsA++;
					pWrj->m_nPtClsA++;
				}

				// only solutions with the same window width
				if( ww==wwj ) {
					if(r < rMin)
						r = rMin;
					if(r < rc) {
						pRes->m_dClsP += 1/pow(r, exp);
						pRes->m_nPtClsW++;
						pWrj->m_dClsP += 1/pow(r, exp);
						pWrj->m_nPtClsW++;
					}
				}
			}
		}
		g_comp = (double)(compMax - i*(i-1))/compMax;
		if( old > (compMax - i*(i-1)) || step < 0.1 && g_comp != 0) {
            pMainFrm->DrawProgress( 1-g_comp, color );
			old += step;
		}
		// update progress bar

		pRes->m_dClsP = pow(pRes->m_dClsP, pRes->m_nPtClsW);
		if(m_clsNum.max < pRes->m_dClsP)
			m_clsNum.max = pRes->m_dClsP;
		if(maxPtClusA < pRes->m_nPtClsA )
			maxPtClusA = pRes->m_nPtClsA;
	}

	////////////////// normalize solutions to be in <0, 1> ///////////////////////////////////////////////
	double min = 0, max = 0;
	for(i = 0; i < n; i++ ) {
		// power clustering
		pRes =  m_results[i];
		if( (pRes->m_dClsP > 0) && (m_clsNum.max > 0) )
			pRes->m_dClsP = log(pRes->m_dClsP)/log(m_clsNum.max); 
		else
			pRes->m_dClsP = 0.0;

		// normal clustering not normalizing here, only advanced computing taking into account number of all solutions in cluster
		if( maxPtClusA > 0 )
			pRes->m_dClsN = pRes->m_nPtClsW *( (double)pRes->m_nPtClsA / maxPtClusA) ;
		else
			pRes->m_dClsN = 0.0;
		if( pRes->m_dClsN > max) {
			max = pRes->m_dClsN;	}
	}
	m_clsNum.max = 1.0;

	// normalize "normal" clustering here, logarithmic
	//int hst[101];
	n = m_results.GetCount();
	for(i = 0; i < n; i++ ) {
		pRes =  m_results[i]; 
		if( (pRes->m_dClsN >= 1) && (max > 0) )
			pRes->m_dClsN = log(pRes->m_dClsN)/log(max); 
		else
			pRes->m_dClsN = 0.0;
		int l;
		//l = (int)(pRes->m_dClsN*100);
		//hst[l]++;
	}
	//// histogram
	//for(i = 1; i < 101; i++ ) {
	//	hst[i] += hst[i-1];
	//}
	//// equalize
	//double m = 100.0/m_results.GetCount();
	//int tmp = hst[0];
	//for(i = 0; i < 101; i++ ) {
	//	hst[i] -= tmp; 
	//	//hst[i] = (int) (hst[i]*m);
	//}
	//for(i = 0; i < n; i++ ) {
	//	pRes =  m_results[i]; 
	//	int l = (int) (pRes->m_dClsN*100);
	//	pRes->m_dClsN = (double)hst[l]/hst[100];
	//}
	////////////////// end normaslize ////////////////////////////////////////////////////////////////////

	ShowSolutions();
	g_comp = 100;
	pMainFrm->DrawProgress( 1.0, color );

	return 0;
}

// shows all solutions over trash
int CMeth2DLin::ShowSolutions(double dTresh)
{
	SetTrash( dTresh );
	for( int i =0; i < (int)m_results.GetSize(); i++) {
		CResult* pRes =  m_results[i];
		if( m_nClstType == CLST_NORMAL ) {
			if(pRes->m_dClsN >= m_dTrashN ) {
				pRes->m_bGrey = FALSE;}
			else {
				pRes->m_bGrey = TRUE ;}
		}
		if( m_nClstType == CLST_POWER ) {
			if(pRes->m_dClsP >= m_dTrashP ) {
				pRes->m_bGrey = FALSE;}
			else {
				pRes->m_bGrey = TRUE;}
		}
	}

	return 0;
}

bool CMeth2DLin::RemoveSolutions(bool bHiden)
{
	if( bHiden) {
		int m = (int)m_results.GetSize();
		for( int i =0; i < m; i++) {
			CResult* pRes =  m_results[i];
			if( pRes->m_bGrey || !pRes->m_bShow) {
				m_results.RemoveAt(i);
				delete pRes;
				i--;
			}
			m=(int)m_results.GetSize();
		}
		return true;
	}
	return false;
}

double CMeth2DLin::GetTrash(void)
{
	if( m_nClstType == CLST_NORMAL ) {
		return m_dTrashN;
	}
	if( m_nClstType == CLST_POWER ) {
		return m_dTrashP;
	}
	return 0;
}

void CMeth2DLin::SetTrash(double dTrash)
{
	if( m_nClstType == CLST_NORMAL ) {
		m_dTrashN = dTrash;
	}
	if( m_nClstType == CLST_POWER ) {
		m_dTrashP = dTrash;
	}
}

double CMeth2DLin::GetClsRadius(void)
{
	if( m_nClstType == CLST_NORMAL ) {
		return m_dClsRadN;
	}
	if( m_nClstType == CLST_POWER ) {
		return m_dClsRadP;
	}
	return 0;
}

void CMeth2DLin::SetClsRadius(double dRad)
{
	if( m_nClstType == CLST_NORMAL ) {
		m_dClsRadN = dRad;
	}
	if( m_nClstType == CLST_POWER ) {
		m_dClsRadP = dRad;
	}
}

typedef enum DrawSymbol {
	CROSS,
	CIRCLE,
	ARROW,
	RECTANGLE
} DSYMBOL;

void DrawSymbol(CDC *pDC, int x, int y, int length, int width, COLORREF color, DSYMBOL symbol, bool bFill, int angle)
{
	//TRACE("Entering CGraphLine::DrawSymbol\n");
	int l = length;		// velkost krizika
	double PI = 3.1415926535897932384626433832795;

	CPen pen, *pPen;
	pen.CreatePen(PS_SOLID, width, color);
	pPen = pDC->SelectObject(&pen);
	if(symbol == CROSS) {
		pDC->MoveTo(x+l, y);
		pDC->LineTo(x-l, y);
		pDC->MoveTo(x, y+l);
		pDC->LineTo(x, y-l);
		return;
	}
	if(symbol == ARROW) {
		pDC->MoveTo(x, y);
		int xx = int (x + 3*l*cos(angle/10*PI/180)),
			yy = int (y + 3*l*sin(angle/10*PI/180));
		pDC->LineTo(xx, yy);
		return;
	}
	if( (symbol == CIRCLE) || (symbol == RECTANGLE)) {
		LOGBRUSH logBrush;
		if(bFill) {
			logBrush.lbStyle = BS_SOLID;
		}
		else {
			logBrush.lbStyle = BS_HOLLOW;
		}
		logBrush.lbColor = color;
		logBrush.lbHatch = HS_CROSS;

		CBrush brush;
		brush.CreateBrushIndirect(&logBrush);
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);
		
		if(symbol == CIRCLE) {
			pDC->Ellipse(x-l, y-l, x+l, y+l);
		}
		
		if(symbol == RECTANGLE) {
			pDC->Rectangle(x-l, y-l, x+l, y+l);
		}
		
		pDC->SelectObject(pOldBrush);
	}
	pDC->SelectObject( pPen );
}

void CMeth2DLin::Draw(CDC *pDC, double scx, double offx, double scy, double offy)
{
	TRACE("Entering CMeth2DLinWerner::Draw\n");
	CSize ls(2, 0);
	pDC->DPtoLP( &ls );
	int l = ls.cx;

	if( m_bVisible ) {
		double	x, y;
		double dClusNum, dTrash;
		int		i;
		CResult *pRes;

		// first draw hiden solutions
		for( i=0; i<m_results.GetSize(); i++) {
			pRes = m_results[i];
			if( pRes->m_bGrey && pRes->m_bShow) {
				x = offx + scx*pRes->m_dX;
				y = offy + scy*pRes->m_dZ;
				DrawSymbol(pDC, (int)x, (int)y, l, 1, RGB(230, 230, 230), CROSS, false, 0);
			}
		}
		// visible solutions
		for( i=0; i<m_results.GetSize(); i++ ) {
			// draw solution that are checked
			pRes = m_results[i];
			if( !pRes->m_bGrey && pRes->m_bShow)  {
				switch( m_nClstType ) {
					case CLST_NORMAL:
						dClusNum = pRes->m_dClsN;
						dTrash = m_dTrashN;
						break;
					case CLST_POWER:
						dClusNum = pRes->m_dClsP;
						dTrash = m_dTrashP;
						break;
					case CLST_NONE:
						dClusNum = 0;
						break;
				}
				x = pRes->m_dX;
				y = pRes->m_dZ;
				// draw only solutions that are within graph
				if( (x > m_prof.min) && (x < m_prof.max) && (y > m_depth.min) && (y < m_depth.max) ) {
					x = offx + scx*x;	// (offset + scale * value)
					y = offy + scy*y;
					//if(m_bSymbol) {
						//UINT symbL = (UINT) (50 + nPtInClus*symbSc);
						UINT symbL = (UINT) (3*l*(dClusNum-dTrash)/(1-dTrash));
						DrawSymbol(pDC, (int) x, (int) y, symbL, 1, m_color, CROSS, false, 0);
						/*if(m_bDip) {
							int dip = int (pWR->GetAt(pos)->m_dDip*10);
							DrawSymbol(pDC, (int) x, (int) y, symbL, 1, m_logPen.lopnColor, ARROW, FALSE, dip);
						}*/
					//}
				}
			}
		}
	}
}


void CMeth2DLin::Properties(void)
{
	CDlgMethodProperties dlg;
	CString str;

	str.Format("%d", m_results.GetSize() );
	dlg.m_strSolNum = str;
	str.Format("min=%d,  max=%d, step=%d", m_ip.min, m_ip.max, m_ip.step );
	dlg.m_strIp = str;
	str.Format("min=%d,  max=%d, step=%d", m_ww.min, m_ww.max, m_ww.step );
	dlg.m_strWw = str;
	dlg.m_color = m_color;
	dlg.m_strID = m_strID;
	dlg.m_strName = m_strName;
	dlg.m_strProf = m_strPfofID;
	dlg.m_strType = "WERNER";
	if( dlg.DoModal() == IDOK ) {
		m_color = dlg.m_color;
		m_strName = dlg.m_strName;
	}
}

int CMeth2DLin::FilterResults(FLTRES flt)
{
	int n = (int)m_results.GetSize();
	for( int i =0; i < n; i++) {
		CResult* pWr = m_results[i];
		if( (pWr->m_dCondNum <= flt.dCondMax) && (pWr->m_dChiSq < flt.dChiMax) &&
			//(pWr->m_dZ > flt.dAltMin) && (pWr->m_dZ < flt.dAltMax) &&
			(pWr->m_nIP >= flt.nIpMin) && (pWr->m_nIP <= flt.nIpMax) &&
			//(pWr->m_nWndIndex == flt.m_nWndIndex) &&
			(pWr->m_nWndWdth >= flt.nWwMin) && (pWr->m_nWndWdth <= flt.nWwMax)) {
			if(flt.nTag == WF_ALL) {
				pWr->m_bShow = TRUE;
			}
			if( (flt.nTag == WF_SELECTED) && (pWr->m_bShow) ) {
				pWr->m_bShow = TRUE;
			}
		}
		else {
			pWr->m_bShow = FALSE;
		}
	}
	return 0;
}
