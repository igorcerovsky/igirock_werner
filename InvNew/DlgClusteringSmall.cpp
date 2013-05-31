// DlgClusteringSmall.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgClusteringSmall.h"


// CDlgClusteringSmall dialog

IMPLEMENT_DYNAMIC(CDlgClusteringSmall, CDialog)
CDlgClusteringSmall::CDlgClusteringSmall(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClusteringSmall::IDD, pParent)
	, m_nClstType(0)
	, m_dTresh(0)
	, m_dRad(0)
	, m_dExp(0)
	, m_bChi(FALSE)
	, m_bCond(FALSE)
{
}

CDlgClusteringSmall::~CDlgClusteringSmall()
{
}

void CDlgClusteringSmall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CLUST, m_nClstType);
	DDX_Text(pDX, IDC_EDIT_TRESH, m_dTresh);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dRad);
	DDX_Text(pDX, IDC_EDIT_EXP, m_dExp);
	DDX_Control(pDX, IDC_SPIN_TRESH, m_ctrlSpin);
	DDX_Control(pDX, IDC_SPIN_EXP, m_ctrlSpinExp);
}


BEGIN_MESSAGE_MAP(CDlgClusteringSmall, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, OnBnClickedButtonCompute)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_RADIO_CLUST, OnBnClickedRadioClust)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDlgClusteringSmall::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_dRad = (float) m_pMeth->GetClsRadius();
	m_dExp = (float) m_pMeth->GetClsExp();
	m_dTresh = (float) m_pMeth->GetTrash();
	m_ctrlSpin.SetRange(0, 100);
	m_ctrlSpin.SetPos( (int)(m_dTresh*100) );
	m_ctrlSpinExp.SetRange(0, 200);
	m_ctrlSpinExp.SetPos( (int)(100+m_dExp*10) );

	UpdateData( false );
	Invalidate( true );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDlgClusteringSmall message handlers

void CDlgClusteringSmall::OnBnClickedButtonCompute()
{
	UpdateData();
	//m_pMeth->Clustering( m_dRad, m_dExp);
	m_pMeth->SetClusteringPram( m_nClstType, m_dRad, m_dExp );
	m_pView->GetDocument()->Clustering( m_pMeth );
	//AfxBeginThread( (AFX_THREADPROC)ThreadClustering, m_pMeth, THREAD_PRIORITY_NORMAL );
	//m_pView->m_pDoc->UpdateAllViews();
}

void CDlgClusteringSmall::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString str;
	if(pScrollBar->GetDlgCtrlID() == IDC_SPIN_TRESH) {
		m_dTresh = (float)nPos/100; 
		str.Format("%.2f", m_dTresh);
		((CSpinButtonCtrl*) pScrollBar)->GetBuddy()->SetWindowText(str);
		m_pMeth->ShowSolutions( m_dTresh );
		m_pView->m_pDoc->UpdateAllViews();
	}
	if(pScrollBar->GetDlgCtrlID() == IDC_SPIN_EXP) {
		m_dExp = (float)((float)nPos-100)/10; 
		str.Format("%.2f", m_dExp);
		((CSpinButtonCtrl*) pScrollBar)->GetBuddy()->SetWindowText(str);
	}
}


void CDlgClusteringSmall::OnBnClickedRadioClust()
{
	// TODO: Add your control notification handler code here
}

void CDlgClusteringSmall::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
}
