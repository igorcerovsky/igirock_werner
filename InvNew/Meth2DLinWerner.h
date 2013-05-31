// Meth2DLinWerner.h: interface for the CMeth2DLinWerner class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

int ThreadWerner(LPVOID pParam);

#include "Method2D.h"
#include "WernerResults.h"
#include "InducedField.h"

typedef enum WernerType{
	WERNER_NORMAL,
	WERNER_VARALT
} WERNERTYPE;

class CMeth2DLinWerner : public CMeth2DLin  
{
friend class CDlgClustering;

DECLARE_SERIAL(CMeth2DLinWerner)

public:
	bool IsEmpty();
	virtual void Info();
	CMeth2DLinWerner();
	CMeth2DLinWerner(const char* strID, const char* strName, MMSI ip, MMSI ww, MINMAXD depth, MMSD prof, bool bVisible = TRUE);
	virtual ~CMeth2DLinWerner();

	virtual CString GetClassName() {return "CMeth2DLinWerner";}
	virtual void	Serialize(CArchive &ar);
	virtual bool	IsCaptured(CPoint pt);
	//virtual void	Draw(CDC* pDC, double scx, double offx, double sxy, double offy);
	virtual void	Properties(CDocument* pDoc);

	virtual bool Compute();
	virtual void FilterResults(FILTSTRCT filtRes);
	virtual void Export();
	virtual bool RemoveSolution(int nIndex = -1);
	virtual bool ShowSolution(int nIndex = -1, bool bShow = TRUE);	// show or hide solution
	//virtual bool RemoveSolutions(bool bHiden = TRUE);
	virtual bool ShowAllSolutions();
	virtual int  GetSolCount();

	void ComputeDip(double bb, double strike);
	void ComputeWernerVarAlt();
	void ComputeWerner();
	void SetInducedField(CInducedField IF)	{ m_InducedField = IF;	}

protected:
	int				m_nWrType;
	CInducedField	m_InducedField;

protected: //functions
	void ComputeWernerRusults(double a[], double w[], int nu, double dChiSquare, BYTE ip, int ww, int ind);
	void ComputeWernerRusultsVarAlt(double a[], double w[], int nu, double dChiSquare, 
		BYTE ip, int ww, int ind);		// nu is number of uknowns
	void BaseFuncs(double x, double y, double &b, double p[], int np);
	void BaseFuncs(double x, double y, double z, double &b, double p[], int np);
	void SvdFit(double x[], double y[], double z[], double sig[], int ndata, double a[], int ma, double **u, double **v, double w[], double *chisq, double *b, double *afunc);

private:
	double m_dElevShift;	// shift maximum elevation to origo
public:
};

