// AppMandel.h : main header file for the APPMANDEL application
//

#if !defined(AFX_APPMANDEL_H__DF6683B7_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
#define AFX_APPMANDEL_H__DF6683B7_729E_11D5_A544_0050BF4EB3F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "blz3/system/b3Thread.h"

/////////////////////////////////////////////////////////////////////////////
// CAppMandelApp:
// See AppMandel.cpp for the implementation of this class
//

class CAppMandelApp : public CWinApp
{
public:
	CAppMandelApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppMandelApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAppMandelApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPMANDEL_H__DF6683B7_729E_11D5_A544_0050BF4EB3F3__INCLUDED_)
