#if !defined(AFX_MYBUTTON_H__737FBC24_7207_4B98_8D4E_505A380C9D6F__INCLUDED_)
#define AFX_MYBUTTON_H__737FBC24_7207_4B98_8D4E_505A380C9D6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF SetTxtColor(COLORREF color);
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
	CBitmap m_bkBmp;
	CBrush  m_bkBrush;
	CBitmap m_bkBmpPush;
	CBrush  m_bkBrushPush;

	COLORREF m_txtColor;
	COLORREF m_randColor;
	CPen	m_randPen;
	CPen	m_randPenCur;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__737FBC24_7207_4B98_8D4E_505A380C9D6F__INCLUDED_)
