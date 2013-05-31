// WernerResults.h: interface for the CResultWerner class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>
#include <afxcoll.h>
#include "Result.h"

class CResultWerner : public CResult  
{
public:
	DECLARE_SERIAL(CResultWerner)
	double	m_dSusc;		// susceptibility
	double	m_dDip;			// dip of a dike
	double  m_dA;			// werner deconvolution parameter A
	double	m_dB;			// werner deconvolution parameter B
	
	CResultWerner()
	{
	}

	CResultWerner(double dPosX, double dPosZ, double	dSusc, double dDip, double dCondNum, 
		double dChiSq, double dA, double dB, int nWndIndex, int nWw, BYTE nIp, bool bVisible):
		CResult( dPosX, dPosZ, dCondNum, dChiSq, nWndIndex, nWw, nIp, bVisible)
	{
		m_dSusc = dSusc;
		m_dDip = dDip;
		m_dA = dA;
		m_dB = dB;
	}

	CResultWerner(const CResultWerner& wr)	//copy constructor
	{
		m_dX = wr.m_dX;
		m_dZ = wr.m_dZ;
		m_dSusc = wr.m_dSusc;
		m_dDip = wr.m_dDip;
		m_dCondNum = wr.m_dCondNum;
		m_dChiSq = wr.m_dChiSq;
		m_dA = wr.m_dA;
		m_dB = wr.m_dB;
		m_nWndIndex = wr.m_nWndIndex;
		m_nIP = wr.m_nIP;
		m_nWndWdth = wr.m_nWndWdth;;
		m_bShow = wr.m_bShow;
		m_bGrey = wr.m_bGrey;
		m_nPtClsW = wr.m_nPtClsW;
		m_dClsP = wr.m_dClsP;
		m_dClsP = wr.m_dClsP;
	}
	
	const CResultWerner& operator =(const CResultWerner& wr)
	{
		m_dX = wr.m_dX;
		m_dZ = wr.m_dZ;
		m_dSusc = wr.m_dSusc;
		m_dDip = wr.m_dDip;
		m_dCondNum = wr.m_dCondNum;
		m_dChiSq = wr.m_dChiSq;
		m_dA = wr.m_dA;
		m_dB = wr.m_dB;
		m_nWndIndex = wr.m_nWndIndex;
		m_nIP = wr.m_nIP;
		m_nWndWdth = wr.m_nWndWdth;;
		m_bShow = wr.m_bShow;
		m_bGrey = wr.m_bGrey;
		m_nPtClsW = wr.m_nPtClsW;
		m_dClsP = wr.m_dClsP;
		m_dClsP = wr.m_dClsP;
		return *this;
	}

	bool operator ==(const CResultWerner& wr) const
	{
		if((m_dX == wr.m_dX) &&	(m_dZ == wr.m_dZ) && (m_dSusc == wr.m_dSusc) &&
			(m_dDip == wr.m_dDip) && (m_dCondNum == wr.m_dCondNum) && (m_dChiSq == wr.m_dChiSq) &&	
			(m_dA == wr.m_dA) && (m_dB == wr.m_dB) && (m_nWndIndex == wr.m_nWndIndex) && (m_nIP == wr.m_nIP) && (m_nWndWdth == wr.m_nWndWdth) ) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	bool operator !=(const CResultWerner& wr) const
	{
		return !(*this == wr);
	}

	virtual ~CResultWerner();
	virtual void Serialize(CArchive& ar);
	void GetParameters(double &dPosX, double &dPosZ, double &dSusc, double &dDip, double &dCondNum, 
		double &dChiSq, double &dA, double &dB, BYTE &nIp, int nWwIndex, int &nWw);
	void ShowResult(bool visible = TRUE);
	CResultWerner* GetWernerResult() {return this;}

};

