// Result.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "Result.h"


// CResult
IMPLEMENT_SERIAL(CResult, CObject, 0)

CResult::CResult()
{
}

CResult::~CResult()
{
}


// CResult member functions

void CResult::Serialize(CArchive& ar)
{
    if( ar.IsStoring() ) {
		ar << m_dX << m_dZ << 
			m_dCondNum << m_dChiSq << 
			m_dClsN << m_dClsP <<
			m_nWndIndex << m_nWndWdth << 
			m_nPtClsW << m_nPtClsA 
			<< m_nIP 
			<< m_bShow
			<< m_bGrey;
	}
    else {
		ar >> m_dX >> m_dZ >> 
			m_dCondNum >> m_dChiSq >> 
			m_dClsN >> m_dClsP >>
			m_nWndIndex >> m_nWndWdth >> 
			m_nPtClsW >> m_nPtClsA 
			>> m_nIP 
			>> m_bShow
			>> m_bGrey;
	}
}
