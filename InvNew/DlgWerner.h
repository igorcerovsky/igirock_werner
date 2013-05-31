#pragma once
// DlgWerner.h : header file
//

#include "Database.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgWerner dialog

class CDlgWerner : public CDialog
{
// Construction
public:
	CDlgWerner(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWerner)
	enum { IDD = IDD_DLG_WERNER };
	CComboBox	m_combProf;
	CComboBox	m_combPos;
	CComboBox	m_combField;
	CComboBox	m_combElev;
	double	m_dDeclination;
	double	m_dFieldStrength;
	double	m_dInclination;
	BYTE	m_nIpEnd;
	BYTE	m_nIpStart;
	CString	m_strSolutionName;
	UINT	m_nWwEnd;
	UINT	m_nWwStart;
	UINT	m_nWwStep;
	double	m_dMaxElev;
	double	m_dMinElev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWerner)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWerner)
	virtual void OnOK();
	afx_msg void OnSelendokChannelAltitude();
	afx_msg void OnSelendokWdProf();
	afx_msg void OnSelendokChannelField();
	afx_msg void OnSelendokChannelPos();
	afx_msg void OnKillfocusMindepth();
	afx_msg void OnKillfocusMaxdepth();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//*********** my code *********************
public:
	CDatabase* m_pDtb;
	CDtbProf* m_pProf;
	CDtbProfChan* m_pChanX;
	CDtbProfChan* m_pChanY;
	CDtbProfChan* m_pChanZ;
	UINT	m_nNProfPoints;
	BOOL	m_bVariableAltitude;
	CString	m_strCurProf;
	CString m_strFld;
	CString m_strPos;
	CString m_strElev;

private:
	BOOL ExtraValidation();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

