#pragma once

#include "Method2D.h"
#include "afxcmn.h"
#include "InvView.h"
// CDlgClustering dialog

class CDlgClustering : public CDialog
{
	DECLARE_DYNAMIC(CDlgClustering)

public:
	CDlgClustering(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClustering();

// Dialog Data
	enum { IDD = IDD_DLG_CLUSTERING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCompute();
	int m_nClstType;
	float m_dTresh;
	float m_dRad;
	CString m_strName;
	CString m_strProf;
	CString m_strWw;
	CString m_strIp;

	// clustered method
	CMeth2DLin	*m_pMeth;

	// current view
	CInvView *m_pView;


	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_ctrlSpin;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
