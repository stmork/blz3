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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.92  2006/06/15 21:12:04  sm
**	- Fixed b3LinkFileName
**
**	Revision 1.91  2006/06/15 18:36:48  sm
**	- Fixed recursive file list creation.
**	
**	Revision 1.90  2006/04/29 11:25:48  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**	
**	Revision 1.89  2006/03/19 14:47:17  sm
**	- Fixed missing initiailization problems in b3BBox.
**	- Moved some dialog elements into system library.
**	
**	Revision 1.88  2006/03/05 22:12:31  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.87  2005/05/16 08:24:30  sm
**	- Including self test into Lines.
**	
**	Revision 1.86  2005/01/23 20:57:21  sm
**	- Moved some global static variables into class static ones.
**	
**	Revision 1.85  2005/01/13 20:05:15  sm
**	- Some Lines bugfixes
**	
**	Revision 1.84  2005/01/09 19:58:30  sm
**	- Added more convenient plugin path maintainance
**	
**	Revision 1.83  2005/01/09 15:04:30  sm
**	- Added better search path support.
**	
**	Revision 1.82  2004/12/23 22:22:41  sm
**	- Adjusted further Visual C++ options
**	
**	Revision 1.81  2004/12/22 21:36:36  sm
**	- Changed development environment to Visual C++ .net 2003
**	
**	Revision 1.80  2004/10/16 17:00:51  sm
**	- Moved lighting into own class to ensure light setup
**	  after view setup.
**	- Fixed lighting for scene and simple overview
**	- Fixed Light cutoff exponent deadloop.
**	- Corrected OpenGL define (BLZ3_USE_OPENGL)
**	
**	Revision 1.79  2004/06/06 14:45:57  sm
**	- Added quick material/bump edit support.
**	- Added material to bump copy on wooden materials.
**	
**	Revision 1.78  2004/05/19 15:35:03  sm
**	- Hope of having fixed ticket no. 13.
**	
**	Revision 1.77  2004/05/17 13:00:33  sm
**	- Fixed inverse/reverse handling of object editing.
**	- Added diverse handling vor object loading/replacing.
**	
**	Revision 1.76  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.75  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.74  2004/05/10 15:12:08  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.73  2004/05/07 18:19:08  sm
**	- Added some menu entries and toolbar buttons
**	- Fixed missing default title of CB3PropertyPages
**	
**	Revision 1.72  2004/05/06 18:13:51  sm
**	- Added support for changed only b3Items for a
**	  better preview performance.
**	
**	Revision 1.71  2004/04/26 14:28:10  sm
**	- Added marble dialog
**	- Resorted controls
**	
**	Revision 1.70  2004/04/26 12:27:42  sm
**	- Added following dialogs:
**	  o granite
**	  o chess
**	- Added scaling to wood properties
**	
**	Revision 1.69  2004/04/25 19:28:21  sm
**	- Added available b3Items as list to maintain dialog.
**	- Preview is done only on auto refresh activated.
**	
**	Revision 1.68  2004/04/25 14:56:55  sm
**	- Completed simple material dialog.
**	
**	Revision 1.67  2004/04/25 10:34:51  sm
**	- Completed Cook/Torrance dialog
**	
**	Revision 1.66  2004/04/24 15:40:12  sm
**	- Started slide material dialog implementation
**	- Added simple property sheet/preview dialog template
**	
**	Revision 1.65  2004/04/24 08:54:20  sm
**	- Simplified property sheets inside dialogs.
**	
**	Revision 1.64  2004/04/23 18:46:17  sm
**	- Fixed bump sampler: Now using initialized derivativs
**	
**	Revision 1.63  2004/04/22 20:23:55  sm
**	- Fixed wrong ON_MESSAGE signature on call function.
**	- Reordered context menu of object editor.
**	- Expand root item of tree view in object editor
**	
**	Revision 1.62  2004/04/21 20:44:56  sm
**	- Added bump sampler to their dialogs.
**	- Added bbox dimensions for bump sampler
**	
**	Revision 1.61  2004/04/10 14:33:25  sm
**	- Added oak plank support.
**	
**	Revision 1.60  2004/04/09 17:30:31  sm
**	- Wood dialog fine tuning.
**	
**	Revision 1.59  2004/04/09 12:08:05  sm
**	- New CStatic control introduced for material sampling.
**	
**	Revision 1.58  2004/04/04 19:28:25  sm
**	- New wood dialog
**	
**	Revision 1.57  2003/07/22 19:12:34  sm
**	- Raytracing items should be registered in Lines, too!
**	
**	Revision 1.56  2003/07/12 17:44:46  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.55  2003/06/09 17:33:30  sm
**	- New item maintainance dialog added.
**	
**	Revision 1.54  2003/05/24 13:46:49  sm
**	- Added plugin support
**	- Fixed b3FileList on non existing directory.
**	
**	Revision 1.53  2003/02/26 19:13:05  sm
**	- Update scene/object views after color redefinition.
**	- Beautofied the app properties dialog.
**	
**	Revision 1.52  2003/02/26 16:36:16  sm
**	- Sorted drawing colors and added configuration support
**	  to dialog.
**	
**	Revision 1.51  2003/02/25 15:56:20  sm
**	- Added SplineRot to control grid drawing.
**	- Added support for pixel format selection in dialog items
**	- Restructured b3PickInfo
**	
**	Revision 1.50  2003/02/22 17:21:31  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.49  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.48  2003/02/09 13:58:14  sm
**	- cleaned up file selection dialogs
**	
**	Revision 1.47  2003/02/02 14:22:31  sm
**	- Added TGF import facility.
**	
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
