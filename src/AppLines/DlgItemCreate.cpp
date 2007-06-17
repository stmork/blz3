/*
**
**	$Filename:	DlgItemCreate.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Creating an item
**
**	(C) Copyright 2003  Steffen A. Mork
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

#include "DlgItemCreate.h"
#include "blz3/system/b3Plugin.h"
#include "blz3/system/b3FileReg.h"

/*************************************************************************
**                                                                      **
**                        CDlgItemCreate implementation                 **
**                                                                      **
*************************************************************************/


CDlgItemCreate::CDlgItemCreate(CAppRenderDoc *pDoc,b3_u32 class_id,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemCreate::IDD, pParent)
{
	m_pDoc = pDoc;
	m_Item = null;
	m_ClassId = class_id;
	//{{AFX_DATA_INIT(CDlgItemCreate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgItemCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgItemCreate)
	DDX_Control(pDX, IDC_ITEMLIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgItemCreate, CDialog)
	//{{AFX_MSG_MAP(CDlgItemCreate)
	ON_NOTIFY(NM_DBLCLK, IDC_ITEMLIST, OnDblclkItemlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgItemCreate message handlers

b3Item * CDlgItemCreate::b3Create(CAppRenderDoc *pDoc,b3_u32 class_id)
{
	CDlgItemCreate dlg(pDoc,class_id);

	return dlg.DoModal() == IDOK ? dlg.m_Item : null;
}

BOOL CDlgItemCreate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3InitClassTypeList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgItemCreate::b3InitClassTypeList()
{
	b3Loader        &loader = b3Loader::b3GetLoader();
	b3_plugin_info  *info;
	b3Array<b3_u32>  class_types;
	b3_u32           class_type;
	b3_count         i;
	int              size = 48;
	b3_index         img = 0;
	HICON            unknown = AfxGetApp()->LoadIcon(IDI_ITEM_UNKNOWN);
	LVITEM           listitem;

	memset(&listitem,0,sizeof(listitem));
	listitem.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	listitem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
	loader.b3GetClassTypes(class_types,m_ClassId);
	m_ImageList.Create(size,size,ILC_COLOR32,30,8);
	m_ImageList.SetBkColor(m_ListCtrl.GetBkColor());
	m_ListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ListCtrl.SetIconSpacing(128,64);
	for (i = 0;i < class_types.b3GetCount();i++)
	{
		class_type = class_types[i];
		info = loader.b3FindInfo(class_type);
		if (info->m_CreateFunc != null)
		{
			m_ImageList.Add(info->m_Icon != null ? info->m_Icon : unknown);
			listitem.iImage  = img;
			listitem.lParam  = class_type;
			listitem.state   = 0;
			listitem.pszText = info->m_Description;
			m_ListCtrl.InsertItem(&listitem);
			img++;
		}
	}
}

void CDlgItemCreate::OnDblclkItemlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOK();
	*pResult = 0;
}

void CDlgItemCreate::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos;
	int     index;
	b3_u32  class_type = 0;

	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		index      = m_ListCtrl.GetNextSelectedItem(pos);
		class_type = m_ListCtrl.GetItemData(index);
	}
	CDialog::OnOK();
	if (class_type != 0)
	{
		CString    section;
		b3FileReg  file;
		b3_u32    *buffer;
		b3_size    size = 0;

		section.Format("item %08x.default",class_type);
		buffer = (b3_u32 *)file.b3ReadBuffer(section,size);
		if (buffer != null)
		{
			m_Item = b3Loader::b3GetLoader().b3Create(buffer,m_pDoc);
		}
		else
		{
			m_Item = b3Loader::b3GetLoader().b3Create(class_type,m_pDoc);
		}
	}
}
