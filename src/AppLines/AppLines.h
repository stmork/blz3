/*
**
**	$Filename:	AppLines.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1357 $
**	$Date: 2006-04-18 22:20:59 +0200 (Di, 18 Apr 2006) $
**	$Author: sm $
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
#include "blz3/system/b3DisplayView.h"
#include "blz3/base/b3OceanWave.h"

/////////////////////////////////////////////////////////////////////////////
// CAppLinesApp:
// See AppLines.cpp for the implementation of this class
//

class CAppRaytraceDoc;
class CAppLinesDoc;
class CAppObjectDoc;

class b3BBox;
class b3World;
class b3SearchPath;

class CAppLinesApp : public CB3App
{
	b3OceanWave m_OW;

	// Last view parameter
	b3_bool            m_Filtered;
	b3_display_mode    m_Mode;

	// Document templates (we can handle many file types!)
	CMultiDocTemplate *m_pImageTemplate;
	CMultiDocTemplate *m_pSceneTemplate;
	CMultiDocTemplate *m_pObjectTemplate;

	static const CLSID m_SceneClsID;
	static const CLSID m_ObjectClsID;

public:
	b3_count           m_RowRefreshCount;
	b3_s32             m_PrintBufferSize;
	UINT               m_ClipboardFormatForBlizzardObject;

public:
	                 CAppLinesApp();
	CAppRaytraceDoc *b3CreateRaytraceDoc();
	CAppObjectDoc   *b3CreateObjectDoc(CAppLinesDoc *LinesDoc,b3BBox *bbox);
	void             b3CloseObjectDoc(CAppLinesDoc *LinesDoc);
	void             b3UpdateAllViews();

	// Cut & paste operation
	b3_bool          b3PutClipboard(b3BBox *bbox);
	b3BBox          *b3PasteClipboard(b3World *world);
	b3_bool          b3WriteBBox(b3BBox *bbox,b3FileAbstract *file);
	b3_bool          b3WriteBBox(b3BBox *bbox,const char *filename);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
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
	afx_msg void OnImportArcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void b3SetupSearchPath(b3SearchPath &search,CString &path);
	void b3SetupPluginPaths(b3SearchPath &search);
};

#define CB3GetLinesApp()  ((CAppLinesApp *)AfxGetApp())
#define B3_MSG_ERROR(exception) (b3Runtime::b3MessageBox((exception).b3GetErrorMsg(),B3_MSGBOX_ERROR))

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINES_H)
