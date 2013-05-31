// DlgProfChan.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgProfChan.h"


// CDlgProfChan dialog

IMPLEMENT_DYNAMIC(CDlgProfChan, CDialog)
CDlgProfChan::CDlgProfChan(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProfChan::IDD, pParent)
{
}

CDlgProfChan::~CDlgProfChan()
{
}

void CDlgProfChan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROF, m_combProf);
	DDX_Control(pDX, IDC_COMBO_CHAN, m_combChan);
}


BEGIN_MESSAGE_MAP(CDlgProfChan, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PROF, OnCbnSelchangeComboProf)
	ON_CBN_SELCHANGE(IDC_COMBO_CHAN, OnCbnSelchangeComboChan)
	ON_CBN_SELENDOK(IDC_COMBO_PROF, OnCbnSelendokComboProf)
	ON_CBN_SELENDOK(IDC_COMBO_CHAN, OnCbnSelendokComboChan)
END_MESSAGE_MAP()


// CDlgProfChan message handlers

void CDlgProfChan::OnCbnSelchangeComboProf()
{
	// TODO: Add your control notification handler code here
}

void CDlgProfChan::OnCbnSelchangeComboChan()
{
	// TODO: Add your control notification handler code here
}

BOOL CDlgProfChan::OnInitDialog()
{
	CDialog::OnInitDialog();

	ProfileArray* pProfArray = m_pDtb->GetProfArray();
	for( int i=0; i<pProfArray->GetSize(); i++) {
		CString strID = pProfArray->GetAt(i)->GetID();
		m_combProf.AddString( strID );
		if( strID == m_strCurProf ) {
			m_combProf.SetCurSel( i);
			OnCbnSelendokComboProf();
		}
	}
	//m_combProf.AddString( "All" );

	if(m_combProf.GetCount() < 1) {
		AfxMessageBox( "No Profile in Database");
		OnCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProfChan::OnCbnSelendokComboProf()
{
	int nIndex = m_combProf.GetCurSel();

	if (nIndex != LB_ERR) {
		//POSITION pos = m_pDtb->GetProfList()->FindIndex(nIndex);
		m_pProf = m_pDtb->GetProfArray()->GetAt(nIndex);	
		TRACE("Selected profile : %s\n", m_pProf->GetID());

		m_combChan.ResetContent();		//removimg all items
		ChannnelArray* pChanArray = m_pProf->GetChanArray();
		for(int i=0; i<pChanArray->GetSize(); i++) {
			CString str;
			str = pChanArray->GetAt(i)->GetName();
			m_combChan.AddString( pChanArray->GetAt(i)->GetName() );		// adding new items 
			if( str == m_strFld ) {
				m_combChan.SetCurSel( i);
				OnCbnSelendokComboChan();
			}
		}
	}
}

void CDlgProfChan::OnCbnSelendokComboChan()
{
	int nIndex = m_combChan.GetCurSel();
	if (nIndex != LB_ERR) {
		m_pChan = m_pProf->GetChanArray()->GetAt(nIndex);
		TRACE("Selected channel: %s\n", m_pChan->GetID());
	}
}
