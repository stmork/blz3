/*
**
**	$Filename:	AppLines.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Lines application
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifdef _DEBUG
#define B3_USE_SPIN_TEST
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

#include "DlgSearchPathList.h"
#include "DlgProperties.h"
#include "b3Splash.h"
#include "b3Profile.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	m_lastGC = 0;
	m_lastDC = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAppLinesApp object

CAppLinesApp theApp;

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

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

const char AppLinesVersionString[] = "$Name$";

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
	char buffer[1024];
	CString pattern;
	CString copyright;
	CString version;
	b3Date  today;

	copyright.Format("Copyright (C) %lu by\nBlizzard III",today.year);
#ifdef _DEBUG
	copyright += " (Debug version)";
#endif
	pattern.Format("%cName: %%s %c",'$','$');
	if (sscanf(AppLinesVersionString,pattern,buffer) != 1)
	{
		buffer[0] = 0;
	}
	else
	{
		if (strcmp(buffer,"$") == 0)
		{
			buffer[0] = 0;
		}
	}
	version.Format("%s %s",CB3ClientString(),buffer);
	m_CtrlVersion.SetWindowText(version);
	m_CtrlCopyright.SetWindowText(copyright);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#ifdef B3_USE_SPIN_TEST
#include "DlgSpinTest.h"
#endif

// App command to run the dialog
void CAppLinesApp::OnAppAbout()
{
#ifndef B3_USE_SPIN_TEST
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
#else
	CDlgSpinTest dlg;
	dlg.DoModal();
#endif
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
		path += ((const char *)((b3Path *)entry));
	}
	WriteProfileString(b3ClientName(),"texture search path",path);
}

void CAppLinesApp::b3SelectRenderContext(HDC dc,HGLRC gc)
{
	if ((dc != m_lastDC) || (gc != m_lastGC) || (dc == 0) || (gc == 0))
	{
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

	dlg.DoModal();
}
