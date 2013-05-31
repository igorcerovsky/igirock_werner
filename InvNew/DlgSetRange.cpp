// DlgSetRange.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgSetRange.h"


// CDlgSetRange dialog

IMPLEMENT_DYNAMIC(CDlgSetRange, CDialog)
CDlgSetRange::CDlgSetRange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetRange::IDD, pParent)
	, m_dMin(0)
	, m_dMax(0)
{
}

CDlgSetRange::~CDlgSetRange()
{
}

void CDlgSetRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MIN, m_dMin);
	DDX_Text(pDX, IDC_EDIT_MAX, m_dMax);
}


BEGIN_MESSAGE_MAP(CDlgSetRange, CDialog)
END_MESSAGE_MAP()


// CDlgSetRange message handlers

void CDlgSetRange::OnOK()
{
	UpdateData();
	if( m_dMin >= m_dMax ) {
		AfxMessageBox("Ooops min >= max.", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}
