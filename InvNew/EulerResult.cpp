// EulerResults.cpp: implementation of the CResultEuler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "EulerResult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CResultEuler, CObject, 0)

CResultEuler::CResultEuler()
{
	m_bSel = FALSE;
}

CResultEuler::~CResultEuler()
{

}

void CResultEuler::Serialize(CArchive &ar)
{
    if( ar.IsStoring() ) {
		ar << m_dX << m_dZ << m_dCondNum << m_dChiSq << m_nIP << m_dSi << m_nWndWdth << m_nPtInClus << m_bVisible;
	}
    else {
		ar >> m_dX >> m_dZ >> m_dCondNum >> m_dChiSq >> m_nIP >> m_dSi >> m_nWndWdth >> m_nPtInClus >> m_bVisible;
	}
}

void CResultEuler::GetParam(double &dX, double &dZ, double &dCondNum, double &dChiSquare, 
		double &dWwsp, double &dWwep, UINT &nIp, double &dSi, UINT &nWw)
{
		dX = m_dX;
		dZ = m_dZ;
		m_dCondNum = m_dCondNum;
		dChiSquare = m_dChiSq;
		nIp = m_nIP;
		nWw = m_nWndWdth;;
		dSi = m_dSi;
}

void CResultEuler::SetSelected(bool sel)
{
	m_bSel = sel;
}
