// GraphAxis.h: interface for the CGraphAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHAXIS_H__B2BB108B_1365_4CE2_8970_F311108F128A__INCLUDED_)
#define AFX_GRAPHAXIS_H__B2BB108B_1365_4CE2_8970_F311108F128A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shape.h"

typedef enum OrientationTag{
	AXOR_NORMAL = 1,
	AXOR_REVERSE = -1
} ORIENTATION;

typedef struct TickLabeTag{
	BOOL	bT;			// visible tick
	int		nTlen;		// tick length - axis
	BOOL	bL;			// Label visible  
	int		nLOffsetX;	// Label Offset in axis direction x
	int		nLOffsetY;	// Label Offset in axis direction y
	int		nLDecimals;	// Label decimal digits
	ORIENTATION axOr;	// tick and label orientation i.e. left from y axis, up from x axis
	// temporary
} TICKLABEL;

typedef struct TitlTag{
	CString strTitle;	// axis title
	int		nOffsetX;	// text offsets
	int		nOffsetY;
} TITLE;

typedef struct TickTag{
	double	start;	// start value
	double	step;	// step between major ticks
	int		n;		// number ao major ticks
} TICKTAG;


typedef struct AxisParamTag{
	ORIENTATION	Orientation;	// axis orientation
	double Scale;				// scale and offset for graph plot
	double Offset;
} AXISPARAM;


class CGraphAxis : public CShape  
{
DECLARE_SERIAL(CGraphAxis)

friend class CDlgAxisProperties;

public:
	CGraphAxis();
	CGraphAxis(const char* strID, const char* strName, CPoint ptOrigo, int len, 
		ORIENTATION or, DATALIMITS datLim, BOOL bVisible);
	virtual ~CGraphAxis();
	virtual CString GetClassName()	{return "CGraphAxis";}
	virtual void Serialize(CArchive &ar);
	virtual void Scale();
	void FindAxisMinMaxStep(DWORD n = 6);
	void FindDummyStep(DWORD n);

	AXISPARAM	GetParam(CPoint &ptOrigo, int &len);	
	void		SetParam(CPoint ptOrigo, int len);
	DATALIMITS	GetDatLim()							{return m_datLim;}
	void		SetDatLim(DATALIMITS datLim);
	TICKLABEL	GetMajorTL()						{return m_majorTL;}
	void		SetMajorTL(TICKLABEL majorTL)		{m_majorTL = majorTL;}
	TICKLABEL	GetMinorTL()						{return m_minorTL;}
	void		SetMinorTL(TICKLABEL minorTL)		{m_minorTL = minorTL;}
	TITLE		GetTitle()							{return m_title;}
	void		SetTitle(TITLE title)				{m_title = title;}
	void		SetAxOrientation(ORIENTATION or)	{m_Orientation = or;}
	void		SetTickLabelOrientation(ORIENTATION or);
	double		GetScale()							{return m_dScale;}
	double		GetOffset()							{return m_dOffset;}

protected:
	int			m_len;			// axis length
	double		m_dScale;		
	double		m_dOffset;
	ORIENTATION	m_Orientation;	// axis orientation
	DATALIMITS	m_datLim;		// axis min, max
	TICKTAG		m_tick;		// axis ticks, minimum tick value, number of ticks, step between them
	TICKLABEL	m_majorTL;		// major ticks and asociated labels
	TICKLABEL	m_minorTL;		// minor ticks and asociated labels	
	TITLE		m_title;		// axis title
	LOGFONT		m_logFontML;	// logfont structure, taht defines font for drawing major label
	LOGFONT		m_logFontNL;	// logfont structure, taht defines font for drawing minor label
	LOGFONT		m_logFontTitle;	// logfont structure, taht defines font for drawing title
	int			m_al;			// Arrow Length in points
	int			m_aw;			// Arrow Width in points
	int			m_all;			// Axix length + all for arrow not to be on tick in points

protected:	// functions
	void DoubleToStr(CString& str, double x, int decimals);

};

#endif // !defined(AFX_GRAPHAXIS_H__B2BB108B_1365_4CE2_8970_F311108F128A__INCLUDED_)
