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
#include "AppObjectDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CAppLinesApp:
// See AppLines.cpp for the implementation of this class
//

class CAppLinesApp : public CB3App
{
	char            m_Hostname[B3_MAXHOSTNAMELEN];
	b3_bool         m_UncheckedContextSwitch;

	// OpenGL general values
	HGLRC           m_lastGC;
	HDC             m_lastDC;

	// Document templates (we can handle many file types!)
	CMultiDocTemplate *pImageTemplate;
	CMultiDocTemplate *pSceneTemplate;
	CMultiDocTemplate *pObjectTemplate;

public:
	b3_size            m_PrintBufferSize;

public:
	                 CAppLinesApp();
	CAppRaytraceDoc *b3CreateRaytraceDoc();
	CAppObjectDoc   *b3CreateObjectDoc(CAppLinesDoc *LinesDoc,b3BBox *bbox);
	void             b3CloseObjectDoc(CAppLinesDoc *LinesDoc);
	void             b3SelectRenderContext(HDC dc,HGLRC gc);

	UINT m_ClipboardFormatForBlizzardObject;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_SceneServer;
	COleTemplateServer m_ObjectServer;
		// Server object for document creation
	//{{AFX_MSG(CAppLinesApp)
	afx_msg void OnAppAbout();
	afx_msg void OnChangeTexturePath();
	afx_msg void OnFileNew();
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define CB3GetLinesApp()  ((CAppLinesApp *)AfxGetApp())

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINES_H)
