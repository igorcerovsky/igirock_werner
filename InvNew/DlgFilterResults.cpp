// DlgFilterResults.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgFilterResults.h"


// CDlgFilterResults dialog

IMPLEMENT_DYNAMIC(CDlgFilterResults, CDialog)
CDlgFilterResults::CDlgFilterResults(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFilterResults::IDD, pParent)
	, m_nWwMin(4)
	, m_nWwMax(100)
	, m_dCondMax(1.0e18)
	, m_dChiMax(1.e18)
	, m_nIpMin(0)
	, m_nIpMax(10)
	, m_nFilterTag(0)
	, m_dWwMin(0)
	, m_dWwMax(10000)
{
}

CDlgFilterResults::~CDlgFilterResults()
{
}

void CDlgFilterResults::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WW_START, m_nWwMin);
	DDX_Text(pDX, IDC_EDIT_WW_END, m_nWwMax);
	DDX_Text(pDX, IDC_EDIT_CONDNUM, m_dCondMax);
	DDX_Text(pDX, IDC_EDIT_CHISQ, m_dChiMax);
	DDX_Text(pDX, IDC_EDIT_IP_START, m_nIpMin);
	DDX_Text(pDX, IDC_EDIT_IP_END, m_nIpMax);
	DDX_Radio(pDX, IDC_RADIO_ALL, m_nFilterTag);
	DDX_Text(pDX, IDC_EDIT_WW_STARTPOS, m_dWwMin);
	DDX_Text(pDX, IDC_EDIT_WW_ENDPOS, m_dWwMax);
	DDV_MinMaxInt(pDX, m_nWwMin, 4, 10000);
	DDV_MinMaxInt(pDX, m_nWwMax, 4, 10000);
	DDV_MinMaxInt(pDX, m_nIpMin, 0, 10);
	DDV_MinMaxInt(pDX, m_nIpMax, 0, 10);
}


BEGIN_MESSAGE_MAP(CDlgFilterResults, CDialog)
	ON_BN_CLICKED(IDC_SHOW_ALL, OnBnClickedShowAll)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CDlgFilterResults message handlers

void CDlgFilterResults::OnBnClickedShowAll()
{
	// TODO: Add your control notification handler code here
}

void CDlgFilterResults::OnBnClickedButtonApply()
{
	UpdateData(TRUE);
	FLTRES flt;
	flt.dChiMax = m_dChiMax;
	//flt.dChiMin
	flt.dCondMax = m_dCondMax;
	//flt.dCondMin
	//flt.dDptMax
	//flt.dDptMin
	flt.dWwMax = m_dWwMax;
	flt.dWwMin = m_dWwMin;
	flt.nIpMax = m_nIpMax;
	flt.nIpMin = m_nIpMin;
	flt.nTag = m_nFilterTag;
	flt.nWwMax = m_nWwMax; 
	flt.nWwMin = m_nWwMin;

	if(m_pMeth) {
		m_pMeth->FilterResults(flt);
		m_pMeth->FilterResults(flt);
		m_pDoc->UpdateAllViews();
	}
}

BOOL CDlgFilterResults::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
