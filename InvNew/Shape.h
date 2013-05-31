// Shape.h: interface for the CShape class.
// wrap class for all graphical shapes or methods
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "afxtempl.h"

typedef struct DataLimitsTag{
	double min;			// Axis Minimum 
	double max;			// Axis Maximum
	double TickStep;	// Step
} DATALIMITS;

class CShape : public CObject  
{
	DECLARE_SERIAL(CShape)

public:
	CShape()
	{
		m_nShpBound = 200;	// 2 mm
		m_bVisible = TRUE;

		m_logPen.lopnColor = RGB(0, 0, 0);
		m_logPen.lopnStyle = PS_SOLID;
		m_logPen.lopnWidth = CPoint(0,0);
	}

	CShape(const char* strID) : m_strID(strID)
	{
		m_bVisible = TRUE;
		m_nShpBound = 200;	// 2 mm

		m_logPen.lopnColor = RGB(0, 0, 0);
		m_logPen.lopnStyle = PS_SOLID;
		m_logPen.lopnWidth = CPoint(0,0);
	}

	CShape(const char* strID, const char* strName) : m_strID(strID), m_strName(strName)
	{
		m_bVisible = TRUE;
		m_nShpBound = 200;	// 2 mm

		m_logPen.lopnColor = RGB(0, 0, 0);
		m_logPen.lopnStyle = PS_SOLID;
		m_logPen.lopnWidth = CPoint(0,0);
	}

	CShape(const char* strID, const char* strName, BOOL bVisible) : m_strID(strID), m_strName(strName)
	{
		m_bVisible = bVisible;
		m_nShpBound = 200;	// 2 mm

		m_logPen.lopnColor = RGB(0, 0, 0);
		m_logPen.lopnStyle = PS_SOLID;
		m_logPen.lopnWidth = CPoint(0,0);
	}

	CShape(	const char* strID, const char* strName, CPoint ptUL, CPoint ptLR, BOOL bVisible) : m_strID(strID), m_strName(strName)
	{
		m_ptUL = ptUL;
		m_ptLR = ptLR;
		m_bVisible = bVisible;
		m_nShpBound = 200;	// 2 mm

		m_logPen.lopnColor = RGB(0, 0, 0);
		m_logPen.lopnStyle = PS_SOLID;
		m_logPen.lopnWidth = CPoint(0,0);
	}

	virtual ~CShape();

public:
	virtual void	SetBoundRect(CDC* pDC);
	virtual void	Serialize(CArchive &ar);
	virtual void	Draw(CDC* pDC);
	virtual CString GetClassName()	{return "CShape";}
	virtual void	Move(CPoint pt);
	virtual void	MoveTo(CPoint pt);
	virtual void	Properties(LPVOID pParam);
	virtual BOOL	IsCaptured(CPoint pt);
	virtual void	Info() {;}

	CString GetID()						{return m_strID;}
	void	SetID(CString strID)		{m_strID = strID;}
	CString GetName()					{return m_strName;}
	void	SetName(CString strName)	{m_strName = strName;}
	void	SetVisible(BOOL bVisible = TRUE)	{m_bVisible = bVisible;}
	BOOL	IsVisible()					{return m_bVisible;}
	CRect	GetBoundRect()				{return m_boundRect;}

protected:		// variables
	CString		m_strID;
	CString		m_strName;

	CPoint		m_ptOrigo;		// Origo
	CPoint		m_pt;			// centroid
	CPoint		m_ptUL;			// Upper Left corner of bounding rectangle
	CPoint		m_ptLR;			// Lower Right corner of bounding rectangle
	CRect		m_boundRect;	// bounding rectangle
	LOGPEN		m_logPen;		// defines pen for drawing
	//LOGFONT		m_logFont;		// logfont structure, taht defines font for drawing
	BOOL		m_bVisible;
	int			m_nShpBound;		// bounding extend aroun shape, for mouse selection
	
};

typedef CTypedPtrList<CObList, CShape*> ShapeList;

