#pragma once


// CDlgSetRange dialog

class CDlgSetRange : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetRange)

public:
	CDlgSetRange(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetRange();

// Dialog Data
	enum { IDD = IDD_DLG_SETRANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_dMin;
	float m_dMax;
protected:
	virtual void OnOK();
};
