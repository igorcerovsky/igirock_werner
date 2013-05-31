#pragma once

#include "Database.h"
#include "afxwin.h"

// CDlgSelectChannels dialog

class CDlgSelectChannels : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectChannels)

public:
	CDlgSelectChannels(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelectChannels();

	CDatabase* m_pDtb;

// Dialog Data
	enum { IDD = IDD_DLG_SELCHAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_combPos;
	CComboBox m_combElev;

	CString m_strElev;
	CString m_strPos;
	CString m_strFld;
	afx_msg void OnCbnSelendokChanPos();
	afx_msg void OnCbnSelendokChanElev();
	CComboBox m_combFld;
	afx_msg void OnCbnSelendokChanFld();
protected:
	virtual void OnOK();
};
