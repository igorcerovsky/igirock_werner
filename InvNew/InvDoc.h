// InvDoc.h : interface of the CInvDoc class
//


#pragma once

#include "Database.h"
#include "Method2D.h"
#include "Method2D.h"

int ThreadClustering(LPVOID pParam);

class CInvDoc : public CDocument
{
friend class CInvView;

protected: // create from serialization only
	CInvDoc();
	DECLARE_DYNCREATE(CInvDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CInvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// profile
	CDatabase	m_dtb;
	CString		m_strElev;		// elevation channel name
	CString		m_strPos;		// position channel name
	CString		m_strFld;		// field channel name for scale
	int			m_nCurProf;
	MethodArray m_methArray;
	int			m_nMethID;

	// selected method
	CMeth2DLin *m_pMethSel;

	// computing thread
	CWinThread *m_pThread;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDatabaseImport();
	afx_msg void OnUpdateDatabaseImport(CCmdUI *pCmdUI);
	bool IsEmpty(void);
	bool IsCurProfileOK(void);
	CDtbProf* GetCurProf(void);
	void UpdateAllViews(void);
	afx_msg void OnMethodWerner();
	void SelectChannels(void);
	afx_msg void OnViewSetChannels();
	afx_msg void OnUpdateViewSetChannels(CCmdUI *pCmdUI);
	afx_msg void OnEditRemovehiden();
	afx_msg void OnUpdateEditRemovehiden(CCmdUI *pCmdUI);
	afx_msg void OnEditMethod();
	afx_msg void OnUpdateEditMethod(CCmdUI *pCmdUI);

	CMeth2DLin* GetSelMethod()				{ return m_pMethSel;	}
	void SelectMethod(CMeth2DLin* pMeth)	{ m_pMethSel = pMeth;	}
	afx_msg void OnMethodExport();
	afx_msg void OnUpdateMethodExport(CCmdUI *pCmdUI);
	afx_msg void OnDatabaseProfileFirst();
	afx_msg void OnUpdateDatabaseProfileFirst(CCmdUI *pCmdUI);
	afx_msg void OnDatabaseProfileLast();
	afx_msg void OnUpdateDatabaseProfileLast(CCmdUI *pCmdUI);
	afx_msg void OnDatabaseProfileNext();
	afx_msg void OnUpdateDatabaseProfileNext(CCmdUI *pCmdUI);
	afx_msg void OnDatabaseProfilePrev();
	afx_msg void OnUpdateDatabaseProfilePrev(CCmdUI *pCmdUI);
	bool SetCurrentProfile(int n);
	afx_msg void OnDatabaseCompute();
	afx_msg void OnUpdateDatabaseCompute(CCmdUI *pCmdUI);
	afx_msg void OnEditDeleteMethod();
	afx_msg void OnUpdateEditDeleteMethod(CCmdUI *pCmdUI);
	int Clustering(CMeth2DLin* pMeth);
	virtual void OnCloseDocument();
	void	ThreadFinished(void);

	afx_msg void OnMethodFilterResults();
	afx_msg void OnUpdateMethodFilterResults(CCmdUI *pCmdUI);
};

struct ComputeStrct {
	CMeth2DLin*	pMethod;
	CInvDoc*	pDoc;
};




