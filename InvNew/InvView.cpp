// InvView.cpp : implementation of the CInvView class
//

#include "stdafx.h"
#include "InvNew.h"

#include "InvDoc.h"
#include "InvView.h"
#include "Meth2DLinWerner.h"
#include "DlgSetRange.h"
#include "DlgClustering.h"
#include "DlgClusteringSmall.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern double	g_x;
extern double	g_z;
extern int		g_prof;
extern CString g_strMeth;

// CInvView

IMPLEMENT_DYNCREATE(CInvView, CView)

BEGIN_MESSAGE_MAP(CInvView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_ZOOM_PAGE, OnViewZoomPage)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_METHOD_CLUSTERING, OnMethodClustering)
	ON_UPDATE_COMMAND_UI(ID_METHOD_CLUSTERING, OnUpdateMethodClustering)
	ON_COMMAND(ID_VIEW_REDRAW, OnViewRedraw)
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
//	ON_WM_SIZING()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CInvView construction/destruction

CInvView::CInvView()
{
	m_bInitialized = FALSE;

	m_scBarVrtFld.SetType(SB_VERT);
	m_scBarVrtDpt.SetType(SB_VERT);
	m_scBarVrtVirtual.SetType(SB_VERT);

	m_nScBarCaptured = SCB_NONE;

	m_nCurProf = 0;

	m_bmpBkg.LoadBitmap(IDB_BKG);
	m_bScaleEq = FALSE;
	m_dScEq = 1;
	m_dResize = 0.5;

	m_fldPen.SetSize(100);
	m_fldPen.GetAt(0).CreatePen(PS_SOLID, 1, RGB(0,0,180) );
	m_fldPen.GetAt(1).CreatePen(PS_SOLID, 1, RGB(180,0,0) );
	m_fldPen.GetAt(2).CreatePen(PS_SOLID, 1, RGB(0,180,0) );
	m_fldPen.GetAt(3).CreatePen(PS_SOLID, 1, RGB(0,180,180) );
	m_fldPen.GetAt(4).CreatePen(PS_DOT, 1, RGB(0,0,180) );
	m_fldPen.GetAt(5).CreatePen(PS_DOT, 1, RGB(180,0,0) );
	m_fldPen.GetAt(6).CreatePen(PS_DOT, 1, RGB(0,180,0) );
	m_fldPen.GetAt(7).CreatePen(PS_DOT, 1, RGB(0,180,180) );
	m_fldPen.GetAt(8).CreatePen(PS_DASH, 1, RGB(0,0,180) );
	m_fldPen.GetAt(9).CreatePen(PS_DASH, 1, RGB(180,0,0) );
	m_fldPen.GetAt(10).CreatePen(PS_DASH, 1, RGB(0,180,0) );
	m_fldPen.GetAt(11).CreatePen(PS_DASH, 1, RGB(0,180,180) );
}

CInvView::~CInvView()
{
}

void CInvView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pDoc = GetDocument();
	ASSERT_VALID(m_pDoc);

	// profile drawing
	if( !m_pDoc->m_dtb.IsEmpty() && m_pDoc->IsCurProfileOK() ) {
		double min, max, h;
		CDtbProf *pProf = m_pDoc->GetCurProf();
		CDtbProfChan *pChan;
		
		// position channel
		pChan = pProf->GetChannel( m_pDoc->m_strPos); 
		if( pChan == NULL) {
			AfxMessageBox("Error postion channel not found!");
			return;		
		}
		m_pDoc->m_dtb.GetMinMax(pProf->GetChannelNum( m_pDoc->m_strPos), min, max);
		m_scBarHrz.SetMaxDataExtent(min, max );
		m_scBarHrz.SetDataExtend(min, max );
		h=max-min;
		m_scBarVrtDpt.SetMaxDataExtent(min-h, 0 );
		m_scBarVrtDpt.SetDataExtend(min-h, 0 );

		
		// elevation channel
		pChan = pProf->GetChannel( m_pDoc->m_strElev ); 
		if( pChan != NULL) {
			pChan->UpdateMinMax();
			pChan->GetMinMaxValue(min, max);
			m_scBarVrtDpt.SetMaxDataExtent(min-h, max );
			m_scBarVrtDpt.SetDataExtend(min-h, max );
		}
		else {
			m_scBarVrtDpt.SetMaxDataExtent( -h, 0 );
			m_scBarVrtDpt.SetDataExtend( -h, 0 );
		}
		
		// already initialized here
		m_bInitialized = TRUE;
		ReSize();

		// field channel
		pChan = pProf->GetChannel( m_pDoc->m_strFld ); 
		if( pChan == NULL) {
			AfxMessageBox("Error field channel not found!");
			return;
		}
		m_pDoc->m_dtb.GetMinMax( pProf->GetChannelNum( m_pDoc->m_strFld), min, max);
		m_scBarVrtFld.SetMaxDataExtent(min, max );
		m_scBarVrtFld.SetDataExtend(min, max );
	}
	// solutions drawing
}

void CInvView::ReSize(void)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect rect;
	GetClientRect(&rect);
	dc.DPtoLP(&rect);
	//***************** new *************************************************************************
	double	sc;		// csale
	double	lx, ly;				// area coordinates
	double	minX, minY, maxX, maxY, min, max;
	int		dx, dy;			// logical coordinates device
	CSize	gap(12,12);
	CSize	wh;				// width & hiht in logical coord
	CPoint	ptO;			// origo
	int		mid;
	CRect	m_rectDrw;

	// compute new drawing rectanfle
	dc.DPtoLP( &gap );
	wh = rect.Size();
	mid = (int)(wh.cy*m_dResize);

	m_rectDrw = CRect(gap.cx, -gap.cy, wh.cx-gap.cx, wh.cy+gap.cy );
	m_rectFld = CRect(m_rectDrw.left, m_rectDrw.top, m_rectDrw.right, mid+gap.cy);
	m_rectInv = CRect(m_rectDrw.left, mid-gap.cy, m_rectDrw.right, m_rectDrw.bottom);

	//scale to rectangles
	m_scBarHrz.GetDataMinMax(minX, maxX);
	lx = maxX-minX;
	dx = m_rectDrw.Width();
	m_scOffX.sc = fabs(dx/lx);

	m_scBarVrtDpt.GetDataMinMax(minY, maxY);
	ly = maxY-minY;
	dy = m_rectFld.Height();
	m_scOffYDpt.sc = fabs(dy/ly);

	if( m_bScaleEq ) {
		sc = min(m_scOffX.sc, m_dScEq*m_scOffYDpt.sc);
		m_scOffX.sc = sc;
		m_scOffYDpt.sc = m_dScEq*sc;
		dx = (int) (m_scOffX.sc*lx);
		dy = (int) (m_scOffYDpt.sc*ly);
	}
	dy = abs(dy);	// in following section assumed to be positive
	m_scBarHrz.SetDrawRect(m_rectFld.left, m_rectFld.bottom, m_rectFld.left+dx, m_rectFld.bottom-gap.cy);
	m_scBarVrtDpt.SetDrawRect(0, m_rectInv.top, gap.cy, m_rectInv.bottom);
	m_scBarVrtFld.SetDrawRect(0, m_rectFld.top, gap.cy, m_rectFld.bottom);
	m_scBarVrtVirtual.SetDrawRect(0, m_rectFld.top, gap.cy, m_rectFld.bottom);

	//m_rectDrw = CRect(gap.cx, -gap.cy, dx-gap.cx, -dy+gap.cy );
	m_scBarHrz.GetDataExtent(min, max);
	m_scBarHrz.SetDataExtend(min, max );
	m_scBarVrtDpt.GetDataExtent(min, max);
	m_scBarVrtDpt.SetDataExtend(min, max );

	// set origo
	ptO = CPoint( m_rectFld.left, m_rectFld.bottom );
	m_axX.SetParam( ptO, m_rectFld.Width());
	m_axFldY.SetParam( ptO, abs(m_rectFld.Height()) );
	ptO = CPoint( m_rectInv.left, m_rectInv.bottom );
	m_axDptY.SetParam( ptO, abs(m_rectInv.Height()) );

	m_scOffX.off = m_rectFld.left - m_scOffX.sc*minX;	//sc*m_ox0;
	m_scOffYDpt.off = m_rectFld.bottom - m_scOffYDpt.sc*minY;	//sc*m_oy0;

	//***************** new *************************************************************************
}

int CInvView::ResizeDrwRect(CPoint lPt)
{
	return 0;
}

BOOL CInvView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CInvView drawing
void CInvView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if( !pDC->IsPrinting() ) {
		CRect rc;
		int lx = 10000, ly = 10000;

		GetClientRect(rc);
		pDC->SetMapMode(MM_ISOTROPIC);
		pDC->SetWindowExt(lx, ly);					// logical coordinates 
		pDC->SetViewportExt(rc.right, -rc.bottom);
		pDC->SetWindowOrg(0, 0);					// logical coord.
		pDC->SetViewportOrg(0, 0);
		pDC->SetBkMode(TRANSPARENT);
	}
	else {
		pDC->SetMapMode( MM_TWIPS );
		ReSize();
	}
	//CView::OnPrepareDC(pDC, pInfo);
}

bool CInvView::Scale(void)
{
	return false;
}

void CInvView::DrawBmpBkg(CDC *pDC) 
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(m_bmpBkg);
	BITMAP bmp;
	m_bmpBkg.GetBitmap( &bmp);
	pDC->StretchBlt(0, 0, 10000, -10000, &memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}

void CInvView::OnDraw(CDC* pDC)
{
	if( !m_pDoc->IsEmpty() && m_bInitialized  && !pDC->IsPrinting()) {
		CBitmap bmp;
		CRect clRect;
		CRect lpRect;
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		GetClientRect(clRect);
		lpRect = clRect;
		pDC->DPtoLP( &lpRect );
		bmp.CreateCompatibleBitmap(pDC, clRect.right-clRect.left, clRect.bottom-clRect.top);
		CBitmap *pBmp = memDC.SelectObject(&bmp);
		CBrush bkBrush((COLORREF)  ::GetSysColor(COLOR_WINDOW));
		CBrush* pdBrush = memDC.SelectObject(&bkBrush);
		memDC.PatBlt(0, 0, lpRect.right, -lpRect.bottom, PATCOPY);
		OnPrepareDC(&memDC);

		//DrawBmpBkg( &memDC );
		memDC.MoveTo(m_rectDrw.left, m_rectFld.bottom - (m_rectFld.bottom-m_rectInv.top)/2 );
		memDC.LineTo(m_rectDrw.right, m_rectFld.bottom - (m_rectFld.bottom-m_rectInv.top)/2 );
		//ScaleFld();
		//Scale();

		m_scBarHrz.Draw(&memDC);
		m_scBarVrtFld.Draw(&memDC);
		m_scBarVrtDpt.Draw(&memDC);
		DrawField(&memDC);
		DrawMethod(&memDC);
		DrawAxes(&memDC);	

		pDC->BitBlt(0, 0, lpRect.right, lpRect.bottom, &memDC, 0, 0, SRCCOPY  );
		memDC.SelectObject(pBmp);
		memDC.SelectObject(pdBrush);
		bmp.DeleteObject();
	}
	if( !m_pDoc->IsEmpty() && m_bInitialized  && pDC->IsPrinting()) {
		//m_scBarHrz.Draw(pDC);
		//m_scBarVrtFld.Draw( pDC );
		m_scBarVrtDpt.Draw(pDC);
		DrawField(pDC);
		DrawMethod(pDC);
		DrawAxes(pDC);	
	}
}

void CInvView::DrawAxes(CDC* pDC)
{
	double  min, max;
	DATALIMITS datLim;

	//x axis
	m_scBarHrz.GetDataExtent(min, max);
	datLim.min = min;
	datLim.max = max;
	datLim.TickStep = 0;
	m_axX.SetDatLim(datLim);
	m_axX.FindAxisMinMaxStep(4);
	m_axX.Draw(pDC);

	// field y axis
	m_scBarVrtFld.GetDataExtent( min, max );
	datLim.min = min;
	datLim.max = max;
	datLim.TickStep = 0;
	m_axFldY.SetDatLim( datLim );
	m_axFldY.FindAxisMinMaxStep();
	m_axFldY.Draw(pDC);

	// depth y axis
	m_scBarVrtDpt.GetDataExtent( min, max );
	datLim.min = min;
	datLim.max = max;
	datLim.TickStep = 0;
	m_axDptY.SetDatLim( datLim );
	m_axDptY.FindAxisMinMaxStep();
	m_axDptY.Draw(pDC);
}

int CInvView::DrawField(CDC* pDC)
{
	CDtbProf *pProf = m_pDoc->GetCurProf();
	CDtbProfChan *pChan;
	double min, max;
	double scx, offx, scy, offy, sch, offh;
	double *pX, *pY, *pH;	// data double array
	int m, n;				// number of point in array
	int i;				// itereating
	int x, y;			// drawing logical coordinates
	CMyScaleBar	*pScBar;
	CPen* pPen;

	// position channel
	pChan = pProf->GetChannel( m_pDoc->m_strPos ); 
	if( pChan == NULL) {
		AfxMessageBox("Elevation channel not found!");
		return 0;
	}
	n = pChan->GetSize();
	pX = pChan->GetData()->GetData();
	pChan->GetMinMaxValue(min, max);
	m_scBarHrz.GetScOff(scx, offx);
	
	// field channels
	m = pProf->GetNumberOfChannels();
	for( i = 0; i < m; i++) {
		pChan = pProf->GetChannel( i );
		if( pChan->GetName() != m_pDoc->m_strPos && pChan->GetName() != m_pDoc->m_strElev ) {
			if( pChan->GetName() == m_pDoc->m_strFld ) {
				pScBar = &m_scBarVrtFld;
			}
			else {
				pChan->GetMinMaxValue(min, max);
				m_scBarVrtVirtual.SetMaxDataExtent(min, max );
				m_scBarVrtVirtual.SetDataExtend(min, max );
				pScBar = &m_scBarVrtVirtual;
			}
			// draw data
			if( i < 99 ) {
				pPen = pDC->SelectObject( &(m_fldPen.GetAt(i-1)) );
				pY = pChan->GetData()->GetData();
				pScBar->GetScOff( scy, offy );
				x = (int) ( offx + scx*pX[0] );
				y = (int) ( offy + scy*pY[0] );
				pDC->MoveTo( x, y );
				for( int j = 1; j < n; j++) {
					x = (int) ( offx + scx*pX[j] );
					y = (int) ( offy + scy*pY[j] );
					pDC->LineTo( x, y );
				}
				DrawLegend(pDC, i, pChan->GetName() );
				pDC->SelectObject( pPen);
			}
		}
	}

	// elevation channel
	pChan = pProf->GetChannel(m_pDoc->m_strElev); 
	if( pChan != NULL) {
		pChan->GetMinMaxValue(min, max);
		pH = pChan->GetData()->GetData();
		m_scBarVrtDpt.GetScOff( sch, offh );

		x = (int) ( offx + scx*pX[0] );
		y = (int) ( offh + sch*pH[0] );
		pDC->MoveTo( x, y );
		for( int i = 1; i < n; i++) {
			x = (int) ( offx + scx*pX[i] );
			y = (int) ( offh + sch*pH[i] );
			pDC->LineTo( x, y );
		}
	}
	
	return 1;
}

int CInvView::DrawLegend(CDC* pDC, int i, CString str)
{
	CSize sz(20,20);
	int right = m_rectFld.right, top = m_rectFld.top;

	pDC->DPtoLP( &sz );
	pDC->MoveTo(right-sz.cx, top-sz.cy*i);
	pDC->LineTo(right, top-sz.cy*i);
	pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );
	pDC->TextOut(right-sz.cx, top-sz.cy*i, str );
	return 0;
}

void CInvView::DrawMethod(CDC* pDC)
{
	MethodArray *pMa = &(m_pDoc->m_methArray);
	CMeth2DLin	*pMet;
	double scx, offx, scy, offy;

	//for( int i=0; i < (int) pMa->GetSize(); i++ ) {
	//	if( pMa->GetAt(i)->GetMethod() == METH_WERNER ) {
	//		pWr = (CMeth2DLinWerner*) pMa->GetAt(i);
	//		m_scBarHrz.GetScOff( scx, offx );
	//		m_scBarVrtDpt.GetScOff( scy, offy );
	//		pWr->Draw(pDC, scx, offx, scy, offy);
	//	}
	//}
	for( int i=0; i < (int) pMa->GetSize(); i++ ) {
		if( pMa->GetAt(i)->GetMethod() == METH_WERNER ) {
			pMet = pMa->GetAt(i);
			if( pMet->GetProfID() == m_pDoc->GetCurProf()->GetID() ) {
				//m_pDoc->m_pMethSel = pMet;
				m_scBarHrz.GetScOff( scx, offx );
				m_scBarVrtDpt.GetScOff( scy, offy );
				pMet->Draw(pDC, scx, offx, scy, offy);
			}
		}
	}
}

//********************* end drawing ********************************************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// CInvView printing

BOOL CInvView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage( 1 );
	return DoPreparePrinting(pInfo);
}

void CInvView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CInvView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CInvView diagnostics

#ifdef _DEBUG
void CInvView::AssertValid() const
{
	CView::AssertValid();
}

void CInvView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInvDoc* CInvView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInvDoc)));
	return (CInvDoc*)m_pDocument;
}
#endif //_DEBUG


// CInvView message handlers

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//***************** mouse *********************************************************************************
void CInvView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint lPt = point;	
	dc.DPtoLP(&lPt,1);

	// horizontal scale bar
	if( m_scBarHrz.IsClicked(lPt) != SB_NON ) {
		if( nFlags == (MK_CONTROL | MK_LBUTTON) ) {
			SetScBarRange( &m_scBarHrz );
			return;
		}
		m_nScBarCaptured = SCB_POS;
		SetCapture();
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HSCROLL));
		return;
	}

	// vertical field scale bar
	if( m_scBarVrtFld.IsClicked(lPt) != SB_NON ) {
		if( nFlags == (MK_CONTROL | MK_LBUTTON) ) {
			SetScBarRange( &m_scBarVrtFld );
			return;
		}
		m_nScBarCaptured = SCB_FLD;
		SetCapture();
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_VSCROLL));
		return;
	}

	// vertical field scale bar
	if( m_scBarVrtDpt.IsClicked(lPt) != SB_NON ) {
		if( nFlags == (MK_CONTROL | MK_LBUTTON) ) {
			SetScBarRange( &m_scBarVrtDpt );
			return;
		}
		m_nScBarCaptured = SCB_DPT;
		SetCapture();
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_VSCROLL));
		return;
	}
}

void CInvView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint lPt = point;	
	dc.DPtoLP(&lPt,1);

	// horizontal scale bar
	if( m_scBarHrz.IsClicked(lPt) != SB_NON && nFlags == MK_RBUTTON ) {
		SetScBarRange( &m_scBarHrz );
		return;
	}

	// vertical field scale bar
	if( m_scBarVrtFld.IsClicked(lPt) != SB_NON && nFlags == MK_RBUTTON ) {
		SetScBarRange( &m_scBarVrtFld );
		return;
	}

	// vertical field scale bar
	if( m_scBarVrtDpt.IsClicked(lPt) != SB_NON  && nFlags == MK_RBUTTON  ) {
		SetScBarRange( &m_scBarVrtDpt );
		return;
	}
}
void CInvView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint lPt = point;	// point in logical coordinates
	dc.DPtoLP(&lPt,1);

	if( m_nScBarCaptured != SCB_NONE ) {
		ReleaseCapture();
		m_nScBarCaptured = SCB_NONE;
		Invalidate( FALSE );
		return;
	}
}

void CInvView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint lPt = point;	// point in logical coordinates
	dc.DPtoLP(&lPt,1);

	SetIndicators( lPt);
	// move only if something is clicked
	if( m_nScBarCaptured != SCB_NONE ) {
/*
		// zooming
		if(m_bZooming) {
			m_zoomRect.bottom = lPt.y;
			m_zoomRect.right = lPt.x;
			Invalidate(FALSE);
			return;
		}
*/
		// position scale bar
		if(m_nScBarCaptured == SCB_POS) {
			m_scBarHrz.Move(lPt);
			Invalidate(FALSE);
			return;
		}

		// field scale bar
		if(m_nScBarCaptured == SCB_FLD) {
			m_scBarVrtFld.Move(lPt);
			Invalidate(FALSE);
			return;
		}

		// inversion (depth) scale bar
		if(m_nScBarCaptured == SCB_DPT) {
			m_scBarVrtDpt.Move(lPt);
			Invalidate(FALSE);
			return;
		}
	}
}
//***************** end mouse *****************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//************** zooming *************************************************************************************
int CInvView::SetScBarRange(CMyScaleBar* pScBar)
{
	CDlgSetRange dlg;
	double min, max;

	pScBar->GetDataExtent( min, max );
	dlg.m_dMin = (float)min; dlg.m_dMax = (float)max;
	if(dlg.DoModal() == IDOK ) {
		pScBar->SetMaxDataExtent( dlg.m_dMin, dlg.m_dMax );
		pScBar->SetDataExtend( dlg.m_dMin, dlg.m_dMax );
		Invalidate( FALSE );
	}
	
	return 0;
}

void CInvView::OnViewZoom()
{
/*	SetCapture();
	::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM));
	m_bZoom = TRUE;
	m_bCaptureActive = TRUE;
*/
}

void CInvView::OnViewZoomIn()
{
/*
	SetCapture();
	::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_IN));
	m_bZoomIn = TRUE;
	m_bCaptureActive = TRUE;
*/
}

void CInvView::OnViewZoomOut()
{
/*
	SetCapture();
	::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_OUT));
	m_bZoomOut = TRUE;
	m_bCaptureActive = TRUE;
*/
}

void CInvView::OnViewZoomPage()
{
/*	m_HRowScBar.SetDataExtend(m_gx0, m_gx0 + m_gxSize*(m_nGrdCols-1) );
	m_HColScBar.SetDataExtend(m_gy0, m_gy0 + m_gySize*(m_nGrdRows-1) );
	m_scBarVrtDpt.SetDataExtend(m_dMinZ, m_dMaxZ);
	Scale();
	FindDrawIndex();
	m_pDoc->UpdateMod3DViews();*/
}

void CInvView::Zoom()
{
/*
	double min, max;			// map coordinates
	int l, r, t, b, temp;

	l = m_zoomRect.left;
	r = m_zoomRect.right;
	t = m_zoomRect.top;
	b = m_zoomRect.bottom;
	if(l>r) {
		temp = l; l = r; r = temp;
	}
	if(b>t) {
		temp = b; b = t; t = temp;
	}
	CMyScaleBar *pScBar = GetHScBar();
	// transform to map coordinates
	min = (l - m_xOff)/ m_xSc;
	max = (r - m_xOff)/ m_xSc;
	pScBar->SetDataExtend(min, max);
	max = (t - m_zOff)/ m_zSc;
	min = (b - m_zOff)/ m_zSc;
	m_scBarVrtDpt.SetDataExtend(min, max );
	*/
}

void CInvView::ZoomIn(CPoint lPt)
{
/*
	double min, max;			// map coordinates
	double d, center;


	CMyScaleBar *pScBar = GetHScBar();
	// transform to map coordinates
	center = (lPt.x - m_xOff)/ m_xSc;
	pScBar->GetDataExtent(min, max);
	d = (max-min)*3/8;
	pScBar->SetDataExtend(center-d, center+d);

	center = (lPt.y - m_zOff)/ m_zSc;
	m_scBarVrtDpt.GetDataExtent(min, max);
	d = (max-min)*3/8;
	m_scBarVrtDpt.SetDataExtend( center-d, center+d );
*/
}

void CInvView::ZoomOut(CPoint lPt)
{
/*
	double min, max;			// map coordinates
	double d, center;

	CMyScaleBar *pScBar = GetHScBar();
	// transform to map coordinates
	center = (lPt.x - m_xOff)/ m_xSc;
	pScBar->GetDataExtent(min, max);
	d = (max-min)*(1.1);
	pScBar->SetDataExtend(center-d, center+d);

	center = (lPt.y - m_zOff)/ m_zSc;
	m_scBarVrtDpt.GetDataExtent(min, max);
	d = (max-min)*(1.1);
	m_scBarVrtDpt.SetDataExtend( center-d, center+d );
*/
}

//************** end zooming *********************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CInvView::OnMethodClustering()
{
	CDlgClusteringSmall dlg;
	CMeth2DLin *pMeth = NULL;

	pMeth = m_pDoc->m_pMethSel;
	if( pMeth != NULL) {
		dlg.m_pMeth = pMeth;
		dlg.m_pView = this;
		dlg.DoModal();
		m_pDoc->SetModifiedFlag();
	}
	else
		AfxMessageBox(" No method results selected! Please select results.");
}

void CInvView::OnUpdateMethodClustering(CCmdUI *pCmdUI)
{
	if( m_pDoc->m_methArray.IsEmpty() ) 
		pCmdUI->SetCheck( 0 );
	else
		pCmdUI->SetCheck( 0 );
}

void CInvView::OnViewRedraw()
{
	m_pDoc->UpdateAllViews();
}


BOOL CInvView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( zDelta < 0) {
		m_pDoc->OnDatabaseProfileNext();
	}
	else {
		m_pDoc->OnDatabaseProfilePrev();
	}
	SetIndicators();

	return TRUE;
	//return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CInvView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar) {
		case VK_UP:
			m_pDoc->OnDatabaseProfilePrev();
			break;
		case VK_DOWN:
			m_pDoc->OnDatabaseProfileNext();
			break;
		case VK_NEXT:	// page down
			break;
		case VK_PRIOR:	// [page up
			break;
		case VK_RIGHT:
			m_pDoc->OnDatabaseProfileNext();
			break;
		case VK_LEFT:
			m_pDoc->OnDatabaseProfilePrev();
			break;
		case VK_ESCAPE:
			//::ReleaseCapture();
			break;
	}
	SetIndicators();
	//CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInvView::SetIndicators(CPoint lPt)
{
	double scx, offx, scy, offy;

	m_scBarHrz.GetScOff( scx, offx );
	m_scBarVrtDpt.GetScOff( scy, offy );
	g_x = (lPt.x - offx)/scx;
	g_z = (lPt.y - offy)/scy;
	SetIndicators();
}

void CInvView::SetIndicators()
{
	if( m_pDoc->m_pMethSel != NULL)
		g_strMeth = m_pDoc->m_pMethSel->GetName();
	else
		g_strMeth = "None";
	g_prof = m_pDoc->m_nCurProf;
}

void CInvView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	ReSize();
}

