// ScaleBar.cpp: implementation of the CMyScaleBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "ScaleBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyScaleBar::CMyScaleBar()
{
	m_nType = SB_HORZ;
	m_dZoomIn = 1.2;
	m_dZoomOut = 0.8;

	m_bmpH1.LoadBitmap(IDB_B1);
	m_brushH1.CreatePatternBrush(&m_bmpH1);
	m_bmpH2.LoadBitmap(IDB_B2);
	m_brushH2.CreatePatternBrush(&m_bmpH2);
}

CMyScaleBar::CMyScaleBar(int nType)
{
	m_nType = nType;
	m_dZoomIn = 1.2;
	m_dZoomOut = 0.8;
}

CMyScaleBar::~CMyScaleBar()
{

}

void CMyScaleBar::Draw(CDC *pDC)
{
	CSize	s(10, 10);
	int width;	// screen coordinates
	CPen *pPen, nullPen(PS_NULL, 0, RGB(0, 0,0));
	pPen = pDC->SelectObject(&nullPen);
	CBrush *pBr;

	pDC->DPtoLP( &s );
	if(m_nType == SB_HORZ) {
		width = s.cy;
		pBr = pDC->SelectObject(&m_brushH2);
		pDC->Rectangle(m_nL, m_nT, m_nR, m_nT-width);
		pDC->SelectObject(&m_brushH1);
		pDC->Rectangle(m_nMin, m_nT, m_nMax, m_nT-width);
		pDC->SelectObject(pBr);
	}
	if(m_nType == SB_VERT) {
		width = s.cx;
		pBr = pDC->SelectObject(&m_brushH2);
		pDC->Rectangle(m_nR-width, m_nT, m_nR, m_nB);
		pDC->SelectObject(&m_brushH1);
		pDC->Rectangle(m_nR-width, m_nMax, m_nR, m_nMin);
		pDC->SelectObject(pBr);
		//pDC->Rectangle(m_nL, m_nT, m_nR, m_nB);
		//pDC->SelectObject(&m_brushH1);
		//pDC->Rectangle(m_nL, m_nMax, m_nR, m_nMin);
		//pDC->SelectObject(pBr);
	}
	pDC->SelectObject(pPen);
}

void CMyScaleBar::SetDrawRect(int left, int top, int right, int bottom)
{
	m_nT = top;
	m_nL = left;
	m_nR = right;
	m_nB = bottom;
/*	if(m_nType == SB_HORZ) {
		m_nMinExt = left;
		m_nMaxExt = right;
		m_nMin = left;
		m_nMax = right;
	}
	if(m_nType == SB_VERT) {
		m_nMinExt = bottom;
		m_nMaxExt = top;
		m_nMin = bottom;
		m_nMax = top;
	}*/
}

void CMyScaleBar::SetMaxDataExtent(double min, double max)
{
	m_dMinData = min;
	m_dMaxData = max;
	//m_dMin = min;
	//m_dMax = max;
}

void CMyScaleBar::SetDataExtend(double min, double max)
{
	if(min >= m_dMinData)
		m_dMin = min;
	else
		m_dMin = m_dMinData;
	if(max <= m_dMaxData)
		m_dMax = max;
	else
		m_dMax = m_dMaxData;
	if(m_dMin < m_dMax) {
		Scale();
		m_nMax = (int) (m_off + m_sc*m_dMax);
		m_nMin = (int) (m_off + m_sc*m_dMin);
	}
}

int CMyScaleBar::IsClicked(CPoint lPt)
{
	int l, *pB;
	long *px, *py;

	if(m_nType == SB_VERT) {
		px = &lPt.y;
		py = &lPt.x;
		l = (m_nR - m_nL);
		pB = &m_nL;
	}
	if(m_nType == SB_HORZ) {
		px = &lPt.x;
		py = &lPt.y;
		pB = &m_nB;
		l = (m_nT - m_nB);
	}

	if( ((m_nMin-l/2) < (*px)) && ((m_nMin) >= (*px)) && ((*pB) < (*py)) && ((*pB+l) > (*py)) ) {
		m_nClk = SB_MIN;
		return SB_MIN;
	}
	if( ((m_nMax) <= (*px)) && ((m_nMax+l/2) > (*px)) && ((*pB) < (*py)) && ((*pB+l) > (*py)) ) {
		m_nClk = SB_MAX;
		return SB_MAX;
	}
	if( ((m_nMin) < (*px)) && ((m_nMax) > (*px)) && ((*pB) < (*py)) && ((*pB+l) > (*py)) ) {
		m_nClk = SB_MIDDLE;
		m_ndl = *px - m_nMin;
		m_ndr = m_nMax - *px;
		return SB_MIDDLE;
	}
	return SB_NON;
}

void CMyScaleBar::Scale()
{
	int b, a;
	if(m_nType == SB_HORZ) {
		b = (m_nR-m_nL);
		a = m_nL;
	}
	if(m_nType == SB_VERT) {
		b = (m_nT-m_nB);
		a = m_nB;
	}

	m_sc  = (double) b / (m_dMaxData-m_dMinData);
	m_off = a - m_sc*m_dMinData;
}

void CMyScaleBar::Move(CPoint lPt)
{
	long *px, *py;
	int  *pL, *pR;

	if(m_nType == SB_VERT) {
		px = &lPt.y;
		py = &lPt.x;
		pL = &m_nB;
		pR = &m_nT;
	}
	if(m_nType == SB_HORZ) {
		px = &lPt.x;
		py = &lPt.y;
		pL = &m_nL;
		pR = &m_nR;
	}
	//if((*py >= m_nB) && (*py <= m_nT)) {
		if( m_nClk == SB_MIN ){
			if(*px > *pL && *px < m_nMax)
				m_nMin = *px;
			else
				m_nMin = *pL;
		}
		if( m_nClk == SB_MAX ) {
			if(*px < *pR && *px > m_nMin)
				m_nMax = *px;
			else
				m_nMax = *pR;
		}
		if( m_nClk == SB_MIDDLE ) {
			if( (*pL < (*px - m_ndl)) && (*pR > (*px + m_ndr)) ) {
				m_nMin = *px - m_ndl;
				m_nMax = *px + m_ndr; 
			}
			else {
				if( *pL > (*px - m_ndl) ) {
					m_nMin = *pL;
					m_nMax = *pL + m_ndr + m_ndl;
				}
				if( *pR < (*px + m_ndr) ) {
					m_nMax = *pR;
					m_nMin = *pR - m_ndr - m_ndl;
				}
			}
		}
	//}

}

void CMyScaleBar::Zoom(double min, double max)
{
	SetDataExtend(min, max);
}

/*void CMyScaleBar::Zoom(double min, double max)
{
	SetDataExtend(min, max);
}
void CMyScaleBar::Zoom(double min, double max)
{
	SetDataExtend(min, max);
}*/

void CMyScaleBar::GetDataExtent(double &min, double &max)
{
	min = (m_nMin - m_off)/m_sc;
	max = (m_nMax - m_off)/m_sc;
}


void CMyScaleBar::Zoom(CPoint lPt, BOOL bIn)
{
	double zoom;
	if(bIn)
		zoom = m_dZoomIn;
	else
		zoom = m_dZoomOut;

	long *px, *py;
	int  *pL, *pR;

	if(m_nType == SB_VERT) {
		px = &lPt.y;
		py = &lPt.x;
		pL = &m_nB;
		pR = &m_nT;
	}
	if(m_nType == SB_HORZ) {
		px = &lPt.x;
		py = &lPt.y;
		pL = &m_nL;
		pR = &m_nR;
	}

	int ndl = (int) ( ((double)(m_nMax - m_nMin)/2)*zoom);
	
		if( (*pL < (*px - ndl)) && (*pR > (*px + ndl)) ) {
			m_nMin = *px - ndl;
			m_nMax = *px + ndl; 
		}
		else {
			m_nMin = *px - ndl;
			m_nMax = *px + ndl; 
			if( *pL > m_nMin ) {
				m_nMin = *pL;
			}
			if( *pR < m_nMax ) {
				m_nMax = *pR;
			}
		}
}

void CMyScaleBar::GetScOff(double& sc, double& off)
{
	double min, max;
	int b, a;
	if(m_nType == SB_HORZ) {
		b = (m_nR-m_nL);
		a = m_nL;
	}
	if(m_nType == SB_VERT) {
		b = (m_nT-m_nB);
		a = m_nB;
	}

	min = (m_nMin - m_off)/m_sc;
	max = (m_nMax - m_off)/m_sc;

	sc  = (double) b / (max-min);
	off = a - sc*min;
}

void CMyScaleBar::GetDataMinMax(double& min, double& max)
{
	min = m_dMin;
	max = m_dMax;
}

int CMyScaleBar::ZoomToMax(void)
{
	SetDataExtend(m_dMinData, m_dMaxData);
	return 0;
}
