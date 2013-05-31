#pragma once

#include <afxtempl.h>
#include <afxcoll.h>

// CResult command target

class CResult : public CObject
{
public:
	DECLARE_SERIAL(CResult)
	double	m_dX;			// x coordinate of a dike
	double	m_dZ;			// z coordinate of a dike
	double	m_dCondNum;		// condition number
	double	m_dChiSq;		// Chi saquare	
	double	m_dClsN;		// clustering number normal clustering
	double	m_dClsP;		// clustering number power clustering
	int		m_nIP;			// interference polynomial
	int		m_nWndIndex;	// window start index
	int		m_nWndWdth;		// window width
	int		m_nPtClsW;		// number of points in cluster -> normal clustering only same window width
	int		m_nPtClsA;		// number of points in cluster -> normal clustering all solutions
	BOOL	m_bShow;		// show or not for filtering
	BOOL	m_bGrey;		// is drawn grey

public:
	CResult();
	virtual ~CResult();
	CResult(double dPosX, double dPosZ, double dCondNum, 
		double dChiSq, int nWndIndex, int nWw, BYTE nIp, bool bVisible)
	{
		m_dX = dPosX;
		m_dZ = dPosZ;
		m_dCondNum = dCondNum;
		m_dChiSq = dChiSq;
		m_dClsN = 0;
		m_dClsP = 0;
		m_nIP = nIp;
		m_nWndIndex = nWndIndex;
		m_nWndWdth = nWw;
		m_nPtClsW = 0;
		m_nPtClsA = 0;
		m_bShow = bVisible;
		m_bGrey = FALSE;
	}

	CResult(const CResult& res)	//copy constructor
	{
		m_dX = res.m_dX;
		m_dZ = res.m_dZ;
		m_dCondNum = res.m_dCondNum;
		m_dChiSq = res.m_dChiSq;
		m_dClsN = res.m_dClsN;
		m_dClsP = res.m_dClsP;
		m_nIP = res.m_nIP;
		m_nWndIndex = res.m_nWndIndex;
		m_nWndWdth = res.m_nWndWdth;;
		m_nPtClsW = res.m_nPtClsW;
		m_nPtClsA = res.m_nPtClsA;
		m_bGrey = res.m_bGrey;
		m_bShow = res.m_bShow;
	}
	
	const CResult& operator =(const CResult& res)
	{
		m_dX = res.m_dX;
		m_dZ = res.m_dZ;
		m_dCondNum = res.m_dCondNum;
		m_dChiSq = res.m_dChiSq;
		m_dClsN = res.m_dClsN;
		m_dClsP = res.m_dClsP;
		m_nIP = res.m_nIP;
		m_nWndIndex = res.m_nWndIndex;
		m_nWndWdth = res.m_nWndWdth;;
		m_nPtClsW = res.m_nPtClsW;
		m_nPtClsA = res.m_nPtClsA;
		m_bGrey = res.m_bGrey;
		m_bShow = res.m_bShow;
		return *this;
	}

	bool operator ==(const CResult& res) const
	{
		if( (m_dX == res.m_dX) &&	(m_dZ == res.m_dZ) &&
			(m_dCondNum == res.m_dCondNum) && (m_dChiSq == res.m_dChiSq) &&
			(m_nWndIndex == res.m_nWndIndex) && (m_nWndWdth == res.m_nWndWdth) && 
			(m_nIP == res.m_nIP) ) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	bool operator !=(const CResult& res) const
	{
		return !(*this == res);
	}
	void Show(bool bShow = true) {m_bShow = bShow;}

	virtual void Serialize(CArchive& ar);
};

typedef CTypedPtrArray<CObArray, CResult*> ResArray;


