/*
**
**	$Filename:	DlgNewObject.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting a new shape or object
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
#include "DlgNewObject.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2002/05/10 15:24:23  sm
**	- Corrected some exceptions in b3Tx
**	- Added double click support in list controls when creating
**	  a new shape.
**	- Some minor fixes done.
**
**	Revision 1.3  2002/02/24 17:45:31  sm
**	- Added CSG edit dialogs
**	- Corrected shape edit inheritance.
**	
**	Revision 1.2  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**	Revision 1.1  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgNewObject implementation                  **
**                                                                      **
*************************************************************************/

CDlgNewObject::CDlgNewObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewObject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewObject)
	m_ModeCSG = 0;
	//}}AFX_DATA_INIT
	m_BBox = null;
	m_InsertAfter = null;
	m_NewItem = null;
}


void CDlgNewObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewObject)
	DDX_Control(pDX, IDC_OBJECTLIST, m_ListCtrl);
	DDX_Radio(pDX, IDC_CSGMODE_UNION, m_ModeCSG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewObject, CDialog)
	//{{AFX_MSG_MAP(CDlgNewObject)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJECTLIST, OnObjectChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTLIST, OnObjectDoubleClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewObject message handlers

BOOL CDlgNewObject::OnInitDialog() 
{
	b3Base<b3Item> *ShapeBase;
	CWinApp        *app = AfxGetApp();
	b3_u32          ClassType = 0;

	CDialog::OnInitDialog();
	m_ImageList.b3Create(64);

	// TODO: Add extra initialization here
	B3_ASSERT(m_BBox != null);

	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ListCtrl.SetIconSpacing(128,96);

	ShapeBase = m_BBox->b3GetShapeHead();
	if (ShapeBase->First == null)
	{
		ClassType = app->GetProfileInt(CB3ClientString(),"new item.all shapes",SPHERE);
		m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_SHAPE,ClassType);
		m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_CSG,  ClassType);
		m_ModeCSG = app->GetProfileInt(CB3ClientString(),"new item.csg mode",0);
	}
	else
	{
		switch(ShapeBase->b3GetClass())
		{
		case CLASS_SHAPE:
			ClassType = app->GetProfileInt(CB3ClientString(),"new item.simple shape",SPHERE);
			m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_SHAPE,ClassType);
			break;
		case CLASS_CSG:
			m_ModeCSG = app->GetProfileInt(CB3ClientString(),"new item.csg mode",0);
			ClassType = app->GetProfileInt(CB3ClientString(),"new item.csg shape",CSG_SPHERE);
			m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_CSG,ClassType);
			break;

		default:
			ClassType = 0;
			b3PrintF(B3LOG_NORMAL,"Unknown shape class: %08x\n",
				ShapeBase->b3GetClass());
			return FALSE;
		}
	}

	// Add BBox
	if (m_InsertAfter == null)
	{
		m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_BBOX,ClassType);
	}
	b3UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

b3_u32 CDlgNewObject::b3GetSelectedClassType()
{
	POSITION pos;
	int     index;
	b3_u32  ClassType = 0;

	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		index     = m_ListCtrl.GetNextSelectedItem(pos);
		ClassType = m_ListCtrl.GetItemData(index);
	}
	return ClassType;
}

void CDlgNewObject::b3UpdateUI()
{
	b3_bool is_csg = (b3GetSelectedClassType() & 0xffff0000) == CLASS_CSG;;

	GetDlgItem(IDC_CSGMODE_UNION)->EnableWindow(is_csg);
	GetDlgItem(IDC_CSGMODE_INTERSECT)->EnableWindow(is_csg);
	GetDlgItem(IDC_CSGMODE_SUB)->EnableWindow(is_csg);
}

void CDlgNewObject::OnObjectChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	b3UpdateUI(); 
	*pResult = 0;
}

void CDlgNewObject::OnOK() 
{
	// TODO: Add extra validation here
	b3_u32 Class,ClassType;
	b3Base<b3Item> *shapes;
	b3Base<b3Item> *bboxes;
	b3BBox         *bbox;
	b3SimpleShape  *shape;
	b3CSGShape     *csg;
	CDlgCreateItem  dlg;
	CWinApp        *app = AfxGetApp();

	CDialog::OnOK();
	ClassType = b3GetSelectedClassType();
	Class     = ClassType & 0xffff0000;

	bboxes = m_BBox->b3GetBBoxHead();
	shapes = m_BBox->b3GetShapeHead();
	if ((Class != CLASS_BBOX) && (shapes->First == null))
	{
		// If shape list of BBox is empty we can init
		// with the selected shape class.
		shapes->b3InitBase(Class);
		app->WriteProfileInt(CB3ClientString(),"new item.all shapes",ClassType);
	}

	// Create object and append into parent BBox
	switch(Class)
	{
	case CLASS_BBOX:
		dlg.m_ClassType   = CLASS_BBOX;
		dlg.m_MaxNameLen  = B3_BOXSTRINGLEN;
		dlg.m_ItemBase    = bboxes;
		dlg.m_NoNameCheck = true;
		dlg.m_Suggest.LoadString(IDS_NEW_OBJECT);
		if (dlg.DoModal() == IDOK)
		{
			m_NewItem = b3World::b3AllocNode(BBOX | ((m_BBox->b3GetClassType() & 0xffff) + 1));
			bbox = (b3BBox *)m_NewItem;
			strcpy (bbox->m_BoxName,dlg.m_NewName);
			m_Base = bboxes;
			m_InsertAfter = bboxes->Last;
		}
		else
		{
			m_Base    = null;
			m_NewItem = null;
		}
		break;

	case CLASS_SHAPE:
		m_Base    = shapes;
		m_NewItem = b3World::b3AllocNode(ClassType);
		shape = (b3SimpleShape *)m_NewItem;
		app->WriteProfileInt(CB3ClientString(),"new item.simple shape",ClassType);
		break;

	case CLASS_CSG:
		m_Base    = shapes;
		m_NewItem = b3World::b3AllocNode(ClassType);
		csg = (b3CSGShape *)m_NewItem;
		csg->m_Operation = csg->m_CSGMode[m_ModeCSG];
		app->WriteProfileInt(CB3ClientString(),"new item.csg mode",m_ModeCSG);
		app->WriteProfileInt(CB3ClientString(),"new item.csg shape",ClassType);
		break;

	default:
		m_Base    = null;
		m_NewItem = null;
		break;
	}
}

void CDlgNewObject::OnObjectDoubleClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOK();
	*pResult = 0;
}
