// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "InvNew.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double	g_x = 0, g_z = 0, g_comp=0;
int		g_prof;
CString g_strMeth;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//	ON_WM_CREATE()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	// status bar indicators
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_X, OnUpdateCoordinateX)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_Z, OnUpdateCoordinateZ)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_METHOD, OnUpdateMethod)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PROFILE, OnUpdateProfile)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COMPUTE, OnUpdateCompute)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROFILE,
	ID_INDICATOR_METHOD,
	ID_INDICATOR_X,
	ID_INDICATOR_Z,
};

void CMainFrame::OnUpdateCompute(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
	CString str;
	str.Format( "Computed %3.0f %%", (float) g_comp ); 
	pCmdUI->SetText( str ); 
}
void CMainFrame::OnUpdateProfile(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
	CString str;
	str.Format( "Profile %d", g_prof ); 
	pCmdUI->SetText( str ); 
}
void CMainFrame::OnUpdateMethod(CCmdUI *pCmdUI)
{
	pCmdUI->SetText( g_strMeth ); 
}
void CMainFrame::OnUpdateCoordinateX(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
	CString str;
	str.Format( "x = %6.6f", (float) g_x ); 
	pCmdUI->SetText( str ); 
}

void CMainFrame::OnUpdateCoordinateZ(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
	CString str;
	str.Format( "z = %f", (float) g_z ); 
	pCmdUI->SetText( str ); 
}


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	/*if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}*/

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) /*||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	// open new wiew automatically
	PostMessage(WM_COMMAND,ID_FILE_MRU_FILE1,0);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::DrawProgress(double dCount, COLORREF color)
{
	if( dCount >= 0 && dCount <= 1 )	{
		CRect indRect;
		CBrush	brush( color ), *pBrush;
		CBrush	hollowBrush; hollowBrush.CreateSysColorBrush(COLOR_BTNFACE);
		CStatusBarCtrl& pStatusCtrl = GetStatusBar()->GetStatusBarCtrl();
		CDC *pDC = pStatusCtrl.GetDC();

		if( dCount == 1.0 ) 		pStatusCtrl.SetText("Computation finished.", 0, SBT_NOBORDERS );
		if( dCount == 0.0 )			pStatusCtrl.SetText("Computation is runnning", 0, SBT_NOBORDERS );
		pStatusCtrl.GetRect( 1, &indRect );
		pBrush = pDC->SelectObject( &hollowBrush );
		pDC->Rectangle( indRect.left-3, indRect.top, indRect.left-3-(int)(indRect.Width()), indRect.bottom);
		pDC->SelectObject( pBrush );
		pBrush = pDC->SelectObject( &brush );
		pDC->Rectangle( indRect.left-3-indRect.Width(), indRect.top, indRect.left-3-(int)(indRect.Width()*(1-dCount)), indRect.bottom);
		pDC->SelectObject( pBrush );
		brush.DeleteObject();
		hollowBrush.DeleteObject();
		ReleaseDC( pDC );
	}
}

