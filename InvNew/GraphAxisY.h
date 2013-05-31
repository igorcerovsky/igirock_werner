// GraphAxisY.h: interface for the CGraphAxisY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHAXISY_H__3CCD033F_5BA1_49A5_8FDC_35569BB170EA__INCLUDED_)
#define AFX_GRAPHAXISY_H__3CCD033F_5BA1_49A5_8FDC_35569BB170EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphAxis.h"

class CGraphAxisY : public CGraphAxis  
{
DECLARE_SERIAL(CGraphAxisY)

friend class CGraphLine;
friend class CMeth2DLinWerner;

public:
	virtual void Properties(CDocument* pDoc);
	CGraphAxisY();
	CGraphAxisY( const char* strID, const char* strName, CPoint ptOrigo, int len, 
				ORIENTATION or, DATALIMITS datLim, const char* strTitle, BOOL bVisible = TRUE);
	virtual ~CGraphAxisY();

	virtual CString GetClassName() {return "CGraphAxisY";}
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

	virtual void SetBoundRectangle(CDC* pDC);
};

#endif // !defined(AFX_GRAPHAXISY_H__3CCD033F_5BA1_49A5_8FDC_35569BB170EA__INCLUDED_)
