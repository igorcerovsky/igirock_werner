// DlgSelectChannels.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgSelectChannels.h"


// CDlgSelectChannels dialog

IMPLEMENT_DYNAMIC(CDlgSelectChannels, CDialog)
CDlgSelectChannels::CDlgSelectChannels(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectChannels::IDD, pParent)
{
}

CDlgSelectChannels::~CDlgSelectChannels()
{
}

void CDlgSelectChannels::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAN_POS, m_combPos);
	DDX_Control(pDX, IDC_CHAN_ELEV, m_combElev);
	DDX_Control(pDX, IDC_CHAN_FLD, m_combFld);
}


BEGIN_MESSAGE_MAP(CDlgSelectChannels, CDialog)
	ON_CBN_SELENDOK(IDC_CHAN_POS, OnCbnSelendokChanPos)
	ON_CBN_SELENDOK(IDC_CHAN_ELEV, OnCbnSelendokChanElev)
	ON_CBN_SELENDOK(IDC_CHAN_FLD, OnCbnSelendokChanFld)
END_MESSAGE_MAP()


// CDlgSelectChannels message handlers

BOOL CDlgSelectChannels::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	ChannnelArray* pChanArray = m_pDtb->GetProfArray()->GetAt(0)->GetChanArray();
	for( int i=0; i<pChanArray->GetSize(); i++) {
		m_combPos.AddString( pChanArray->GetAt(i)->GetName() );
		m_combElev.AddString( pChanArray->GetAt(i)->GetName() );
		m_combFld.AddString( pChanArray->GetAt(i)->GetName() );
	}
	m_combElev.AddString( "None" );
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectChannels::OnCbnSelendokChanPos()
{
	int nIndex = m_combPos.GetCurSel();
	if (nIndex != LB_ERR) {
		m_strPos = m_pDtb->GetProfArray()->GetAt(0)->GetChanArray()->GetAt(nIndex)->GetName();
	}
}

void CDlgSelectChannels::OnCbnSelendokChanElev()
{
	int nIndex = m_combElev.GetCurSel();
	if (nIndex != LB_ERR) {
		CString str;
		m_combElev.GetWindowText( str );
		if( str != "None" ) {
			m_strElev = m_pDtb->GetProfArray()->GetAt(0)->GetChanArray()->GetAt(nIndex)->GetName();}
		else
			m_strElev = "None";
	}
}

void CDlgSelectChannels::OnCbnSelendokChanFld()
{
	int nIndex = m_combFld.GetCurSel();
	if (nIndex != LB_ERR) {
		m_strFld = m_pDtb->GetProfArray()->GetAt(0)->GetChanArray()->GetAt(nIndex)->GetName();
	}
}

void CDlgSelectChannels::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}
