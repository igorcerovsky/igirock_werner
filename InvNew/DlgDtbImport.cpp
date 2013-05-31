// DlgDtbImport.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgDtbImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDtbImport dialog


CDlgDtbImport::CDlgDtbImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDtbImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDtbImport)
	m_strChanName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDtbImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDtbImport)
	DDX_Control(pDX, IDC_LIST_CHAN, m_listChan);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strChanName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDtbImport, CDialog)
	//{{AFX_MSG_MAP(CDlgDtbImport)
	ON_LBN_SELCHANGE(IDC_LIST_CHAN, OnSelchangeListChan)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, OnKillfocusEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDtbImport message handlers

BOOL CDlgDtbImport::OnInitDialog() 
{
	m_nIndex = -1;
	CDialog::OnInitDialog();
	
	POSITION pos1 = m_chanSmpList.GetHeadPosition();
	POSITION pos2 = m_chanNameList.GetHeadPosition();
	while(pos1 && pos2) {
		CString str, str1, str2, s;
		str1 = m_chanSmpList.GetNext(pos1);
		str2 = m_chanNameList.GetNext(pos2);
		s.Format("%20s", (LPCTSTR) str2);
		str = str1 + s;
		m_listChan.AddString(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDtbImport::OnSelchangeListChan() 
{
	int nIndex = m_listChan.GetCurSel();
	int nCount = m_listChan.GetCount();
	if ((nIndex != LB_ERR) && (nCount > 1)) {
		CString str;
		POSITION pos = m_chanNameList.FindIndex(nIndex);
		if(pos)
			str = m_chanNameList.GetAt(pos);
		SetDlgItemText(IDC_EDIT_NAME, str);
		m_nIndex = nIndex;
	}
}

void CDlgDtbImport::OnKillfocusEditName() 
{
	if(m_nIndex >= 0) {
		CString s, str, str1;
		GetDlgItemText(IDC_EDIT_NAME, str);
		str1 = m_chanSmpList.GetAt(m_chanSmpList.FindIndex(m_nIndex));
		s.Format("%20s", (LPCTSTR) str);
		str1 += s; 
		m_listChan.DeleteString(m_nIndex);
		m_listChan.InsertString(m_nIndex, str1);
		m_chanNameList.SetAt(m_chanNameList.FindIndex(m_nIndex), str);
	}
}
