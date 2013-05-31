// DlgClustering.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgClustering.h"


// CDlgClustering dialog

IMPLEMENT_DYNAMIC(CDlgClustering, CDialog)
CDlgClustering::CDlgClustering(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClustering::IDD, pParent)
	, m_nClstType(0)
	, m_dTresh(0)
	, m_dRad(0)
	, m_strName(_T(""))
	, m_strProf(_T(""))
	, m_strWw(_T(""))
	, m_strIp(_T(""))
{
}

CDlgClustering::~CDlgClustering()
{
}

void CDlgClustering::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CLUST, m_nClstType);
	DDX_Text(pDX, IDC_EDIT_TRESH, m_dTresh);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dRad);
	DDX_Text(pDX, IDC_STR_SOLNAME, m_strName);
	DDX_Text(pDX, IDC_STR_PROFILE, m_strProf);
	DDX_Text(pDX, IDC_STR_WW, m_strWw);
	DDX_Text(pDX, IDC_STR_IP, m_strIp);
	DDX_Control(pDX, IDC_SPIN_TRESH, m_ctrlSpin);
}


BEGIN_MESSAGE_MAP(CDlgClustering, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, OnBnClickedButtonCompute)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

BOOL CDlgClustering::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_dRad = (float) m_pMeth->GetClsRadius();
	m_dTresh = (float) m_pMeth->GetTrash();
	m_ctrlSpin.SetRange(0, 100);
	m_ctrlSpin.SetPos( (int)(m_dTresh*100) );

	UpdateData( false );
	Invalidate( true );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDlgClustering message handlers

void CDlgClustering::OnBnClickedButtonCompute()
{
	UpdateData();
	//m_pMeth->Clustering( m_dRad);
	//m_pMeth->SetClusteringPram( m_nClstType, m_dRad, m_dE
	m_pView->m_pDoc->UpdateAllViews();
}

void CDlgClustering::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString str;
	if(pScrollBar->GetDlgCtrlID() == IDC_SPIN_TRESH) {
		m_dTresh = (float)nPos/100; 
		str.Format("%.2f", m_dTresh);
		((CSpinButtonCtrl*) pScrollBar)->GetBuddy()->SetWindowText(str);
		m_pMeth->ShowSolutions( m_dTresh );
		m_pView->m_pDoc->UpdateAllViews();
	}

	//CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
