// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	m_bkBmp.LoadBitmap(IDB_B1);
	m_bkBrush.CreatePatternBrush(&m_bkBmp);
	m_bkBmpPush.LoadBitmap(IDB_B2);
	m_bkBrushPush.CreatePatternBrush(&m_bkBmpPush);

	m_txtColor = RGB(0, 51, 102);
	m_randColor = RGB(160, 160, 160);
	m_randPen.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
	m_randPenCur.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SetBkMode(TRANSPARENT);
	CPoint rPt(12, 8);

	UINT uStyle = DFCS_BUTTONPUSH;

	// This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	// If drawing selected, add the pushed style to DrawFrameControl.
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;

	// Draw the button frame.
	//::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);

	CBrush *pBr;
	CPen *pPen;
	if(lpDrawItemStruct->itemAction == ODA_SELECT) {
		pPen = pDC->SelectObject(&m_randPenCur);
		pBr = pDC->SelectObject(&m_bkBrush);
	}
	else {
		pPen = pDC->SelectObject(&m_randPen);
		pBr = pDC->SelectObject(&m_bkBrushPush);
	}
/*	if(lpDrawItemStruct->itemState == ODS_SELECTED) {
		pBr = pDC->SelectObject(&m_bkBrush);
	}
	else {
		pBr = pDC->SelectObject(&m_bkBrushPush);
	}
*/	pDC->RoundRect(&lpDrawItemStruct->rcItem, rPt);
	pDC->SelectObject(pBr);


	// Get the button's text.
	CString strText;
	GetWindowText(strText);

	// Draw the button text using the text color red.
	pDC->SetTextColor(m_txtColor);
	//COLORREF crOldColor = ::SetTextColor(m_txtColor);
	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
		&lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	//::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}

COLORREF CMyButton::SetTxtColor(COLORREF color)
{
	COLORREF prevColor = m_txtColor;
	m_txtColor = color;
	return prevColor;
}
