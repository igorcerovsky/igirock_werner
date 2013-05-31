// WernerResults.cpp: implementation of the CWernerResults class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WernerResults.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CResultWerner, CResult, 0)

CResultWerner::~CResultWerner()
{

}

void CResultWerner::Serialize(CArchive& ar)
{
	//TRACE("Entering CResultWerner::Serialize\n");

	CResult::Serialize( ar );
    if( ar.IsStoring() ) {
		ar << m_dSusc << m_dDip << m_dA << m_dB;
	}
    else {
		ar >> m_dSusc >> m_dDip >> m_dA >> m_dB;
	}
}

void CResultWerner::ShowResult(bool visible)
{
	m_bShow = visible;
}

void CResultWerner::GetParameters(double &dPosX, double &dPosZ, double	&dSusc, double &dDip, double &dCondNum, 
		double &dChiSq, double &dA, double &dB, BYTE &nIp, int nWwIndex, int &nWw)
{
		dPosX = m_dX;
		dPosZ = m_dZ;
		dSusc = m_dSusc;
		dDip = m_dDip;
		dCondNum = m_dCondNum;
		dChiSq = m_dChiSq;
		dA = m_dA;
		dB = m_dB;
		nWwIndex = m_nWndIndex;
		nIp = m_nIP;
		nWw = m_nWndWdth;;
}

