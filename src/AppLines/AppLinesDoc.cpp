/*
**
**	$Filename:	AppLinesDoc.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for scenes
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

#include "AppLines.h"
#include "AppLinesDoc.h"
#include "AppLinesView.h"
#include "AppRaytraceDoc.h"
#include "AppRaytraceView.h"
#include "MainFrm.h"

#include "DlgHierarchy.h"
#include "DlgScene.h"
#include "DlgSuperSampling.h"
#include "DlgNebular.h"
#include "DlgLensFlare.h"
#include "DlgModellerInfo.h"
#include "DlgDistributed.h"
#include "DlgCreateItem.h"
#include "DlgLight.h"
#include "DlgLDC.h"
#include "DlgCreateItem.h"
#include "DlgObjectCopy.h"
#include "b3SelectObject.h"

#include "b3ExampleScene.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3FileMem.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.72  2002/08/21 20:13:32  sm
**	- Introduced distributed raytracing with all sampling methods
**	  and filter computations. This made some class movements
**	  inside files necessary. The next step would be to integrate
**	  motion blur.
**
**	Revision 1.71  2002/08/19 16:50:39  sm
**	- Now having animation running, running, running...
**	- Activation handling modified to reflect animation
**	  and user transformation actions.
**	- Made some architectual redesigns.
**	
**	Revision 1.70  2002/08/18 13:05:17  sm
**	- First try to animate. We have to relink the control points which
**	  are stored in separate Blizzard classes to the b3AnimElement
**	  class.
**	
**	Revision 1.69  2002/08/17 17:31:22  sm
**	- Introduced animation support (Puh!)
**	
**	Revision 1.68  2002/08/09 13:20:18  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.67  2002/08/08 15:14:22  sm
**	- Some problems concerning b3Mem::b3Realloc fixed.
**	- Further error messages added.
**	
**	Revision 1.66  2002/08/07 17:25:01  sm
**	- Added new error messages
**	- Changed exception handling a little bit
**	
**	Revision 1.65  2002/08/07 14:26:23  sm
**	- Introduced mapping from Blizzard III error codes to human
**	  readable error messages supplied from Windows resources.
**	
**	Revision 1.64  2002/08/05 16:04:54  sm
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
**	Revision 1.63  2002/08/04 13:24:55  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**	
**	Revision 1.62  2002/07/31 11:57:10  sm
**	- The nVidia OpenGL init bug fixed by using following work
**	  around: The wglMakeCurrent() method is invoked on
**	  every OnPaint(). This is configurable depending on the
**	  hostname.
**	
**	Revision 1.61  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**	Revision 1.60  2002/02/14 16:32:33  sm
**	- Added activation via mouse selection
**	
**	Revision 1.59  2002/02/13 16:13:08  sm
**	- Created spotlight view
**	- Changed camera properties dialog to reflect scene units
**	  on example camera settings.
**	
**	Revision 1.58  2002/02/12 18:39:02  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.57  2002/02/01 17:22:44  sm
**	- Added icons for shapes
**	- Added shape support for hierarchy when shape editing
**	
**	Revision 1.56  2002/01/24 15:55:57  sm
**	- Fixed key handling on TreeCtrl (hierarchy dialog bar)
**	- Added support for conext menu depending on scene/object edit.
**	
**	Revision 1.55  2002/01/19 19:57:55  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.54  2002/01/18 16:49:34  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.53  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
**	Revision 1.52  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.51  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.50  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.49  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**	
**	Revision 1.48  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**	Revision 1.47  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**	
**	Revision 1.46  2002/01/09 17:47:53  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.45  2002/01/08 16:21:58  sm
**	- Added center to copy dialog
**	
**	Revision 1.44  2002/01/08 16:04:08  sm
**	- New copy dialog added
**	- Merge with daily work
**	
**	Revision 1.43  2002/01/08 15:45:50  sm
**	- Added support for repeating CButtons for button movement/rotation mode.
**	
**	Revision 1.42  2002/01/07 16:18:51  sm
**	- Added b3Item clone
**	- Added Drag & Drop
**	
**	Revision 1.41  2002/01/06 16:30:47  sm
**	- Added Load/Save/Replace object
**	- Enhanced "New world"
**	- Added some non static methods to b3Dir (Un*x untested, yet!)
**	- Fixed missing sphere/ellipsoid south pole triangles
**	- Fixed Spline cylinder/ring triangle indexing
**	
**	Revision 1.40  2002/01/05 22:17:47  sm
**	- Recomputing bounding boxes correctly
**	- Found key input bug: The accelerator are the problem
**	- Code cleanup
**	
**	Revision 1.39  2002/01/04 17:53:53  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.38  2002/01/03 19:07:27  sm
**	- Cleaned up cut/paste
**	
**	Revision 1.37  2002/01/03 15:50:14  sm
**	- Added cut/copy/paste
**	
**	Revision 1.36  2002/01/02 15:48:37  sm
**	- Added automated expand/collapse to hierarchy tree.
**	
**	Revision 1.35  2002/01/01 19:14:53  sm
**	- Added "rest deactivate"
**	
**	Revision 1.34  2001/12/31 16:39:40  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.33  2001/12/30 18:24:35  sm
**	- Added missing b3AnimControl class
**	- Some minor bug fixes done:
**	  o Missed some SetModifiedFlag()
**	  o b3Scene::b3SetCamera() calls added which now puts the
**	    selected camera in front of the b3Special list so that Lines III
**	    select it when reloading.
**	
**	Revision 1.32  2001/12/30 16:54:15  sm
**	- Inserted safe b3Write() into Lines III
**	- Fixed b3World saving: b3StoreXXX() methods must ensure
**	  buffer before the buffer itself is used.
**	- Extended b3Shape format with shape activation flag. Nice: The
**	  new data structures don't confuse the old Lines II/Blizzard II and
**	  even stores these new values.
**	
**	Revision 1.31  2001/12/28 15:17:44  sm
**	- Added clipboard-copy to raytraced view
**	- Added printing to raytraced view
**	- Much minor UI tuning done:
**	  o added accelerators
**	  o open maximized window
**	  o fixed some UpdateUI methods
**	  o changed exception handling in CB3ScrollView and CB3BitmapDxB
**	
**	Revision 1.30  2001/12/27 21:33:35  sm
**	- Further docking handling done
**	- CDocument cleanups done
**	
**	Revision 1.29  2001/12/26 18:17:55  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
**	Revision 1.28  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**	Revision 1.27  2001/12/07 16:36:12  sm
**	- Added simple LDC editing dialog.
**	
**	Revision 1.26  2001/12/03 18:37:51  sm
**	- Added light distribution curve control.
**	
**	Revision 1.25  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.24  2001/11/28 16:54:55  sm
**	- Dialog for modeller info.
**	
**	Revision 1.23  2001/11/25 12:25:31  sm
**	- Completing some dialogs:
**	  o super sampling
**	  o distributed raytracing
**	
**	Revision 1.22  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.21  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**	Revision 1.20  2001/11/09 18:58:52  sm
**	- Fixed JPEG handling
**	
**	Revision 1.19  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.18  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
**	
**	Revision 1.17  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.16  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.15  2001/10/29 19:34:02  sm
**	- Added new define B3_DELETE_BASE.
**	- Added support to abort raytrace processing.
**	- Added search path to world loading.
**	- Fixed super sampling.
**	- Fixed memory leak in raytracing row processing.
**	
**	Revision 1.14  2001/10/24 14:59:08  sm
**	- Some GIG bug fixes
**	- An image viewing bug fixed in bimg3
**	
**	Revision 1.13  2001/10/03 20:17:55  sm
**	- Minor bugfixes
**	
**	Revision 1.12  2001/09/30 15:46:06  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**	Revision 1.11  2001/09/23 15:37:15  sm
**	- Introducing raytracing for Lines III. There is much work
**	  for a b3Display-CScrollView.
**	
**	Revision 1.10  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**	
**	Revision 1.9  2001/09/01 15:54:53  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**	
**	Revision 1.8  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**	
**	Revision 1.7  2001/08/14 13:34:39  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**	
**	Revision 1.6  2001/08/13 15:05:01  sm
**	- Now we can scale and move around with stacked views.
**	
**	Revision 1.5  2001/08/12 19:47:47  sm
**	- Now having correct orthogonal projection incl. aspect ratio
**	
**	Revision 1.4  2001/08/10 18:28:58  sm
**	- Some bug fixing
**	- Update functions per view inserted. Now with redraw when loading.
**	
**	Revision 1.3  2001/08/09 15:27:34  sm
**	- Following shapes are newly supported now:
**	  o disk
**	  o cylinder
**	  o cone
**	  o ellipsoid
**	  o torus
**	  o triangles
**	- Done some makefile fixes
**	- Everything is Windozable
**	
**	Revision 1.2  2001/08/06 19:58:58  sm
**	- Drawing area - the first shape we can see with OpenGL
**	
**	Revision 1.1  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesDoc implementation                   **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppLinesDoc, CAppRenderDoc)

BEGIN_MESSAGE_MAP(CAppLinesDoc, CAppRenderDoc)
	//{{AFX_MSG_MAP(CAppLinesDoc)
	ON_COMMAND(ID_RAYTRACE, OnRaytrace)
	ON_COMMAND(ID_DLG_SCENE, OnDlgScene)
	ON_UPDATE_COMMAND_UI(ID_RAYTRACE, OnUpdateRaytrace)
	ON_COMMAND(ID_MODELLER_INFO, OnModellerInfo)
	ON_COMMAND(ID_LIGHT_NEW, OnLightNew)
	ON_COMMAND(ID_LIGHT_DELETE, OnLightDelete)
	ON_COMMAND(ID_LIGHT_PROPERTIES, OnLightProperties)
	ON_COMMAND(ID_LIGHT_ENABLE, OnLightEnable)
	ON_COMMAND(ID_LIGHT_SOFT, OnLightSoft)
	ON_CBN_SELCHANGE(ID_LIGHT_SELECT, OnLightSelect)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_DELETE, OnUpdateLightDelete)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_ENABLE, OnUpdateLightEnable)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SOFT, OnUpdateLightSoft)
	ON_COMMAND(ID_LIGHT_LDC, OnLightLDC)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_LDC, OnUpdateLightLDC)
	ON_COMMAND(ID_LIGHT_SPOT, OnLightSpot)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SPOT, OnUpdateLightSpot)
	ON_UPDATE_COMMAND_UI(ID_DLG_SCENE, OnUpdateGlobal)
	ON_COMMAND(ID_ACTIVATE, OnActivate)
	ON_COMMAND(ID_DEACTIVATE, OnDeactivate)
	ON_COMMAND(ID_DEACTIVATE_REST, OnDeactivateRest)
	ON_COMMAND(ID_ALL_DEACTIVATE, OnAllDeactivate)
	ON_COMMAND(ID_ALL_ACTIVATE, OnAllActivate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateSelectedBBox)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE_SUB, OnEditPasteSub)
	ON_COMMAND(ID_ALL_DEACTIVATE_REST, OnAllDeactivateRest)
	ON_COMMAND(ID_OBJECT_NEW, OnObjectNew)
	ON_COMMAND(ID_OBJECT_NEW_SUB, OnObjectNewSub)
	ON_COMMAND(ID_OBJECT_DELETE, OnObjectDelete)
	ON_COMMAND(ID_OBJECT_LOAD, OnObjectLoad)
	ON_COMMAND(ID_OBJECT_SAVE, OnObjectSave)
	ON_COMMAND(ID_OBJECT_REPLACE, OnObjectReplace)
	ON_COMMAND(ID_OBJECT_COPY, OnObjectCopy)
	ON_COMMAND(ID_OBJECT_EDIT, OnObjectEdit)
	ON_COMMAND(ID_ANIM_START, OnAnimStart)
	ON_COMMAND(ID_ANIM_STEP_BACKWARD, OnAnimStepBack)
	ON_COMMAND(ID_ANIM_STOP, OnAnimStop)
	ON_COMMAND(ID_ANIM_PLAY, OnAnimPlay)
	ON_COMMAND(ID_ANIM_PAUSE, OnAnimPause)
	ON_COMMAND(ID_ANIM_STEP_FORWARD, OnAnimStepForward)
	ON_COMMAND(ID_ANIM_END, OnAnimEnd)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_NEW, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_NEW_SUB, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_DELETE, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_COPY, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_EDIT, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LOAD, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SAVE, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_REPLACE, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_SUB, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_MODELLER_INFO, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_NEW, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_DELETE, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_PROPERTIES, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ENABLE, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TO_FULCRUM, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SELECT, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_NEW, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_PROPERTIES, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SELECT, OnUpdateGlobal)
	ON_UPDATE_COMMAND_UI(ID_ACTIVATE, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_DEACTIVATE, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_DEACTIVATE_REST, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_ALL_DEACTIVATE_REST, OnUpdateSelectedBBox)
	ON_UPDATE_COMMAND_UI(ID_ANIM_START, OnUpdateAnimStart)
	ON_UPDATE_COMMAND_UI(ID_ANIM_STEP_BACKWARD, OnUpdateAnimStepBack)
	ON_UPDATE_COMMAND_UI(ID_ANIM_STOP, OnUpdateAnimStop)
	ON_UPDATE_COMMAND_UI(ID_ANIM_PLAY, OnUpdateAnimPlay)
	ON_UPDATE_COMMAND_UI(ID_ANIM_PAUSE, OnUpdateAnimPause)
	ON_UPDATE_COMMAND_UI(ID_ANIM_STEP_FORWARD, OnUpdateAnimStepForward)
	ON_UPDATE_COMMAND_UI(ID_ANIM_END, OnUpdateAnimEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAppLinesDoc, CAppRenderDoc)
	//{{AFX_DISPATCH_MAP(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAppLines to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {72D69519-8984-11D5-A54F-0050BF4EB3F3}
static const IID IID_IAppLines =
{ 0x72d69519, 0x8984, 0x11d5, { 0xa5, 0x4f, 0x0, 0x50, 0xbf, 0x4e, 0xb3, 0xf3 } };

BEGIN_INTERFACE_MAP(CAppLinesDoc, CAppRenderDoc)
	INTERFACE_PART(CAppLinesDoc, IID_IAppLines, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc construction/destruction

CAppLinesDoc::CAppLinesDoc()
{
	m_Scene        = null;
	m_Anim         = null;
	m_Playing      = false;
	EnableAutomation();

	AfxOleLockApp();
	m_CameraLight = new b3CameraPart(CAMERA);
}

CAppLinesDoc::~CAppLinesDoc()
{
	AfxOleUnlockApp();
	delete m_CameraLight;
}

BOOL CAppLinesDoc::OnNewDocument()
{
	CMainFrame *main = CB3GetMainFrame();
	b3Path      filename;
	b3Path      filepath;
	BOOL        result = FALSE;

	if (!CAppRenderDoc::OnNewDocument())
	{
		return result;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	try
	{
		// Build filename
		b3Path::b3SplitFileName(
			AfxGetApp()->GetProfileString(CB3ClientString(),"Saved world filename",""),
			filepath,
			null);
		filename.b3LinkFileName(filepath,GetTitle());
		filename.b3RemoveExt();
		strcat(filename,".bwd");

		m_Scene = b3ExampleScene::b3CreateNew(filename);
		m_Anim  = m_Scene->b3GetAnimation();
		m_Info  = m_Scene->b3GetModellerInfo();
		m_Light = m_Scene->b3GetLight(true);
		m_Fulcrum.b3Update(b3GetFulcrum());
		
		main->b3SetStatusMessage(IDS_DOC_REORG);
		m_Scene->b3Reorg();

		main->b3SetStatusMessage(IDS_DOC_PREPARE);
		m_Scene->b3Prepare(0,0);

		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_Scene->b3AllocVertices(&m_Context);
		m_World.b3SetFirst(m_Scene);
		b3PrintF(B3LOG_DEBUG,"# %d vertices\n", m_Context.glVertexCount);
		b3PrintF(B3LOG_DEBUG,"# %d triangles\n",m_Context.glPolyCount);
		b3PrintF(B3LOG_DEBUG,"# %d lines\n",    m_Context.glGridCount);

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();
		SetPathName(filename);
		result = TRUE;
	}
	catch(b3ExceptionBase &e)
	{
		b3PrintF(B3LOG_NORMAL,"ERROR creating %s\n",GetPathName());
		B3_MSG_ERROR(e);
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"ERROR creating %s\n",GetPathName());
	}

	main->b3SetStatusMessage(AFX_IDS_IDLEMESSAGE);
	return result;
}

BOOL CAppLinesDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CMainFrame *main = CB3GetMainFrame();
	CString     message;
	BOOL        result = FALSE;

	// TODO: Add your specialized creation code here
	try
	{
		message.Format(IDS_DOC_READ,lpszPathName);
		main->b3SetStatusMessage(message);
		m_World.b3Read(lpszPathName);
		m_Scene = (b3Scene *)m_World.b3GetFirst();
		m_Scene->b3SetFilename(lpszPathName);
		m_Anim  = m_Scene->b3GetAnimation();
		m_Info  = m_Scene->b3GetModellerInfo();
		m_Light = m_Scene->b3GetLight(true);
		m_Fulcrum.b3Update(b3GetFulcrum());

		main->b3SetStatusMessage(IDS_DOC_REORG);
		m_Scene->b3Reorg();

		main->b3SetStatusMessage(IDS_DOC_PREPARE);
		m_Scene->b3Prepare(0,0);

		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_Scene->b3AllocVertices(&m_Context);
		b3PrintF(B3LOG_DEBUG,"# %d vertices\n", m_Context.glVertexCount);
		b3PrintF(B3LOG_DEBUG,"# %d triangles\n",m_Context.glPolyCount);
		b3PrintF(B3LOG_DEBUG,"# %d lines\n",    m_Context.glGridCount);

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();
		result = TRUE;
	}
	catch(b3ExceptionBase &e)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"                       %s\n",e.b3GetErrorMsg());
		B3_MSG_ERROR(e);
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
	}

	main->b3SetStatusMessage(AFX_IDS_IDLEMESSAGE);
	return result;
}

BOOL CAppLinesDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	CB3GetLinesApp()->b3CloseObjectDoc(this);
	return CAppRenderDoc::SaveModified();
}

BOOL CAppLinesDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL     result = false;
	b3_index i      = 1;
	b3Path   name,new_name;
	b3Path   path;
	b3Path   filename;

	try
	{
		// Suggest a temporary filename
		b3Path::b3SplitFileName(lpszPathName,path,name);
		name.b3RemoveExt();
		do
		{
			sprintf(new_name,"%s-new%d.bwd",(const char *)name,i++);
			filename.b3LinkFileName(path,new_name);
		}
		while(b3Dir::b3Exists(filename) != B3_NOT_EXISTANT);

		// Write!
		m_DlgHierarchy->b3GetData();
		m_Scene->b3SetFilename(lpszPathName);
		m_World.b3Write(filename);

		// ...and rename to original name
		remove(lpszPathName);
		if (rename(filename,lpszPathName) == 0)
		{
			// Mark if OK!
			SetModifiedFlag(FALSE);
			result = TRUE;
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"Error renaming %s into %s\n",
				(const char *)filename,lpszPathName);
		}
	}
	catch(b3WorldException &w)
	{
		remove(filename);
		B3_MSG_ERROR(w);
	}
	catch(b3ExceptionBase &e)
	{
		B3_MSG_ERROR(e);
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Saving %s\n",lpszPathName);
	}

	AfxGetApp()->WriteProfileString(CB3ClientString(),"Saved world filename",lpszPathName);
	return result;
}

void CAppLinesDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_RaytraceDoc != null)
	{
		m_RaytraceDoc->OnCloseDocument();
		m_RaytraceDoc = null;
	}
	CAppRenderDoc::OnCloseDocument();
}



/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc serialization

void CAppLinesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc diagnostics

#ifdef _DEBUG
void CAppLinesDoc::AssertValid() const
{
	CAppRenderDoc::AssertValid();
}

void CAppLinesDoc::Dump(CDumpContext& dc) const
{
	CAppRenderDoc::Dump(dc);
}
#endif //_DEBUG

/*************************************************************************
**                                                                      **
**                        General scene commands                        **
**                                                                      **
*************************************************************************/

void CAppLinesDoc::b3ComputeBounds()
{
	m_Scene->b3ComputeBounds(&m_Lower,&m_Upper);
}

b3_bool CAppLinesDoc::b3WriteBBox(b3BBox *bbox,const char *filename)
{
	b3File  file(filename,B_WRITE);

	return b3WriteBBox(bbox,&file);
}

b3_bool CAppLinesDoc::b3WriteBBox(b3BBox *bbox,b3FileAbstract *file)
{
	b3World  world;

	world.m_AutoDelete = false;
	world.b3SetFirst(bbox);
	return (world.b3Write(file) == B3_WORLD_OK);
}

void CAppLinesDoc::OnUpdateGlobal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!b3IsRaytracing());
}

void CAppLinesDoc::b3InitTree()
{
	TV_INSERTSTRUCT insert;
	HTREEITEM       root;

	insert.hParent      = TVI_ROOT;
	insert.hInsertAfter = TVI_FIRST;
	insert.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	insert.item.pszText = m_Scene->b3GetName();
	insert.item.lParam  = 0;
	insert.item.iImage         = 4;
	insert.item.iSelectedImage = 9;
	root = m_DlgHierarchy->m_Hierarchy.InsertItem (&insert);

	m_DlgHierarchy->b3AddBBoxes (root,m_Scene->b3GetFirstBBox(),false);
	m_DlgHierarchy->m_Hierarchy.Expand(root,TVE_EXPAND);
}

void CAppLinesDoc::b3ContextMenu(HTREEITEM item)
{
	CMenu          menu;
	CMenu         *submenu;
	CPoint point;

	if (menu.LoadMenu(IDR_CONTEXT_SCENE))
	{
		submenu = menu.GetSubMenu(0);
		ASSERT(submenu != NULL);

		m_DlgHierarchy->m_Hierarchy.SelectItem(item);
		::GetCursorPos(&point);
		submenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			point.x, point.y,
			CB3GetMainFrame()); // use main window for cmds
	}
}

/*************************************************************************
**                                                                      **
**                        Hierarchy drag & drop operations              **
**                                                                      **
*************************************************************************/

void CAppLinesDoc::b3DragBegin()
{
}

HTREEITEM CAppLinesDoc::b3Dragging(HTREEITEM dragitem,HTREEITEM dropitem)
{
	b3BBox         *srcBBox;
	b3BBox         *dstBBox;

	srcBBox = (b3BBox *)m_DlgHierarchy->m_Hierarchy.GetItemData(dragitem);
	dstBBox = (b3BBox *)m_DlgHierarchy->m_Hierarchy.GetItemData(dropitem);
	return
		(b3BBox::b3FindBBox(srcBBox->b3GetBBoxHead(),dstBBox) ||
		(srcBBox == dstBBox) ? null : dropitem);
}

void CAppLinesDoc::b3Drop(HTREEITEM dragitem,HTREEITEM dropitem)
{
	b3BBox         *srcBBox;
	b3BBox         *dstBBox;
	b3Base<b3Item> *srcBase;
	b3Base<b3Item> *dstBase;

	// Get BBoxes an their bases
	srcBBox = (b3BBox *)m_DlgHierarchy->m_Hierarchy.GetItemData(dragitem);
	dstBBox = (b3BBox *)m_DlgHierarchy->m_Hierarchy.GetItemData(dropitem);
	srcBase = m_Scene->b3FindBBoxHead(srcBBox);
	dstBase = (dstBBox != null ? dstBBox->b3GetBBoxHead() : m_Scene->b3GetBBoxHead());

	// Mark every ancestor before relink as changed
	m_Scene->b3BacktraceRecompute(srcBBox);

	// Relink BBox
	srcBase->b3Remove(srcBBox);
	dstBase->b3Append(srcBBox);

	// Mark every ancestor after relink as changed
	m_Scene->b3BacktraceRecompute(srcBBox);

	// Some recomputations...
	b3BBox::b3Recount(m_Scene->b3GetBBoxHead());
	b3ComputeBounds();

	// ... with some UI updates
	UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
	SetModifiedFlag();
	m_DlgHierarchy->b3InitTree(this,true);
}

/*************************************************************************
**                                                                      **
**                        Raytracing methods                            **
**                                                                      **
*************************************************************************/

b3_u32 CAppLinesDoc::b3RaytracingThread(void *ptr)
{
	CAppLinesDoc *pDoc = (CAppLinesDoc *)ptr;
	CWinApp      *app  = AfxGetApp();
	CMainFrame   *main = (CMainFrame *)app->m_pMainWnd;

	pDoc->m_Scene->b3Raytrace(pDoc->m_Display);

	if (pDoc->m_Display != null)
	{
		// Wait to finish raytracing...
		pDoc->m_Display->b3Wait();

		// Delete display (= view reference)
		delete pDoc->m_Display;
		pDoc->m_Display = null;

		// Update some controls...
//		main->PostMessage(WM_USER_UPDATE_CONTROLS);
	}
	return 42;
}

void CAppLinesDoc::b3StartRaytrace()
{
	CString title;
	b3_res  xSize,ySize;
	b3_bool valid;
	b3Path  name;

	if (!b3IsRaytracing())
	{
		valid = m_Scene->b3GetDisplaySize(xSize,ySize);
		m_Display = valid ?
			m_RaytraceDoc->b3GetDisplay(xSize,ySize,"Raytracing...") :
			m_RaytraceDoc->b3GetDisplay("Raytracing...");

		m_Scene->b3GetTitle(name);
		title.Format(IDS_RAYTRACE_TITLE,(const char *)name,xSize,ySize);
		m_RaytraceDoc->SetTitle(title);
		m_Raytracer->b3Start(&b3RaytracingThread,this);
	}
}

void CAppLinesDoc::b3StopRaytrace()
{
	b3Display *old_display;
	CWinApp   *app  = AfxGetApp();

	if (b3IsRaytracing())
	{
		// Rember display
		old_display = m_Display;

		// The next command is atomic:
		// Turn off output window waiting...
		m_Display = null;

		// Remove rows to compute
		m_Scene->b3AbortRaytrace();

		// Wait for pending rows then delete
		// processed rows
		m_Raytracer->b3Wait();

		// Nobody accesses the display now. So we
		// can delete it without using critical sections.
		delete old_display;

		// Update some controls...
		app->m_pMainWnd->PostMessage(WM_USER_UPDATE_CONTROLS);
	}
}

/*************************************************************************
**                                                                      **
**                        Scene commands                                **
**                                                                      **
*************************************************************************/

void CAppLinesDoc::OnRaytrace()
{
	// TODO: Add your command handler code here
	CAppLinesApp *app = (CAppLinesApp *)AfxGetApp();

	if (m_RaytraceDoc == null)
	{
		m_RaytraceDoc = (CAppRaytraceDoc *)app->b3CreateRaytraceDoc();
		m_RaytraceDoc->b3SetRenderDoc(this);
	}
	else
	{
		m_RaytraceDoc->b3ActivateView();
	}
	b3ToggleRaytrace();
}

void CAppLinesDoc::OnDlgScene() 
{
	// TODO: Add your command handler code here
	CPropertySheet    dlg_sheets;
	CDlgScene         dlg_scene;
	CDlgNebular       dlg_nebular;
	CDlgSuperSampling dlg_super_sampling;
	CDlgLensFlare     dlg_lens_flare;
	CDlgDistributed   dlg_distributed;
	CString           title;

	dlg_scene.m_Scene                = m_Scene;
	dlg_distributed.m_Distributed    = m_Scene->b3GetDistributed(true);
	dlg_super_sampling.m_SuperSample = m_Scene->b3GetSuperSample(true);
	dlg_nebular.m_Nebular            = m_Scene->b3GetNebular(true);
	dlg_lens_flare.m_LensFlare       = m_Scene->b3GetLensFlare(true);

	dlg_sheets.AddPage(&dlg_scene);
	dlg_sheets.AddPage(&dlg_distributed);
	dlg_sheets.AddPage(&dlg_super_sampling);
	dlg_sheets.AddPage(&dlg_nebular);
	dlg_sheets.AddPage(&dlg_lens_flare);

	title.LoadString(IDS_TITLE_SCENE);
	dlg_sheets.SetTitle(title);
	if (dlg_sheets.DoModal() == IDOK)
	{
		SetModifiedFlag();
	}
}

void CAppLinesDoc::OnModellerInfo() 
{
	// TODO: Add your command handler code here
	CDlgModellerInfo  dlg;

	dlg.m_ModellerInfo = m_Info;
	if (dlg.DoModal() == IDOK)
	{
		UpdateAllViews(NULL,B3_UPDATE_FULCRUM|B3_UPDATE_LIGHT);
		SetModifiedFlag();
	}
}

void CAppLinesDoc::OnUpdateRaytrace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(b3IsRaytracing());
}

/*************************************************************************
**                                                                      **
**                        Light commands                                **
**                                                                      **
*************************************************************************/

void CAppLinesDoc::OnLightNew() 
{
	// TODO: Add your command handler code here
	CDlgCreateItem  dlg;

	dlg.m_Label.LoadString(IDS_NEW_LIGHT);
	dlg.m_ItemBase   = m_Scene->b3GetLightHead();
	dlg.m_MaxNameLen = B3_BOXSTRINGLEN;
	dlg.m_Suggest    = m_Light->b3GetName();
	if (dlg.DoModal() == IDOK)
	{
		m_Light = new b3Light(AREA_LIGHT);
		strcpy (m_Light->b3GetName(),dlg.m_NewName);
		m_Scene->b3GetLightHead()->b3Append(m_Light);

		SetModifiedFlag();
		CB3GetMainFrame()->b3UpdateLightBox(m_Scene,m_Light);
		UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	}
}

void CAppLinesDoc::OnLightDelete() 
{
	// TODO: Add your command handler code here
	b3Light *select;

	if (AfxMessageBox(IDS_ASK_DELETE_LIGHT,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		// Determine new light
		select = (b3Light *)m_Light->Prev;
		if (select == null)
		{
			select = (b3Light *)m_Light->Succ;
		}
		m_Scene->b3GetLightHead()->b3Remove(m_Light);
		delete m_Light;

		// Select new light
		m_Light = select;
		CB3GetMainFrame()->b3UpdateLightBox(m_Scene,m_Light);

		// Set document to right state
		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	}
}

void CAppLinesDoc::OnLightProperties() 
{
	// TODO: Add your command handler code here
	CDlgLight   dlg;

	dlg.m_LightBase = m_Scene->b3GetLightHead();
	dlg.m_Light     = m_Light;
	if (dlg.DoModal() == IDOK)
	{
		m_Light = dlg.m_Light;
		CB3GetMainFrame()->b3UpdateLightBox(m_Scene,m_Light);

		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	}
}

void CAppLinesDoc::OnLightLDC() 
{
	// TODO: Add your command handler code here
	CDlgLDC     dlg;

	dlg.m_Light = m_Light;
	if (dlg.DoModal() == IDOK)
	{
		SetModifiedFlag();
		CB3GetMainFrame()->b3UpdateLightBox(m_Scene,dlg.m_Light);
		UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	}
}

void CAppLinesDoc::OnLightEnable() 
{
	// TODO: Add your command handler code here
	m_Light->m_LightActive = !m_Light->m_LightActive;
	UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	SetModifiedFlag();
}

void CAppLinesDoc::OnLightSoft() 
{
	// TODO: Add your command handler code here
	m_Light->m_SoftShadow = !m_Light->m_SoftShadow;
	SetModifiedFlag();
}

void CAppLinesDoc::OnLightSpot() 
{
	// TODO: Add your command handler code here
	m_Light->m_SpotActive = !m_Light->m_SpotActive;
	UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	SetModifiedFlag();
}

void CAppLinesDoc::OnLightSelect() 
{
	// TODO: Add your command handler code here
	m_Light = CB3GetMainFrame()->b3GetSelectedLight();
	UpdateAllViews(NULL,B3_UPDATE_LIGHT);
}

void CAppLinesDoc::OnUpdateLightDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((m_Scene->b3GetLightHead()->b3Count() > 1) && (!b3IsRaytracing()));
}

void CAppLinesDoc::OnUpdateLightLDC(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_Light != null)
	{
		pCmdUI->Enable(
			m_Light->m_LightActive && 
			m_Light->m_SpotActive &&
			(!b3IsRaytracing()));
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CAppLinesDoc::OnUpdateLightEnable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!b3IsRaytracing());
	pCmdUI->SetCheck(m_Light != null ? m_Light->m_LightActive : FALSE);
}

void CAppLinesDoc::OnUpdateLightSoft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_Light != null)
	{
		pCmdUI->Enable(m_Light->m_LightActive && (!b3IsRaytracing()));
		pCmdUI->SetCheck(m_Light->m_SoftShadow);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CAppLinesDoc::OnUpdateLightSpot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_Light != null)
	{
		pCmdUI->Enable(m_Light->m_LightActive && (!b3IsRaytracing()));
		pCmdUI->SetCheck(m_Light->m_SpotActive);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

/*************************************************************************
**                                                                      **
**                        Activation commands                           **
**                                                                      **
*************************************************************************/

void CAppLinesDoc::OnActivate() 
{
	// TODO: Add your control notification handler code here
	b3BBox *BBox = m_DlgHierarchy->b3GetSelectedBBox();

	if (BBox != null)
	{
		BBox->b3Activate();
		m_DlgHierarchy->b3UpdateActivation();
		if (BBox->Succ != null)
		{
			m_DlgHierarchy->b3SelectItem(BBox->Succ);
		}

		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

void CAppLinesDoc::OnDeactivate() 
{
	// TODO: Add your control notification handler code here
	b3BBox *BBox = m_DlgHierarchy->b3GetSelectedBBox();

	if (BBox != null)
	{
		BBox->b3Activate(false);
		m_DlgHierarchy->b3UpdateActivation();
		if (BBox->Succ != null)
		{
			m_DlgHierarchy->b3SelectItem(BBox->Succ);
		}
		
		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

void CAppLinesDoc::OnDeactivateRest() 
{
	// TODO: Add your control notification handler code here
	b3BBox *BBox = m_DlgHierarchy->b3GetSelectedBBox();
	b3BBox *aux;

	if (BBox != null)
	{
		BBox->b3Activate(true);
		for (aux = (b3BBox *)BBox->Prev;aux != null;aux = (b3BBox *)aux->Prev)
		{
			aux->b3Activate(false);
		}
		for (aux = (b3BBox *)BBox->Succ;aux != null;aux = (b3BBox *)aux->Succ)
		{
			aux->b3Activate(false);
		}
		m_DlgHierarchy->b3UpdateActivation();

		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

void CAppLinesDoc::OnAllDeactivate() 
{
	// TODO: Add your control notification handler code here
	m_Scene->b3Activate(false);
	m_DlgHierarchy->b3UpdateActivation();
	
	SetModifiedFlag();
	UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CAppLinesDoc::OnAllActivate() 
{
	// TODO: Add your control notification handler code here
	m_Scene->b3Activate();
	m_DlgHierarchy->b3UpdateActivation();

	SetModifiedFlag();
	UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CAppLinesDoc::OnAllDeactivateRest() 
{
	// TODO: Add your command handler code here
	b3BBox *BBox = m_DlgHierarchy->b3GetSelectedBBox();

	if (BBox != null)
	{
		m_Scene->b3Activate(false);
		BBox->b3Activate(true);
		m_DlgHierarchy->b3UpdateActivation();

		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

void CAppLinesDoc::OnUpdateSelectedBBox(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_DlgHierarchy->b3GetSelectedBBox() != null);
}

void CAppLinesDoc::b3Select(b3_line *dir,b3_bool activate,b3_bool add)
{
	b3BBox    *bbox;
	b3_line64  line;
	int        i,max;

	if (!add)
	{
		// Deactivate all bboxes first
		m_Scene->b3Activate(false);
	}

	// Prepare search criteria
	b3Vector::b3Init(&line.pos,dir->pos.x,dir->pos.y,dir->pos.z);
	b3Vector::b3Init(&line.dir,dir->dir.x,dir->dir.y,dir->dir.z);

	// Search!
	m_Scene->b3CollectBBoxes(&line,&m_HitBBoxes);

	// Make selection on found bboxes
	max = m_HitBBoxes.b3GetCount();
	for (i = 0;i < max;i++)
	{
		bbox = m_HitBBoxes[i];
		bbox->b3Activate(activate);
	}

	// Mark as modified and update
	SetModifiedFlag();
	m_DlgHierarchy->b3UpdateActivation();
	UpdateAllViews(NULL,B3_UPDATE_VIEW);
}

void CAppLinesDoc::b3Select(
	b3_vector *lower,
	b3_vector *upper,
	b3_bool    activate,
	b3_bool    add)
{
	b3BBox *bbox;
	int     i,max;

	if (!add)
	{
		// Deactivate all bboxes first
		m_Scene->b3Activate(false);
	}

	// Prepare search criteria
	b3Vector::b3Sort(lower,upper);

	// Search!
	m_Scene->b3CollectBBoxes(lower,upper,&m_HitBBoxes);

	// Make selection on found bboxes
	max = m_HitBBoxes.b3GetCount();
	for (i = 0;i < max;i++)
	{
		bbox = m_HitBBoxes[i];
		bbox->b3Activate(activate);
	}

	// Mark as modified and update
	SetModifiedFlag();
	m_DlgHierarchy->b3UpdateActivation();
	UpdateAllViews(NULL,B3_UPDATE_VIEW);
}

/*************************************************************************
**                                                                      **
**                        Copy/paste commands                           **
**                                                                      **
*************************************************************************/

void CAppLinesDoc::b3ObjectCreate(b3_bool insert_sub)
{
	CAppLinesApp   *app     = (CAppLinesApp *)AfxGetApp();
	CMainFrame     *main    = CB3GetMainFrame();
	CDlgCreateItem  dlg;
	b3BBox         *selected;
	b3BBox         *bbox;
	b3Item         *insert_after;
	b3Base<b3Item> *base;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	if (insert_sub)
	{
		base = selected->b3GetBBoxHead();
		insert_after = base->Last;
	}
	else
	{
		base = m_Scene->b3FindBBoxHead(selected);
		insert_after = selected;
	}
	B3_ASSERT(base != null);

	dlg.m_ClassType   = CLASS_BBOX;
	dlg.m_MaxNameLen  = B3_BOXSTRINGLEN;
	dlg.m_ItemBase    = base;
	dlg.m_NoNameCheck = true;
	dlg.m_Suggest.LoadString(IDS_NEW_OBJECT);
	if (dlg.DoModal() == IDOK)
	{
		bbox = new b3BBox(BBOX);
		strcpy (bbox->m_BoxName,dlg.m_NewName);
		base->b3Insert(insert_after,bbox);
		b3BBox::b3Recount(m_Scene->b3GetBBoxHead());

		main->b3SetStatusMessage(IDS_DOC_PREPARE);
		bbox->b3Prepare();

		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_Scene->b3AllocVertices(&m_Context);

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();

		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		m_DlgHierarchy->b3InitTree(this,true);
		m_DlgHierarchy->b3SelectItem(bbox);
	}
}

void CAppLinesDoc::OnObjectNew() 
{
	// TODO: Add your command handler code here
	b3ObjectCreate(false);
}

void CAppLinesDoc::OnObjectNewSub() 
{
	// TODO: Add your command handler code here
	b3ObjectCreate(true);
}

void CAppLinesDoc::OnObjectDelete() 
{
	// TODO: Add your command handler code here
	CMainFrame     *main    = CB3GetMainFrame();
	b3Base<b3Item> *base;
	b3Item         *select;
	b3BBox         *selected;

	if (AfxMessageBox(IDS_ASK_DELETE_OBJECT,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		selected = m_DlgHierarchy->b3GetSelectedBBox();
		select   = (selected->Succ != null ? selected->Succ : selected->Prev);
		B3_ASSERT(selected != null);
		base = m_Scene->b3FindBBoxHead(selected);

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		m_Scene->b3BacktraceRecompute(selected);
		base->b3Remove(selected);
		delete selected;

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();

		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		m_DlgHierarchy->b3InitTree(this,true);
		m_DlgHierarchy->b3SelectItem(select);
	}
}

b3_bool CAppLinesDoc::b3PutClipboard(b3BBox *bbox)
{
	CAppLinesApp *app     = (CAppLinesApp *)AfxGetApp();
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
							::SetClipboardData(app->m_ClipboardFormatForBlizzardObject,handle);
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

void CAppLinesDoc::OnEditCut() 
{
	// TODO: Add your command handler code here
	b3BBox         *bbox = m_DlgHierarchy->b3GetSelectedBBox();
	b3Item         *select;
	b3Item         *prev;
	b3Base<b3Item> *base;

	m_DlgHierarchy->b3GetData();
	if (bbox != null)
	{
		base   = m_Scene->b3FindBBoxHead(bbox);
		prev   = bbox->Prev;
		select = (bbox->Succ != null ? bbox->Succ : bbox->Prev);
		m_Scene->b3BacktraceRecompute(bbox);
		base->b3Remove(bbox);
		if(!b3PutClipboard(bbox))
		{
			base->b3Insert(prev,bbox);
		}
		else
		{
			delete bbox;

			CB3GetMainFrame()->b3SetStatusMessage(IDS_DOC_BOUND);
			b3ComputeBounds();
			SetModifiedFlag();
			UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
			m_DlgHierarchy->b3InitTree(this,true);
			m_DlgHierarchy->b3SelectItem(select);
		}
	}
}

void CAppLinesDoc::OnEditCopy() 
{
	// TODO: Add your command handler code here
	b3BBox         *bbox = m_DlgHierarchy->b3GetSelectedBBox();
	b3Item         *prev;
	b3Base<b3Item> *base;

	m_DlgHierarchy->b3GetData();
	if (bbox != null)
	{
		base = m_Scene->b3FindBBoxHead(bbox);
		prev = bbox->Prev;
		base->b3Remove(bbox);
		b3PutClipboard(bbox);
		base->b3Insert(prev,bbox);
	}
}

void CAppLinesDoc::b3PasteClipboard(b3_bool insert_sub) 
{
	// TODO: Add your command handler code here
	CAppLinesApp   *app  = (CAppLinesApp *)AfxGetApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	b3BBox         *selected;
	b3BBox         *bbox;
	b3Item         *insert_after;
	b3Base<b3Item> *base;
	b3_size         size;
	b3_count        level;
	void           *ptr;
	HANDLE          handle;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	if (insert_sub)
	{
		base = selected->b3GetBBoxHead();
		insert_after = base->Last;
	}
	else
	{
		base = m_Scene->b3FindBBoxHead(selected);
		insert_after = selected;
	}
	B3_ASSERT(base != null);

	if (main->OpenClipboard())
	{
		handle = ::GetClipboardData(app->m_ClipboardFormatForBlizzardObject);
		if (handle != 0)
		{
			size = ::GlobalSize(handle);
			ptr  = ::GlobalLock(handle);
			try
			{
				b3FileMem file(B_WRITE);
				b3World   world;

				file.b3Write(ptr,size);
				file.b3Seek(0,B3_SEEK_START);

				if(world.b3Read(&file) == B3_WORLD_OK)
				{
					main->b3SetStatusMessage(IDS_DOC_REORG);
					bbox  = (b3BBox *)world.b3GetFirst();
					level = bbox->b3GetClassType() & 0xffff;
					b3BBox::b3Reorg(world.b3GetHead(),base,level,1,insert_after);
					b3BBox::b3Recount(m_Scene->b3GetBBoxHead());

					main->b3SetStatusMessage(IDS_DOC_PREPARE);
					bbox->b3Prepare();

					main->b3SetStatusMessage(IDS_DOC_VERTICES);
					m_Scene->b3AllocVertices(&m_Context);

					main->b3SetStatusMessage(IDS_DOC_BOUND);
					m_Scene->b3BacktraceRecompute(bbox);
					b3ComputeBounds();

					SetModifiedFlag();
					UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
					m_DlgHierarchy->b3InitTree(this,true);
					m_DlgHierarchy->b3SelectItem(bbox);
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
}

void CAppLinesDoc::OnEditPaste() 
{
	b3PasteClipboard(false);
}

void CAppLinesDoc::OnEditPasteSub() 
{
	// TODO: Add your command handler code here
	b3PasteClipboard(true);
}

void CAppLinesDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAppLinesApp *app  = (CAppLinesApp *)AfxGetApp();

	pCmdUI->Enable(
		(m_DlgHierarchy->b3GetSelectedBBox() != null) &&
		(!b3IsRaytracing()) &&
		(::IsClipboardFormatAvailable(app->m_ClipboardFormatForBlizzardObject)));
}

void CAppLinesDoc::OnObjectLoad() 
{
	// TODO: Add your command handler code here
	CAppLinesApp   *app  = (CAppLinesApp *)AfxGetApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	CString         suggest;
	b3BBox         *selected;
	b3BBox         *bbox;
	b3Base<b3Item> *base;
	b3_count        level;
	b3Path          filepath;
	b3Path          result;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	base     = (selected == null ? m_Scene->b3GetBBoxHead() : m_Scene->b3FindBBoxHead(selected));
	suggest  = app->GetProfileString(CB3ClientString(),"Loaded object filename","");

	b3Path::b3SplitFileName(suggest,filepath,null);
	if (selected != null)
	{
		result.b3LinkFileName(filepath,selected->b3GetName());
		result.b3RemoveExt();
		strcat((char *)result,".bod");
	}
	else
	{
		strcpy((char *)result,suggest);
	}
	
	if (CB3SelectObject::b3Select((char *)result))
	{
		app->WriteProfileString(CB3ClientString(),"Loaded object filename",result);
		
		try
		{
			b3File  file(result,B_READ);
			b3World world;

			if(world.b3Read(&file) == B3_WORLD_OK)
			{
				main->b3SetStatusMessage(IDS_DOC_REORG);
				bbox  = (b3BBox *)world.b3GetFirst();
				level = bbox->b3GetClassType() & 0xffff;
				b3BBox::b3Reorg(world.b3GetHead(),base,level,1,selected);
				b3BBox::b3Recount(m_Scene->b3GetBBoxHead());

				main->b3SetStatusMessage(IDS_DOC_PREPARE);
				bbox->b3Prepare();

				main->b3SetStatusMessage(IDS_DOC_VERTICES);
				m_Scene->b3AllocVertices(&m_Context);

				main->b3SetStatusMessage(IDS_DOC_BOUND);
				m_Scene->b3BacktraceRecompute(bbox);
				b3ComputeBounds();

				SetModifiedFlag();
				UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
				m_DlgHierarchy->b3InitTree(this,true);
				m_DlgHierarchy->b3SelectItem(bbox);
			}
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading object from file %s (code: %d)\n",
				(const char *)result,f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: reading object from file %s (code: %d)\n",
				(const char *)result,w.b3GetError());
			B3_MSG_ERROR(w);
		}
	}
}

void CAppLinesDoc::OnObjectSave() 
{
	// TODO: Add your command handler code here
	CString         suggest;
	CString         ext;
	CString         filter;
	CWinApp        *app = AfxGetApp();
	b3Base<b3Item> *base;
	b3BBox         *selected;
	b3Item         *prev;
	b3Path          filepath;
	b3Path          filename;
	b3Path          result;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	prev = selected->Prev;
	base = m_Scene->b3FindBBoxHead(selected);

	suggest  = app->GetProfileString(CB3ClientString(),"Saved object filename","");
	b3Path::b3SplitFileName(suggest,filepath,null);
	b3Path::b3LinkFileName(filename,filepath,selected->b3GetName());
	suggest = filename;

	filter.LoadString(IDS_OBJECT_FILTER);
	if (b3SaveDialog(suggest,ext,filter,result))
	{
		app->WriteProfileString(CB3ClientString(),"Saved object filename",result);

		base->b3Remove(selected);
		try
		{
			CWaitCursor  wait_for_end_of_workday;
			b3Display   *display;
			b3Scene     *scene;
			b3Tx         tx;

			b3WriteBBox(selected,result);

			tx.b3AllocTx(120,120,24);
			display = new b3Display(&tx);
			scene = b3ExampleScene::b3CreateBBox(selected);
			scene->b3Raytrace(display);
			result.b3RemoveExt();
			strcat(result,".TGA");
			tx.b3SaveImage(result);
			delete scene;
			delete display;
		}
		catch(b3TxException &t)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: Creating object thumbnail %s from %s (code: %d)\n",
				(const char *)result,selected->b3GetName(),t.b3GetError());
			B3_MSG_ERROR(t);
		}
		catch(b3DisplayException &d)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: Creating object thumbnail %s (code: %d)\n",
				selected->b3GetName(),d.b3GetError());
			B3_MSG_ERROR(d);
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: writing object %s to file %s (code: %d)\n",
				selected->b3GetName(),(const char *)result,f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: writing object %s to file %s (code: %d)\n",
				selected->b3GetName(),(const char *)result,w.b3GetError());
			B3_MSG_ERROR(w);
		}
		catch(...)
		{
			b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: writing object %s to file %s\n",
				selected->b3GetName(),(const char *)result);
		}
		base->b3Insert(prev,selected);
	}
}

void CAppLinesDoc::OnObjectReplace() 
{
	// TODO: Add your command handler code here
	CAppLinesApp   *app  = (CAppLinesApp *)AfxGetApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	CString         suggest;
	b3BBox         *selected;
	b3BBox         *bbox;
	b3Base<b3Item> *base;
	b3_count        level;
	b3Path          filepath;
	b3Path          result;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	base     = m_Scene->b3FindBBoxHead(selected);

	suggest  = app->GetProfileString(CB3ClientString(),"Replaced object filename","");
	b3Path::b3SplitFileName(suggest,filepath,null);
	result.b3LinkFileName(filepath,selected->b3GetName());
	result.b3RemoveExt();
	strcat((char *)result,".bod");

	if (CB3SelectObject::b3Select((char *)result))
	{
		app->WriteProfileString(CB3ClientString(),"Replaced object filename",result);
		
		try
		{
			b3File  file(result,B_READ);
			b3World world;

			if(world.b3Read(&file) == B3_WORLD_OK)
			{
				main->b3SetStatusMessage(IDS_DOC_REORG);
				bbox  = (b3BBox *)world.b3GetFirst();
				level = bbox->b3GetClassType() & 0xffff;
				b3BBox::b3Reorg(world.b3GetHead(),base,level,1,selected);
				base->b3Remove(selected);
				delete selected;
				b3BBox::b3Recount(m_Scene->b3GetBBoxHead());

				main->b3SetStatusMessage(IDS_DOC_PREPARE);
				bbox->b3Prepare();

				main->b3SetStatusMessage(IDS_DOC_VERTICES);
				m_Scene->b3AllocVertices(&m_Context);

				main->b3SetStatusMessage(IDS_DOC_BOUND);
				m_Scene->b3BacktraceRecompute(bbox);
				b3ComputeBounds();

				SetModifiedFlag();
				UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
				m_DlgHierarchy->b3InitTree(this,true);
				m_DlgHierarchy->b3SelectItem(bbox);
			}
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading object from file %s (code: %d)\n",
				(const char *)result,f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: reading object from file %s (code: %d)\n",
				(const char *)result,w.b3GetError());
			B3_MSG_ERROR(w);
		}
	}
}

void CAppLinesDoc::OnObjectCopy() 
{
	// TODO: Add your command handler code here
	CAppLinesApp   *app  = (CAppLinesApp *)AfxGetApp();
	CMainFrame     *main = CB3GetMainFrame();
	CDlgObjectCopy  dlg;
	b3BBox         *selected;
	b3BBox         *bbox;
	b3BBox         *cloned;
	b3Base<b3Item> *base;
	b3_count        i;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	base     = m_Scene->b3FindBBoxHead(selected);
	dlg.m_OrigBBox = selected;
	dlg.m_Center   = b3GetFulcrum();
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor     wait;
	
		main->b3SetStatusMessage(IDS_DOC_PREPARE);
		bbox = selected;
		for (i = 0;i < dlg.m_NumCopies;i++)
		{
			cloned = (b3BBox *)b3World::b3Clone(bbox);
			cloned->b3Transform(&dlg.m_Transformation,true,true);
			cloned->b3Prepare();
			base->b3Insert(bbox,cloned);
			bbox = cloned;
		}

		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_Scene->b3AllocVertices(&m_Context);

		main->b3SetStatusMessage(IDS_DOC_BOUND);
		m_Scene->b3BacktraceRecompute(selected);
		b3ComputeBounds();

		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		m_DlgHierarchy->b3InitTree(this,true);
	}
}

void CAppLinesDoc::OnObjectEdit() 
{
	// TODO: Add your command handler code here
	CAppLinesApp  *app  = (CAppLinesApp *)AfxGetApp();
	CAppObjectDoc *pDoc;
	b3BBox        *selected;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	if (selected != null)
	{
		B3_ASSERT(b3BBox::b3FindBBox(m_Scene->b3GetBBoxHead(),selected));
		pDoc = app->b3CreateObjectDoc(this,selected);
	}
}

void CAppLinesDoc::b3FinishEdit(
	b3BBox *original,
	b3BBox *bbox)
{
	b3Base<b3Item> *base;

	if ((original != null) && (bbox != null))
	{
		base = m_Scene->b3FindBBoxHead(original);
		base->b3Insert(original,bbox);
		base->b3Remove(original);
		SetModifiedFlag();
	}
}

/*************************************************************************
**                                                                      **
**                        Animation methods                             **
**                                                                      **
*************************************************************************/

b3_bool CAppLinesDoc::b3HasAnimation()
{
	return m_Anim != null;
}

b3_bool CAppLinesDoc::b3IsPlaying()
{
	return m_Playing;
}

void CAppLinesDoc::b3ContinuePlay()
{
	if (b3IsPlaying())
	{
		POSITION position;

		position = GetFirstViewPosition();
		while(position != NULL)
		{
			CAppRenderView *view = (CAppRenderView *)GetNextView(position);

			view->b3Paint();
		}
		b3PrintF(B3LOG_FULL,"Sending play continue request to main frame...\n");
		AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND,MAKELONG(ID_ANIM_PLAY,0),0);
	}
}

b3_f64 CAppLinesDoc::b3SetAnimation(b3_f64 t)
{
	m_TimePoint = m_Anim->b3ClipTimePoint(t);

	b3PrintF(B3LOG_FULL,"Animation to time point %.3lf...\n",m_TimePoint);
	m_Scene->b3SetAnimation(m_TimePoint);
	UpdateAllViews(NULL,B3_UPDATE_ALL);
	b3PrintF(B3LOG_FULL,"Animation done...\n");
	return m_TimePoint;
}

b3_bool CAppLinesDoc::b3IsAnimClipped(b3_f64 t)
{
	return t != m_Anim->b3ClipTimePoint(t);
}

void CAppLinesDoc::OnAnimStart() 
{
	// TODO: Add your command handler code here
	if (b3HasAnimation())
	{
		m_Scene->b3ResetAnimation();
		b3SetAnimation(m_Anim->m_Start);
	}
}

void CAppLinesDoc::OnAnimStepBack() 
{
	// TODO: Add your command handler code here
	if (b3HasAnimation())
	{
		b3SetAnimation(m_TimePoint - 0.5);
	}
	m_Playing = false;
}

void CAppLinesDoc::OnAnimStop() 
{
	// TODO: Add your command handler code here
	m_Playing = false;
}

void CAppLinesDoc::OnAnimPlay() 
{
	// TODO: Add your command handler code here
	if (b3HasAnimation())
	{
		b3_f64 t;
		b3Time now;

		if (!b3IsPlaying())
		{
			m_Time.b3Now();
			t = m_Anim->m_Start;
			m_Playing = true;
		}
		else
		{
			b3_f64 fps = 1.0 / (now - m_Last);

			b3PrintF(B3LOG_FULL,"%2.3f frames/second.\n",fps);
			t = now - m_Time + m_Anim->m_Start;
			m_Playing = !b3IsAnimClipped(t);
		}
		m_Last = now;
		b3SetAnimation(t);
		b3ContinuePlay();
	}
}

void CAppLinesDoc::OnAnimPause() 
{
	// TODO: Add your command handler code here
	m_Playing = false;
}

void CAppLinesDoc::OnAnimStepForward() 
{
	// TODO: Add your command handler code here
	if (b3HasAnimation())
	{
		b3SetAnimation(m_TimePoint + 0.5);
	}
	m_Playing = false;
}

void CAppLinesDoc::OnAnimEnd() 
{
	// TODO: Add your command handler code here
	if (b3HasAnimation != null)
	{
		m_Scene->b3ResetAnimation();
		b3SetAnimation(m_Anim->m_End);
	}
}

void CAppLinesDoc::OnUpdateAnimStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && !b3IsPlaying());
}

void CAppLinesDoc::OnUpdateAnimStepBack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && !b3IsAnimClipped(m_TimePoint - 0.5));
}

void CAppLinesDoc::OnUpdateAnimStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && b3IsPlaying());
}

void CAppLinesDoc::OnUpdateAnimPlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(b3IsPlaying());
}

void CAppLinesDoc::OnUpdateAnimPause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && b3IsPlaying());
}

void CAppLinesDoc::OnUpdateAnimStepForward(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && !b3IsAnimClipped(m_TimePoint + 0.5));
}

void CAppLinesDoc::OnUpdateAnimEnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && !b3IsPlaying());
}
