#pragma once

#include "Method2D.h"
#include "afxcmn.h"
#include "InvView.h"

// CDlgClusteringSmall dialog

class CDlgClusteringSmall : public CDialog
{
	DECLARE_DYNAMIC(CDlgClusteringSmall)

public:
	CDlgClusteringSmall(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClusteringSmall();

// Dialog Data
	enum { IDD = IDD_DLG_CLUSTRING_SMALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nClstType;
	float m_dTresh;
	float m_dRad;
	float m_dExp;
	afx_msg void OnBnClickedButtonCompute();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioClust();
	afx_msg void OnBnClickedRadio2();

public:
	// clustered method
	CMeth2DLin	*m_pMeth;

	// current view
	CInvView *m_pView;

	CSpinButtonCtrl m_ctrlSpin;
	CSpinButtonCtrl m_ctrlSpinExp;
	BOOL m_bChi;
	BOOL m_bCond;
};
