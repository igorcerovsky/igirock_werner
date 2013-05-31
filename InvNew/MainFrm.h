// MainFrm.h : interface of the CMainFrame class
//


#pragma once
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	CStatusBar*	GetStatusBar() { return &m_wndStatusBar;}

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;

// Generated message map functions
protected:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCoordinateX(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCoordinateZ(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMethod(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCompute(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	void DrawProgress(double dCount, COLORREF color);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


