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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	m_pDoc  = null;
	m_Scene = null;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHierarchy message handlers

BOOL CDlgHierarchy::OnInitDialog() 
{
	CB3Dialogbar::OnInitDialog();
	
	m_ImageList.Create(16,16,ILC_COLOR8,5,5);
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_EMPTY));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SHAPES));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SHAPES_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_WORLD));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_EMPTY));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_S_SHAPES));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_S_SHAPES_SUB));
	m_ImageList.Add (AfxGetApp()->LoadIcon(IDI_BBOX_WORLD));

	m_Hierarchy.SetImageList (&m_ImageList,TVSIL_NORMAL);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHierarchy::b3AddBBoxes (
	HTREEITEM  parent,
	b3BBox    *BBox)
{
	TV_INSERTSTRUCT insert;
	HTREEITEM       item;
	long            imgNum;
	b3BBox         *sub;

	while (BBox != null)
	{
		imgNum = 0;
		sub = (b3BBox *)BBox->b3GetBBoxHead()->First;
		if (sub                           != null) imgNum += 1;
		if (BBox->b3GetShapeHead()->First != null) imgNum += 2;

		insert.hParent      = parent;
		insert.hInsertAfter = TVI_LAST;
		insert.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insert.item.pszText        = BBox->m_BoxName;
		insert.item.cchTextMax     = B3_BOXSTRINGLEN;
		insert.item.lParam         = (long)BBox;
		insert.item.iImage         = imgNum;
		insert.item.iSelectedImage = imgNum + 5;

		item = m_Hierarchy.InsertItem (&insert);
		if (sub != null)
		{
			b3AddBBoxes (item,sub);
			if (BBox->b3IsExpanded())
			{
				m_Hierarchy.Expand(item,TVE_EXPAND);
			}
		}

		BBox = (b3BBox *)BBox->Succ;
	}
}

void CDlgHierarchy::b3InitTree()
{
	TV_INSERTSTRUCT insert;
	HTREEITEM       root;

	m_Hierarchy.DeleteAllItems();
	b3Free();

	if (m_Scene != null)
	{
		insert.hParent      = TVI_ROOT;
		insert.hInsertAfter = TVI_FIRST;
		insert.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insert.item.pszText = m_Scene->b3GetName();
		insert.item.lParam  = 0;
		insert.item.iImage         = 4;
		insert.item.iSelectedImage = 9;
		root = m_Hierarchy.InsertItem (&insert);

		b3AddBBoxes (root,m_Scene->b3GetFirstBBox());
		m_Hierarchy.Expand(root,TVE_EXPAND);
	}
}

HTREEITEM CDlgHierarchy::b3FindBBox(HTREEITEM parent,b3BBox *BBox)
{
	HTREEITEM  item,result;

	for(item  = m_Hierarchy.GetNextItem(parent,TVGN_CHILD);
	    item != NULL;
		item  = m_Hierarchy.GetNextItem(item,TVGN_NEXT))
	{
#ifdef _DEBUG
		b3PrintF(B3LOG_FULL,"BBox: %s\n",((b3BBox *)(m_Hierarchy.GetItemData(item)))->b3GetName());
#endif
		if (BBox == (b3BBox *)m_Hierarchy.GetItemData(item))
		{
			// Found! Done...
			return item;
		}

		// Fast way out!
		result = b3FindBBox(item,BBox);
		if (result != null)
		{
			return result;
		}
	}
	return NULL;
}

b3_count CDlgHierarchy::b3Traverse(HTREEITEM parent)
{
	HTREEITEM  item;
	b3BBox    *BBox;
	b3_count   index = 0;

	for(item  = m_Hierarchy.GetNextItem(parent,TVGN_CHILD);
	    item != NULL;
		item  = m_Hierarchy.GetNextItem(item,TVGN_NEXT))
	{
		index += b3Traverse(item);
		index++;
		BBox = (b3BBox *)m_Hierarchy.GetItemData(item);
		BBox->b3Expand(m_Hierarchy.GetItemState(item,0) & TVIS_EXPANDED);
	}
	return index;
}

void CDlgHierarchy::b3GetData()
{
	m_Scene = (m_pDoc == null ? null : m_pDoc->m_Scene);
	b3Traverse(m_Hierarchy.GetRootItem());

	if (m_Scene == null)
	{
		m_Hierarchy.DeleteAllItems();
		b3Free();
	}
}

void CDlgHierarchy::b3SetData()
{
	UpdateData(FALSE);
	m_Scene = (m_pDoc == null ? null : m_pDoc->m_Scene);
	b3InitTree ();
}

void CDlgHierarchy::b3SelectBBox(b3BBox *BBox)
{
	HTREEITEM item;

	item = b3FindBBox(m_Hierarchy.GetRootItem(),BBox);
	if (item != null)
	{
		m_Hierarchy.SelectItem(item);
	}
}

b3BBox *CDlgHierarchy::b3GetSelectedBBox()
{
	b3BBox *BBox = null;

	if (m_Scene != null)
	{
		HTREEITEM  item = m_Hierarchy.GetSelectedItem();

		if (item != NULL)
		{
			BBox = (b3BBox *)m_Hierarchy.GetItemData(item);
		}
	}
	return BBox;
}

void CDlgHierarchy::OnEndLabelEditHierarchy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO *pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	b3BBox      *BBox;
	// TODO: Add your control notification handler code here
	
	if (BBox->m_BoxName,pTVDispInfo->item.pszText)
	{
		BBox = (b3BBox *)pTVDispInfo->item.lParam;
		strcpy (BBox->m_BoxName,pTVDispInfo->item.pszText);
		m_pDoc->SetModifiedFlag();
		m_pDoc->UpdateAllViews(null,B3_UPDATE_VIEW);
		*pResult = 1;
	}
	else
	{
		*pResult = 0;
	}
}
