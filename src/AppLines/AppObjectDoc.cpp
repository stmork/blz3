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

#include "DlgNewObject.h"

#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	ON_UPDATE_COMMAND_UI(ID_OBJECT_DELETE, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_ALL_DEACTIVATE_REST, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_DEACTIVATE_REST, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_ACTIVATE, OnUpdateSelectedItem)
	ON_UPDATE_COMMAND_UI(ID_DEACTIVATE, OnUpdateSelectedItem)
	ON_COMMAND(ID_OBJECT_EDIT, OnObjectEdit)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_EDIT, OnUpdateObjectEdit)
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

// {72D6951A-8984-11D5-A54F-0050BF4EB3F3}
static const IID IID_IAppObject =
{ 0x72d6951a, 0x8984, 0x11d5, { 0xa5, 0x4f, 0x0, 0x50, 0xbf, 0x4e, 0xb3, 0xf3 } };

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
	b3MatrixUnit(&m_OriginalPosition);
	EnableAutomation();

	AfxOleLockApp();
}

CAppObjectDoc::~CAppObjectDoc()
{
	AfxOleUnlockApp();
	delete m_Info;
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
	m_Original = original;
	m_LinesDoc = LinesDoc;
	b3SetBBox((b3BBox *)b3World::b3Clone(original));
	SetPathName(m_BBox->b3GetName(),FALSE);
}

void CAppObjectDoc::b3SetBBox(b3BBox *bbox)
{
	CMainFrame     *main = CB3GetMainFrame();
	CString         message;
	b3_matrix       inverse;

	main->b3SetStatusMessage(IDS_DOC_REORG);
	b3BBox::b3Recount(
		bbox->b3GetBBoxHead(),
		bbox->b3GetClassType() & 0xffff);

	main->b3SetStatusMessage(IDS_DOC_PREPARE);
	bbox->b3Prepare();

	main->b3SetStatusMessage(IDS_DOC_VERTICES);
	bbox->b3AllocVertices(m_LinesDoc != null ? &m_LinesDoc->m_Context : &m_Context);

	m_OriginalPosition = bbox->m_Matrix;
	b3MatrixInv(&bbox->m_Matrix,&inverse);
	bbox->b3Transform(&inverse,true,true);

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
			level  = first->b3GetClassType() & 0xffff;
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
	catch(b3FileException *f)
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error code %d\n",f->b3GetError());
		B3_MSG_ERROR(f);
	}
	catch(b3WorldException *e)
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error loading %s\n",lpszPathName);
		b3PrintF(B3LOG_NORMAL,"Blizzard III Object loader: Error code %d\n",e->b3GetError());
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
				m_BBox->b3Transform(&m_OriginalPosition,true,true);
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

void CAppObjectDoc::b3ComputeBounds()
{
	m_Lower.x =  FLT_MAX;
	m_Lower.y =  FLT_MAX;
	m_Lower.z =  FLT_MAX;
	m_Upper.x = -FLT_MAX;
	m_Upper.y = -FLT_MAX;
	m_Upper.z = -FLT_MAX;

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

void CAppObjectDoc::b3ContextMenu(HTREEITEM item)
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
		m_BBox->b3GetClassType() & 0xffff);
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

void CAppObjectDoc::OnObjectEdit() 
{
	// TODO: Add your command handler code here
	b3Item         *selected = m_DlgHierarchy->b3GetSelectedItem();
	b3ItemEditCall  call;

	if (selected != null)
	{
		call = CB3ImageList::b3GetEditCall(selected);
		if (call != null)
		{
			if (call(selected,false) == IDOK)
			{
				// Init data
				m_DlgHierarchy->b3GetData();
				m_BBox->b3BacktraceRecompute(m_BBox->b3FindParentBBox((b3Shape *)selected));
				b3ComputeBounds();

				SetModifiedFlag();
				UpdateAllViews(null,B3_UPDATE_GEOMETRY);
				m_DlgHierarchy->b3InitTree(this,true);
				m_DlgHierarchy->b3SelectItem(selected);
			}
		}
	}
}

void CAppObjectDoc::OnObjectNew() 
{
	// TODO: Add your command handler code here
	b3BBox         *bbox;
	b3Shape        *shape;
	b3ItemEditCall  call;
	CDlgNewObject   dlg;
	int             result;

	shape = m_DlgHierarchy->b3GetSelectedShape();
	bbox  = (shape != null ?
		m_BBox->b3FindParentBBox(shape) :
		m_DlgHierarchy->b3GetSelectedBBox());
	if (bbox != null)
	{
		dlg.m_InsertAfter = shape;
		dlg.m_BBox        = bbox;
		if ((dlg.DoModal() == IDOK) && (dlg.m_NewItem != null))
		{
			// Open edit dialog if available
			call   = CB3ImageList::b3GetEditCall(dlg.m_NewItem);
			result = (call != null ? call(dlg.m_NewItem,true) : IDOK);
			if (result == IDOK)
			{
				// Manually insert to prevent uninitialized redraw
				// of new item.
				dlg.m_Base->b3Insert(dlg.m_InsertAfter,dlg.m_NewItem);

				// Init data
				m_DlgHierarchy->b3GetData();
				m_BBox->b3Prepare();
				m_BBox->b3AllocVertices(m_LinesDoc != null ? &m_LinesDoc->m_Context : &m_Context);
				m_BBox->b3BacktraceRecompute(bbox);
				b3ComputeBounds();

				SetModifiedFlag();
				UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
				m_DlgHierarchy->b3InitTree(this,true);
				m_DlgHierarchy->b3SelectItem(dlg.m_NewItem);
			}
			else
			{
				delete dlg.m_NewItem;
			}
		}
	}
}

void CAppObjectDoc::OnObjectDelete() 
{
	// TODO: Add your command handler code here
	CMainFrame     *main    = CB3GetMainFrame();
	b3Base<b3Item> *base;
	b3BBox         *parent;
	b3Item         *select;
	b3Item         *selected;
	int             answere;

	selected = m_DlgHierarchy->b3GetSelectedItem();
	if (selected != null)
	{
		// Security question depending on item type
		switch(selected->b3GetClass())
		{
		case CLASS_BBOX:
			answere = AfxMessageBox(IDS_ASK_DELETE_OBJECT,MB_ICONQUESTION|MB_YESNO);
			base    = m_BBox->b3FindBBoxHead((b3BBox *)selected);
			m_BBox->b3BacktraceRecompute((b3BBox *)selected);
			break;

		case CLASS_SHAPE:
		case CLASS_CSG:
			answere = AfxMessageBox(IDS_ASK_DELETE_SHAPE,MB_ICONQUESTION|MB_YESNO);
			parent  = m_BBox->b3FindParentBBox((b3Shape *)selected);
			base    = parent->b3GetShapeHead();
			m_BBox->b3BacktraceRecompute(parent);
			break;

		default:
			answere = IDCANCEL;
			base    = null;
			break;
		}

		// If the user really wants to delete item...
		if(answere == IDYES)
		{
			B3_ASSERT(base != null);
			select   = (selected->Succ != null ? selected->Succ : selected->Prev);

			main->b3SetStatusMessage(IDS_DOC_BOUND);
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
