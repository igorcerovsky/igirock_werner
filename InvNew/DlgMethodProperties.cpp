// DlgMethodProperties.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgMethodProperties.h"


// CDlgMethodProperties dialog

IMPLEMENT_DYNAMIC(CDlgMethodProperties, CDialog)
CDlgMethodProperties::CDlgMethodProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMethodProperties::IDD, pParent)
	, m_strProf(_T(""))
	, m_strType(_T(""))
	, m_strID(_T(""))
	, m_strIp(_T(""))
	, m_strName(_T(""))
	, m_strSolNum(_T(""))
	, m_strWw(_T(""))
{
}

CDlgMethodProperties::~CDlgMethodProperties()
{
}

void CDlgMethodProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROF, m_strProf);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_strType);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIp);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_NUMSOL, m_strSolNum);
	DDX_Text(pDX, IDC_EDIT_WW, m_strWw);
}


BEGIN_MESSAGE_MAP(CDlgMethodProperties, CDialog)
	ON_BN_CLICKED(IDC_BUT_COLOR, OnBnClickedButColor)
END_MESSAGE_MAP()


// CDlgMethodProperties message handlers

void CDlgMethodProperties::OnBnClickedButColor()
{
	CColorDialog cDlg(m_color, CC_SOLIDCOLOR);
	if(cDlg.DoModal() == IDOK) {
		m_color = cDlg.GetColor();
	}

}
