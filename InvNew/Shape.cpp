// Shape.cpp: implementation of the CShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CShape, CObject, 0);

CShape::~CShape()
{

}

void CShape::Draw(CDC *pDC)
{

}

void CShape::Serialize(CArchive &ar)
{
    TRACE("Entering CShape::Serialize\n");
	if( ar.IsStoring() ) {
		ar  << m_strID << m_strName << m_bVisible << m_nShpBound <<
			m_ptUL.x << m_ptUL.y << m_ptLR.x << m_ptLR.y <<
			m_pt.x << m_pt.y << m_ptOrigo.x << m_ptOrigo.y <<
			m_logPen.lopnColor << m_logPen.lopnStyle << m_logPen.lopnWidth.x << m_logPen.lopnWidth.y;

	}
    else {
		ar >> m_strID >> m_strName >> m_bVisible >> m_nShpBound >>
			m_ptUL.x >> m_ptUL.y >> m_ptLR.x >> m_ptLR.y >>
			m_pt.x >> m_pt.y >> m_ptOrigo.x >> m_ptOrigo.y >>
			m_logPen.lopnColor >> m_logPen.lopnStyle >> m_logPen.lopnWidth.x >> m_logPen.lopnWidth.y;
	}
}

void CShape::Move(CPoint pt)
{
	// moves retlative to current coordinates
	m_ptOrigo -= pt;
	m_ptUL -= pt;
	m_ptLR -= pt;
}

void CShape::MoveTo(CPoint pt)
{
	m_ptOrigo = pt;
}

BOOL CShape::IsCaptured(CPoint pt)
{
	// in logical coordinates
	if((pt.x > m_ptUL.x) && (pt.x < m_ptLR.x) && (pt.y < m_ptUL.y) && (pt.y > m_ptLR.y) ) 
		return TRUE;
	else
		return FALSE;
}

void CShape::Properties(LPVOID pParam)
{

}

void CShape::SetBoundRect(CDC* pDC)
{

}
