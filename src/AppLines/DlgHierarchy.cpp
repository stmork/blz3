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
	b3BBox    *BBox,
	HTREEITEM  parent)
{
	TV_INSERTSTRUCT item;
	HTREEITEM       newItem;
	long            imgNum;
	b3BBox         *sub;

	while (BBox != null)
	{
		imgNum = 0;
		sub = (b3BBox *)BBox->b3GetBBoxHead()->First;
		if (sub                           != null) imgNum += 1;
		if (BBox->b3GetShapeHead()->First != null) imgNum += 2;

		item.hParent      = parent;
		item.hInsertAfter = TVI_LAST;
		item.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		item.item.pszText        = BBox->m_BoxName;
		item.item.cchTextMax     = B3_BOXSTRINGLEN;
		item.item.lParam         = (long)BBox;
		item.item.iImage         = imgNum;
		item.item.iSelectedImage = imgNum + 5;

		newItem = m_Hierarchy.InsertItem (&item);
		if (sub != null)
		{
			b3AddBBoxes (sub,newItem);
		}

		BBox = (b3BBox *)BBox->Succ;
	}
}

void CDlgHierarchy::b3InitTree()
{
	TV_INSERTSTRUCT item;
	long            num;

	m_Hierarchy.DeleteAllItems();
	b3Free();

	if (m_Scene != null)
	{
		num = m_Scene->b3GetBBoxCount();
		m_TreeItems = (HTREEITEM *)b3Alloc(sizeof(HTREEITEM) * num);
		if (m_TreeItems == null)
		{
			return;
		}

		item.hParent      = TVI_ROOT;
		item.hInsertAfter = TVI_FIRST;
		item.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		item.item.pszText = "Welt";
		item.item.lParam  = 0;
		item.item.iImage         = 4;
		item.item.iSelectedImage = 9;
		m_TreeItems[0]    = m_Hierarchy.InsertItem (&item);

		b3AddBBoxes (m_Scene->b3GetFirstBBox(),m_TreeItems[0]);
	}
}

void CDlgHierarchy::b3GetData()
{
	b3_count   items,index = 0;
	HTREEITEM  item = m_Hierarchy.GetSelectedItem();

	m_Scene = (m_pDoc == null ? null : m_pDoc->m_Scene);
	items = m_Hierarchy.GetCount();
	for(item  = m_Hierarchy.GetRootItem();
	    item != NULL;
		item  = m_Hierarchy.GetNextItem(item,TVGN_NEXT))
	{
		index++;
	}

	m_Hierarchy.DeleteAllItems();
	b3Free();
}

void CDlgHierarchy::b3SetData()
{
	UpdateData(FALSE);
	m_Scene = (m_pDoc == null ? null : m_pDoc->m_Scene);
	b3InitTree ();
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
