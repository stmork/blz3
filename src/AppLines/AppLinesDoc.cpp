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

#include "AppLinesInclude.h"

#include "DlgAnimation.h"
#include "DlgHierarchy.h"
#include "DlgItemMaintain.h"
#include "DlgScene.h"
#include "DlgSuperSampling.h"
#include "DlgNebular.h"
#include "DlgLensFlare.h"
#include "DlgModellerInfo.h"
#include "DlgDistributed.h"
#include "DlgLight.h"
#include "DlgLDC.h"
#include "b3SelectObject.h"
#include "b3UndoCutPaste.h"
#include "b3UndoAction.h"
#include "b3UndoObject.h"
#include "b3UndoLight.h"
#include "b3ExampleScene.h"
#include "b3Action.h"

#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"

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
	ON_UPDATE_COMMAND_UI(ID_ANIM_START, OnUpdateAnimStart)
	ON_UPDATE_COMMAND_UI(ID_ANIM_STEP_BACKWARD, OnUpdateAnimStepBack)
	ON_UPDATE_COMMAND_UI(ID_ANIM_STOP, OnUpdateAnimStop)
	ON_UPDATE_COMMAND_UI(ID_ANIM_PLAY, OnUpdateAnimPlay)
	ON_UPDATE_COMMAND_UI(ID_ANIM_PAUSE, OnUpdateAnimPause)
	ON_UPDATE_COMMAND_UI(ID_ANIM_STEP_FORWARD, OnUpdateAnimStepForward)
	ON_UPDATE_COMMAND_UI(ID_ANIM_END, OnUpdateAnimEnd)
	ON_COMMAND(ID_ANIM_PROPERTIES, OnAnimProperties)
	ON_UPDATE_COMMAND_UI(ID_ANIM_PROPERTIES, OnUpdateAnimProperties)
	ON_COMMAND(ID_COB_LOAD, OnCobLoad)
	ON_COMMAND(ID_TGF_LOAD, OnTgfLoad)
	ON_COMMAND(ID_MAINTAIN_SPECIAL, OnMaintainSpecial)
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
	ON_COMMAND(ID_DELETE_TRANSFORM_HISTORY, OnDeleteTransformHistory)
	ON_UPDATE_COMMAND_UI(ID_DELETE_TRANSFORM_HISTORY, OnUpdateDeleteTransformHistory)
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

// {B495FF74-0DA8-4580-8358-6D6A89F7C721}
static const IID IID_IAppLines =
{ 0xb495ff74, 0xda8, 0x4580, { 0x83, 0x58, 0x6d, 0x6a, 0x89, 0xf7, 0xc7, 0x21 } };

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

const char *CAppLinesDoc::b3GetDocumentName()
{
	return "_Lines_III_Scene_Document";
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
		CString saved_world_filename = AfxGetApp()->GetProfileString(CB3ClientString(),"Saved world filename","");

		// Build filename
		switch (b3Dir::b3Exists(saved_world_filename))
		{
		case B3_TYPE_FILE:
			b3Path::b3SplitFileName(
				saved_world_filename,
				filepath,
				null);
			filename.b3LinkFileName(filepath,GetTitle());
			break;

		case B3_TYPE_DIR:
			filename.b3LinkFileName(saved_world_filename, GetTitle());
			break;

		default:
			strcpy(filename, GetTitle());
			break;
		}
		filename.b3RemoveExt();
		filename.b3Append(".bwd");

		m_Scene = b3ExampleScene::b3CreateNew(filename);
		m_Anim  = m_Scene->b3GetAnimation();
		m_Info  = m_Scene->b3GetModellerInfo();
		m_Light = m_Scene->b3GetLight(true);
		m_Fulcrum.b3Update(b3GetFulcrum());
		m_World.b3SetFirst(m_Scene);
		b3Prepare(true,false);
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

		b3Prepare(true, false, true, true);
		SetModifiedFlag(FALSE);
		result = TRUE;

#ifdef _DEBUG
		m_Scene->b3Dump(1);
#endif
	}
	catch(b3WorldException &w)
	{
		m_World.b3Free();

		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"                       %s\n",w.b3GetErrorMsg());
		B3_MSG_ERROR(w);
	}
	catch(b3ExceptionBase &e)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"                       %s\n",e.b3GetErrorMsg());
		B3_MSG_ERROR(e);
	}
#ifndef _DEBUG
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
	}
#endif

	main->b3SetStatusMessage(AFX_IDS_IDLEMESSAGE);
	return result;
}

BOOL CAppLinesDoc::OnImportArcon(LPCTSTR lpszPathName)
{
	CMainFrame *main = CB3GetMainFrame();
	CString     message;
	BOOL        result = FALSE;

	// TODO: Add your specialized creation code here
	try
	{
		message.Format(IDS_DOC_READ,lpszPathName);
		main->b3SetStatusMessage(message);
		m_Scene = b3Scene::b3ReadTGF(lpszPathName);
		m_Anim  = m_Scene->b3GetAnimation();
		m_Info  = m_Scene->b3GetModellerInfo();
		m_Light = m_Scene->b3GetLight(true);
		m_Fulcrum.b3Update(b3GetFulcrum());

		m_World.b3SetFirst(m_Scene);
		b3Prepare(true,false,true);
		SetModifiedFlag(FALSE);
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
			new_name.b3Format("%s-new%d.bwd",(const char *)name,i++);
			filename.b3LinkFileName(path,new_name);
		}
		while(b3Dir::b3Exists(filename) != B3_NOT_EXISTANT);

		// Write!
		m_DlgHierarchy->b3GetData();
		m_Scene->b3Recount();
		m_Scene->b3SetFilename(lpszPathName);
#ifdef _DEBUG
		m_Scene->b3Dump(1);
#endif
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

	m_DlgHierarchy->b3InitTree(this,true);
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

void CAppLinesDoc::b3Prepare(
	b3_bool geometry_changed,
	b3_bool structure_changed,
	b3_bool reorg,
	b3_bool material_changed)
{
	CMainFrame *main   = CB3GetMainFrame();
	LPARAM      update = 0;

	if (reorg)
	{
		main->b3SetStatusMessage(IDS_DOC_REORG);
		m_Scene->b3Reorg();
	}

	if (geometry_changed)
	{
		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_Scene->b3SetupVertexMemory(&m_Context);
	}

	if (geometry_changed || structure_changed || reorg)
	{
		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();
	}

	if (geometry_changed)
	{
		main->b3SetStatusMessage(IDS_DOC_PREPARE);
		m_Scene->b3PrepareScene(0,0);
	}

	if (geometry_changed || structure_changed)
	{
		SetModifiedFlag();
		update |= B3_UPDATE_GEOMETRY;
	}

	if (material_changed)
	{
		main->b3SetStatusMessage(IDS_DOC_UPDATE_MATERIAL);
		m_Scene->b3UpdateMaterial();
		update |= B3_UPDATE_VIEW;
	}

	if (structure_changed)
	{
		m_DlgHierarchy->b3InitTree(this,true);
	}

	if (update != 0)
	{
		UpdateAllViews(NULL,update);
	}

	b3PrintF(B3LOG_DEBUG,"# %d vertices\n", m_Context.glVertexCount);
	b3PrintF(B3LOG_DEBUG,"# %d triangles\n",m_Context.glPolyCount);
	b3PrintF(B3LOG_DEBUG,"# %d lines\n",    m_Context.glGridCount);
	main->b3SetStatusMessage(AFX_IDS_IDLEMESSAGE);
}

void CAppLinesDoc::b3ComputeBounds()
{
	m_Scene->b3ComputeBounds(&m_Lower,&m_Upper);
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

	srcBBox = (b3BBox *)m_DlgHierarchy->m_Hierarchy.GetItemData(dragitem);
	dstBBox = (b3BBox *)m_DlgHierarchy->m_Hierarchy.GetItemData(dropitem);
	b3AddOp(new b3OpDrop(m_Scene,srcBBox,dstBBox));
}

void CAppLinesDoc::b3AddUndoAction(CB3Action *action)
{
	b3Operation *op;

	if (action->b3IsObject())
	{
		op = new b3OpObjectAction(m_Scene,action->b3GetTransformation());
		b3AddOp(op);
	}
	if (action->b3IsCamera())
	{
		op = new b3OpCameraAction(
			CB3GetMainFrame()->b3GetSelectedCamera(),
			action->b3GetTransformation());
		b3AddOp(op);
	}
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

		m_Scene->b3GetTitle(name,B3_FILESTRINGLEN);
		title.Format(IDS_RAYTRACE_TITLE,(const char *)name,xSize,ySize);
		m_RaytraceDoc->SetTitle(title);
		m_RaytraceDoc->b3UpdateTitle(name);
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
	CDlgScene         dlg_scene(m_Scene->b3GetClassType());
	CDlgNebular       dlg_nebular(m_Scene->b3GetClassType());
	CDlgSuperSampling dlg_super_sampling;
	CDlgLensFlare     dlg_lens_flare(m_Scene->b3GetClassType());
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
		b3ClearOp();
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
		b3ClearOp();
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
	b3AddOp(new b3OpLightCreate(m_Scene,m_Light));
}

void CAppLinesDoc::OnLightDelete() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpLightDelete(m_Scene,m_Light));
}

void CAppLinesDoc::OnLightProperties() 
{
	// TODO: Add your command handler code here
	CDlgLight   dlg(m_Scene->b3GetClassType());

	dlg.m_LightBase = m_Scene->b3GetLightHead();
	dlg.m_Light     = m_Light;
	if (dlg.DoModal() == IDOK)
	{
		m_Light = dlg.m_Light;
		CB3GetMainFrame()->b3UpdateLightBox(m_Scene,m_Light);

		b3ClearOp();
		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	}
}

void CAppLinesDoc::OnLightLDC() 
{
	// TODO: Add your command handler code here
	CDlgLDC dlg(m_Scene->b3GetClassType());

	dlg.m_Light = m_Light;
	if (dlg.DoModal() == IDOK)
	{
		b3ClearOp();
		SetModifiedFlag();
		CB3GetMainFrame()->b3UpdateLightBox(m_Scene,dlg.m_Light);
		UpdateAllViews(NULL,B3_UPDATE_LIGHT);
	}
}

void CAppLinesDoc::OnLightEnable() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpLightEnable(m_Light));
}

void CAppLinesDoc::OnLightSoft() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpLightSoft(m_Light));
}

void CAppLinesDoc::OnLightSpot() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpLightSpot(m_Light));
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
	pCmdUI->Enable((m_Scene->b3GetLightHead()->b3GetCount() > 1) && (!b3IsRaytracing()));
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

void CAppLinesDoc::b3Select(
	b3_line *dir,
	b3_bool  activate,
	b3_bool  add)
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
	b3AddOp(new b3OpObjectCreate(m_Scene,m_DlgHierarchy,insert_sub));
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
	b3AddOp(new b3OpObjectDelete(m_Scene,m_DlgHierarchy));
}

void CAppLinesDoc::OnEditCut() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpCut(m_DlgHierarchy,m_Scene));
}

void CAppLinesDoc::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CAppLinesApp   *app  = CB3GetLinesApp();
	b3BBox         *bbox = m_DlgHierarchy->b3GetSelectedBBox();
	b3Item         *prev;
	b3Base<b3Item> *base;

	m_DlgHierarchy->b3GetData();
	if (bbox != null)
	{
		base = m_Scene->b3FindBBoxHead(bbox);
		prev = bbox->Prev;
		base->b3Remove(bbox);
		app->b3PutClipboard(bbox);
		base->b3Insert(prev,bbox);
	}
}

void CAppLinesDoc::OnEditPaste() 
{
	b3AddOp(new b3OpPaste(m_DlgHierarchy,m_Scene,false));
}

void CAppLinesDoc::OnEditPasteSub() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpPaste(m_DlgHierarchy,m_Scene,true));
}

void CAppLinesDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CAppLinesApp *app  = CB3GetLinesApp();

	pCmdUI->Enable(
		(m_DlgHierarchy->b3GetSelectedBBox() != null) &&
		(!b3IsRaytracing()) &&
		(::IsClipboardFormatAvailable(app->m_ClipboardFormatForBlizzardObject)));
}

void CAppLinesDoc::OnObjectLoad() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpObjectLoad(m_Scene,m_DlgHierarchy));
}

void CAppLinesDoc::OnObjectSave() 
{
	// TODO: Add your command handler code here
	CAppLinesApp   *app = CB3GetLinesApp();
	b3Base<b3Item> *base;
	b3BBox         *selected;
	b3Item         *prev;
	b3Path          result = "";

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	prev = selected->Prev;
	base = m_Scene->b3FindBBoxHead(selected);

	if (CB3SelectSaveObject::b3Select(result,selected->b3GetName()))
	{
		base->b3Remove(selected);
		try
		{
			CWaitCursor  wait_for_end_of_workday;
			b3Display   *display;
			b3Scene     *scene;
			b3Tx         tx;

			app->b3WriteBBox(selected,result);

			tx.b3AllocTx(120,120,128);
			display = new b3Display(&tx);
			scene = b3ExampleScene::b3CreateBBox(selected,m_Scene->b3GetClassType());
			scene->b3GetSuperSample();
			scene->b3Raytrace(display);
			result.b3RemoveExt();
			result.b3Append(".tga");
			tx.b3SaveImage(result);
			delete scene;
			delete display;
		}
		catch(b3TxException &t)
		{
			b3PrintF(B3LOG_NORMAL,"IMAGE ERROR: Creating object thumbnail %s from %s (code: %d)\n",
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
			b3PrintF(B3LOG_NORMAL,"DATA ERROR: writing object %s to file %s (code: %d)\n",
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
	b3AddOp(new b3OpObjectReplace(m_Scene,m_DlgHierarchy));
}

void CAppLinesDoc::OnCobLoad() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpObjectLoadCob(m_Scene,m_DlgHierarchy));
}

void CAppLinesDoc::OnTgfLoad() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpObjectLoadTgf(m_Scene,m_DlgHierarchy));
}

void CAppLinesDoc::OnObjectCopy() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpObjectCopy(m_Scene,m_DlgHierarchy,b3GetFulcrum()));
}

b3_bool CAppLinesDoc::b3Edit()
{
	CWnd *pWnd = CB3GetMainFrame()->GetActiveFrame();
	b3_bool result = false;

	if (pWnd != null)
	{
		result = pWnd->PostMessage(WM_COMMAND, ID_OBJECT_EDIT, 0);
	}
	return result;
}

void CAppLinesDoc::OnObjectEdit() 
{
	CAppLinesApp  *app  = (CAppLinesApp *)AfxGetApp();
	CAppObjectDoc *pDoc = null;
	b3BBox        *selected;

	selected = m_DlgHierarchy->b3GetSelectedBBox();
	if (selected != null)
	{
		B3_ASSERT(b3BBox::b3FindBBox(m_Scene->b3GetBBoxHead(),selected));
		pDoc = app->b3CreateObjectDoc(this, selected);
		if (pDoc != null)
		{
			b3ClearOp();
		}
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
		m_Scene->b3Recount();
//	Brute force! Try without it.
		m_Scene->b3RecomputeMaterial();
		SetModifiedFlag();
		UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
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
		b3SetAnimation(m_TimePoint - 1.0 / m_Anim->m_FramesPerSecond);
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
		b3SetAnimation(m_TimePoint + 1.0 / m_Anim->m_FramesPerSecond);
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
	pCmdUI->Enable(b3HasAnimation() && !b3IsAnimClipped(m_TimePoint - 1.0 / m_Anim->m_FramesPerSecond));
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
	pCmdUI->Enable(b3HasAnimation() && !b3IsAnimClipped(m_TimePoint + 1.0 / m_Anim->m_FramesPerSecond));
}

void CAppLinesDoc::OnUpdateAnimEnd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation() && !b3IsPlaying());
}

void CAppLinesDoc::OnAnimProperties() 
{
	// TODO: Add your command handler code here
	CDlgAnimation dlg(m_Scene->b3GetAnimation(true));

	m_Anim = m_Scene->b3GetAnimation(true);
	if (dlg.DoModal())
	{
		SetModifiedFlag();
	}
}

void CAppLinesDoc::OnUpdateAnimProperties(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3HasAnimation());
}

void CAppLinesDoc::OnMaintainSpecial() 
{
	// TODO: Add your command handler code here
	CDlgItemMaintain dlg(this,m_Scene->b3GetSpecialHead());

	dlg.DoModal();
}

void CAppLinesDoc::OnDeleteTransformHistory() 
{
	// TODO: Add your command handler code here
	b3BBox *selected = m_DlgHierarchy->b3GetSelectedBBox();

	if (selected != null)
	{
		if (b3Runtime::b3MessageBox(IDS_ASK_DELETE_TRANSFORMATION_HISTORY,B3_MSGBOX_YES_NO) == B3_MSG_YES)
		{
			selected->b3ResetTransformation();
			SetModifiedFlag();
		}
	}
}

void CAppLinesDoc::OnUpdateDeleteTransformHistory(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3BBox *selected = m_DlgHierarchy->b3GetSelectedBBox();

	pCmdUI->Enable(selected != null);
}
