/*
**
**	$Filename:	AppObjectDoc.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for objects
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "AppObjectDoc.h"
#include "AppObjectView.h"
#include "MainFrm.h"

#include "b3UndoShape.h"
#include "b3UndoShapeSurface.h"

#include "DlgItemMaintain.h"
#include "DlgKnotControl.h"
#include "DlgNewObject.h"

#include "blz3/base/b3Matrix.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Bump.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.55  2005/06/12 11:38:51  sm
**	- Fix ticket no.30. Surface property changes are reflected
**	  to document modified changes.
**
**	Revision 1.54  2005/05/15 10:19:26  sm
**	- Fixed picking operations for undo/redo
**	
**	Revision 1.53  2005/05/14 19:01:24  sm
**	- Added shape property copy to undo/redo operations
**	
**	Revision 1.52  2005/05/13 15:33:54  sm
**	- Tested surface property operations.
**	
**	Revision 1.51  2005/05/12 20:16:12  sm
**	- Some more undo/redo surface operations.
**	
**	Revision 1.50  2005/05/12 12:16:25  sm
**	- Added surface property editing for undo/redo editing.
**	
**	Revision 1.49  2005/05/11 16:13:19  sm
**	- Added following undo/redo operations for object editor:
**	  o shape edit
**	  o shape delete
**	
**	Revision 1.48  2005/05/11 14:19:28  sm
**	- Added first Undo operation for object editor.
**	
**	Revision 1.47  2005/04/27 13:55:01  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**	
**	Revision 1.46  2005/01/23 20:57:22  sm
**	- Moved some global static variables into class static ones.
**	
**	Revision 1.45  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.44  2005/01/02 19:15:24  sm
**	- Fixed signed/unsigned warnings
**	
**	Revision 1.43  2004/12/11 17:05:01  sm
**	- Fixed update/draw problem in object editor
**	
**	Revision 1.42  2004/10/16 17:00:51  sm
**	- Moved lighting into own class to ensure light setup
**	  after view setup.
**	- Fixed lighting for scene and simple overview
**	- Fixed Light cutoff exponent deadloop.
**	- Corrected OpenGL define (BLZ3_USE_OPENGL)
**	
**	Revision 1.41  2004/09/24 13:45:35  sm
**	- Extracted OpenGL extension vector buffer objects into own files.
**	- Some cleanup for Lines.
**	
**	Revision 1.40  2004/07/03 13:49:30  sm
**	- Added spline knot control dialog which is not completed yet.
**	
**	Revision 1.39  2004/07/02 19:28:03  sm
**	- Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**	- Recoupled b3Scene include from CApp*Doc header files to allow
**	  faster compilation.
**	- Removed intersection counter completely because of a mysterious
**	  destruction problem of b3Mutex.
**	
**	Revision 1.38  2004/06/28 18:42:34  sm
**	- Corrected some input types of texture dialogs.
**	
**	Revision 1.37  2004/06/27 11:36:54  sm
**	- Changed texture dialog for editing negative direction in
**	  contrast to length.
**	- Set document to modified if materials or bumps are changed.
**	- Check for empty textures inside OpenGL subsystem. May this
**	  be ticket no. 21?
**	- Animation values initialization fix.
**	
**	Revision 1.36  2004/06/20 15:33:10  sm
**	- Update material when edited.
**	
**	Revision 1.35  2004/06/06 14:45:57  sm
**	- Added quick material/bump edit support.
**	- Added material to bump copy on wooden materials.
**	
**	Revision 1.34  2004/05/29 13:38:10  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.33  2004/05/19 15:35:03  sm
**	- Hope of having fixed ticket no. 13.
**	
**	Revision 1.32  2004/05/11 14:01:14  sm
**	- Added unified invert/revert for object editing.
**	- Added deletion of transform history in scene
**	  editor (= transformed history) and object editor
**	  (= original form)
**	
**	Revision 1.31  2004/05/11 09:58:25  sm
**	- Added raytraced quick preview for bject editing.
**	
**	Revision 1.30  2004/05/10 15:12:08  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.29  2004/05/07 16:30:33  sm
**	- Bug #13 fixed. The BBox hierarchy is recounted on every
**	  object edit finish.
**	- Wooden materials contain a dark and a separate light
**	  material. Changes were also made in Lines.
**	- Introduced shape property copy including all materials,
**	  bumps and conditions. Multiple copy modes are possible.
**	
**	Revision 1.28  2004/04/10 20:05:12  sm
**	- Introduced bump map editing in Lines III
**	
**	Revision 1.27  2003/07/20 07:48:30  sm
**	- Added legend to object printing
**	
**	Revision 1.26  2003/07/13 12:19:07  sm
**	- Added unit/measurement on object print
**	- Adjusted bhc tool for level scaling
**	
**	Revision 1.25  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
**	Revision 1.24  2003/02/25 15:56:20  sm
**	- Added SplineRot to control grid drawing.
**	- Added support for pixel format selection in dialog items
**	- Restructured b3PickInfo
**	
**	Revision 1.23  2003/02/23 21:15:41  sm
**	- First shape picking
**	
**	Revision 1.22  2003/02/22 15:17:18  sm
**	- Added support for selected shapes in object modeller
**	- Glued b3Shape and b3ShapeRenderObject. There was no
**	  distinct access method in use.
**	- Made some b3Shape methods inline and/or static which
**	  saves some memory.
**	
**	Revision 1.21  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**	
**	Revision 1.20  2003/02/08 14:04:18  sm
**	- Started support for document wise bar state
**	
**	Revision 1.19  2003/01/07 16:14:38  sm
**	- Lines III: object editing didn't prepared any more. Fixed.
**	- Some prepare optimizations.
**	
**	Revision 1.18  2002/08/09 13:20:18  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.17  2002/08/08 15:14:22  sm
**	- Some problems concerning b3Mem::b3Realloc fixed.
**	- Further error messages added.
**	
**	Revision 1.16  2002/08/04 13:24:55  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**	
**	Revision 1.15  2002/02/26 20:43:28  sm
**	- Moved creation dialogs into property sheets
**	- Added material creation dialog
**	
**	Revision 1.14  2002/02/24 17:45:31  sm
**	- Added CSG edit dialogs
**	- Corrected shape edit inheritance.
**	
**	Revision 1.13  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**	Revision 1.12  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**	Revision 1.11  2002/02/01 17:22:44  sm
**	- Added icons for shapes
**	- Added shape support for hierarchy when shape editing
**	
**	Revision 1.10  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.9  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.8  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
**	Revision 1.7  2002/01/16 17:01:08  sm
**	- Some minor fixes done
**	
**	Revision 1.6  2002/01/16 16:17:12  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.5  2002/01/15 16:17:31  sm
**	- Checked OLE support
**	- Checked icons
**	- Added two bwd files which create icons
**	
**	Revision 1.4  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.3  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.2  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.1  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesDoc implementation                   **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppObjectDoc, CAppRenderDoc)

BEGIN_MESSAGE_MAP(CAppObjectDoc, CAppRenderDoc)
	//{{AFX_MSG_MAP(CAppObjectDoc)
	ON_COMMAND(ID_OBJECT_NEW, OnObjectNew)
	ON_COMMAND(ID_ALL_ACTIVATE, OnAllActivate)
	ON_COMMAND(ID_ALL_DEACTIVATE, OnAllDeactivate)
	ON_COMMAND(ID_ALL_DEACTIVATE_REST, OnAllDeactivateRest)
	ON_COMMAND(ID_DEACTIVATE_REST, OnDeactivateRest)
	ON_COMMAND(ID_ACTIVATE, OnActivate)
	ON_COMMAND(ID_DEACTIVATE, OnDeactivate)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_NEW, OnUpdateSelectedItem)
	ON_COMMAND(ID_OBJECT_DELETE, OnObjectDelete)
	ON_COMMAND(ID_OBJECT_EDIT, OnObjectEdit)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_EDIT, OnUpdateObjectEdit)
	ON_COMMAND(ID_EDIT_MATERIAL, OnEditMaterial)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MATERIAL, OnUpdateEditMaterial)
	ON_COMMAND(ID_EDIT_BUMP, OnEditBump)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BUMP, OnUpdateEditBump)
	ON_COMMAND(ID_COPY_PROPERTIES, OnCopyProperties)
	ON_UPDATE_COMMAND_UI(ID_COPY_PROPERTIES, OnUpdateCopyProperties)
	ON_COMMAND(ID_DELETE_TRANSFORM_HISTORY, OnDeleteTransformHistory)
	ON_COMMAND(ID_EDIT_MATERIAL_DIRECT, OnEditMaterialDirect)
	ON_COMMAND(ID_EDIT_BUMP_DIRECT, OnEditBumpDirect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MATERIAL_DIRECT, OnUpdateEditMaterialDirect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BUMP_DIRECT, OnUpdateEditBumpDirect)
	ON_COMMAND(ID_COPY_MATERIAL_TO_BUMP, OnCopyMaterialToBump)
	ON_UPDATE_COMMAND_UI(ID_COPY_MATERIAL_TO_BUMP, OnUpdateCopyMaterialToBump)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_DELETE, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_ALL_DEACTIVATE_REST, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_DEACTIVATE_REST, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_ACTIVATE, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_DEACTIVATE, OnUpdateSelectedItem)
	ON_COMMAND(ID_SPLINE_CONTROL, OnSplineControl)
	ON_UPDATE_COMMAND_UI(ID_SPLINE_CONTROL, OnUpdateSplineControl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAppObjectDoc, CAppRenderDoc)
	//{{AFX_DISPATCH_MAP(CAppObjectDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAppObject to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {592D13F9-C0EC-40d9-9F57-0E7A3C700E44}
static const IID IID_IAppObject =
{ 0x592d13f9, 0xc0ec, 0x40d9, { 0x9f, 0x57, 0xe, 0x7a, 0x3c, 0x70, 0xe, 0x44 } };

BEGIN_INTERFACE_MAP(CAppObjectDoc, CAppRenderDoc)
	INTERFACE_PART(CAppObjectDoc, IID_IAppObject, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc construction/destruction

CAppObjectDoc::CAppObjectDoc()
{
	// TODO: add one-time construction code here
	m_Info     = new b3ModellerInfo(LINES_INFO);
	m_LinesDoc = null;
	m_BBox     = null;
	m_Original = null;
	b3Matrix::b3Unit(&m_OriginalPosition);
	EnableAutomation();

	AfxOleLockApp();
}

CAppObjectDoc::~CAppObjectDoc()
{
	AfxOleUnlockApp();
	delete m_Info;
}

const char *CAppObjectDoc::b3GetDocumentName()
{
	return "_Lines_III_Object_Document";
}

b3Scene *CAppObjectDoc::b3GetParentScene()
{
	return m_LinesDoc != null ? m_LinesDoc->m_Scene : null;
}

b3_u32 CAppObjectDoc::b3GetParentShading()
{
	return (m_LinesDoc != null) && (m_LinesDoc->m_Scene != null) ? m_LinesDoc->m_Scene->b3GetClassType() : TRACEPHOTO_MORK;
}

BOOL CAppObjectDoc::OnNewDocument()
{
	if (!CAppRenderDoc::OnNewDocument())
	{
		return FALSE;
	}
	m_LinesDoc = null;
	m_Original = null;
	m_BBox     = null;

	// TODO: Add your specialized creation code here
	return TRUE;
}

void CAppObjectDoc::b3EditBBox(CAppLinesDoc *LinesDoc,b3BBox *original)
{
	m_Original              = original;
	m_LinesDoc              = LinesDoc;
	m_Info->m_Unit          = LinesDoc->m_Info->m_Unit;
	m_Info->m_Measure       = LinesDoc->m_Info->m_Measure;
	m_Info->m_CustomMeasure = LinesDoc->m_Info->m_CustomMeasure;
	b3SetBBox((b3BBox *)b3World::b3Clone(original));
	SetPathName(m_BBox->b3GetName(),FALSE);
}

void CAppObjectDoc::b3SetBBox(b3BBox *bbox)
{
	CMainFrame     *main = CB3GetMainFrame();
	CString         message;

	main->b3SetStatusMessage(IDS_DOC_REORG);
	b3BBox::b3Recount(
		bbox->b3GetBBoxHead(),
		bbox->b3GetType());

	main->b3SetStatusMessage(IDS_DOC_VERTICES);
	bbox->b3SetupVertexMemory(m_LinesDoc != null ? &m_LinesDoc->m_Context : &m_Context);

	main->b3SetStatusMessage(IDS_DOC_PREPARE);
	bbox->b3PrepareBBox(true);
	bbox->b3Update();
	bbox->b3Inverse(&m_OriginalPosition);

	// Now everything necessary is initialized. So we can mark this
	// document as valid
	B3_ASSERT(m_BBox == null);
	m_BBox = bbox;

	main->b3SetStatusMessage(IDS_DOC_BOUND);
	b3ComputeBounds();

	UpdateAllViews(NULL,B3_UPDATE_ALL|B3_UPDATE_OBJECT);
	m_DlgHierarchy->b3InitTree(this,true);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

BOOL CAppObjectDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	BOOL result = FALSE;
	
	try
	{
		if (strlen(lpszPathName) > 0)
		{
			CMainFrame     *main = CB3GetMainFrame();
			CString         message;
			b3Base<b3Item>  base;
			b3Item         *first;
			b3_count        level;
		
			message.Format(IDS_DOC_READ,lpszPathName);
			main->b3SetStatusMessage(message);

			m_World.b3Read(lpszPathName);
			first  = m_World.b3GetFirst();
			level  = first->b3GetType();
			b3BBox::b3Reorg(m_World.b3GetHead(),&base,level,1);
			b3SetBBox((b3BBox *)first);
		}
		else
		{
			b3PrintF(B3LOG_FULL,"Preparing CAppObjectDoc for scene object editing...\n");
		}
		m_LinesDoc = null;
		result = TRUE;
	}
	catch(b3FileException &f)
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error code %d\n",f.b3GetError());
		B3_MSG_ERROR(f);
	}
	catch(b3WorldException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error code %d\n",e.b3GetError());
		B3_MSG_ERROR(e);
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"UNKNOWN ERROR: Loading %s\n",lpszPathName);
	}
	return result;
}

BOOL CAppObjectDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	return (m_LinesDoc == null ? CAppRenderDoc::SaveModified() : TRUE);
}

BOOL CAppObjectDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL     result = false;
	
	if (m_LinesDoc != null)
	{
		// Give it back to scene!
		result = true;
	}
	else
	{
		// Ordinary save
	}
	return result;
}

void CAppObjectDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWaitCursor waiting_for_good_weather;

	if (m_BBox != null)
	{
		if (m_LinesDoc != null)
		{
			// We have to give back this object
			if (IsModified())
			{
				m_LinesDoc->b3FinishEdit(m_Original,m_BBox);
				m_BBox->b3Reverse(&m_OriginalPosition);
				m_LinesDoc->b3ComputeBounds();
				delete m_Original;
			}
			else
			{
				delete m_BBox;
			}
			m_Original = null;
			m_BBox     = null;
			m_LinesDoc = null;
		}
		else
		{
			// The framework is responsible to save a modified
			// object which was loaded stand alone.
			delete m_BBox;
			m_BBox = null;
		}
	}
	CAppRenderDoc::OnCloseDocument();
}



/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc serialization

void CAppObjectDoc::Serialize(CArchive& ar)
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
void CAppObjectDoc::AssertValid() const
{
	CAppRenderDoc::AssertValid();
}

void CAppObjectDoc::Dump(CDumpContext& dc) const
{
	CAppRenderDoc::Dump(dc);
}
#endif //_DEBUG

/*************************************************************************
**                                                                      **
**                        General scene commands                        **
**                                                                      **
*************************************************************************/

b3RenderContext *CAppObjectDoc::b3GetRenderContext()
{
	return m_LinesDoc != null ? &m_LinesDoc->m_Context : &m_Context;
}

b3_u32 CAppObjectDoc::b3GetSceneType()
{
	return m_LinesDoc->m_Scene->b3GetClassType();
}

void CAppObjectDoc::b3Prepare(
	b3_bool geometry_changed,
	b3_bool structure_changed,
	b3_bool reorg,
	b3_bool material_changed)
{
	CMainFrame      *main    = CB3GetMainFrame();
	b3RenderContext *context = b3GetRenderContext();
	LPARAM           update  = 0;

	if (reorg)
	{
		main->b3SetStatusMessage(IDS_DOC_REORG);
	}

	if (geometry_changed)
	{
		main->b3SetStatusMessage(IDS_DOC_VERTICES);
		m_BBox->b3SetupVertexMemory(context);
	}

	if (geometry_changed || structure_changed || reorg)
	{
		main->b3SetStatusMessage(IDS_DOC_BOUND);
		b3ComputeBounds();
	}

	if (geometry_changed)
	{
		b3PrepareInfo prep;

		main->b3SetStatusMessage(IDS_DOC_PREPARE);

		prep.b3CollectBBoxes(m_BBox);
		prep.b3Prepare(b3Scene::b3PrepareBBoxThread);
	}

	if (geometry_changed || structure_changed)
	{
		SetModifiedFlag();
		update |= B3_UPDATE_GEOMETRY;
	}

	if (material_changed)
	{
		main->b3SetStatusMessage(IDS_DOC_UPDATE_MATERIAL);
		m_BBox->b3UpdateMaterial();
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

	b3PrintF(B3LOG_DEBUG,"# %d vertices\n", context->glVertexCount);
	b3PrintF(B3LOG_DEBUG,"# %d triangles\n",context->glPolyCount);
	b3PrintF(B3LOG_DEBUG,"# %d lines\n",    context->glGridCount);
	main->b3SetStatusMessage(AFX_IDS_IDLEMESSAGE);
}

void CAppObjectDoc::b3ComputeBounds()
{
	b3Vector::b3InitBound(&m_Lower,&m_Upper);
	m_BBox->b3ComputeBounds(&m_Lower,&m_Upper,0.02);
}

void CAppObjectDoc::b3InitTree()
{
	if (m_BBox != null)
	{
		B3_ASSERT((m_BBox->Prev == null) && (m_BBox->Succ == null));
		m_DlgHierarchy->b3AddBBoxes (null,m_BBox,true);
	}
}

b3Shape *CAppObjectDoc::b3GetSelectedShape()
{
	return m_DlgHierarchy->b3GetSelectedShape();
}

void CAppObjectDoc::b3HierarchySelectionChanged()
{
	b3Shape *shape = b3GetSelectedShape();
	m_Context.b3SetSelected(shape);
	UpdateAllViews(NULL,B3_UPDATE_VIEW|B3_UPDATE_PICK);
}

void CAppObjectDoc::b3ContextMenu(HTREEITEM item)
{
	CMenu          menu;
	CMenu         *submenu;
	CPoint point;

	if (menu.LoadMenu(IDR_CONTEXT_OBJECT))
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

void CAppObjectDoc::b3DropBBox(b3BBox *srcBBox,b3BBox *dstBBox)
{
	b3Base<b3Item> *srcBase;
	b3Base<b3Item> *dstBase;

	srcBase = m_BBox->b3FindBBoxHead(srcBBox);
	dstBase = dstBBox->b3GetBBoxHead();

	srcBase->b3Remove(srcBBox);
	dstBase->b3Append(srcBBox);
	b3BBox::b3Recount(
		m_BBox->b3GetBBoxHead(),
		m_BBox->b3GetType());
}

b3_bool CAppObjectDoc::b3IsLinesDoc(CAppLinesDoc *LinesDoc)
{
	return m_LinesDoc == LinesDoc;
}

b3_bool CAppObjectDoc::b3IsObjectAlreadyOpen(
	CAppLinesDoc *LinesDoc,
	b3BBox       *bbox)
{
	b3_bool is_open = false;

	if (m_Original != null)
	{
		is_open = ((bbox == m_Original) ||
			(b3BBox::b3FindBBox(m_Original->b3GetBBoxHead(),bbox)) ||
			(b3BBox::b3FindBBox(bbox->b3GetBBoxHead(),m_Original)));
	}
	return is_open;
}

void CAppObjectDoc::OnUpdateSelectedItem(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_DlgHierarchy->b3GetSelectedItem() != null);
}

/*************************************************************************
**                                                                      **
**                        Object maintainance                           **
**                                                                      **
*************************************************************************/

void CAppObjectDoc::OnObjectNew() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpShapeCreate(m_BBox, this, m_DlgHierarchy));
}

void CAppObjectDoc::OnObjectEdit() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpShapeEdit(m_BBox, this, m_DlgHierarchy));
}

void CAppObjectDoc::OnObjectDelete() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpShapeDelete(m_BBox, this, m_DlgHierarchy));
}

void CAppObjectDoc::OnUpdateObjectEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Item *selected = m_DlgHierarchy->b3GetSelectedItem();
	b3ItemEditCall  call = null;

	if (selected != null)
	{
		call = CB3ImageList::b3GetEditCall(selected);
	}
	pCmdUI->Enable(call != null);
}

/*************************************************************************
**                                                                      **
**                        Object activation                             **
**                                                                      **
*************************************************************************/

void CAppObjectDoc::b3ActivateItem(
	b3Item  *item,
	b3_bool  activate)
{
	b3BBox  *bbox;
	b3Shape *shape;

	switch (item->b3GetClass())
	{
	case CLASS_BBOX:
		bbox = (b3BBox *)item;
		bbox->b3Activate(activate);
		break;
	case CLASS_SHAPE:
	case CLASS_CSG:
		shape = (b3Shape *)item;
		shape->b3Activate(activate);
		break;
	}
}

void CAppObjectDoc::OnAllActivate() 
{
	// TODO: Add your command handler code here
	m_BBox->b3Activate();
	m_DlgHierarchy->b3UpdateActivation();

	SetModifiedFlag();
	UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CAppObjectDoc::OnAllDeactivate() 
{
	// TODO: Add your command handler code here
	m_BBox->b3Activate(false);
	m_DlgHierarchy->b3UpdateActivation();

	SetModifiedFlag();
	UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CAppObjectDoc::OnAllDeactivateRest() 
{
	// TODO: Add your command handler code here
	b3Item *item;

	m_BBox->b3Activate(false);
	item = m_DlgHierarchy->b3GetSelectedItem();
	if (item != null)
	{
		b3ActivateItem(item);
	}
	m_DlgHierarchy->b3UpdateActivation();

	SetModifiedFlag();
	UpdateAllViews(null,B3_UPDATE_VIEW);
}

void CAppObjectDoc::OnDeactivateRest() 
{
	// TODO: Add your command handler code here
	b3Item *item = m_DlgHierarchy->b3GetSelectedItem();
	b3Item *aux;

	if (item != null)
	{
		b3ActivateItem(item);
		for (aux = item->Prev;aux != null;aux = aux->Prev)
		{
			b3ActivateItem(item,false);
		}
		for (aux = item->Succ;aux != null;aux = aux->Succ)
		{
			b3ActivateItem(item,false);
		}
		m_DlgHierarchy->b3UpdateActivation();

		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

void CAppObjectDoc::OnActivate() 
{
	// TODO: Add your command handler code here
	b3Item *item = m_DlgHierarchy->b3GetSelectedItem();

	if (item != null)
	{
		b3ActivateItem(item);
		m_DlgHierarchy->b3UpdateActivation();
		if (item->Succ != null)
		{
			m_DlgHierarchy->b3SelectItem(item->Succ);
		}

		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

void CAppObjectDoc::OnDeactivate() 
{
	// TODO: Add your command handler code here
	b3Item *item = m_DlgHierarchy->b3GetSelectedItem();

	if (item != null)
	{
		b3ActivateItem(item,false);
		m_DlgHierarchy->b3UpdateActivation();
		if (item->Succ != null)
		{
			m_DlgHierarchy->b3SelectItem(item->Succ);
		}

		SetModifiedFlag();
		UpdateAllViews(null,B3_UPDATE_VIEW);
	}
}

/*************************************************************************
**                                                                      **
**                        Material maintainance                         **
**                                                                      **
*************************************************************************/

void CAppObjectDoc::b3UpdateSurface(b3Shape *shape)
{
	if (shape != null)
	{
		SetModifiedFlag();
		shape->b3Prepare();
		shape->b3RecomputeMaterial();
		shape->b3UpdateMaterial();
		UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
	}
}

void CAppObjectDoc::OnEditMaterial() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpShapeMaterialEditList(m_BBox, this, m_DlgHierarchy));
}

void CAppObjectDoc::OnEditMaterialDirect() 
{
	// TODO: Add your command handler code here
	b3Shape *shape = b3GetSelectedShape();
	
	if (shape != null)
	{
		if (shape->b3GetMaterialHead()->b3GetCount() == 1)
		{
			b3AddOp(new b3OpShapeMaterialEdit(m_BBox, shape, this, m_DlgHierarchy));
		}
		else
		{
			b3AddOp(new b3OpShapeMaterialEditList(m_BBox, this, m_DlgHierarchy));
		}
	}
}

b3_bool CAppObjectDoc::b3CopyMaterialToBump()
{
	b3Shape *shape = b3GetSelectedShape();
	b3Item  *src;
	b3Item  *dst;
	b3_bool  changed = false;

	if (shape != null)
	{
		shape->b3GetBumpHead()->b3Free();
		B3_FOR_BASE(shape->b3GetMaterialHead(),src)
		{
			switch(src->b3GetClassType())
			{
			case TEXTURE:
				dst = b3World::b3AllocNode(BUMP_TEXTURE);
				if (dst != null)
				{
					b3MatTexture  *material = (b3MatTexture *)src;
					b3BumpTexture *bump     = (b3BumpTexture *)dst;
					
					bump->m_xStart = material->m_xStart;
					bump->m_yStart = material->m_yStart;
					bump->m_xScale = material->m_xScale;
					bump->m_yScale = material->m_yScale;
					bump->m_xTimes = material->m_xTimes;
					bump->m_yTimes = material->m_yTimes;
					bump->m_Flags  = material->m_Flags;
					strcpy(bump->m_Name,material->m_Name);

					changed = true;
				}
				break;

			case MARBLE:
				dst = b3World::b3AllocNode(BUMP_MARBLE);
				if (dst != null)
				{
					b3MatMarble  *material   = (b3MatMarble *)src;
					b3BumpMarble *bump       = (b3BumpMarble *)dst;
					b3Scaling    *srcScaling = material;
					b3Scaling    *dstScaling = bump;

					*dstScaling  = *srcScaling;

					changed = true;
				}
				break;

			case WOOD:
				dst = b3World::b3AllocNode(BUMP_WOOD);
				if (dst != null)
				{
					b3MatWood  *material   = (b3MatWood *)src;
					b3BumpWood *bump       = (b3BumpWood *)dst;
					b3Wood     *srcWood    = material;
					b3Scaling  *srcScaling = material;
					b3Wood     *dstWood    = bump;
					b3Scaling  *dstScaling = bump;

					*dstWood     = *srcWood;
					*dstScaling  = *srcScaling;

					changed = true;
				}
				break;

			case OAKPLANK:
				dst = b3World::b3AllocNode(BUMP_OAKPLANK);
				if (dst != null)
				{
					b3MatOakPlank  *material    = (b3MatOakPlank *)src;
					b3BumpOakPlank *bump        = (b3BumpOakPlank *)dst;
					b3Wood         *srcWood     = material;
					b3OakPlank     *srcOakPlank = material;
					b3Scaling      *srcScaling  = material;
					b3Wood         *dstWood     = bump;
					b3OakPlank     *dstOakPlank = bump;
					b3Scaling      *dstScaling  = bump;

					*dstWood     = *srcWood;
					*dstOakPlank = *srcOakPlank;
					*dstScaling  = *srcScaling;

					changed = true;
				}
				break;

			default:
				dst = null;
				break;
			}

			if (dst != null)
			{
				shape->b3GetBumpHead()->b3Append(dst);
			}
		}
	}
	return changed;
}

void CAppObjectDoc::OnCopyMaterialToBump() 
{
	// TODO: Add your command handler code here
	if (b3CopyMaterialToBump())
	{
		b3UpdateSurface(b3GetSelectedShape());
	}
}

void CAppObjectDoc::OnUpdateEditMaterial(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Shape *shape = m_DlgHierarchy->b3GetSelectedShape();

	pCmdUI->Enable(shape != null);
}

void CAppObjectDoc::OnUpdateEditMaterialDirect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Shape *shape = m_DlgHierarchy->b3GetSelectedShape();

	pCmdUI->Enable(shape != null);
}

void CAppObjectDoc::OnUpdateCopyMaterialToBump(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Shape *shape = b3GetSelectedShape();
	b3Item  *item;
	b3_bool  enabled = false;

	if (shape != null)
	{
		B3_FOR_BASE (shape->b3GetMaterialHead(),item)
		{
			switch(item->b3GetClassType())
			{
			case TEXTURE:
			case MARBLE:
			case WOOD:
			case OAKPLANK:
				enabled = true;
				break;
			}
		}
	}
	pCmdUI->Enable(enabled);
}

/*************************************************************************
**                                                                      **
**                        Bump map maintainance                         **
**                                                                      **
*************************************************************************/

void CAppObjectDoc::OnEditBump() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpShapeBumpEditList(m_BBox, this, m_DlgHierarchy));
}

void CAppObjectDoc::OnEditBumpDirect() 
{
	// TODO: Add your command handler code here
	b3Shape *shape = b3GetSelectedShape();
	
	if (shape != null)
	{
		if (shape->b3GetMaterialHead()->b3GetCount() == 1)
		{
			b3AddOp(new b3OpShapeBumpEdit(m_BBox, shape, this, m_DlgHierarchy));
		}
		else
		{
			b3AddOp(new b3OpShapeBumpEditList(m_BBox, this, m_DlgHierarchy));
		}
	}
}

void CAppObjectDoc::OnUpdateEditBump(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Shape *shape = m_DlgHierarchy->b3GetSelectedShape();

	pCmdUI->Enable(shape != null);
}

void CAppObjectDoc::OnUpdateEditBumpDirect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Shape *shape = m_DlgHierarchy->b3GetSelectedShape();

	pCmdUI->Enable(shape != null);
}

/*************************************************************************
**                                                                      **
**                        Surface properties maintainance               **
**                                                                      **
*************************************************************************/

void CAppObjectDoc::OnCopyProperties() 
{
	// TODO: Add your command handler code here
	b3AddOp(new b3OpShapeCopySurface(m_BBox, this, m_DlgHierarchy));
}

void CAppObjectDoc::OnUpdateCopyProperties(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Shape *shape = m_DlgHierarchy->b3GetSelectedShape();

	pCmdUI->Enable(shape != null);
}

void CAppObjectDoc::OnDeleteTransformHistory() 
{
	// TODO: Add your command handler code here
	if (b3Runtime::b3MessageBox(IDS_ASK_DELETE_TRANSFORMATION_HISTORY,B3_MSGBOX_YES_NO) == B3_MSG_YES)
	{
		b3Matrix::b3Unit(&m_OriginalPosition);
		m_BBox->b3ResetTransformation();
		SetModifiedFlag();
	}
}

b3Item *CAppObjectDoc::b3FindItem(b3Base<b3Item> *head,b3_u32 class_type)
{
	b3Item *item;

	B3_FOR_BASE(head,item)
	{
		if (item->b3GetClassType() == class_type)
		{
			return item;
		}
	}
	return null;
}

b3Item *CAppObjectDoc::b3EnsureSingleItem(b3Base<b3Item> *head,b3_u32 class_type)
{
	b3Item *item = b3FindItem(head,class_type);

	if (item != null)
	{
		head->b3Remove(item);
	}
	else
	{
		item = b3World::b3AllocNode(class_type);
	}
	if (item != null)
	{
		head->b3Free();
		head->b3Append(item);
	}
	return item;
}

b3SplineShape *CAppObjectDoc::b3GetSplineShape()
{
	b3SplineShape *spline_shape = null;
	b3Shape       *shape = b3GetSelectedShape();

	if (shape != null)
	{
		b3_u32 class_type = shape->b3GetClassType();

		if ((class_type == SPLINES_AREA) ||
			(class_type == SPLINES_CYL) ||
			(class_type == SPLINES_RING))
		{
			spline_shape = (b3SplineShape *)shape;
		}
	}
	return spline_shape;
}

void CAppObjectDoc::OnSplineControl() 
{
	// TODO: Add your command handler code here
	b3SplineShape *spline_shape = b3GetSplineShape();

	if (spline_shape != null)
	{
		CDlgKnotControl dlg(&spline_shape->m_Spline[0]);

		dlg.DoModal();
	}
}

void CAppObjectDoc::OnUpdateSplineControl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(b3GetSplineShape() != null);
}
