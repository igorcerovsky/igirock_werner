// 2DMetLinEuler.h: interface for the CMeth2DLinEuler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DMETLINEULER_H__14C620EA_11B0_447A_BCC0_24BF327DB3E5__INCLUDED_)
#define AFX_2DMETLINEULER_H__14C620EA_11B0_447A_BCC0_24BF327DB3E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Method2D.h"
#include "EulerResult.h"

class CMeth2DLinEuler : public CMeth2DLin
{
public:
	DECLARE_SERIAL(CMeth2DLinEuler)

	CMeth2DLinEuler()
	{
		m_nSel = -1;
	}

	virtual ~CMeth2DLinEuler()
	{
	}

	CMeth2DLinEuler(const char* strID, const char* strSolName) : CMeth2DLin(strID, strSolName) 
	{
		m_nSel = -1;
	}

	CMeth2DLinEuler(const char* strID, const char* strName, STARTSTEPEND ip, STARTSTEPEND ww, BOOL bVisible = TRUEdouble dSiStart, double dSiEnd, double dSiStep) 
		: CMeth2DLin(strID, strSolName, strName, profStart, profEnd,
		minDepth, maxDepth, nIpStart, nIpEnd, nWWStart, nWWEnd, nWWStep)
	{
		m_dSiStart = dSiStart;
		m_dSiEnd   = dSiEnd;
		m_dSiStep  = dSiStep; 
		m_nSel = -1;
	}
	
	//++++++++++++++ end constructors, destructor ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	CMeth2DLinEuler(const CMeth2DLinEuler &eul) : C2DMetLin(eul.m_strID, eul.m_strName,
		eul.m_prof.min, eul.m_prof.max,	eul.m_depth.min, eul.m_depth.max, eul.m_ip.min, eul.m_ip.max,
		eul.m_ww.min, eul.m_ww.max, eul.m_ww.step)
	{

	}

	const CMeth2DLinEuler operator =(const CMeth2DLinEuler &eul)
	{
		m_strName = eul.m_strName;			// name of the profile from which method is computed
		m_depth.max = eul.m_depth.max;		// max depth solution assumed
		m_depth.min = eul.m_depth.min;		// minimum depth solution assumed
		m_prof.min = eul.m_prof.min;	// profile start 
		m_prof.max = eul.m_prof.max;
		m_ip.min = eul.m_ip.min;		// interfernce polynomial options
		m_ip.max = eul.m_ip.max;
		m_ww.min = eul.m_ww.min;		// window width options	
		m_ww.max = eul.m_ww.max;
		m_ww.step = eul.m_ww.step;
		m_dClusRadius = eul.m_dClusRadius;
		m_dSiStart = eul.m_dSiStart;
		m_dSiEnd   = eul.m_dSiEnd;
		m_dSiStep  = eul.m_dSiStep; 
						
		int n = m_results.GetSize();
		for(int i = 0; i< n; i++) {
			CResultEuler* pEul = new CResultEuler();
			*pEul = *(eul.m_results[i]);
			m_results.Add(pEul);
		}

		return *this;
	}

	virtual void Serialize(CArchive &ar);

	virtual CString GetClassName() {return "2DMetLinEuler";}

	EulResArray* GetEulResArray()	{return &m_results;}

	void ComputeEuler(CList<double, double>* px, CList<double, double>* pz, 
		CList<double, double>* pf, CList<double, double>* pfdx, CList<double, double>* pfdz, double h = 0);

	void Clustering(double ClusterRadius);
	
	int GetMaxPtInClus() {return m_nMaxPtInClus;}

	void FilterResults(double dCondNum, double dMinDepth, double dMaxDepth, BYTE nIpEnd,
			BYTE nIpStart, int nWwEnd, double dWEndPos, int nWwStart, double dWStartPos, double dSiStart, double dSiEnd);

	BOOL Export(CString strFileName, double dMinDepth, double dMaxDepth, 
							 int nIpStart, int nIpEnd, double dWStartPos, double dWEndPos, 
							 int nWwStart, int nWwEnd, int nMinPtInClus, int nMaxPtInClus, 
							 double dSiStart, double dSiEnd, double dCondNum);

	double GetSiStart() {return m_dSiStart;}
	double GetSiEnd() {return m_dSiEnd;}
	double GetSiStep() {return m_dSiStep;}

	double GetMinCond() {return m_dMinCond;}
	double GetMaxCond() {return m_dMaxCond;}
	double GetMaxChi() {return m_dMaxChi;}
	double GetMinChi() {return m_dMinChi;}

	int	 GetSel() {return m_nSel;}
	void SetSel(int nSel) {m_nSel = nSel;}


protected:	// variables
	EulResArray m_results;
	double m_dClusRadius;
	int m_nMaxPtInClus;
	int m_nSel;	// selected result (-1) is reserved for non selection

	double m_dSiStart;	// structural index
	double m_dSiEnd;
	double m_dSiStep;

	double m_dMaxCond;
	double m_dMinCond;
	double m_dMaxChi;
	double m_dMinChi;

protected:	// functions
	void SvdFit(double x[],double z[], double f[], double fdx[], double fdz[], double sig[], int ndata, double a[], int ma,	double **u, double **v, double w[], double *chisq, double si);
	virtual void BaseFuncs(double fdx, double fdz, double p[], double si, int np);
	void ComputeEulerRusults(double a[], double w[], int nu, double dChiSquare, 
		double spos, double epos, int ip, double dSi, int ww, double **cvm);

};

#endif // !defined(AFX_2DMETLINEULER_H__14C620EA_11B0_447A_BCC0_24BF327DB3E5__INCLUDED_)
