// InvNew.h : main header file for the InvNew application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Enums.h"

// CInvApp:
// See InvNew.cpp for the implementation of this class
//

class CInvApp : public CWinApp
{
public:
	CInvApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CInvApp theApp;