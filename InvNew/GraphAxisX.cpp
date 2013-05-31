// GraphAxisX.cpp: implementation of the CGraphAxisX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphAxisX.h"
//#include "DlgAxisProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CGraphAxisX, CGraphAxis, VERSIONABLE_SCHEMA|1 );

CGraphAxisX::CGraphAxisX()
{
	m_title.nOffsetX = 0;
	m_title.nOffsetY = 0;
	m_title.strTitle = "";

	// fill LOFFONT structure for title
	m_logFontTitle.lfHeight = 120;	// !!! CreatePointFontIndirect
	m_logFontTitle.lfWidth = 0;
	m_logFontTitle.lfEscapement = 0;
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

CGraphAxisX::CGraphAxisX(const char* strID, const char* strName, CPoint ptOrigo, int len, ORIENTATION or, 
						 DATALIMITS datLim, const char* strTitle, BOOL bVisible) : CGraphAxis(strID, strName, ptOrigo, len, or, datLim, bVisible)
{
	m_ptUL.x = ptOrigo.x;
	m_ptUL.y = ptOrigo.y + 200;			// !!! depends on Maping mode
	m_ptLR.x = ptOrigo.x + len;
	m_ptLR.y = ptOrigo.y - 200;
	
	m_title.nOffsetX = 0;
	m_title.nOffsetY = 1000;
	m_title.strTitle = strTitle;

	// fill LOFFONT structure for title
	m_logFontTitle.lfHeight = 120;	// !!! CreatePointFontIndirect
	m_logFontTitle.lfWidth = 0;
	m_logFontTitle.lfEscapement = 0;
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

CGraphAxisX::~CGraphAxisX()
{

}

void CGraphAxisX::Serialize(CArchive &ar)
{
    TRACE("Entering CGraphAxisX::Serialize\n");
	CGraphAxis::Serialize(ar);
	if( ar.IsStoring() ) {

	}
    else {

	}
}

void CGraphAxisX::Draw(CDC *pDC)
{
	if(m_bVisible) {
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
		CPen* p1 = pDC->SelectObject(&pen);
		// create fonts
		CFont fontML, fontNL;	// major and minor lalbel fonts
		CFont *pFontML, *pFontNL;
		fontML.CreatePointFontIndirect(&m_logFontML, pDC);
		fontNL.CreatePointFontIndirect(&m_logFontNL, pDC);

		pDC->MoveTo(m_ptOrigo);
		int nl = m_len+sl.cx;
		pDC->LineTo(m_ptOrigo.x + nl, m_ptOrigo.y);
		pDC->LineTo(m_ptOrigo.x + nl - sa.cx, m_ptOrigo.y + sl.cy);
		pDC->MoveTo(m_ptOrigo.x + nl, m_ptOrigo.y);
		pDC->LineTo(m_ptOrigo.x + nl - sa.cx, m_ptOrigo.y - sl.cy);
		int ud = m_majorTL.axOr;	// labels Up or Down

		//************* ticks ***********************
		pDC->MoveTo(m_ptOrigo.x, m_ptOrigo.y);
		pDC->LineTo(m_ptOrigo.x , m_ptOrigo.y - smt.cy*ud);
		double xx = m_tick.start;
		double step = m_tick.step;
		while(xx <= m_datLim.max) {
			//+++++++++++ major ticks +++++++++++
			if(m_majorTL.bT && xx >= m_datLim.min) {
				int ud = m_majorTL.axOr;	// labels Up or Down
				pDC->MoveTo((int) (m_dOffset + m_dScale * xx) , m_ptOrigo.y);
				pDC->LineTo((int) (m_dOffset + m_dScale * xx) , m_ptOrigo.y - smt.cy * ud);
				if(m_majorTL.bL) {
					pFontML = pDC->SelectObject(&fontML);
					DoubleToStr(label, xx, m_majorTL.nLDecimals);
					if(ud == -1) {
						pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
					}
					else {
						pDC->SetTextAlign(TA_CENTER);
					}
					pDC->TextOut((int) (m_dOffset + m_dScale*xx + m_majorTL.nLOffsetX), 
						m_ptOrigo.y - smt.cy * ud - m_majorTL.nLOffsetY * ud, label);
					pDC->SelectObject(pFontML);
				}
			}
			//++++++++++ end major ticks ++++++++
 			xx += step;
			//-------------minor ticks ----------
			double xn = xx - step/2;
			if( m_minorTL.bT && (xn <= m_datLim.max)  && xn >= m_datLim.min ) {
				int ud = m_minorTL.axOr;	// labels Up or Down
				pDC->MoveTo((int) (m_dOffset + m_dScale * xn) , m_ptOrigo.y);
				pDC->LineTo((int) (m_dOffset + m_dScale * xn) , m_ptOrigo.y - snt.cy * ud);
				if(m_minorTL.bL) {
					pFontNL = pDC->SelectObject(&fontNL);
					DoubleToStr(label, xn, m_minorTL.nLDecimals);
					if(ud == -1) {
						pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
					}
					else {
						pDC->SetTextAlign(TA_CENTER);
					}
					pDC->TextOut((int) (m_dOffset + m_dScale * xn + m_minorTL.nLOffsetX), 
						m_ptOrigo.y - smt.cy * ud - m_minorTL.nLOffsetY * ud, label);
					pDC->SelectObject(pFontNL);
				}
			}
			
			//---------- end minor ticks --------
		}
		//************ end ticks ********************
		fontML.DeleteObject();
		fontNL.DeleteObject();
		pDC->SelectObject(p1);

		// drawing axis title
		pDC->SetTextAlign(TA_CENTER | TA_BASELINE);//*/
		CFont titleFont;
		CFont *pFontTitle;
		VERIFY(titleFont.CreatePointFontIndirect(&m_logFontTitle, pDC));
		pFontTitle = pDC->SelectObject(&titleFont);
		pDC->TextOut(m_ptOrigo.x + m_len/2 + m_title.nOffsetX, m_ptOrigo.y - m_title.nOffsetY * ud, m_title.strTitle);
		pDC->SelectObject(pFontTitle);
		titleFont.DeleteObject();//*/
	}
}


void CGraphAxisX::Properties(CDocument* pDoc)
{
/*	CDlgAxisProperties dlg;

	dlg.m_pDoc = pDoc;
	dlg.m_pAx = this;
	if(dlg.DoModal() == IDOK) {

	}*/
}

void CGraphAxisX::SetBoundRectangle(CDC* pDC)
{
	int y1, y2, ud;
	CSize size(m_logFontML.lfHeight/10, 0);

	pDC->DPtoHIMETRIC(&size);
	ud = m_majorTL.axOr;
	if(m_title.strTitle == "") {
		y1 = m_ptOrigo.y - ud*(m_majorTL.nLOffsetY + m_majorTL.nTlen + size.cx);//m_majorTL.nTlen * m_majorTL.axOr;
	}
	else {
		y1 = m_ptOrigo.y - m_majorTL.axOr*(m_title.nOffsetY + size.cx);
	}
	y2 = m_ptOrigo.y + 50 * m_majorTL.axOr;
	if(y1 < y2) {
		int yPom = y1;
		y1 = y2;
		y2 = yPom;
	}

	m_ptUL.x = m_ptOrigo.x;
	m_ptUL.y = y1;			// !!! depends on Maping mode
	m_ptLR.x = m_ptOrigo.x + m_len*m_Orientation;
	m_ptLR.y = y2;

	m_boundRect = CRect(m_ptUL, m_ptLR);
}

