// EulerResults.h: interface for the CResultEuler class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>

class CResultEuler : public CObject  
{
public:	// variables
	DECLARE_SERIAL(CResultEuler)
	double	m_dX;		// x coordinate of a dike
	double	m_dZ;		// z coordinate of a dike
	double	m_dCondNum;	// condition number
	double	m_dChiSq;	// Chi saquare	
	double	m_dSi;
	int		m_nIP;			// interference polynomial
	int		m_nWndWdth;;
	int		m_nPtInClus;
	bool	m_bVisible;
	bool	m_bSel;

public: // functions
	CResultEuler();

	virtual ~CResultEuler();

	CResultEuler(double dX, double dZ, double dCondNum, double dChiSquare, 
		double dWwsp, double dWwep, UINT nIp, double dSi, UINT nWw)
	{
		m_dX = dX;
		m_dZ = dZ;
		m_dCondNum = dCondNum;
		m_dChiSq = dChiSquare;
		m_nIP = nIp;
		m_nWndWdth = nWw;
		m_bVisible = TRUE;
		m_nPtInClus = 0;
		m_dSi = dSi;
		m_bSel = FALSE;
	}

	CResultEuler(const CResultEuler& er)	//copy constructor
	{
		m_dX = er.m_dX;
		m_dZ = er.m_dZ;
		m_dCondNum = er.m_dCondNum;
		m_dChiSq = er.m_dChiSq;
		m_nIP = er.m_nIP;
		m_nWndWdth = er.m_nWndWdth;;
		m_dSi = er.m_dSi;
	}
	
	const CResultEuler& operator =(const CResultEuler& er)
	{
		m_dX = er.m_dX;
		m_dZ = er.m_dZ;
		m_dCondNum = er.m_dCondNum;
		m_dChiSq = er.m_dChiSq;
		m_nIP = er.m_nIP;
		m_nWndWdth = er.m_nWndWdth;;
		m_bVisible = er.m_bVisible;
		m_nPtInClus = er.m_nPtInClus;
		m_dSi = er.m_dSi;
		return *this;
	}

	bool operator ==(const CResultEuler& er) const
	{
		if((m_dX == er.m_dX) &&	(m_dZ == er.m_dZ) && (m_dCondNum == er.m_dCondNum) && 
			(m_dChiSq == er.m_dChiSq) && (m_nIP == er.m_nIP) && 
			(m_nWndWdth == er.m_nWndWdth) && (m_dSi == er.m_dSi)) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	bool operator !=(const CResultEuler& er) const
	{
		return !(*this == er);
	}

	virtual void Serialize(CArchive &ar);

	void GetParam(double &dX, double &dZ, double &dCondNum, double &nChiSquare, 
		double &dWwsp, double &dWwep, UINT &nIp, double &dSi, UINT &nWw);
	
	CResultEuler* GetEulerResult() {return this;}
	
	void ShowResult(bool visible = TRUE) {m_bVisible = visible;}

	void SetSelected(bool sel = TRUE);

};

typedef CTypedPtrArray<CObArray, CResultEuler*> EulResArray;

