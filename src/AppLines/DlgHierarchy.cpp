/*
**
**	$Filename:	DlgHierarchy.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Object (de-)activating
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "AppLines.h"
#include "AppLinesView.h"
#include "DlgHierarchy.h"
#include "blz3/raytrace/b3BBox.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.25  2004/10/16 17:00:52  sm
**	- Moved lighting into own class to ensure light setup
**	  after view setup.
**	- Fixed lighting for scene and simple overview
**	- Fixed Light cutoff exponent deadloop.
**	- Corrected OpenGL define (BLZ3_USE_OPENGL)
**
**	Revision 1.24  2004/04/22 20:23:55  sm
**	- Fixed wrong ON_MESSAGE signature on call function.
**	- Reordered context menu of object editor.
**	- Expand root item of tree view in object editor
**	
**	Revision 1.23  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.22  2003/02/22 15:17:18  sm
**	- Added support for selected shapes in object modeller
**	- Glued b3Shape and b3ShapeRenderObject. There was no
**	  distinct access method in use.
**	- Made some b3Shape methods inline and/or static which
**	  saves some memory.
**	
**	Revision 1.21  2003/01/12 19:21:37  sm
**	- Some other undo/redo actions added (camera etc.)
**	
**	Revision 1.20  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.19  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**	Revision 1.18  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**	Revision 1.17  2002/02/18 17:50:31  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.16  2002/02/12 18:39:03  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.15  2002/02/10 20:03:18  sm
**	- Added grid raster
**	- Changed icon colors of shapes
**	
**	Revision 1.14  2002/02/01 17:22:44  sm
**	- Added icons for shapes
**	- Added shape support for hierarchy when shape editing
**	
**	Revision 1.13  2002/02/01 15:04:09  sm
**	- Prepared shapes for icon conversion
**	- Added to save selected/first visible item in
**	  hierarchy dialog.
**	- Some print cleanups done.
**	- Fixed activation of b3SuperSample.
**	
**	Revision 1.12  2002/01/24 15:55:58  sm
**	- Fixed key handling on TreeCtrl (hierarchy dialog bar)
**	- Added support for conext menu depending on scene/object edit.
**	
**	Revision 1.11  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.10  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**	Revision 1.9  2002/01/07 16:18:51  sm
**	- Added b3Item clone
**	- Added Drag & Drop
**	
**	Revision 1.8  2002/01/04 17:53:53  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.7  2002/01/02 17:05:19  sm
**	- Minor bug fixes done: recurse the CDlgHierarchy::b3FindBBox() correctly
**	
**	Revision 1.6  2002/01/02 15:48:37  sm
**	- Added automated expand/collapse to hierarchy tree.
**	
**	Revision 1.5  2001/12/31 16:39:40  sm
**	- Made hierarchy dialog a CDialogBar
**	
**	Revision 1.4  2001/11/11 15:09:56  sm
**	- Introduced scene properties for:
**	  o scene itself (done)
**	  o distributed raytracing (controls layouted)
**	  o super sampling (controls layouted)
**	  o nebular (controls layouted)
**	  o lens flares (controls layouted)
**	
**	Revision 1.3  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgHierarchy implementation                  **
**                                                                      **
*************************************************************************/

CDlgHierarchy::CDlgHierarchy(CWnd* pParent /*=NULL*/)
	: CB3Dialogbar()
{
	//{{AFX_DATA_INIT(CDlgHierarchy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc     = null;
	m_DragItem = null;
	m_DropItem = null;
}

CDlgHierarchy::~CDlgHierarchy()
{
}


void CDlgHierarchy::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialogbar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHierarchy)
	DDX_Control(pDX, IDC_HIERARCHY, m_Hierarchy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHierarchy, CB3Dialogbar)
	//{{AFX_MSG_MAP(CDlgHierarchy)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_HIERARCHY, OnEndLabelEditHierarchy)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_HIERARCHY, OnBeginlabeleditHierarchy)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_HIERARCHY, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_RCLICK, IDC_HIERARCHY, OnContextMenu)
	ON_NOTIFY(TVN_SELCHANGED, IDC_HIERARCHY, OnHierarchySelectionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHierarchy message handlers

BOOL CDlgHierarchy::OnInitDialog() 
{
	CB3Dialogbar::OnInitDialog();
	m_ImageList.b3Create();
	m_Hierarchy.SetImageList (&m_ImageList,TVSIL_NORMAL);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHierarchy::b3AddBBoxes (
	HTREEITEM  parent,
	b3BBox    *BBox,
	b3_bool    AddShapes)
{
	TV_INSERTSTRUCT  insert;
	HTREEITEM        new_treeitem;
	CString          shape_title;
	b3Item          *item;
	b3BBox          *sub;
	b3Shape         *shape;
	long             imgNum;

	if (parent == null)
	{
		parent = TVI_ROOT;
	}
	while (BBox != null)
	{
		imgNum = CB3ImageList::b3ComputeImgNum(BBox);
		insert.hParent      = parent;
		insert.hInsertAfter = TVI_LAST;
		insert.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insert.item.pszText        = BBox->m_BoxName;
		insert.item.cchTextMax     = B3_BOXSTRINGLEN;
		insert.item.lParam         = (long)BBox;
		insert.item.iImage         = imgNum;
		insert.item.iSelectedImage = imgNum;

		new_treeitem = m_Hierarchy.InsertItem (&insert);
		sub = (b3BBox *)BBox->b3GetBBoxHead()->First;
		if (sub != null)
		{
			b3AddBBoxes (new_treeitem,sub,AddShapes);
			if (BBox->b3IsExpanded())
			{
				m_Hierarchy.Expand(new_treeitem,TVE_EXPAND);
			}
		}

		if (AddShapes)
		{
			B3_FOR_BASE(BBox->b3GetShapeHead(),item)
			{
				shape = (b3Shape *)item;

				imgNum = CB3ImageList::b3ComputeText(shape,shape_title);
				insert.hParent      = new_treeitem;
				insert.hInsertAfter = TVI_LAST;
				insert.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				insert.item.pszText        = (char *)((const char *)shape_title);
				insert.item.cchTextMax     = B3_BOXSTRINGLEN;
				insert.item.lParam         = (long)shape;
				insert.item.iImage         = imgNum;
				insert.item.iSelectedImage = imgNum;
				m_Hierarchy.InsertItem (&insert);
			}
			if (parent == TVI_ROOT)
			{
				m_Hierarchy.Expand(new_treeitem,TVE_EXPAND);
			}
		}

		BBox = (b3BBox *)BBox->Succ;
	}
}

b3_bool CDlgHierarchy::b3InitTree(CAppRenderDoc *pDoc,b3_bool force_refresh)
{
#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL,"CDlgHierarchy::b3InitTree(%p,%s) [%p]\n",
		pDoc,force_refresh ? "TRUE" : "FALSE",m_pDoc);
#endif
	
	if ((m_pDoc == pDoc) && (!force_refresh))
	{
		return false;
	}
	m_pDoc  = pDoc;
	m_Hierarchy.DeleteAllItems();
	b3Free();
	if (m_pDoc != null)
	{
		HTREEITEM root;
		m_pDoc->b3InitTree();

		root = m_Hierarchy.GetRootItem();
		if (m_pDoc->m_Selected != null)
		{
			m_Hierarchy.SelectItem(m_Hierarchy.b3FindLParam(root,m_pDoc->m_Selected));
		}
		if (m_pDoc->m_FirstVisible != null)
		{
			m_Hierarchy.SelectSetFirstVisible(m_Hierarchy.b3FindLParam(root,m_pDoc->m_FirstVisible));
		}
	}

	return true;
}

void CDlgHierarchy::b3UpdateIcons(HTREEITEM parent)
{
	HTREEITEM item;
	TVITEM    info;
	long      imgnum;

	memset(&info,0,sizeof(info));
	info.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	for(item  = m_Hierarchy.GetNextItem(parent,TVGN_CHILD);
	    item != NULL;
		item  = m_Hierarchy.GetNextItem(item,TVGN_NEXT))
	{
		imgnum = CB3ImageList::b3ComputeImgNum((b3Item *)m_Hierarchy.GetItemData(item));
		info.hItem          = item;
		info.iImage         = imgnum;
		info.iSelectedImage = imgnum;
		m_Hierarchy.SetItem(&info);

		b3UpdateIcons(item);
	}
}

void CDlgHierarchy::b3UpdateActivation()
{
	HTREEITEM root = m_Hierarchy.GetRootItem();
	
	if (root != null)
	{
		b3UpdateIcons(root);
	}
}

HTREEITEM CDlgHierarchy::b3FindItem(HTREEITEM parent,b3Item *item)
{
	return m_Hierarchy.b3FindLParam(parent,(LPARAM)item);
}

b3_count CDlgHierarchy::b3GetExpansion(HTREEITEM parent)
{
	HTREEITEM  item;
	b3BBox    *BBox;
	b3_count   index = 0;

	for(item  = m_Hierarchy.GetNextItem(parent,TVGN_CHILD);
	    item != NULL;
		item  = m_Hierarchy.GetNextItem(item,TVGN_NEXT))
	{
		index += b3GetExpansion(item);
		index++;
		BBox = (b3BBox *)m_Hierarchy.GetItemData(item);
		if (BBox != null)
		{
			BBox->b3Expand(m_Hierarchy.GetItemState(item,0) & TVIS_EXPANDED);
		}
	}
	return index;
}

LPARAM CDlgHierarchy::b3GetLParam(HTREEITEM item)
{
	return item != null ? m_Hierarchy.GetItemData(item) : null;
}

void CDlgHierarchy::b3GetData()
{
	if (m_pDoc == null)
	{
		m_Hierarchy.DeleteAllItems();
		b3Free();
	}
	else
	{
		HTREEITEM root = m_Hierarchy.GetRootItem();

		if (root != null)
		{
			b3GetExpansion(root);
			m_pDoc->m_FirstVisible = b3GetLParam(m_Hierarchy.GetFirstVisibleItem());
			m_pDoc->m_Selected     = b3GetLParam(m_Hierarchy.GetSelectedItem());
		}
		else
		{
			m_pDoc->m_FirstVisible = null;
			m_pDoc->m_Selected     = null;
		}
	}
}

void CDlgHierarchy::b3SetData()
{
}

HTREEITEM CDlgHierarchy::b3SelectItem(b3Item *item)
{
	HTREEITEM treeitem;

	treeitem = b3FindItem(m_Hierarchy.GetRootItem(),item);
	if (treeitem != null)
	{
		m_Hierarchy.SelectItem(treeitem);
	}

	if (m_pDoc != null)
	{
		m_pDoc->m_Selected = (LPARAM)item;
	}
	return treeitem;
}

b3Item *CDlgHierarchy::b3GetSelectedItem()
{
	HTREEITEM  treeitem = m_Hierarchy.GetSelectedItem();
	b3Item    *item = null;

	if (treeitem != NULL)
	{
		item = (b3Item *)m_Hierarchy.GetItemData(treeitem);
	}
	return item;
}

b3BBox *CDlgHierarchy::b3GetSelectedBBox()
{
	HTREEITEM  treeitem = m_Hierarchy.GetSelectedItem();
	b3Item    *item = null;

	if (treeitem != NULL)
	{
		item = (b3Item *)m_Hierarchy.GetItemData(treeitem);
		if (item != null)
		{
			if (item->b3GetClass() != CLASS_BBOX)
			{
				item = null;
			}
		}
	}
	return (b3BBox *)item;
}

b3Shape *CDlgHierarchy::b3GetSelectedShape()
{
	HTREEITEM  treeitem = m_Hierarchy.GetSelectedItem();
	b3Item    *item = null;

	if (treeitem != NULL)
	{
		item = (b3Item *)m_Hierarchy.GetItemData(treeitem);
		if (item != null)
		{
			b3_u32 item_class = item->b3GetClass();

			if ((item_class != CLASS_SHAPE) && (item_class != CLASS_CSG))
			{
				item = null;
			}
		}
	}
	return (b3Shape *)item;
}

void CDlgHierarchy::OnBeginlabeleditHierarchy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO *pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	b3Item      *item;

	// TODO: Add your control notification handler code here
	*pResult = 1; // Mark as uneditable by default
	item = (b3Item *)pTVDispInfo->item.lParam;
	if (item != null)
	{
		if (item->b3GetClass() == CLASS_BBOX)
		{
			m_Hierarchy.GetEditControl()->LimitText(B3_BOXSTRINGLEN);
			*pResult = 0;
		}
	}
}

void CDlgHierarchy::OnEndLabelEditHierarchy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO *pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	b3BBox      *BBox;

	// TODO: Add your control notification handler code here
	
	BBox = (b3BBox *)pTVDispInfo->item.lParam;
	if ((BBox != null) && (pTVDispInfo->item.pszText != null))
	{
		BBox->b3SetName(pTVDispInfo->item.pszText);
		
		m_pDoc->SetModifiedFlag();
		m_pDoc->UpdateAllViews(null,B3_UPDATE_VIEW);
		*pResult = 1;
	}
	else
	{
		*pResult = 0;
	}
}

void CDlgHierarchy::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if ((pNMTreeView->itemNew.hItem != null) && (pNMTreeView->itemNew.lParam))
	{
		m_DragItem = pNMTreeView->itemNew.hItem;
		SetCapture();
		m_pDoc->b3DragBegin();
	}
	*pResult = (m_DragItem != null);
}

void CDlgHierarchy::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_DragItem != null)
	{
		UINT      flags;
		HTREEITEM item;

		item = m_Hierarchy.HitTest(point,&flags);
		if (item != null)
		{
			m_DropItem = m_pDoc->b3Dragging(m_DragItem,item);
			m_Hierarchy.SelectDropTarget(m_DropItem);
		}
	}
	CB3Dialogbar::OnMouseMove(nFlags, point);
}

void CDlgHierarchy::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_DragItem != null)
	{
		::ReleaseCapture();
		m_Hierarchy.SelectDropTarget(NULL);
		if (m_DropItem != null)
		{
			b3GetData();

			m_pDoc->b3Drop(m_DragItem,m_DropItem);
		}
		m_DragItem = null;
	}
	CB3Dialogbar::OnLButtonUp(nFlags, point);
}

void CDlgHierarchy::OnContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point;

	if ((m_pDoc != null) && GetCursorPos(&point))
	{
		m_Hierarchy.ScreenToClient(&point);
		m_pDoc->b3ContextMenu(m_Hierarchy.HitTest(point));
	}
	*pResult = 0;
}

void CDlgHierarchy::OnHierarchySelectionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_pDoc != null)
	{
		m_pDoc->b3HierarchySelectionChanged();
	}
	*pResult = 0;
}
