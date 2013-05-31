// GraphAxisY.cpp: implementation of the CGraphAxisY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphAxisY.h"
//#include "DlgAxisProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CGraphAxisY, CGraphAxis, VERSIONABLE_SCHEMA|1 );

CGraphAxisY::CGraphAxisY()
{
	m_majorTL.nLOffsetY = 110;

	m_minorTL.nLOffsetY = 120;

	m_title.nOffsetX = - 1500;
	m_title.nOffsetY = 0;
	// fill LOFFONT structure for title
	m_logFontTitle.lfHeight = 120;	// !!! CreatePointFontIndirect
	m_logFontTitle.lfWidth = 0;
	m_logFontTitle.lfEscapement = -900;
	m_logFontTitle.lfOrientation = 0;
	m_logFontTitle.lfWeight = FW_NORMAL;
	m_logFontTitle.lfItalic = FALSE;
	m_logFontTitle.lfUnderline = FALSE;
	m_logFontTitle.lfStrikeOut = 0;
	m_logFontTitle.lfCharSet = ANSI_CHARSET;
	m_logFontTitle.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontTitle.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFontTitle.lfQuality = DEFAULT_QUALITY;
	m_logFontTitle.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	strcpy(m_logFontTitle.lfFaceName, "Arial");        	
}

CGraphAxisY::CGraphAxisY(const char* strID, const char* strName, CPoint ptOrigo, int len, ORIENTATION or, 
						 DATALIMITS datLim, const char* strTitle, BOOL bVisible) : CGraphAxis(strID, strName, ptOrigo, len, or, datLim, bVisible)
{
	
	m_title.nOffsetX = - 1500;
	m_title.nOffsetY = 0;
	m_title.strTitle = strTitle;
	
	// fill LOFFONT structure for title
	m_logFontTitle.lfHeight = 120;	// !!! CreatePointFontIndirect
	m_logFontTitle.lfWidth = 0;
	m_logFontTitle.lfEscapement = -900;
	m_logFontTitle.lfOrientation = 0;
	m_logFontTitle.lfWeight = FW_NORMAL;
	m_logFontTitle.lfItalic = FALSE;
	m_logFontTitle.lfUnderline = FALSE;
	m_logFontTitle.lfStrikeOut = 0;
	m_logFontTitle.lfCharSet = ANSI_CHARSET;
	m_logFontTitle.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontTitle.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFontTitle.lfQuality = DEFAULT_QUALITY;
	m_logFontTitle.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	strcpy(m_logFontTitle.lfFaceName, "Arial");        	
}

CGraphAxisY::~CGraphAxisY()
{

}

void CGraphAxisY::Serialize(CArchive &ar)
{
    TRACE("Entering CGraphAxisY::Serialize\n");
	CGraphAxis::Serialize(ar);
	if( ar.IsStoring() ) {

	}
    else {

	}
}

void CGraphAxisY::Draw(CDC *pDC)
{
	if(m_bVisible)  {
		Scale();
		SetBoundRectangle(pDC);
		CString label;
		CSize	sl(m_all, m_aw);	// arrow
		CSize	sa(m_al, m_aw);	// arrow
		CSize	smt(0, m_majorTL.nTlen);	// major ticks
		CSize	snt(0, m_minorTL.nTlen);	// minor ticks


		pDC->DPtoLP( &sl );
		pDC->DPtoLP( &sa );
		pDC->DPtoLP( &smt );
		pDC->DPtoLP( &snt );
		// create pen
		CPen pen;
		pen.CreatePenIndirect(&m_logPen);
		CPen* pen1 = pDC->SelectObject(&pen);
		// create fonts
		CFont fontML, fontNL, titleFont;	// major and minor lalbel fonts
		CFont *pFontML, *pFontNL, *pTitleFont;
		fontML.CreatePointFontIndirect(&m_logFontML, pDC);
		fontNL.CreatePointFontIndirect(&m_logFontNL, pDC);
		
		pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
		titleFont.CreatePointFontIndirect(&m_logFontTitle, pDC);
		pTitleFont = pDC->SelectObject(&titleFont);
		int lr = m_majorTL.axOr;	// labels Left or Right
		pDC->TextOut(m_ptOrigo.x + m_title.nOffsetX * lr + m_logFontTitle.lfHeight, m_ptOrigo.y + m_Orientation * m_len / 2 + m_title.nOffsetY, m_title.strTitle);
		pDC->SelectObject(pTitleFont);
		titleFont.DeleteObject();
		
		int nl = m_len+sl.cx;
		pDC->MoveTo(m_ptOrigo.x, m_ptOrigo.y);
		pDC->LineTo(m_ptOrigo.x, m_ptOrigo.y + m_Orientation * nl);
		pDC->LineTo(m_ptOrigo.x + sa.cy, m_ptOrigo.y + m_Orientation * nl - m_Orientation * sa.cx);
		pDC->MoveTo(m_ptOrigo.x, m_ptOrigo.y + m_Orientation * nl);
		pDC->LineTo(m_ptOrigo.x - sa.cy, m_ptOrigo.y + m_Orientation * nl - m_Orientation * sa.cx);
		
		//************* ticks ***********************
		pDC->MoveTo(m_ptOrigo.x, m_ptOrigo.y);
		pDC->LineTo(m_ptOrigo.x - smt.cy*m_majorTL.axOr, m_ptOrigo.y );
		double yy = m_tick.start;
		double step = m_tick.step;
		while(yy <= m_datLim.max) {
			//+++++++++++ major ticks +++++++++++
			if(m_majorTL.bT && yy >= m_datLim.min) {
				int lr = m_majorTL.axOr;	// labels Left or Right
				pDC->MoveTo(m_ptOrigo.x, (int) ((m_dOffset + m_Orientation * m_dScale * yy)));
				pDC->LineTo(m_ptOrigo.x - smt.cy * lr, (int) ((m_dOffset + m_Orientation * m_dScale * yy)));
				if(m_majorTL.bL) {
					pFontML = pDC->SelectObject(&fontML);
					DoubleToStr(label, yy, m_majorTL.nLDecimals);
					if(lr == 1) {
						pDC->SetTextAlign(TA_RIGHT);
					}
					else {
						pDC->SetTextAlign(TA_LEFT);
					}
					pDC->TextOut(m_ptOrigo.x - smt.cy * lr + m_majorTL.nLOffsetX * lr, (int) (m_dOffset + m_Orientation*yy*m_dScale + m_majorTL.nLOffsetY), label);
					pDC->SelectObject(pFontML);
				}
			}
			//++++++++++ end major ticks ++++++++
			yy += step;
			//-------------minor ticks ----------
			double yn = yy - step/2;
			if((m_minorTL.bT) && (yn <= m_datLim.max) && yy >= m_datLim.min) {
				int lr = m_minorTL.axOr;	// labels Left or Right
				pDC->MoveTo(m_ptOrigo.x, (int) ((m_dOffset + m_Orientation * m_dScale * yn)));
				pDC->LineTo(m_ptOrigo.x - snt.cy * lr, (int) ((m_dOffset + m_Orientation * m_dScale * yn)));
				if(m_minorTL.bL) {
					pFontNL = pDC->SelectObject(&fontNL);
					DoubleToStr(label, yn, m_minorTL.nLDecimals);
					if(lr == 1) {
						pDC->SetTextAlign(TA_RIGHT);
					}
					else {
						pDC->SetTextAlign(TA_LEFT);
					}
					pDC->TextOut(m_ptOrigo.x - snt.cy * lr + m_minorTL.nLOffsetX * lr, (int) (m_dOffset + m_Orientation * m_dScale * yn+ m_minorTL.nLOffsetY), label);
					pDC->SelectObject(pFontNL);;
				}
			}
			//---------- end minor ticks --------
		}
		//************ end ticks ********************
		pDC->SelectObject(pen1);
		fontML.DeleteObject();
		fontNL.DeleteObject();
	}
}

void CGraphAxisY::Properties(CDocument* pDoc)
{
/*	CDlgAxisProperties dlg;
	dlg.m_pDoc = pDoc;
	dlg.m_pAx = this;
	if(dlg.DoModal() == IDOK) {
	}*/
}

void CGraphAxisY::SetBoundRectangle(CDC* pDC)
{
	int x1, x2;
	CSize size(m_logFontML.lfHeight/10, 0);

	pDC->DPtoHIMETRIC(&size);
	x1 = m_ptOrigo.x + m_majorTL.axOr*(m_title.nOffsetX - size.cx);
	x2 = m_ptOrigo.x + 50 * m_majorTL.axOr;
	if(x1 > x2) {
		int xPom = x1;
		x1 = x2;
		x2 = xPom;
	}

	m_ptUL.x = x1;
	m_ptUL.y = m_ptOrigo.y + m_len * m_Orientation;
	m_ptLR.x = x2;
	m_ptLR.y = m_ptOrigo.y;			

	m_boundRect = CRect(m_ptUL, m_ptLR);
}
