// DlgEditMethod.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgEditMethod.h"


// CDlgEditMethod dialog

IMPLEMENT_DYNAMIC(CDlgEditMethod, CDialog)
CDlgEditMethod::CDlgEditMethod(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditMethod::IDD, pParent)
	, m_nShowProf(0)
{
	m_pMethArray = NULL;
}

CDlgEditMethod::~CDlgEditMethod()
{
}

void CDlgEditMethod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_METH, m_listMeth);
	DDX_Radio(pDX, IDC_RADIO_SHOW_PROF, m_nShowProf);
}


BEGIN_MESSAGE_MAP(CDlgEditMethod, CDialog)
	ON_BN_CLICKED(IDC_BUT_SHOW, OnBnClickedButShow)
	ON_BN_CLICKED(IDC_BUT_DEL, OnBnClickedButDel)
	ON_BN_CLICKED(IDC_BUT_PROP, OnBnClickedButProp)
	ON_BN_CLICKED(IDC_BUT_REMHID, OnBnClickedButRemhid)
	ON_LBN_SELCHANGE(IDC_LIST_METH, OnLbnSelchangeListMeth)
	ON_BN_CLICKED(IDC_BUT_METHCUR, OnBnClickedButMethcur)
	ON_BN_CLICKED(IDC_BUT_EXPORT, OnBnClickedButExport)
	ON_BN_CLICKED(IDC_RADIO_SHOW_PROF, OnBnClickedRadioShowProf)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_LBN_DBLCLK(IDC_LIST_METH, OnLbnDblclkListMeth)
END_MESSAGE_MAP()


// CDlgEditMethod message handlers
BOOL CDlgEditMethod::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( m_pMethArray != NULL ) {
		if( m_pMethArray->IsEmpty()) {
			AfxMessageBox("No method, no edit...");
			OnCancel();
		}
	}
	else
		OnCancel();

	// initialization
	for( int i = 0; i < m_pMethArray->GetSize(); i++) {
		CMeth2DLin *pMeth = m_pMethArray->GetAt(i);
		if( m_nShowProf != 0 || m_strCurProfID == pMeth->GetProfID() ) {
			CMeth2DLin *pMeth = m_pMethArray->GetAt(i);
			m_listMeth.AddString( pMeth->GetName() );
			m_nMethIndex = -1;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditMethod::OnBnClickedButShow()
{
	if( m_pMeth != NULL ) {
		m_pMeth->Show( !m_pMeth->IsVisible() );
		m_pDoc->UpdateAllViews();
	}
	else
		AfxMessageBox("Select method.");
}

void CDlgEditMethod::OnBnClickedButDel()
{
	OnLbnSelchangeListMeth();
	if (m_pMeth != NULL) {
		m_pMethArray->RemoveAt( m_nMethIndex );
		delete m_pMeth;
		m_pMeth = NULL;
		m_pDoc->SelectMethod( NULL );
		m_pDoc->UpdateAllViews();
		m_listMeth.ResetContent();
		OnInitDialog();
	}
	else
		AfxMessageBox("Select method.");
}

void CDlgEditMethod::OnBnClickedButProp()
{
	if( m_pMeth != NULL ) {
		m_pMeth->Properties();
		m_listMeth.ResetContent();
		OnInitDialog();
		m_pDoc->UpdateAllViews();
	}
	else
		AfxMessageBox("Select method.");
}

void CDlgEditMethod::OnBnClickedButRemhid()
{
	if( m_pMeth != NULL ) {
		m_pMeth->RemoveSolutions();
		m_pDoc->UpdateAllViews();
	}
	else
		AfxMessageBox("Select method.");
}


void CDlgEditMethod::OnLbnSelchangeListMeth()
{
	int nIndex = m_listMeth.GetCurSel();
	int j=0;
	CMeth2DLin	*pMeth;

	if (nIndex != LB_ERR) {
		//m_pMeth = m_pMethArray->GetAt( nIndex );
		for( int i = 0; i < m_pMethArray->GetSize(); i++) {
			pMeth = m_pMethArray->GetAt( i );
			CString strCurProf = pMeth->GetProfID();
			if( m_nShowProf != 0 || m_strCurProfID == strCurProf) {
				if( j == nIndex) {
					m_pMeth = pMeth;
					m_nMethIndex = i;
				}
				j++;
			}
		}
	}
}

void CDlgEditMethod::OnBnClickedButMethcur()
{
	int nIndex = m_listMeth.GetCurSel();
	if (nIndex != LB_ERR) {
		m_pDoc->SelectMethod( m_pMeth );
	}
}

void CDlgEditMethod::OnBnClickedButExport()
{
	if( m_pMeth != NULL ) {
		m_pMeth->Export();
	}
	else
		AfxMessageBox("Select method.");
}

void CDlgEditMethod::OnBnClickedRadioShowProf()
{
	UpdateData();
	m_listMeth.ResetContent();
	OnInitDialog();
}

void CDlgEditMethod::OnBnClickedRadio2()
{
	UpdateData();
	m_listMeth.ResetContent();
	OnInitDialog();
}

void CDlgEditMethod::OnLbnDblclkListMeth()
{
	int nIndex = m_listMeth.GetCurSel();
	int j=0;
	CMeth2DLin	*pMeth;

	if (nIndex != LB_ERR) {
		//m_pMeth = m_pMethArray->GetAt( nIndex );
		for( int i = 0; i < m_pMethArray->GetSize(); i++) {
			pMeth = m_pMethArray->GetAt( i );
			CString strCurProf = pMeth->GetProfID();
			if( m_nShowProf != 0 || m_strCurProfID == strCurProf) {
				if( j == nIndex) {
					m_pMeth = pMeth;
					m_nMethIndex = i;
					m_pDoc->SelectMethod( pMeth );
					m_pDoc->UpdateAllViews();
					OnOK();
				}
				j++;
			}
		}
	}
}
