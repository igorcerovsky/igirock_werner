#pragma once
#include "afxwin.h"
#include "Database.h"


// CDlgProfChan dialog

class CDlgProfChan : public CDialog
{
	DECLARE_DYNAMIC(CDlgProfChan)

public:
	CDlgProfChan(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProfChan();

// Dialog Data
	enum { IDD = IDD_DLG_SETPROFCHAN };

	CDatabase* m_pDtb;
	CDtbProf* m_pProf;
	CDtbProfChan* m_pChan;
	CString	m_strCurProf;
	CString m_strFld;
	CString m_strPos;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboProf();
	afx_msg void OnCbnSelchangeComboChan();
	CComboBox m_combProf;
	CComboBox m_combChan;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelendokComboProf();
	afx_msg void OnCbnSelendokComboChan();
};
