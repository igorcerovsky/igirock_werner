#pragma once

#include "Method2D.h"
#include "InvDoc.h"


// CDlgFilterResults dialog

class CDlgFilterResults : public CDialog
{
	DECLARE_DYNAMIC(CDlgFilterResults)

public:
	CDlgFilterResults(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFilterResults();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER_RESULTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMeth2DLin*	m_pMeth;
	CInvDoc*	m_pDoc;
	int		m_nWwMin;
	int		m_nWwMax;
	double	m_dCondMax;
	double	m_dChiMax;
	int		m_nIpMin;
	int		m_nIpMax;
	int		m_nFilterTag;
	afx_msg void OnBnClickedShowAll();
	double m_dWwMin;
	double m_dWwMax;
	afx_msg void OnBnClickedButtonApply();
	virtual BOOL OnInitDialog();
};
