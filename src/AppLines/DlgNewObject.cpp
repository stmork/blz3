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

static b3_csg_operation csg_modes[] =
{
	B3_CSG_UNION,
	B3_CSG_INTERSECT,
	B3_CSG_SUB
};

CDlgNewObject::CDlgNewObject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewObject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewObject)
	m_ModeCSG = 0;
	//}}AFX_DATA_INIT
	m_BBox = null;
	m_InsertAfter = null;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewObject message handlers

BOOL CDlgNewObject::OnInitDialog() 
{
	b3Base<b3Item> *ShapeBase;

	CDialog::OnInitDialog();
	m_ImageList.b3Create(64);

	// TODO: Add extra initialization here
	B3_ASSERT(m_BBox != null);

	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ListCtrl.SetIconSpacing(96,96);
	if (m_InsertAfter == null)
	{
		// Add BBox
		m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_BBOX);
	}
	ShapeBase = m_BBox->b3GetShapeHead();
	if (ShapeBase->First == null)
	{
		m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_SHAPE);
		m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_CSG);
	}
	else
	{
		switch(ShapeBase->b3GetClass())
		{
		case CLASS_SHAPE:
			m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_SHAPE);
			break;
		case CLASS_CSG:
			m_ImageList.b3InsertListEntries(&m_ListCtrl,CLASS_CSG);
			break;

		default:
			b3PrintF(B3LOG_NORMAL,"Unknown shape class: %08x\n",
				ShapeBase->b3GetClass());
			return FALSE;
		}
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
			bbox = new b3BBox(BBOX | ((m_BBox->b3GetClassType() & 0xffff) + 1));
			strcpy (bbox->m_BoxName,dlg.m_NewName);
			bboxes->b3Append(bbox);
		}
		break;

	case CLASS_SHAPE:
		shape = (b3SimpleShape *)b3World::b3AllocNode(ClassType);
		shapes->b3Insert(m_InsertAfter,shape);
		break;

	case CLASS_CSG:
		csg = (b3CSGShape *)b3World::b3AllocNode(ClassType);
		csg->m_Operation = csg_modes[m_ModeCSG];
		shapes->b3Insert(m_InsertAfter,csg);
		break;
	}
}
