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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "blz3/system/b3File.h"
#include "blz3/system/b3FileDialog.h"
#include "blz3/system/b3Plugin.h"
#include "blz3/system/b3SelfTest.h"
#include "blz3/system/b3SimplePreviewDialog.h"
#include "blz3/system/b3Version.h"
#include "blz3/base/b3FileMem.h"

#include "DlgSearchPathList.h"
#include "DlgProperties.h"

#include "b3Splash.h"
#include "b3StaticPluginInfoInit.h"
#include "b3Profile.h"
#include "b3ExceptionLogger.h"
#include "b3SelectObject.h"
#include "b3DocManager.h"
#include "b3ImageMultiDocTemplate.h"

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
	ON_COMMAND(ID_IMPORT_ARCON, OnImportArcon)
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
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAppLinesApp object

CAppLinesApp theApp;

IMPLEMENT_DYNAMIC(CB3DocManager,CDocManager)

IMPLEMENT_DYNAMIC(CB3ImageMultiDocTemplate,CMultiDocTemplate)

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {F1E8F37D-3D18-4028-9E52-5A2D141EB769}
const CLSID CAppLinesApp::m_SceneClsID =
{ 0xf1e8f37d, 0x3d18, 0x4028, { 0x9e, 0x52, 0x5a, 0x2d, 0x14, 0x1e, 0xb7, 0x69 } };

// {F346F6D2-0E7B-4b15-B887-D7A9B215EB62}
const CLSID CAppLinesApp::m_ObjectClsID =
{ 0xf346f6d2, 0xe7b, 0x4b15, { 0xb8, 0x87, 0xd7, 0xa9, 0xb2, 0x15, 0xeb, 0x62 } };

/*************************************************************************
**                                                                      **
**                        CAppLinesApp initialization                   **
**                                                                      **
*************************************************************************/

const int CB3SimplePreviewDialog::m_AutoRefreshId   = IDC_AUTO_REFRESH;
const int CB3SimplePreviewDialog::m_RefreshId       = IDC_REFRESH;
const int CB3SimplePreviewDialog::m_PropertySheetId = IDC_PROPERTY;

/*************************************************************************
**                                                                      **
**                        CAppLinesApp initialization                   **
**                                                                      **
*************************************************************************/

void CAppLinesApp::b3SetupSearchPath(b3SearchPath &search,CString &path)
{
	CString sub;
	
	while(!path.IsEmpty())
	{
		sub = path.SpanExcluding(";");
		search.b3AddPath(sub);
		path = path.Right(path.GetLength() - sub.GetLength() - 1);
	}
}

void CAppLinesApp::b3SetupPluginPaths(b3SearchPath &search)
{
	HKEY hSecKey = b3GetSectionKey("Plugins");

	if (hSecKey)
	{
		TCHAR    entry[MAX_PATH];
		DWORD    entry_len = sizeof(entry);
		TCHAR    value[MAX_PATH];
		DWORD    value_len = sizeof(value);
		DWORD    mode = 0;

		for (int i = 0;::RegEnumValue(hSecKey, 
                     i, 
                     entry, 
                     &entry_len,
					 NULL,
                     &mode, 
                     (unsigned char *)value, 
                     &value_len) == ERROR_SUCCESS;i++)
        {
			if (mode == REG_SZ)
			{
				search.b3AddPath(value);
			}
			entry_len = sizeof(entry);
			value_len = sizeof(value);
        }
		::RegCloseKey (hSecKey);
    } 
}

BOOL CAppLinesApp::InitInstance()
{
	// Parse command line for standard shell commands, DDE, file open
	CB3Version          version;
	CB3ExceptionLogger  init_logging;
	CString             path;
	b3Loader           &plugins = b3Loader::b3GetLoader();

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

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T(BLIZZARD3_REG_COMPANY));

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	b3_log_level  level;
#ifndef _DEBUG
	level = (b3_log_level)b3ReadInt("Settings", "DebugLevel", B3LOG_NORMAL);
#else
	level = B3LOG_FULL;
#endif
	b3Log::b3SetLevel(level);

	b3PrintF(B3LOG_NORMAL,"%s %s\n%s\n\n",
		b3ClientName(),
		version.b3GetVersionString(),
		version.b3GetCopyrightString());
	CB3Version::b3DumpOS();
#ifdef _DEBUG
	b3SelfTest::b3TestDataSize();
	b3SelfTest::b3TestDate();
#endif

	// Default texture search path from HKEY_LOCAL_MACHINE
//	path = b3ReadString(b3ClientName(),"texture search path","");
//	b3SetupSearchPath(b3Scene::m_TexturePool,path);

	// Default texture search path from HKEY_CURENT_USER
	path = GetProfileString(b3ClientName(),"texture search path","");
	b3SetupSearchPath(b3Scene::m_TexturePool,path);

	// Add hard wired classes
	b3RaytracingItems::b3Register();
	b3StaticPluginInfoInit::b3Init();

	// FIXME: This should be local computer wide.
	path = GetProfileString(b3ClientName(),"plugin search path","");
	b3SetupSearchPath(plugins,path);
	b3SetupPluginPaths(plugins);
	plugins.b3Load();

	CDlgProperties::b3ReadConfig();
	if (m_pDocManager == NULL)
	{
		// Add custom doc manager
		m_pDocManager = new CB3DocManager;
	}

	m_pSceneTemplate = new CMultiDocTemplate(
		IDR_BLZ3TYPE,
		RUNTIME_CLASS(CAppLinesDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppLinesView));
	AddDocTemplate(m_pSceneTemplate);

	m_pImageTemplate = new CB3ImageMultiDocTemplate(
		IDR_DISPLAYTYPE,
		RUNTIME_CLASS(CAppRaytraceDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppRaytraceView));
	AddDocTemplate(m_pImageTemplate);

	m_pObjectTemplate = new CMultiDocTemplate(
		IDR_OBJECT,
		RUNTIME_CLASS(CAppObjectDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppObjectView));
	AddDocTemplate(m_pObjectTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_SceneServer.ConnectTemplate( m_SceneClsID,  m_pSceneTemplate,  FALSE);
	m_ObjectServer.ConnectTemplate(m_ObjectClsID, m_pObjectTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// Init Blizzard values...
	b3InitInstance();

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

	B3_FOR_BASE(&b3Profile::m_LinesProfileBase,profile)
	{
		if (!profile->b3Create())
		{
			b3PrintF(B3LOG_NORMAL,"Cannot create curve profile %s!\n",profile->b3GetTitle());
			b3PrintF(B3LOG_NORMAL,"Exiting now!\n");
			return false;
		}
	}

	return CB3App::InitInstance();
}

int CAppLinesApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	b3Loader::b3GetLoader().b3Unload();
	return CB3App::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAppLinesApp message handlers

CAppRaytraceDoc *CAppLinesApp::b3CreateRaytraceDoc()
{
	return (CAppRaytraceDoc *)m_pImageTemplate->OpenDocumentFile(NULL);
}

void CAppLinesApp::b3UpdateAllViews()
{
	CDocument *pDoc;
	POSITION   pos;

	pos = m_pSceneTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		pDoc = (CAppObjectDoc *)m_pObjectTemplate->GetNextDoc(pos);
		pDoc->UpdateAllViews(NULL,B3_UPDATE_VIEW);
	}

	pos = m_pObjectTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		pDoc = (CAppObjectDoc *)m_pObjectTemplate->GetNextDoc(pos);
		pDoc->UpdateAllViews(NULL,B3_UPDATE_VIEW);
	}
}

CAppObjectDoc *CAppLinesApp::b3CreateObjectDoc(
	CAppLinesDoc *LinesDoc,
	b3BBox       *bbox)
{
	CAppObjectDoc *pDoc;
	CWaitCursor    waiting_for_Isabella;
	POSITION       pos;

	// Look if there is any open object edit -> use it.
	pos = m_pObjectTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		pDoc = (CAppObjectDoc *)m_pObjectTemplate->GetNextDoc(pos);
		if (pDoc->b3IsObjectAlreadyOpen(LinesDoc,bbox))
		{
			pos = pDoc->GetFirstViewPosition();
			pDoc->GetNextView(pos)->GetParentFrame()->BringWindowToTop();
			return pDoc;
		}
	}

	// We didn't found the appropriate document -> create new one.
	pDoc = (CAppObjectDoc *)m_pObjectTemplate->OpenDocumentFile(NULL);
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

	pos = m_pObjectTemplate->GetFirstDocPosition();
	while(pos != NULL)
	{
		pDoc = (CAppObjectDoc *)m_pObjectTemplate->GetNextDoc(pos);
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

	dlg.m_SearchPath = &b3Scene::m_TexturePool;
	dlg.DoModal();
	B3_FOR_BASE(&b3Scene::m_TexturePool.m_SearchPath,entry)
	{
		if (count++ > 0)
		{
			path += ";";
		}
		path += ((const char *)*entry);
	}
	WriteProfileString(b3ClientName(),"texture search path",path);
}

void CAppLinesApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	// Force creating new geometry!
	m_pSceneTemplate->OpenDocumentFile(NULL);
}

void CAppLinesApp::OnImportArcon() 
{
	// TODO: Add your command handler code here
	CAppLinesDoc *pDoc;
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	CFrameWnd     *frame;
	b3Path          result = "";

	if (CB3SelectLoadArcon::b3Select(result))
	{
		pDoc = (CAppLinesDoc *)m_pSceneTemplate->CreateNewDocument();
		if (pDoc->OnImportArcon(result))
		{
			frame = m_pSceneTemplate->CreateNewFrame(pDoc,NULL);
			m_pSceneTemplate->InitialUpdateFrame(frame,pDoc);
		}
	}
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
	CB3App  *app = CB3GetApp();
	CString  filename;
	CString  filter_bwd,bwdFilterName,bwdFilterExt;
	CString  filter_bod,bodFilterName,bodFilterExt;
	CString  filter_img;

	// Build scene filter
	m_pSceneTemplate->GetDocString(bwdFilterName,CDocTemplate::filterName);
	m_pSceneTemplate->GetDocString(bwdFilterExt,CDocTemplate::filterExt);
	filter_bwd = bwdFilterName + "|*" + bwdFilterExt + "|";

	// Build object filter
	m_pObjectTemplate->GetDocString(bodFilterName,CDocTemplate::filterName);
	m_pObjectTemplate->GetDocString(bodFilterExt,CDocTemplate::filterExt);
	filter_bod = bodFilterName + "|*" + bodFilterExt + "|";

	// Build image filter
	filter_img.LoadString(IDS_TEXTURE_FILTER);

	// Select filename
	filename = app->GetProfileString(CB3ClientString(),"file load.document",null);
	CB3FileDialog dlg(TRUE,"",filename,OFN_HIDEREADONLY,filter_bwd + filter_bod + filter_img,m_pMainWnd);
	if (dlg.DoModal() == IDOK)
	{
		// and load document
		filename = dlg.GetPathName();
		app->WriteProfileString(CB3ClientString(),"file load.document",filename);
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
	b3BBox::b3Recount(world.b3GetHead(),bbox->b3GetType());
	return world.b3Write(file) == B3_WORLD_OK;
}

/*************************************************************************
**                                                                      **
**                        CAppLinesApp about box                        **
**                                                                      **
*************************************************************************/

class CAboutDlg : public CDialog
{
	static const char m_AppLinesVersionString[];
	static const char m_AppLinesNameString[];
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

const char CAboutDlg::m_AppLinesVersionString[] = "$Revision$";
const char CAboutDlg::m_AppLinesNameString[] = "$Name$";

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

#ifdef _DEBUG
#define DLG_TEST
#endif

#ifdef DLG_TEST
#include "DlgItemMaintain.h"
#include "blz3/raytrace/b3Material.h"
#endif

// App command to run the dialog
void CAppLinesApp::OnAppAbout()
{
#ifdef DLG_TEST
	b3Base<b3Item>        head;
	b3Item               *item = b3World::b3AllocNode(WOOD);
	b3_scene_preparation  prep_info;

	prep_info.m_Scene = null;
	prep_info.m_t     = 0;
	head.b3InitBase(item->b3GetClass());
	head.b3Append(item);
	if (item->b3Prepare(&prep_info))
	{
		CDlgItemMaintain dlg(null,&head);

		dlg.DoModal();
	}
	else
	{
		b3Runtime::b3MessageBox("Preparation failed!",B3_MSGBOX_ERROR);
	}
	head.b3Free();
#else
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
#endif
}
