/*
**
**	$Filename:	AppLines.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Lines application
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPLINES_H)
#define APPLINES_H

#include "blz3/b3Config.h"
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3App.h"

#ifndef BLZ3_USE_OPENGL
#error	"We need the use of OpenGL"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"       // main symbols
#include "AppRaytraceDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CAppLinesApp:
// See AppLines.cpp for the implementation of this class
//

class CAppLinesApp : public CB3App
{
	CMultiDocTemplate *pImageTemplate;
	CMultiDocTemplate *pSceneTemplate;

public:
	CAppLinesApp();
	CAppRaytraceDoc *b3CreateRaytraceDoc();

	UINT m_ClipboardFormatForBlizzardObject;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CAppLinesApp)
	afx_msg void OnAppAbout();
	afx_msg void OnChangeTexturePath();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINES_H)
