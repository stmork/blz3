/*
**
**	$Filename:	AppLines.cpp $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Lines application
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifdef _DEBUG
#define NO__B3_USE_SPIN_TEST
#endif

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "AppLinesDoc.h"
#include "AppLinesView.h"
#include "AppObjectDoc.h"
#include "AppObjectView.h"
#include "AppRaytraceDoc.h"
#include "AppRaytraceView.h"
#include "blz3/image/b3TxPool.h"
#include "blz3/system/b3Date.h"
#include "blz3/system/b3File.h"
#include "blz3/system/b3FileDialog.h"
#include "blz3/system/b3Version.h"
#include "blz3/base/b3FileMem.h"

#include "DlgSearchPathList.h"
#include "DlgProperties.h"
#include "b3Splash.h"
#include "b3Profile.h"
#include "b3ExceptionLogger.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.46  2003/01/12 10:26:52  sm
**	- Undo/Redo of
**	  o Cut & paste
**	  o Drag & drop
**
**	Revision 1.45  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.44  2002/11/01 12:49:07  sm
**	- Some b3SearchPath constructor refinements.
**	- Fixed texture path configuration.
**	
**	Revision 1.43  2002/08/15 13:56:42  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.42  2002/08/11 11:03:40  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**	
**	Revision 1.41  2002/08/10 16:07:46  sm
**	- Added some OS version output
**	- Corrected language specifiers for version output.
**	- Changed CDlgScene CSpinButtonCtrl to CB3IntSpinButtonCtrl
**	  to avoid thousands point.
**	
**	Revision 1.40  2002/08/07 14:26:23  sm
**	- Introduced mapping from Blizzard III error codes to human
**	  readable error messages supplied from Windows resources.
**	
**	Revision 1.39  2002/08/05 16:04:54  sm
**	- Found first texture init bug. This wasn't an OpenGL bug. This
**	  couldn't be because every implementation had got the same
**	  bug. The static aux image for creating textures wasn't initialized
**	  at the right time.
**	- Version handling introduced: The version number is extracted
**	  from the version resource now.
**	- The b3Tx::b3AllocTx() method uses b3Realloc() for better
**	  memory usage.
**	- Some b3World messages removed.
**	- The 0x7fff class is registered into the b3ItemRegister now. This
**	  prevents printing a warning when this class isn't found. Due to
**	  the fact that *every* Blizzard data contains this class every
**	  data read put out this warning.
**	
**	Revision 1.38  2002/08/05 07:32:24  sm
**	- Key word test
**	
**	Revision 1.37  2002/08/01 15:02:55  sm
**	- Found texture missing bug when printing. There weren't any
**	  selected textures inside an other OpenGL rendering context.
**	  Now fixed!
**	
**	Revision 1.36  2002/07/31 16:29:40  sm
**	- Minor changes
**	
**	Revision 1.35  2002/07/31 11:57:10  sm
**	- The nVidia OpenGL init bug fixed by using following work
**	  around: The wglMakeCurrent() method is invoked on
**	  every OnPaint(). This is configurable depending on the
**	  hostname.
**	
**	Revision 1.34  2002/07/22 16:27:45  sm
**	- Fixed some errors concerning texture stencil
**	
**	Revision 1.33  2002/04/07 19:51:27  sm
**	- Added new CB3DocManager to open new
**	  CB3FileFialog...
**	
**	Revision 1.32  2002/04/07 12:59:38  sm
**	- Added support for file dialog with Windows 2000 place bars (Cb3FileDialog)
**	- CB3FileDialog used for CWinApp::OnFileOpen()
**	- Image buttons changed to draw disabled state correctly using
**	  CDC::DrawState()
**	
**	Revision 1.31  2002/03/11 13:48:54  sm
**	- Cleaned up dialog titles
**	- Fixed some texture bugs concerning palette copying.
**	- Added a triangles profile.
**	
**	Revision 1.30  2002/03/08 16:46:14  sm
**	- Added new CB3IntSpinButtonCtrl. This is much
**	  better than standard integer CSpinButtonCtrl.
**	- Added a test control to test spin button controls
**	  and float control.
**	- Made spin button controls and float edit control
**	  DDXable. The spin button controls need only
**	  a simple edit field without any DDX CEdit reference
**	  or value reference inside a dialog.
**	- Changed dialogs to reflect new controls. This was a
**	  major cleanup which shortens the code in an elegant way.
**	
**	Revision 1.29  2002/03/05 20:38:24  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.28  2002/02/08 15:53:37  sm
**	- Cleaned up makefiles for Un*x
**	- New dialog for print buffer size.
**	
**	Revision 1.27  2002/02/05 20:04:12  sm
**	- Added legend to print hard copy.
**	
**	Revision 1.26  2002/01/31 19:30:31  sm
**	- Some OpenGL print optimizations done.
**	- Object renaming inside hierarchy tree added.
**	
**	Revision 1.25  2002/01/30 19:46:41  sm
**	- Trying to print in debug mode (and want to see anything)
**	
**	Revision 1.24  2002/01/25 16:34:46  sm
**	- Added printer support (not running yet)
**	
**	Revision 1.23  2002/01/21 16:56:46  sm
**	- Showing splash dialog only in release version.
**	- Prepared shape icons.
**	
**	Revision 1.22  2002/01/20 12:48:51  sm
**	- Added splash screen
**	- Corrected repeat buttons (capture change)
**	
**	Revision 1.21  2002/01/19 19:57:55  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.20  2002/01/18 16:49:34  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.19  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
**	Revision 1.18  2002/01/15 16:17:31  sm
**	- Checked OLE support
**	- Checked icons
**	- Added two bwd files which create icons
**	
**	Revision 1.17  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.16  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**	
**	Revision 1.15  2001/12/28 15:17:44  sm
**	- Added clipboard-copy to raytraced view
**	- Added printing to raytraced view
**	- Much minor UI tuning done:
**	  o added accelerators
**	  o open maximized window
**	  o fixed some UpdateUI methods
**	  o changed exception handling in CB3ScrollView and CB3BitmapDxB
**	
**	Revision 1.14  2001/12/27 21:33:35  sm
**	- Further docking handling done
**	- CDocument cleanups done
**	
**	Revision 1.13  2001/12/09 13:16:21  sm
**	- Added tag name
**	
**	Revision 1.12  2001/12/09 13:06:56  sm
**	- Added tag name
**	
**	Revision 1.11  2001/12/09 13:03:29  sm
**	- Added tag name
**	
**	Revision 1.10  2001/12/09 13:00:58  sm
**	- Added tag name
**	
**	Revision 1.9  2001/12/09 12:59:41  sm
**	- Added tag name
**	
**	Revision 1.8  2001/12/09 12:53:02  sm
**	- Added tag name
**	- Added watches for test purposes
**	- Customized about dialog
**	
**	Revision 1.7  2001/12/09 12:36:34  sm
**	- Adding Tag name
**	
**	Revision 1.6  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.5  2001/10/10 17:52:24  sm
**	- Texture loading (only reading into memory) running.
**	- Raytracing without OpenGL must be possible!
**	
**	Revision 1.4  2001/09/30 15:46:06  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**	Revision 1.3  2001/08/11 19:59:15  sm
**	- Added orthogonal projection
**	
**	Revision 1.2  2001/08/11 15:59:58  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**	
**	Revision 1.1  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesApp implementation                   **
**                                                                      **
*************************************************************************/

BEGIN_MESSAGE_MAP(CAppLinesApp, CB3App)
	//{{AFX_MSG_MAP(CAppLinesApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_CHANGE_TEXTURE_PATH, OnChangeTexturePath)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*************************************************************************
**                                                                      **
**                        CAppLinesApp construction                     **
**                                                                      **
*************************************************************************/

CAppLinesApp::CAppLinesApp() : CB3App("Lines III")
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_ClipboardFormatForBlizzardObject = 0;
	m_lastGC = (HGLRC)0xdeadbeef;
	m_lastDC = (HDC)0xbadc0ded;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAppLinesApp object

CAppLinesApp theApp;

class CB3DocManager : public CDocManager
{
	DECLARE_DYNAMIC(CB3DocManager)

public:
	virtual BOOL DoPromptFileName(
		CString      &fileName,
		UINT          nIDSTitle,
		DWORD         lFlags,
		BOOL          bOpenFileDialog,
		CDocTemplate *pTemplate)
	{
#if 0
		return CDocManager::DoPromptFileName(fileName,nIDSTitle,lFlags,bOpenFileDialog,pTemplate);
#else
		CString title;
		CString strFilter;
		CString strDefault;
		CString suggest = fileName;
		CString allFilter;
		BOOL    result;

		VERIFY(title.LoadString(nIDSTitle));

		if (pTemplate != NULL)
		{
			CString filterName,filterExt;

			ASSERT_VALID(pTemplate);
			pTemplate->GetDocString(filterName,CDocTemplate::filterName);
			pTemplate->GetDocString(filterExt,CDocTemplate::filterExt);
			strFilter += (filterName + "|*" + filterExt + "|");
		}
		else
		{
			// do for all doc template
			POSITION pos = m_templateList.GetHeadPosition();
			CString filterName,filterExt;

			while (pos != NULL)
			{
				CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

				ASSERT_VALID(pTemplate);
				pTemplate->GetDocString(filterName,CDocTemplate::filterName);
				pTemplate->GetDocString(filterExt,CDocTemplate::filterExt);
				strFilter += (filterName + "|*" + filterExt + "|");
			}
		}

		// append the "*.*" all files filter
		allFilter.LoadString(AFX_IDS_ALLFILTER);
		strFilter += (allFilter + "|*.*||");

		CB3FileDialog dlgFile(bOpenFileDialog,"",suggest,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | lFlags,strFilter);
		dlgFile.m_ofn.lpstrTitle = title;

		result = dlgFile.DoModal() == IDOK;
		if (result)
		{
			fileName = dlgFile.GetPathName();
		}
		return result;
#endif
	}
};

IMPLEMENT_DYNAMIC(CB3DocManager,CDocManager)


class CImageMultiDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CImageMultiDocTemplate)

// Constructors
public:
	CImageMultiDocTemplate(
		UINT           nIDResource,
		CRuntimeClass *pDocClass,
		CRuntimeClass *pFrameClass,
		CRuntimeClass *pViewClass) :
			CMultiDocTemplate(nIDResource,pDocClass,pFrameClass,pViewClass)
	{
	}
	
	virtual Confidence MatchDocType(
		LPCTSTR lpszPathName,
		CDocument*& rpDocMatch)
	{
		Confidence result;
		
		result = CMultiDocTemplate::MatchDocType(lpszPathName,rpDocMatch);
		if(result == yesAttemptForeign)
		{
			b3Path ext;

			ext.b3ExtractExt(lpszPathName);
			if (b3Tx::b3GetFileType(ext) != FT_UNKNOWN)
			{
				result = yesAttemptNative;
			}
		}
		return result;
	}
};

IMPLEMENT_DYNAMIC(CImageMultiDocTemplate,CMultiDocTemplate)

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {72D69517-8984-11D5-A54F-0050BF4EB3F3}
static const CLSID scene_clsid =
{ 0x72d69517, 0x8984, 0x11d5, { 0xa5, 0x4f, 0x0, 0x50, 0xbf, 0x4e, 0xb3, 0xf3 } };

// {72D6951B-8984-11D5-A54F-0050BF4EB3F3}
static const CLSID object_clsid =
{ 0x72d6951b, 0x8984, 0x11d5, { 0xa5, 0x4f, 0x0, 0x50, 0xbf, 0x4e, 0xb3, 0xf3 } };

/*************************************************************************
**                                                                      **
**                        CAppLinesApp initialization                   **
**                                                                      **
*************************************************************************/

BOOL CAppLinesApp::InitInstance()
{
	// Parse command line for standard shell commands, DDE, file open
	CB3Version         version;
	CB3ExceptionLogger init_logging;
	b3Date             today;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
#ifndef _DEBUG
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
#endif

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T(BLIZZARD3_REG_COMPANY));

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
#ifdef _DEBUG
	b3Log_SetLevel(B3LOG_FULL);
#endif
	b3PrintF(B3LOG_NORMAL,"%s %s\n%s\n\n",
		b3ClientName(),
		version.b3GetVersionString(),
		version.b3GetCopyrightString());
	CB3Version::b3DumpOS();
#ifdef _DEBUG
	today.b3Y2K_Selftest();
#endif

	b3InitRaytrace::b3Init();

	CString path = GetProfileString(b3ClientName(),"texture search path","");
	CString sub;
	while(!path.IsEmpty())
	{
		sub = path.SpanExcluding(";");
		texture_pool.b3AddPath(sub);
		path = path.Right(path.GetLength() - sub.GetLength() - 1);
	}

	m_PrintBufferSize = GetProfileInt(b3ClientName(),"print buffer size",32);
	m_RowRefreshCount = GetProfileInt(b3ClientName(),"row refresh count",B3_DISPLAY_ROWREFRESHCOUNT);

	if (m_pDocManager == NULL)
	{
		// Add custom doc manager
		m_pDocManager = new CB3DocManager;
	}

	pSceneTemplate = new CMultiDocTemplate(
		IDR_BLZ3TYPE,
		RUNTIME_CLASS(CAppLinesDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppLinesView));
	AddDocTemplate(pSceneTemplate);

	pImageTemplate = new CImageMultiDocTemplate(
		IDR_DISPLAYTYPE,
		RUNTIME_CLASS(CAppRaytraceDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppRaytraceView));
	AddDocTemplate(pImageTemplate);

	pObjectTemplate = new CMultiDocTemplate(
		IDR_OBJECT,
		RUNTIME_CLASS(CAppObjectDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppObjectView));
	AddDocTemplate(pObjectTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_SceneServer.ConnectTemplate( scene_clsid,  pSceneTemplate,  FALSE);
	m_ObjectServer.ConnectTemplate(object_clsid, pObjectTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	m_ClipboardFormatForBlizzardObject = ::RegisterClipboardFormat("Blizzard Object");

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_SceneServer.UpdateRegistry(OAT_DISPATCH_OBJECT);
	m_ObjectServer.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// Dispatch commands specified on the command line
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	b3Profile *profile;

	B3_FOR_BASE(&LinesProfileBase,profile)
	{
		if (!profile->b3Create())
		{
			b3PrintF(B3LOG_NORMAL,"Cannot create curve profile %s!\n",profile->b3GetTitle());
			b3PrintF(B3LOG_NORMAL,"Exiting now!\n");
			return false;
		}
	}

	CString key("GL unchecked context switch");

	m_UncheckedContextSwitch = b3ReadInt(b3ClientName(),key,0) != 0;
	b3WriteInt(b3ClientName(),key,m_UncheckedContextSwitch);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAppLinesApp message handlers

CAppRaytraceDoc *CAppLinesApp::b3CreateRaytraceDoc()
{
	return (CAppRaytraceDoc *)pImageTemplate->OpenDocumentFile(NULL);
}

CAppObjectDoc *CAppLinesApp::b3CreateObjectDoc(
	CAppLinesDoc *LinesDoc,
	b3BBox       *bbox)
{
	CAppObjectDoc *pDoc;
	CWaitCursor    waiting_for_Isabella;
	POSITION       pos;

	// Look if there is any open object edit -> use it.
	pos = pObjectTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		pDoc = (CAppObjectDoc *)pObjectTemplate->GetNextDoc(pos);
		if (pDoc->b3IsObjectAlreadyOpen(LinesDoc,bbox))
		{
			pos = pDoc->GetFirstViewPosition();
			pDoc->GetNextView(pos)->GetParentFrame()->BringWindowToTop();
			return pDoc;
		}
	}

	// We didn't found the appropriate document -> create new one.
	pDoc = (CAppObjectDoc *)pObjectTemplate->OpenDocumentFile(NULL);
	if (pDoc != null)
	{
		pDoc->b3EditBBox(LinesDoc,bbox);
	}
	return pDoc;
}

void CAppLinesApp::b3CloseObjectDoc(CAppLinesDoc *LinesDoc)
{
	POSITION       pos;
	CAppObjectDoc *pDoc;

	pos = pObjectTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		pDoc = (CAppObjectDoc *)pObjectTemplate->GetNextDoc(pos);
		if (pDoc->b3IsLinesDoc(LinesDoc))
		{
			if (pDoc->IsModified())
			{
				LinesDoc->SetModifiedFlag();
			}
			pDoc->OnCloseDocument();
		}
	}
}

void CAppLinesApp::OnChangeTexturePath() 
{
	// TODO: Add your command handler code here
	CDlgSearchPathList  dlg;
	CString             path  = "";
	b3PathEntry        *entry;
	b3_count            count = 0;

	dlg.m_SearchPath = &texture_pool;
	dlg.DoModal();
	B3_FOR_BASE(&texture_pool.m_SearchPath,entry)
	{
		if (count++ > 0)
		{
			path += ";";
		}
		path += ((const char *)*entry);
	}
	WriteProfileString(b3ClientName(),"texture search path",path);
}

void CAppLinesApp::b3SelectRenderContext(HDC dc,HGLRC gc)
{
	if ((dc != m_lastDC) || (gc != m_lastGC) || (dc == 0) || (gc == 0) || (m_UncheckedContextSwitch))
	{
		b3PrintF(B3LOG_FULL,"######################################### CAppLinesApp::b3SelectRenderContext(HDC:0x%x,HGLRC:0x%x)\n",
			dc,gc);
		wglMakeCurrent(dc,gc);
		m_lastDC = dc;
		m_lastGC = gc;
	}
}

void CAppLinesApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	// Force creating new geometry!
	pSceneTemplate->OpenDocumentFile(NULL);
}

void CAppLinesApp::OnProperties() 
{
	// TODO: Add your command handler code here
	CDlgProperties dlg;

	if (dlg.DoModal() == IDOK)
	{
	}
}

void CAppLinesApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString filename;
	CString filter_bwd,bwdFilterName,bwdFilterExt;
	CString filter_bod,bodFilterName,bodFilterExt;
	CString filter_img;

	// Build scene filter
	pSceneTemplate->GetDocString(bwdFilterName,CDocTemplate::filterName);
	pSceneTemplate->GetDocString(bwdFilterExt,CDocTemplate::filterExt);
	filter_bwd = bwdFilterName + "|*" + bwdFilterExt + "|";

	// Build object filter
	pObjectTemplate->GetDocString(bodFilterName,CDocTemplate::filterName);
	pObjectTemplate->GetDocString(bodFilterExt,CDocTemplate::filterExt);
	filter_bod = bodFilterName + "|*" + bodFilterExt + "|";

	// Build image filter
	filter_img.LoadString(IDS_TEXTURE_FILTER);

	// Select filename
	CB3FileDialog dlg(TRUE,"",NULL,OFN_HIDEREADONLY,filter_bwd + filter_bod + filter_img,m_pMainWnd);
	if (dlg.DoModal() == IDOK)
	{
		// and load document
		filename = dlg.GetPathName();
		OpenDocumentFile(filename);
	}
}

/*************************************************************************
**                                                                      **
**                        CAppLinesApp clipboard operations             **
**                                                                      **
*************************************************************************/

b3_bool CAppLinesApp::b3PutClipboard(b3BBox *bbox)
{
	CMainFrame   *main    = CB3GetMainFrame();
	b3_bool       success = false;
	b3_size       size;
	void         *ptr;
	HANDLE        handle;

	if (main->OpenClipboard())
	{
		try
		{
			b3FileMem  file(B_WRITE);

			if (b3WriteBBox(bbox,&file))
			{
				size = file.b3Size();
				handle = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,size);
				if (handle != 0)
				{
					ptr = ::GlobalLock(handle);
					if (ptr != null)
					{
						file.b3Seek(0,B3_SEEK_START);
						if (file.b3Read(ptr,size) == size)
						{
							::GlobalUnlock(handle);
							::EmptyClipboard();
							::SetClipboardData(m_ClipboardFormatForBlizzardObject,handle);
							success = true;
						}
					}
				}
			}
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: writing object %s to clipboard (code: %d)\n",
				bbox->b3GetName(),f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: writing object %s to clipboard (code: %d)\n",
				bbox->b3GetName(),w.b3GetError());
			B3_MSG_ERROR(w);
		}
		catch(...)
		{
			b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: writing object %s to clipboard %s\n",
				bbox->b3GetName());
		}
		::CloseClipboard();
	}
	return success;
}

b3BBox *CAppLinesApp::b3PasteClipboard(b3World *world)
{
	CMainFrame  *main    = CB3GetMainFrame();
	CWaitCursor  wait;
	void        *ptr;
	b3_size      size;
	HANDLE       handle;
	b3BBox      *bbox = null;

	if (main->OpenClipboard())
	{
		handle = ::GetClipboardData(m_ClipboardFormatForBlizzardObject);
		if (handle != 0)
		{
			size = ::GlobalSize(handle);
			ptr  = ::GlobalLock(handle);
			try
			{
				b3FileMem file(B_WRITE);

				file.b3Write(ptr,size);
				file.b3Seek(0,B3_SEEK_START);

				if(world->b3Read(&file) == B3_WORLD_OK)
				{
					bbox  = (b3BBox *)world->b3GetFirst();
				}
			}
			catch(b3FileException &f)
			{
				b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading object from clipboard (code: %d)\n",
					f.b3GetError());
				B3_MSG_ERROR(f);
			}
			catch(b3WorldException &w)
			{
				b3PrintF(B3LOG_NORMAL,"ERROR: reading object from clipboard (code: %d)\n",
					w.b3GetError());
				B3_MSG_ERROR(w);
			}
			::GlobalUnlock(handle);
		}
		::CloseClipboard();
	}
	return bbox;
}

b3_bool CAppLinesApp::b3WriteBBox(b3BBox *bbox,const char *filename)
{
	b3File  file(filename,B_WRITE);

	return b3WriteBBox(bbox,&file);
}

b3_bool CAppLinesApp::b3WriteBBox(b3BBox *bbox,b3FileAbstract *file)
{
	b3World  world;

	world.m_AutoDelete = false;
	world.b3SetFirst(bbox);
	return (world.b3Write(file) == B3_WORLD_OK);
}

/*************************************************************************
**                                                                      **
**                        CAppLinesApp about box                        **
**                                                                      **
*************************************************************************/

const char AppLinesVersionString[] = "$Revision$";
const char AppLinesNameString[] = "$Name$";

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_CtrlCopyright;
	CStatic	m_CtrlVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COPYRIGHT, m_CtrlCopyright);
	DDX_Control(pDX, IDC_VERSION, m_CtrlVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CB3Version version;

	m_CtrlVersion.SetWindowText(version.b3GetVersionString());
	m_CtrlCopyright.SetWindowText(version.b3GetCopyrightString());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CAppLinesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

