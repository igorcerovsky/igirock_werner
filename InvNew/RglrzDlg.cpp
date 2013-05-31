// RegularizationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "RglrzDlg.h"
#include <fstream>
#include <math.h>
#include "DlgProfChan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgReglrz dialog

CDlgReglrz::CDlgReglrz(CWnd* pParent)
	: CDialog(CDlgReglrz::IDD, pParent)
	, m_nOut(0)
{
	//{{AFX_DATA_INIT(CDlgReglrz)
	m_dRpMax = 1.0e+10f;
	m_dRpMin = 1.0e-10f;
	m_dRpQ = 1.1f;
	m_nRegTag = 0;
	m_fFldCont = 1.0f;
	m_bData = TRUE;
	m_bFFT = TRUE;
	m_bRglrz = TRUE;
	m_bSpec = TRUE;
	//}}AFX_DATA_INIT
	m_nOut = 0;
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDcMem = new CDC;
	m_pBmp = new CBitmap;

	VERIFY(m_fontRp.CreateFont(
	   12,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   "Arial"));                 // lpszFacename

	m_drwRect = CRect(0, 140, 600, 500);
	m_bInit = FALSE;

	m_bmpBk.LoadBitmap(IDB_B1);
	m_bkBrush.CreatePatternBrush(&m_bmpBk);
}

void CDlgReglrz::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReglrz)
	DDX_Control(pDX, IDC_BUT_PREV_MIN, m_butPrevMin);
	DDX_Control(pDX, IDC_BUT_NEXT_MIN, m_butNextMin);
	DDX_Control(pDX, IDC_BUT_REGULARIZE, m_butRglrz);
	DDX_Control(pDX, IDC_BUT_DEFAULT, m_butDflt);
	DDX_Control(pDX, IDC_BUT_INPORT, m_ButImport);
	DDX_Text(pDX, IDC_EDIT_RP_MAX, m_dRpMax);
	DDX_Text(pDX, IDC_EDIT_RP_MIN, m_dRpMin);
	DDX_Text(pDX, IDC_EDIT_RP_Q, m_dRpQ);
	DDX_Radio(pDX, IDC_RADIO_REGTAG, m_nRegTag);
	DDX_Text(pDX, IDC_EDIT_FLD_CONT, m_fFldCont);
	DDX_Check(pDX, IDC_CHECK_DATA, m_bData);
	DDX_Check(pDX, IDC_CHECK_FFT, m_bFFT);
	DDX_Check(pDX, IDC_CHECK_RGLRZ, m_bRglrz);
	DDX_Check(pDX, IDC_CHECK_SPEC, m_bSpec);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CANCEL, m_myCancel);
	DDX_Control(pDX, IDC_CLOSE, m_close);
	DDX_Control(pDX, IDC_BUT_HLP, m_butHlp);
	DDX_Control(pDX, IDC_BUT_SETDATA, m_butSetData);
	DDX_Radio(pDX, IDC_RADIO_OUT, m_nOut);
}

BEGIN_MESSAGE_MAP(CDlgReglrz, CDialog)
	//{{AFX_MSG_MAP(CDlgReglrz)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_INPORT, OnButInport)
	ON_BN_CLICKED(IDC_BUT_REGULARIZE, OnButRegularize)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUT_DEFAULT, OnButDefault)
	ON_BN_CLICKED(IDC_RADIO_REGTAG, OnRadioRegtag)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_CHECK_DATA, OnCheckData)
	ON_BN_CLICKED(IDC_CHECK_RGLRZ, OnCheckRglrz)
	ON_BN_CLICKED(IDC_CHECK_FFT, OnCheckFft)
	ON_BN_CLICKED(IDC_CHECK_SPEC, OnCheckSpec)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUT_PREV_MIN, OnButPrevMin)
	ON_BN_CLICKED(IDC_BUT_NEXT_MIN, OnButNextMin)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUT_SETDATA, OnBnClickedButSetdata)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReglrz message handlers

BOOL CDlgReglrz::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	//***** bitmap ***************
	if(m_pDcMem->GetSafeHdc() == NULL) {
		CClientDC dc(this);
		m_pDcMem->CreateCompatibleDC(&dc);
		m_pBmp->CreateCompatibleBitmap(&dc, 600, 400);
	}
	//****************************

	m_ox = 15;
	m_oy = 160;
	m_lx = 410;
	m_ly = 150;

	m_oxr = 15;
	m_oyr = 320;
	m_lxr = 410;
	m_lyr = 140;
	m_bCaptured = FALSE;

	m_dRp = 0.0;
	m_hCursor = ::GetCursor();

	m_BkBmp.LoadBitmap(IDB_B1);
	CButton *pB = (CButton*) GetDlgItem(IDC_BUT_INPORT);
	pB->SetBitmap( m_BkBmp );

	OnButInport();
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgReglrz::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// painting
		//********* bitmap **************
		CBitmap* pdBitmap = m_pDcMem->SelectObject(m_pBmp);
		CBrush bkBrush( RGB(0, 51, 102) );
		CBrush* pdBrush = m_pDcMem->SelectObject(&bkBrush);
		m_pDcMem->PatBlt(0, 0, 500, 600, PATCOPY);
		//*******************************



		DrawFnc(m_pDcMem);
		DrawRegPar(m_pDcMem);

		// regularization parameter
		CPaintDC dc(this); // device context for painting
		//************** bitmap ***************
		dc.BitBlt(0, 140, 500, 750, m_pDcMem, 0, 0, SRCCOPY);
		m_pDcMem->SelectObject(pdBitmap);
		m_pDcMem->SelectObject(bkBrush);
		//*************************************
		//CDialog::OnPaint();
	}
}

void CDlgReglrz::DrawFnc(CDC *pDC)
{
	int sx, sy, x, y;
	CPen *pPen;
	double *pData;
	double min, max;

	sx = 3;
	sy = 5;
	CPen penAx( PS_SOLID, 1, RGB(255,255,255) );
	pPen = pDC->SelectObject(&penAx);
	// axes
	pDC->MoveTo((m_ox - 2), (m_oy + 1));
	pDC->LineTo((m_ox - 2), (m_oy + 1) - m_ly);
	pDC->LineTo((m_ox - 2) - sx, (m_oy + 1) - m_ly + sy);
	pDC->MoveTo((m_ox - 2), (m_oy + 1) - m_ly);
	pDC->LineTo((m_ox - 2) + sx, (m_oy + 1) - m_ly + sy);
	pDC->MoveTo((m_ox - 2), (m_oy + 1));
	pDC->LineTo((m_ox - 2) + m_lx, (m_oy + 1));
	pDC->LineTo((m_ox - 2) + m_lx - sy, (m_oy + 1) - sx);
	pDC->MoveTo((m_ox - 2) + m_lx, (m_oy + 1));
	pDC->LineTo((m_ox - 2) + m_lx - sy, (m_oy + 1) + sx);
	pDC->SelectObject(pPen);
	// end axes


	// original
	int n = m_rglrz.m_nData;
	pData = m_rglrz.m_data.GetData();
	if(pData != NULL && m_bData) {
		CPen penCrv( PS_SOLID, 1, RGB(255,0, 0)/*RGB(255,255,204)*/ );
		pPen = pDC->SelectObject(&penCrv);
		//FindMinMax(pData, n, m_dMax, m_dMin);
		m_scx = ((double) (m_lx))/(n);		// begin is zero
		m_scy = ((double) (m_ly))/( m_dMax - m_dMin );
		m_offy = m_dMin*m_scy;
		x = (int) (m_ox);
		y = (int) (m_oy + m_offy - pData[0]*m_scy);
		pDC ->MoveTo(x, y);
		for(int i=1; i < n; i++) {
			x = (int) (m_ox + i*m_scx);
			y = (int) (m_oy + m_offy - pData[i]*m_scy);
			pDC ->LineTo(x, y);
		}
		pDC->SelectObject(pPen);
	}

	// FFT
	n = m_rglrz.m_nData;
	pData = m_rglrz.m_noreg.GetData();
	if(pData != NULL && m_bFFT) {
		CPen penFFT( PS_SOLID, 1, RGB(255, 255, 0) );
		pPen = pDC->SelectObject(&penFFT);
		FindMinMax(pData, n, min, max);
		m_scx = ((double) (m_lx))/(n);		// begin is zero
		m_scy = ((double) (m_ly))/( max - min );
		m_offy = min*m_scy;
		x = (int) (m_ox);
		y = (int) (m_oy + m_offy - pData[0]*m_scy);
		pDC ->MoveTo(x, y);
		for(int i=1; i < n; i++) {
			x = (int) (m_ox + i*m_scx);
			y = (int) (m_oy + m_offy - pData[i]*m_scy);
			pDC ->LineTo(x, y);
		}
		pDC->SelectObject(pPen);
	}

	// regularized
	n = m_rglrz.m_nData;
	pData = m_rglrz.m_reg.GetData();
	if(pData != NULL && m_bRglrz) {
		CPen penRgl( PS_SOLID, 1, RGB(255, 255, 255) );
		pPen = pDC->SelectObject(&penRgl);
		double min, max;
		FindMinMax(pData, n, min, max);
		m_scx = ((double) (m_lx))/(n);		// begin is zero
		m_scy = ((double) (m_ly))/( max - min );
		m_offy = min*m_scy;
		x = (int) (m_ox);
		y = (int) (m_oy + m_offy - pData[0]*m_scy);
		pDC ->MoveTo(x, y);
		for(int i=1; i < n; i++) {
			x = (int) (m_ox + i*m_scx);
			y = (int) (m_oy + m_offy - pData[i]*m_scy);
			pDC ->LineTo(x, y);
		}
		pDC->SelectObject(pPen);
	}
}

void CDlgReglrz::DrawRegPar(CDC *pDC)
{
	double rp;
	double normMax, normMin;
	int sx, sy, x, y;
	CPen *pPen;

	sx = 3;
	sy = 5;
	CPen penAx( PS_SOLID, 1, RGB(255,255,255) );
	pPen = pDC->SelectObject(&penAx);
	// axes
	pDC->MoveTo((m_oxr - 2), (m_oyr + 1));
	pDC->LineTo((m_oxr - 2), (m_oyr + 1) - m_lyr);
	pDC->LineTo((m_oxr - 2) - sx, (m_oyr + 1) - m_lyr + sy);
	pDC->MoveTo((m_oxr - 2), (m_oyr + 1) - m_lyr);
	pDC->LineTo((m_oxr - 2) + sx, (m_oyr + 1) - m_lyr + sy);
	pDC->MoveTo((m_oxr - 2), (m_oyr + 1));
	pDC->LineTo((m_oxr - 2) + m_lxr, (m_oyr + 1));
	pDC->LineTo((m_oxr - 2) + m_lxr - sy, (m_oyr + 1) - sx);
	pDC->MoveTo((m_oxr - 2) + m_lxr, (m_oyr + 1));
	pDC->LineTo((m_oxr - 2) + m_lxr - sy, (m_oyr + 1) + sx);
	pDC->SelectObject(pPen);
	// end axes

	// norm of regularized parameter
	//int n = m_rglNorm.GetSize();
	int n = (int) m_rglrz.m_norm.GetSize();
	double *pData = m_rglrz.m_norm.GetData();
	if(pData != NULL) {
		CPen penCrv( PS_SOLID, 1, RGB(0,255,0) );
		pPen = pDC->SelectObject(&penCrv);
		rp = (double) m_dRpMin;
		FindMinMax(pData, n, normMin, normMax);
		//NormMinMax(normMin, normMax);
		m_scxr = (m_lxr) / (log10(m_dRpMax) - log10(m_dRpMin));
		m_scyr = (m_lyr) / (log10(normMax) - log10(normMin));
		m_offxr = -log10(m_dRpMin)*m_scxr;
		m_offyr = -log10(normMin)*m_scyr;
		x = (int) (m_oxr + m_offxr + log10(m_dRpMin)*m_scxr);
		y = (int) (m_oyr - m_offyr - log10(pData[0])*m_scyr);
		pDC ->MoveTo(x, y);
		for(int i=1; i < n; i++) {
			x = (int) (m_oxr + m_offxr + log10(rp)*m_scxr);
			y = (int) (m_oyr - m_offyr - log10(pData[i])*m_scyr);
			pDC->LineTo(x, y);
			rp *= (double) m_dRpQ;
		}
		pDC->SelectObject(pPen);

		// show current reg par
		CPen penS( PS_SOLID, 1, RGB(255,255,255) );
		pPen = pDC->SelectObject(&penS);
		rp = m_rglrz.GetRegPar();
		m_dRp = rp;
		x = (int) (m_oxr + m_offxr + log10(rp)*m_scxr);
		pDC->MoveTo(x, m_oyr);
		pDC->LineTo(x, (m_oyr + 1) - m_lyr);
		pDC->SelectObject(pPen);

		CFont *pFont = pDC->SelectObject(&m_fontRp);
		CString str;
		str.Format("regularization parameter = %e", rp);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor( RGB(255, 255, 255) );
		pDC->TextOut(250, 170, str);
		pDC->SelectObject(pFont);
	}

	n = (int) m_rglrz.m_spec.GetSize();
	pData = m_rglrz.m_spec.GetData();
	if(pData != NULL && m_bSpec) {
		CPen penSpec( PS_SOLID, 1, RGB(255, 185, 185) );
		pPen = pDC->SelectObject(&penSpec);
		double min, max;
		FindMinMax(pData, n, min, max);
		double scx = ((double) (m_lxr))/(n);		// begin is zero
		double scy = ((double) (m_lyr))/( max - min );
		double offy = min*scy;
		x = (int) (m_oxr);
		y = (int) (m_oyr + offy - pData[0]*scy);
		pDC ->MoveTo(x, m_oyr);
		pDC ->LineTo(x, y);
		for(int i=1; i < n; i++) {
			x = (int) (m_oxr + i*scx);
			y = (int) (m_oyr + offy - pData[i]*scy);
			pDC ->MoveTo(x, m_oyr);
			pDC ->LineTo(x, y);
		}
		pDC->SelectObject(pPen);
	}

}

void CDlgReglrz::OnButInport()
{
	CDlgProfChan dlg;
	ChannelData pData;


	dlg.m_pDtb = m_pDtb;
	dlg.m_strCurProf = m_strCurProf ;
	dlg.m_strPos = m_strPos;
	dlg.m_strFld = m_strFld;
	if( dlg.DoModal() == IDOK ) {
		m_pProf = dlg.m_pProf;
		m_pChan = dlg.m_pChan;
		// transfer data from channel to rglrz
		pData.Append( *(m_pChan->GetData()) );
		int i, m, n;
		n = (int)pData.GetSize();
		i = 2;
		while(n > pow(2.0,i) ) {
		i++;}
		m = (int) pow(2.0, i);
		if(n<m) {
			pData.SetSize(m);
			for(i=n; i < m; i++)
				pData[i] = pData[n-1];
		}
		m_bInit = TRUE;
		m_rglrz.SetData(pData.GetData(), (int) pData.GetSize(), /*m_dDelta*/0.2);
		FindMinMax(pData.GetData(), (int) pData.GetSize(), m_dMin, m_dMax);
		InvalidateRect(m_drwRect, FALSE);
		//Invalidate();
		UpdateWindow();
		OnButRegularize();
		//AfxMessageBox("Add code!");
	}
}

void CDlgReglrz::OnButRegularize()
{
	if(m_bInit) {
		// read data
		UpdateData();
		m_rglrz.SetRegPar(m_dRpMin, m_dRpMax, m_dRpQ);
		m_rglrz.Compute(m_nRegTag, m_fFldCont);
		m_rglrz.Compute_Reg(m_nRegTag, m_fFldCont);

		InvalidateRect(m_drwRect, FALSE);
		UpdateWindow();
	}
	else {
		AfxMessageBox("Data please...", MB_ICONINFORMATION);
	}
}

void CDlgReglrz::FindMinMax(double *data, int n, double &min, double &max)
{
	min = data[0];
	max = data[0];
	for(int i=1; i<n; i++) {
		min = min(min, data[i]);
		max = max(max, data[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//************** mouse routines ***********************************************************
void CDlgReglrz::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bInit) {

		if(m_bCaptured) {
			double rp = pow(10, (point.x - m_oxr -m_offxr)/m_scxr);
			m_rglrz.Compute_Reg(m_nRegTag, rp, m_fFldCont);

			InvalidateRect(m_drwRect, FALSE);
			UpdateWindow();
		}
		else {
			int x = (int) (m_oxr + m_offxr + log10(m_dRp)*m_scxr);
			//double rp = pow(10, (point.x - m_oxr -m_offxr)/m_scxr);
			if( x == point.x && (m_oyr+140) > point.y && (m_oyr-m_lyr+140) < point.y) {
				m_hCursor = ::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_REGPAR));
			}
			else {
				::SetCursor(m_hCursor);
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgReglrz::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_bInit) {
		if(nFlags == (MK_CONTROL | MK_LBUTTON   ) ) {
			if((m_oyr+110) > point.y && (m_oyr-m_lyr+110) < point.y) {
				double rp = (float) pow(10, (point.x - m_oxr -m_offxr)/m_scxr);
				if(rp<m_dRpMin)
					AfxMessageBox("rp < rp_min");
				else {
					m_dRpMin = (float) rp;
					UpdateData(FALSE);
					OnButRegularize();
				}

			}
			return;
		}

		int x = (int) (m_oxr + m_offxr + log10(m_dRp)*m_scxr);
		if( x == point.x && (m_oyr+110) > point.y && (m_oyr-m_lyr+110) < point.y ) {
			SetCapture();
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_REGPAR));
			m_bCaptured = TRUE;
		}
	}
	//CDialog::OnLButtonDown(nFlags, point);
}

void CDlgReglrz::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bCaptured) {
		m_bCaptured = FALSE;
		ReleaseCapture();

		InvalidateRect(m_drwRect, FALSE);
		UpdateWindow();
	}

	if(!m_bCaptured && m_bInit) {
		if( (nFlags != 8) && (m_oyr+110) > point.y && (m_oyr-m_lyr+110) < point.y) {
			double rp = pow(10, (point.x - m_oxr -m_offxr)/m_scxr);
			m_rglrz.Compute_Reg(m_nRegTag, rp, m_fFldCont);
			InvalidateRect(m_drwRect, FALSE);
			UpdateWindow();
		}

	}
	//CDialog::OnLButtonUp(nFlags, point);
}
//************** end mouse routines *******************************************************
///////////////////////////////////////////////////////////////////////////////////////////
void CDlgReglrz::OnRButtonDown(UINT nFlags, CPoint point)
{
	if( (nFlags == (MK_CONTROL | MK_RBUTTON) && m_bInit  ) ) {
		if((m_oyr+110) > point.y && (m_oyr-m_lyr+110) < point.y) {
			double rp = (float)pow(10, (point.x - m_oxr -m_offxr)/m_scxr);
			if(rp>m_dRpMax)
				AfxMessageBox("rp > rp max");
			else {
				m_dRpMax = (float) rp;
				UpdateData(FALSE);
				OnButRegularize();
			}
		}
	}
}

void CDlgReglrz::OnButDefault()
{
	m_dRpMax = 1.0e+10f;
	m_dRpMin = 1.0e-10f;
	m_dRpQ = 1.1f;
	UpdateData(FALSE);
	OnButRegularize();
}

void CDlgReglrz::OnRadioRegtag()
{
	UpdateData();
	OnButDefault();
}

void CDlgReglrz::OnRadio2()
{
	UpdateData();
	OnButDefault();
}

void CDlgReglrz::OnRadio3()
{
	UpdateData();
	OnButDefault();
}

void CDlgReglrz::OnCheckData()
{
	UpdateData();
	InvalidateRect(m_drwRect, FALSE);
	UpdateWindow();
}

void CDlgReglrz::OnCheckRglrz()
{
	UpdateData();
	InvalidateRect(m_drwRect, FALSE);
	UpdateWindow();
}

void CDlgReglrz::OnCheckFft()
{
	UpdateData();
	InvalidateRect(m_drwRect, FALSE);
	UpdateWindow();
}

void CDlgReglrz::OnCheckSpec()
{
	UpdateData();
	InvalidateRect(m_drwRect, FALSE);
	UpdateWindow();
}

HBRUSH CDlgReglrz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_EDIT ) {
		CRect rc, rcd;
		GetWindowRect(&rcd);
		pWnd->GetWindowRect(&rc);
		pDC->SetBrushOrg(-rc.left+rcd.left+3, -rc.top+rcd.top+15);
	}
	pDC->SetTextColor( RGB(56, 81, 137) );		// text
	pDC->SetBkMode( TRANSPARENT );				// text bkgnd
	return m_bkBrush;

}

void CDlgReglrz::OnButPrevMin()
{
	m_rglrz.FindRegPar(RP_PREV);
	m_rglrz.Compute_Reg(m_nRegTag, m_rglrz.m_dRegPar, m_fFldCont);
	InvalidateRect(m_drwRect, FALSE);
	UpdateWindow();
}

void CDlgReglrz::OnButNextMin()
{
	m_rglrz.FindRegPar(RP_NEXT);
	m_rglrz.Compute_Reg(m_nRegTag, m_rglrz.m_dRegPar, m_fFldCont);
	InvalidateRect(m_drwRect, FALSE);
	UpdateWindow();

}

void CDlgReglrz::OnBnClickedClose()
{
	CDialog::OnOK();
}

void CDlgReglrz::OnBnClickedCancel()
{
	OnCancel();
}

void CDlgReglrz::OnBnClickedButSetdata()
{
	DArray *pData;
	CString strChan;

	switch( m_nRegTag ) {
		case 0:
			strChan = "dz";
			break;
		case 1:
			strChan = "dx";
			break;
		case 2:
			strChan = "fldcont";
			break;
	}
	switch( m_nOut ) {
		case 0:
			pData = &(m_rglrz.m_noreg);
		break;
		case 1:
			strChan += "_reg";
			pData = &(m_rglrz.m_reg);
			break;
	}
	pData = &(m_rglrz.m_reg);
	m_pProf->SetData( pData, strChan );
}
