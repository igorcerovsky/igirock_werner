#pragma once
#include "afxwin.h"
#include "Method2D.h"
#include "InvDoc.h"

// CDlgEditMethod dialog

class CDlgEditMethod : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditMethod)

public:
	CDlgEditMethod(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditMethod();

// Dialog Data
	enum { IDD = IDD_DLG_METHOD_EDIT };

	MethodArray	*m_pMethArray;
	CMeth2DLin	*m_pMeth;
	CInvDoc		*m_pDoc;
	int			m_nCurProf;
	CString		m_strCurProfID;
	int			m_nMethIndex;	// index in method array

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listMeth;
	afx_msg void OnBnClickedButShow();
	afx_msg void OnBnClickedButDel();
	afx_msg void OnBnClickedButProp();
	afx_msg void OnBnClickedButRemhid();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListMeth();
	afx_msg void OnBnClickedButMethcur();
	afx_msg void OnBnClickedButExport();
	int m_nShowProf;
	afx_msg void OnBnClickedRadioShowProf();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnLbnDblclkListMeth();
};
