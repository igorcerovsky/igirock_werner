// InvView.h : interface of the CInvView class
//


#pragma once

#include "GraphAxisX.h"
#include "GraphAxisY.h"
#include "ScaleBar.h"
#include "InvDoc.h"



typedef struct ScOff {
	double	sc;
	double	off;				// vertical field scaling
} SCOFF;

typedef enum ScaleBar {
	SCB_NONE = 0,
	SCB_POS = 1,
	SCB_FLD = 2,
	SCB_DPT = 3
};

class CInvView : public CView
{
// frind classes 
	friend class CDlgClustering;
	friend class CDlgClusteringSmall;

protected: // create from serialization only
	CInvView();
	DECLARE_DYNCREATE(CInvView)

// Attributes
public:
	CInvDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CInvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CInvDoc* m_pDoc;		// pointer to document
	bool m_bInitialized;	// init tag
	CRect m_rectDrw;		// drawing rectangle
	CRect m_rectFld;		// drawing field
	CRect m_rectInv;		// drawing inversion reults
	int	  m_nMiddle;		// window spliter

	// axes
	CGraphAxisX	m_axX;		// xaxis
	CGraphAxisY	m_axFldY;	// field axis	
	CGraphAxisY	m_axDptY;	// depth axis
	int m_nFldAxis;			// axis for fld scale

	// scale bar
	CMyScaleBar m_scBarHrz;			// position scale bar
	CMyScaleBar m_scBarVrtFld;		// verticald field scale bar
	CMyScaleBar m_scBarVrtDpt;		// vetrical deph bar
	CMyScaleBar m_scBarVrtVirtual;	// vertical scale bar virtual fo all fields which are not currently selected
	BOOL		m_bHScBarClk;
	BOOL		m_bVScBarClk;
	int			m_nScBarCaptured;
	BOOL		m_bResize;
	double		m_dResize;

	// scales  & offset
	ScOff	m_scOffX;		// scale & offset x axis
	ScOff	m_scOffYFld;	// scale & offset y axis field
	ScOff	m_scOffYDpt;	// scale & offset y axis depth
	BOOL	m_bScaleEq;		// equal scale for length an depth axes
	double	m_dScEq;

	// current profile
	int m_nCurProf;

	//
	CBitmap m_bmpBkg;

	// field colors
	CArray<CPen, CPen&> m_fldPen;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnInitialUpdate();
	bool Scale(void);
	void DrawAxes(CDC* pDC);
	int DrawField(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewZoom();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomPage();
	void Zoom();
	void ZoomIn(CPoint pt);
	void ZoomOut(CPoint pt);
	void DrawMethod(CDC* pDC);
	int SetScBarRange(CMyScaleBar* pScBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMethodClustering();
	afx_msg void OnUpdateMethodClustering(CCmdUI *pCmdUI);
	afx_msg void OnViewRedraw();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetIndicators(CPoint lPt);
	void SetIndicators();
	void CInvView::DrawBmpBkg(CDC *pDC);
	void ReSize(void);	
	int ResizeDrwRect(CPoint lPt);
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int DrawLegend(CDC* pDC, int i, CString str);
};

#ifndef _DEBUG  // debug version in InvView.cpp
inline CInvDoc* CInvView::GetDocument() const
   { return reinterpret_cast<CInvDoc*>(m_pDocument); }
#endif

