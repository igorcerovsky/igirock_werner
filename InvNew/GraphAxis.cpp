// GraphAxis.cpp: implementation of the CGraphAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphAxis.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CGraphAxis, CShape, VERSIONABLE_SCHEMA|1 );

CGraphAxis::CGraphAxis()
{
	m_al = 8;	// Arrow Length
	m_all = 10;
	m_aw = 4;	// Arrow Width
	m_ptOrigo = CPoint(0,0);
	//m_len = 1;
	m_Orientation = AXOR_NORMAL;
	//m_datLim.max = datLim.max;
	//m_datLim.min = datLim.min;
	//m_datLim.TickStep = datLim.TickStep;

	m_majorTL.bL = TRUE;
	m_majorTL.bT = TRUE;
	m_majorTL.nLDecimals = 0;
	m_majorTL.nLOffsetX = 0;
	m_majorTL.nLOffsetY = 0;
	m_majorTL.nTlen = 6;
	m_majorTL.axOr = AXOR_REVERSE;

	m_minorTL.bL = TRUE;
	m_minorTL.bT = TRUE;
	m_minorTL.nLDecimals = 0;
	m_minorTL.nLOffsetX = 0;
	m_minorTL.nLOffsetY = 0;
	m_minorTL.nTlen = 4;
	m_minorTL.axOr = AXOR_REVERSE;

	// fill LOFFONT structure for major labels
	m_logFontML.lfHeight = 80;	// !!! CreatePointFontIndirect
	m_logFontML.lfWidth = 0;
	m_logFontML.lfEscapement = 0;
	m_logFontML.lfOrientation = 0;
	m_logFontML.lfWeight = FW_NORMAL;
	m_logFontML.lfItalic = FALSE;
	m_logFontML.lfUnderline = FALSE;
	m_logFontML.lfStrikeOut = 0;
	m_logFontML.lfCharSet = ANSI_CHARSET;
	m_logFontML.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontML.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFontML.lfQuality = DEFAULT_QUALITY;
	m_logFontML.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	strcpy(m_logFontML.lfFaceName, "Arial");        	
	// fill LOFFONT structure for minor labels
	m_logFontNL.lfHeight = 60;	// !!! CreatePointFontIndirect
	m_logFontNL.lfWidth = 0;
	m_logFontNL.lfEscapement = 0;
	m_logFontNL.lfOrientation = 0;
	m_logFontNL.lfWeight = FW_NORMAL;
	m_logFontNL.lfItalic = FALSE;
	m_logFontNL.lfUnderline = FALSE;
	m_logFontNL.lfStrikeOut = 0;
	m_logFontNL.lfCharSet = ANSI_CHARSET;
	m_logFontNL.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontNL.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFontNL.lfQuality = DEFAULT_QUALITY;
	m_logFontNL.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	strcpy(m_logFontNL.lfFaceName, "Arial");  
}

CGraphAxis::CGraphAxis(const char* strID, const char* strName, CPoint ptOrigo, int len, 
					   ORIENTATION or, DATALIMITS datLim, BOOL bVisible) : CShape(strID, strName, bVisible)
{
	m_al = 8;	// Arrow Length
	m_all = 10;
	m_aw = 4;	// Arrow Width
	m_ptOrigo = ptOrigo;
	m_len = len;
	m_Orientation = or;
	m_datLim.max = datLim.max;
	m_datLim.min = datLim.min;
	m_datLim.TickStep = datLim.TickStep;

	m_majorTL.bL = TRUE;
	m_majorTL.bT = TRUE;
	m_majorTL.nLDecimals = 0;
	m_majorTL.nLOffsetX = 0;
	m_majorTL.nLOffsetY = 0;
	m_majorTL.nTlen = 300;
	m_majorTL.axOr = AXOR_NORMAL;

	m_minorTL.bL = FALSE;
	m_minorTL.bT = TRUE;
	m_minorTL.nLDecimals = 2;
	m_minorTL.nLOffsetX = 0;
	m_minorTL.nLOffsetY = 0;
	m_minorTL.nTlen = 150;
	m_minorTL.axOr = AXOR_NORMAL;

	// fill LOFFONT structure for major labels
	m_logFontML.lfHeight = 100;	// !!! CreatePointFontIndirect
	m_logFontML.lfWidth = 0;
	m_logFontML.lfEscapement = 0;
	m_logFontML.lfOrientation = 0;
	m_logFontML.lfWeight = FW_NORMAL;
	m_logFontML.lfItalic = FALSE;
	m_logFontML.lfUnderline = FALSE;
	m_logFontML.lfStrikeOut = 0;
	m_logFontML.lfCharSet = ANSI_CHARSET;
	m_logFontML.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontML.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFontML.lfQuality = DEFAULT_QUALITY;
	m_logFontML.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	strcpy(m_logFontML.lfFaceName, "Arial");        	
	// fill LOFFONT structure for minor labels
	m_logFontNL.lfHeight = 80;	// !!! CreatePointFontIndirect
	m_logFontNL.lfWidth = 0;
	m_logFontNL.lfEscapement = 0;
	m_logFontNL.lfOrientation = 0;
	m_logFontNL.lfWeight = FW_NORMAL;
	m_logFontNL.lfItalic = FALSE;
	m_logFontNL.lfUnderline = FALSE;
	m_logFontNL.lfStrikeOut = 0;
	m_logFontNL.lfCharSet = ANSI_CHARSET;
	m_logFontNL.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontNL.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFontNL.lfQuality = DEFAULT_QUALITY;
	m_logFontNL.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	strcpy(m_logFontNL.lfFaceName, "Arial");        	
}

CGraphAxis::~CGraphAxis()
{

}

void CGraphAxis::Serialize(CArchive &ar)
{
    TRACE("Entering CGraphAxis::Serialize\n");
	
	CShape::Serialize(ar);
	int or = m_Orientation, orMaT = m_majorTL.axOr, orMiT = m_minorTL.axOr;
	if( ar.IsStoring() ) {
		ar  << m_len << m_dScale <<
			m_dOffset << or << m_datLim.max << m_datLim.min << m_datLim.TickStep <<
			orMaT << m_majorTL.bL << m_majorTL.bT << m_majorTL.nLDecimals << m_majorTL.nLOffsetX << m_majorTL.nLOffsetY << m_majorTL.nTlen <<
			orMiT << m_minorTL.bL << m_minorTL.bT << m_minorTL.nLDecimals << m_minorTL.nLOffsetX << m_minorTL.nLOffsetY << m_minorTL.nTlen <<
			m_title.nOffsetX << m_title.nOffsetY << m_title.strTitle <<
			m_al << m_aw << m_all;
		// major labels LOGFONT structure
		ar << m_logFontML.lfCharSet << m_logFontML.lfClipPrecision << m_logFontML.lfEscapement << (CString) m_logFontML.lfFaceName << 
			m_logFontML.lfHeight << m_logFontML.lfItalic << m_logFontML.lfOrientation << m_logFontML.lfOutPrecision << 
			m_logFontML.lfPitchAndFamily << m_logFontML.lfQuality << m_logFontML.lfStrikeOut << m_logFontML.lfUnderline << 
			m_logFontML.lfWeight << m_logFontML.lfWidth;
		// minor labels LOGFONT structure
		ar << m_logFontNL.lfCharSet << m_logFontNL.lfClipPrecision << m_logFontNL.lfEscapement << (CString) m_logFontNL.lfFaceName << 
			m_logFontNL.lfHeight << m_logFontNL.lfItalic << m_logFontNL.lfOrientation << m_logFontNL.lfOutPrecision << 
			m_logFontNL.lfPitchAndFamily << m_logFontNL.lfQuality << m_logFontNL.lfStrikeOut << m_logFontNL.lfUnderline << 
			m_logFontNL.lfWeight << m_logFontNL.lfWidth;
		// title LOGFONT structure
		ar << m_logFontTitle.lfCharSet << m_logFontTitle.lfClipPrecision << m_logFontTitle.lfEscapement << (CString) m_logFontTitle.lfFaceName << 
			m_logFontTitle.lfHeight << m_logFontTitle.lfItalic << m_logFontTitle.lfOrientation << m_logFontTitle.lfOutPrecision << 
			m_logFontTitle.lfPitchAndFamily << m_logFontTitle.lfQuality << m_logFontTitle.lfStrikeOut << m_logFontTitle.lfUnderline << 
			m_logFontTitle.lfWeight << m_logFontTitle.lfWidth;

	}
    else {
		ar  >> m_len >> m_dScale >>
			m_dOffset >> or >> m_datLim.max >> m_datLim.min >> m_datLim.TickStep >>
			orMaT >> m_majorTL.bL >> m_majorTL.bT >> m_majorTL.nLDecimals >> m_majorTL.nLOffsetX >> m_majorTL.nLOffsetY >> m_majorTL.nTlen >>
			orMiT >> m_minorTL.bL >> m_minorTL.bT >> m_minorTL.nLDecimals >> m_minorTL.nLOffsetX >> m_minorTL.nLOffsetY >> m_minorTL.nTlen >>
			m_title.nOffsetX >> m_title.nOffsetY >> m_title.strTitle >>
			m_al >> m_aw >> m_all;
		m_Orientation = (ORIENTATION) or;
		m_majorTL.axOr = (ORIENTATION) orMaT;
		m_minorTL.axOr = (ORIENTATION) orMiT;
		// major labels LOGFONT structure
		ar >> m_logFontML.lfCharSet >> m_logFontML.lfClipPrecision >> m_logFontML.lfEscapement >> (CString) m_logFontML.lfFaceName >> 
			m_logFontML.lfHeight >> m_logFontML.lfItalic >> m_logFontML.lfOrientation >> m_logFontML.lfOutPrecision >> 
			m_logFontML.lfPitchAndFamily >> m_logFontML.lfQuality >> m_logFontML.lfStrikeOut >> m_logFontML.lfUnderline >> 
			m_logFontML.lfWeight >> m_logFontML.lfWidth;
		// minor labels LOGFONT structure
		ar >> m_logFontNL.lfCharSet >> m_logFontNL.lfClipPrecision >> m_logFontNL.lfEscapement >> (CString) m_logFontNL.lfFaceName >> 
			m_logFontNL.lfHeight >> m_logFontNL.lfItalic >> m_logFontNL.lfOrientation >> m_logFontNL.lfOutPrecision >> 
			m_logFontNL.lfPitchAndFamily >> m_logFontNL.lfQuality >> m_logFontNL.lfStrikeOut >> m_logFontNL.lfUnderline >> 
			m_logFontNL.lfWeight >> m_logFontNL.lfWidth;
		// title LOGFONT structure
		ar >> m_logFontTitle.lfCharSet >> m_logFontTitle.lfClipPrecision >> m_logFontTitle.lfEscapement >> (CString) m_logFontTitle.lfFaceName >> 
			m_logFontTitle.lfHeight >> m_logFontTitle.lfItalic >> m_logFontTitle.lfOrientation >> m_logFontTitle.lfOutPrecision >> 
			m_logFontTitle.lfPitchAndFamily >> m_logFontTitle.lfQuality >> m_logFontTitle.lfStrikeOut >> m_logFontTitle.lfUnderline >> 
			m_logFontTitle.lfWeight >> m_logFontTitle.lfWidth;
	}
}

void CGraphAxis::Scale()
{
	m_dScale = (m_len/*-m_all*/)/(m_datLim.max - m_datLim.min);
	if(GetClassName() == "CGraphAxisX")
		m_dOffset = (m_ptOrigo.x) - m_Orientation*m_dScale*m_datLim.min;
	else
		m_dOffset = (m_ptOrigo.y) - m_Orientation*m_dScale*m_datLim.min;
}

void CGraphAxis::FindAxisMinMaxStep(DWORD n)
{
	double a, b, end, dec;

	a = m_datLim.max - m_datLim.min;
	b = 1.0;
	if(a > 1) {
		b = 1.0;
		while(a > 10) {
			a /= 10;
			b *= 10;
		}
	}
	else {
		b = 1.0;
		while(a <= 1) {
			a *= 10;
			b /= 10;
		}
	}
	m_tick.start = floor(m_datLim.min/b) * b;
	end = ceil(m_datLim.max/b) * b;
	a = (end - m_tick.start) /n;
	b = 1.0;
	if(a > 1) {
		while(a >= 10) {
			a /= 10;
			b *= 10;
		}
	}
	else {
		while(a <= 1) {
			a *= 10;
			b /= 10;
		}
	}
	m_tick.step = floor(((end - m_tick.start) /n)/b) * b;
	m_tick.n = n;

	dec = log10(m_tick.step);
	if( dec < 0 ) {
		dec = ceil( fabs(dec) );
		m_majorTL.nLDecimals = (int)dec;
		m_minorTL.nLDecimals = (int)dec+1;
	}
}

void CGraphAxis::FindDummyStep(DWORD n)
{
	// sometimes useful :-) unbeliveable?
	m_datLim.TickStep = (m_datLim.max-m_datLim.min)/n;
}

void CGraphAxis::DoubleToStr(CString& str, double x, int decimals)
{
	CString strFormat;
	CString strDec;
	
	strDec.Format("%d", decimals);
	strFormat = "%." + strDec + "f";
	str.Format(strFormat, x);
}

void CGraphAxis::SetTickLabelOrientation(ORIENTATION or)
{
	m_majorTL.axOr = or;
	m_minorTL.axOr = or;
}

void CGraphAxis::SetParam(CPoint ptOrigo, int len)
{
	m_ptOrigo = ptOrigo;
	m_len = len;
}
void CGraphAxis::SetDatLim(DATALIMITS datLim)		
{
	m_datLim.max = datLim.max;
	m_datLim.min = datLim.min;
}
