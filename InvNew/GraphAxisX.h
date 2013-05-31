// GraphAxisX.h: interface for the CGraphAxisX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHAXISX_H__203DF37E_A494_4A28_A5F2_46A4D6203241__INCLUDED_)
#define AFX_GRAPHAXISX_H__203DF37E_A494_4A28_A5F2_46A4D6203241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphAxis.h"

class CGraphAxisX : public CGraphAxis  
{
DECLARE_SERIAL(CGraphAxisX)

friend class CGraphLine;
friend class CMeth2DLinWerner;

public:
	virtual void Properties(CDocument* pDoc);
	CGraphAxisX();
	CGraphAxisX( const char* strID, const char* strName, CPoint ptOrigo, int len, 
				ORIENTATION or, DATALIMITS datLim, const char* strTitle, BOOL bVisible = TRUE);
	virtual ~CGraphAxisX();

	virtual CString GetClassName() {return "CGraphAxisX";}
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

	virtual void SetBoundRectangle(CDC *pDC);
};

#endif // !defined(AFX_GRAPHAXISX_H__203DF37E_A494_4A28_A5F2_46A4D6203241__INCLUDED_)
