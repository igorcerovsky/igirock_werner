#pragma once


// CDlgMethodProperties dialog

class CDlgMethodProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgMethodProperties)

public:
	CDlgMethodProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMethodProperties();

// Dialog Data
	enum { IDD = IDD_DLG_METHOD_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString		m_strProf;
	CString		m_strType;
	CString		m_strID;
	CString		m_strName;
	CString		m_strSolNum;
	CString		m_strIp;
	CString		m_strWw;
	COLORREF	m_color;

	afx_msg void OnBnClickedButColor();
};
