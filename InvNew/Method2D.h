// Method2D.h: interface for the CMeth2DLin class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "afx.h"
#include <afxtempl.h>
#include "Result.h"

typedef enum MethodType {
	METH_NONE = 0,
	METH_WERNER = 1,
	METH_EULER = 2,
};

typedef enum ClusteringType {
	CLST_NONE = -1,
	CLST_NORMAL = 0,
	CLST_POWER = 1,
};

typedef struct FilterStruct {
	int		nTag;
	int		nWwMin;
	int		nWwMax;
	int		nIpMin;
	int		nIpMax;
	double	dWwMin;
	double	dWwMax;
	double	dDptMin;
	double	dDptMax;
	double	dCondMin;
	double	dCondMax;
	double	dChiMin;
	double	dChiMax;
} FLTRES;

class CMeth2DLin : public CObject  
{

// friend class CDlgClustering;
// friend class CDlgClusteringR;
// friend class CDlgMethodInfo;

DECLARE_SERIAL(CMeth2DLin)

public:
	CMeth2DLin();
	CMeth2DLin(const char* strID, const char* strName, MMSI ip, MMSI ww, MINMAXD depth, MMSD prof, bool bVisible = TRUE);
	virtual ~CMeth2DLin();

	virtual void Serialize(CArchive &ar);
	virtual CString GetClassName()	{return "CMeth2DLin";}

	// pure virtual method functions, override it 
	virtual bool Compute()													{ return TRUE;	}
	virtual int	 Clustering();
	virtual int FilterResults(FLTRES flt);
	virtual void Export()													{;}
	virtual bool RemoveSolution(int nIndex = -1)							{ return TRUE;	}
	virtual bool ShowSolution(int nIndex = -1, bool bShow = TRUE)			{ return TRUE;	}	// show or hide solution
	virtual bool RemoveSolutions(bool bHiden = true);
	virtual bool ShowAllSolutions()											{ return TRUE;	}
	virtual int	 GetSolCount()												{ return 0;		}
	virtual FILTSTRCT GetFiltInfo();
	virtual void	Draw(CDC* pDC, double scx, double offx, double sxy, double offy);
	virtual int ShowSolutions(double dTresh = 0.7);
	

	void	SetDataPointers(double *pPosX, double *pElev, double *pFld, double *pDrvX, double *pDrvZ, int n);
	void	NullDataPointers();
	
	MINMAXI	GetPtInClus()				{ return m_ptInCls;		}
	MINMAXD	GetClusNum()				{ return m_clsNum;		}
	void	SetCurSol(int curSol)		{ m_nCurSol = curSol;	}
	int		GetCurSol()					{ return	m_nCurSol;	}

	MMSI	GetIP()				{ return m_ip; }
	void	SetIP(MMSI ip)		{ m_ip = ip; }	
	MMSI	GetWndWdht()		{ return m_ww; }
	void	SetWndWdht(MMSI ww)		{ m_ww = ww; }

	MINMAXD	GetChiMinMax()		{ return m_chi;}
	MINMAXD	GetCondMinMax()		{ return m_cond;}
	MINMAXD	GetDepthMinMax()	{ return m_depth;}

	void	SetProfID(CString strID)	{ m_strPfofID = strID;	}
	CString	GetProfID()					{ return m_strPfofID;	}
	int		GetMethod()					{ return m_nMethod;		}
	bool	IsVisible()					{ return m_bVisible;	}
	void	Show(bool bShow = true)		{ m_bVisible = bShow;	}

	int		GetClstType()				{ return m_nClstType;	}
	void	SetClstType(int nType)		{ m_nClstType = nType;	;}

	double GetClstTrashN()				{ return m_dTrashN ;	}
	void   SetClstTrashN(double dTrash) { m_dTrashN = dTrash;	}
	double GetClstTrashP()				{ return m_dTrashP ;	}
	void   SetClstTrashP(double dTrash) { m_dTrashP = dTrash;	}
	ResArray* GetWerResArray()			{ return &m_results;	}

	CString	GetName()					{ return m_strName;		}
	void	SetName( CString strName)					{ m_strName = strName;	}
	void	SetClusteringPram(int nType= CLST_NORMAL, double dRad=1, double dExp = 2.0) { m_nClstType = nType; SetClsRadius( dRad); m_dClsExp = dExp; }


protected:
	bool		m_bVisible;
	CString		m_strID;		// method ID
	CString		m_strName;		// method name
	CString		m_strPfofID;	// profile ID from which the results are computed
	int			m_nMethod;		// method identification

	// data array TEMPORARY!
	double	*m_pPosX;		// position channel
	double	*m_pFld;		// field !!! could be dT or derivative
	double	*m_pDrvX;		// derivative x
	double	*m_pDrvZ;		// derivative z
	double	*m_pElev;		// elevation
	int		m_nPts;			// number of points in data arrays

	MMSI m_ip;			// interference polynomial
	MMSI m_ww;			// window width

	MINMAXD	m_depth;	// min and max depth solution assumed
	MMSD	m_prof;		// profile start end step

	MINMAXD m_cond;		// condition number
	MINMAXD m_chi;		// chi square

	int		m_nClstType;	// clustering type
	double	m_dClsRadN;		// cluster radius - normal clustering
	double	m_dClsRadL;		// cluster radius - local aspect
	double	m_dClsRadP;		// cluster radius - power clustering
	double	m_dClsExp;		// power clustering exponent
	double	m_dTrashN;		// clustering treshold
	double	m_dTrashP;		// clustering treshold

	MINMAXD	m_clsNum;		// minimum and maximum clustering number
	MINMAXI	m_ptInCls;		// minimum and maximum poits in clusters
	int		m_nCurSol;		// selected solution

	bool	m_bElevation;	// draw elevation ?
	ResArray	m_results;

	COLORREF	m_color;
public:
	double GetTrash(void);
	void SetTrash(double dTrash);
	double GetClsRadius(void);
	void SetClsRadius(double dRad);
	double GetClsExp()			{ return m_dClsExp; }
	void SetClsExp(double dExp)	{ m_dClsExp = dExp; }
	void SetColor(COLORREF color)	{ m_color = color;}
	COLORREF GetColor()				{ return m_color;}
	virtual void Properties(void);
};

typedef CTypedPtrArray<CObArray, CMeth2DLin*> MethodArray;