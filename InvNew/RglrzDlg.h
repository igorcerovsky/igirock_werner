// RegularizationDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgReglrz dialog
#include <afxtempl.h>
#include "Rglrz.h"
#include "MyButton.h"
#include "afxwin.h"
#include "Database.h"

class CDlgReglrz : public CDialog
{
// Construction
public:
//	void OnOK();
	void FindMinMax(double *data,  int n, double &min, double &max);
	CDlgReglrz(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReglrz)
	enum { IDD = IDD_REGULARIZATION_DIALOG };
	CMyButton	m_butPrevMin;
	CMyButton	m_butNextMin;
	CMyButton	m_butRglrz;
	CMyButton	m_butDflt;
	CMyButton	m_ButImport;
	CMyButton	m_grp;
	float	m_dRpMax;
	float	m_dRpMin;
	float	m_dRpQ;
	int		m_nRegTag;
	float	m_fFldCont;
	BOOL	m_bData;
	BOOL	m_bFFT;
	BOOL	m_bRglrz;
	BOOL	m_bSpec;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReglrz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CDlgReglrz)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButInport();
	afx_msg void OnButRegularize();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButDefault();
	afx_msg void OnRadioRegtag();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnCheckData();
	afx_msg void OnCheckRglrz();
	afx_msg void OnCheckFft();
	afx_msg void OnCheckSpec();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButPrevMin();
	afx_msg void OnButNextMin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// variables
protected:
	void NormMinMax(double &min, double &max);
	void DrawFnc(CDC *pDC);
	void DrawRegPar(CDC *pDC);
	HICON m_hIcon;
	HCURSOR m_hCursor;

	CDC* m_pDcMem;			// device context for painting
	CBitmap* m_pBmp;
	CBitmap m_BkBmp;


	BOOL m_bInit;
	int m_ox, m_oy, m_lx, m_ly, m_sx, m_sy;	//
	double m_scx, m_scy, m_offy;					// scales

	int m_oxr, m_oyr, m_lxr, m_lyr, m_sxr, m_syr;	//
	double m_scxr, m_scyr, m_offxr, m_offyr;					// scales

	double m_dRp;
	double m_dDelta;
	// data
	double m_dMin, m_dMax;

	// regularization
	CRglrz m_rglrz;

	BOOL m_bCaptured;

	CFont m_fontRp;

	CRect m_drwRect;

	CBrush m_bkBrush;
	CBitmap m_bmpBk;

public:
	// database
	CDatabase* m_pDtb;
	CDtbProf* m_pProf;
	CDtbProfChan* m_pChan;
	CString	m_strCurProf;
	CString m_strFld;
	CString m_strPos;


public:
	afx_msg void OnBnClickedClose();
	CMyButton m_myCancel;
	CMyButton m_close;
	afx_msg void OnBnClickedCancel();
	CMyButton m_butHlp;
	afx_msg void OnBnClickedButSetdata();
	CMyButton m_butSetData;
	int m_nOut;
};
