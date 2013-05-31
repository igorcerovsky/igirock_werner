#if !defined(AFX_DLGDTBIMPORT_H__7FA4364A_DAE0_494D_B6E1_C344281E5863__INCLUDED_)
#define AFX_DLGDTBIMPORT_H__7FA4364A_DAE0_494D_B6E1_C344281E5863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDtbImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDtbImport dialog

#include "afxtempl.h"

class CDlgDtbImport : public CDialog
{
// Construction
public:
	CDlgDtbImport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDtbImport)
	enum { IDD = IDD_DLG_DTB_IMPORT };
	CListBox	m_listChan;
	CString	m_strChanName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDtbImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDtbImport)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListChan();
	afx_msg void OnKillfocusEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// *********** my code *********************
public:
	CList<CString, CString&> m_chanSmpList;		// (item number) + (sample of channel value)
	CList<CString, CString&> m_chanNameList;	// channel Name
	int m_nIndex;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDTBIMPORT_H__7FA4364A_DAE0_494D_B6E1_C344281E5863__INCLUDED_)
