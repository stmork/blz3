/*
**
**	$Filename:	DlgItemMaintain.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Maintaining an item list
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

#include "DlgItemMaintain.h"
#include "b3StaticPluginInfoInit.h"
#include "blz3/system/b3FileReg.h"

/*************************************************************************
**                                                                      **
**                        CDlgItemMaintain implementation               **
**                                                                      **
*************************************************************************/

CDlgItemMaintain::CDlgItemMaintain(
	CAppRenderDoc  *pDoc,
	b3Base<b3Item> *head,
	CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemMaintain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgItemMaintain)
	m_CreationValue = 1;
	//}}AFX_DATA_INIT
	m_Head    = head;
	m_Plugins = &b3Loader::b3GetLoader();
	m_Changed = false;
	m_pDoc    = pDoc;
	m_CreationValue = CB3GetApp()->GetProfileInt(CB3ClientString(),"item maintain.creation source",m_CreationValue);
}


void CDlgItemMaintain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgItemMaintain)
	DDX_Control(pDX, IDC_ITEM_LIST, m_ItemListCtrl);
	DDX_Control(pDX, IDC_CLASS_LIST, m_ClassListCtrl);
	DDX_Radio(pDX, IDC_ITEM_DEFAULT, m_CreationValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgItemMaintain, CDialog)
	//{{AFX_MSG_MAP(CDlgItemMaintain)
	ON_BN_CLICKED(IDC_ITEM_NEW, OnItemNew)
	ON_BN_CLICKED(IDC_ITEM_EDIT, OnItemEdit)
	ON_BN_CLICKED(IDC_ITEM_DELETE, OnItemDelete)
	ON_BN_CLICKED(IDC_ITEM_FIRST, OnItemFirst)
	ON_BN_CLICKED(IDC_ITEM_UP, OnItemUp)
	ON_BN_CLICKED(IDC_ITEM_DOWN, OnItemDown)
	ON_BN_CLICKED(IDC_ITEM_LAST, OnItemLast)
	ON_NOTIFY(NM_DBLCLK, IDC_ITEM_LIST, OnDoItemList)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_LIST, OnClickItemList)
	ON_NOTIFY(NM_DBLCLK, IDC_CLASS_LIST, OnDoClassList)
	ON_NOTIFY(NM_RETURN, IDC_ITEM_LIST, OnDoItemList)
	ON_NOTIFY(NM_RETURN, IDC_CLASS_LIST, OnDoClassList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaintain message handlers

BOOL CDlgItemMaintain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3InitItemList();

	SetWindowText(b3StaticPluginInfoInit::b3GetClassName(m_Head->b3GetClass()));
	b3UpdateList(m_Head->First,true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgItemMaintain::b3InitItemList()
{
	b3Loader        &loader = b3Loader::b3GetLoader();
	b3_plugin_info  *info;
	b3Array<b3_u32>  class_types;
	b3_u32           class_type;
	b3_count         i;
	b3_index         img = 0;
	int              size = 48;
	HICON            unknown = AfxGetApp()->LoadIcon(IDI_ITEM_UNKNOWN);
	LVITEM           listitem;

	memset(&listitem,0,sizeof(listitem));
	listitem.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	listitem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;

	loader.b3GetClassTypes(class_types,m_Head->b3GetClass());
	m_ImageList.Create(size,size,ILC_COLOR32,30,8);

	m_ItemListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ItemListCtrl.SetIconSpacing(128,80);

	m_ClassListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	m_ClassListCtrl.SetIconSpacing(128,80);

	m_ImageList.SetBkColor(m_ItemListCtrl.GetBkColor());
	m_ImageList.Add(unknown);
	for (i = 0;i < class_types.b3GetCount();i++)
	{
		class_type = class_types[i];
		info = loader.b3FindInfo(class_type);
		if (info->m_CreateFunc != null)
		{
			listitem.iImage  = i + 1;
			listitem.lParam  = class_type;
			listitem.state   = 0;
			listitem.pszText = info->m_Description;
			m_ClassListCtrl.InsertItem(&listitem);
		}

		m_ImageList.Add(info->m_Icon != null ? info->m_Icon : unknown);

		if (!m_ClassTypesToImg.b3HasKey(class_type))
		{
			m_ClassTypesToImg.b3Add(class_type,i + 1);
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"Tried to add already allocated class type %08x.\n",class_type);
		}
	}
}

void CDlgItemMaintain::b3UpdateList(b3Item *select,b3_bool set_focus)
{
	b3_plugin_info *info;
	CString     unknown;
	CString     text;
	const char *title;
	const char *desc;
	b3Item     *item;
	b3_u32      class_type;
	int        *imgPtr;
	LVITEM      listitem;

	memset(&listitem,0,sizeof(listitem));
	listitem.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	listitem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;

	unknown.LoadString(IDS_UNKNOWN);
	m_ItemListCtrl.DeleteAllItems();
	B3_FOR_BASE(m_Head,item)
	{
		class_type = item->b3GetClassType();
		info       = m_Plugins->b3FindInfo(item);
		if (info != null)
		{
			desc = info->m_Description;
		}
		else
		{
			desc = unknown;
		}

		title = item->b3GetName();
		if(title != null)
		{
			text.Format("%s (%s)",desc,title);
		}
		else
		{
			text = desc;
		}
		imgPtr = m_ClassTypesToImg.b3Find(class_type);

		listitem.iImage  = imgPtr != null ? *imgPtr : 0;
		listitem.lParam  = (LPARAM)item;
		listitem.state   = item == select ? listitem.stateMask : 0;
		listitem.pszText = info->m_Description;
		m_ItemListCtrl.InsertItem(&listitem);
	}

	b3UpdateUI();
	if (set_focus)
	{
		m_ItemListCtrl.SetFocus();
	}
}

void CDlgItemMaintain::b3UpdateUI()
{
	b3Item         *item  = b3GetSelectedItem();
	b3_plugin_info *info = m_Plugins->b3FindInfo(item);
	long            state = m_Head->b3State(item);

	GetDlgItem(IDC_ITEM_NEW)->EnableWindow();
	GetDlgItem(IDC_ITEM_EDIT)->EnableWindow(b3Plugin::b3HasEditFunc(info));
	GetDlgItem(IDC_ITEM_DELETE)->EnableWindow(state & B3_NODE_NOT_NULL);
	GetDlgItem(IDC_ITEM_FIRST)->EnableWindow( state & B3_NODE_NOT_FIRST);
	GetDlgItem(IDC_ITEM_UP)->EnableWindow(    state & B3_NODE_PREV);
	GetDlgItem(IDC_ITEM_DOWN)->EnableWindow(  state & B3_NODE_SUCC);
	GetDlgItem(IDC_ITEM_LAST)->EnableWindow(  state & B3_NODE_NOT_LAST);
}

b3Item *CDlgItemMaintain::b3GetSelectedItem()
{
	POSITION  pos;
	int       index;
	b3Item   *item = null;

	pos = m_ItemListCtrl.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		index = m_ItemListCtrl.GetNextSelectedItem(pos);
		item  = (b3Item *)m_ItemListCtrl.GetItemData(index);
	}
	return item;
}

void CDlgItemMaintain::OnItemNew() 
{
	// TODO: Add your control notification handler code here
	b3Item   *selected = b3GetSelectedItem();
	b3Item   *item;
	POSITION  pos;
	int       index;
	b3_u32    class_type = 0;

	pos = m_ClassListCtrl.GetFirstSelectedItemPosition();
	if (pos != 0)
	{
		index      = m_ClassListCtrl.GetNextSelectedItem(pos);
		class_type = m_ClassListCtrl.GetItemData(index);
	}
	if (class_type != 0)
	{
		CString    section;
		b3FileReg  file;
		b3_u32    *buffer;
		b3_size    size = 0;

		UpdateData();
		switch(m_CreationValue)
		{
		case 1:
			section.Format("item %08x.default",class_type);
			buffer = (b3_u32 *)file.b3ReadBuffer(section,size);
			break;
		case 0:
		default:
			buffer = null;
			break;
		}
		if (buffer != null)
		{
			item = b3Loader::b3GetLoader().b3Create(buffer,m_pDoc,false);
		}
		else
		{
			item = b3Loader::b3GetLoader().b3Create(class_type,m_pDoc,false);
		}

		if (item != null)
		{
			m_Head->b3Insert(selected,item);
			b3UpdateList(item,true);
			m_Changed = true;
		}
	}
}

void CDlgItemMaintain::OnItemEdit() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();		
	b3Item *edit;

	if (item != null)
	{
		edit = b3World::b3Clone(item);
		if (b3Loader::b3GetLoader().b3Edit(edit, m_pDoc))
		{
			b3Store(edit);
			m_Head->b3Insert(item,edit);
			m_Head->b3Remove(item);
			delete item;

			b3UpdateList(edit,true);
			m_Changed = true;
		}
		else
		{
			delete edit;
		}
	}
}

void CDlgItemMaintain::b3Store(b3Item *item)
{
	CString   section;
	b3FileReg file;

	section.Format("item %08x.default",item->b3GetClassType());
	if (file.b3Open(section,B_WRITE))
	{
		item->b3Store();
		item->b3StoreFile(&file);
		file.b3Close();
	}
}

void CDlgItemMaintain::OnClickItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
	*pResult = 0;
}

void CDlgItemMaintain::OnItemDelete() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
		if (b3Runtime::b3MessageBox(IDS_ASK_DELETE_ITEM,B3_MSGBOX_YES_NO) == B3_MSG_YES)
		{
			b3Item *select = m_Head->b3Reselect(item);
			
			m_Head->b3Remove(item);
			delete item;
			b3UpdateList(select,true);
			m_Changed = true;
		}
	}
}

void CDlgItemMaintain::OnItemFirst() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
		m_Head->b3Remove(item);
		m_Head->b3First(item);
		b3UpdateList(item);
		m_Changed = true;
	}
}

void CDlgItemMaintain::OnItemUp() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();
	b3Item *prev;

	if (item != null)
	{
		prev = item->Prev;
		if (prev != null)
		{
			m_Head->b3Remove(prev);
			m_Head->b3Insert(item,prev);
			b3UpdateList(item);
			m_Changed = true;
		}
	}
}

void CDlgItemMaintain::OnItemDown() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();
	b3Item *succ;

	if (item != null)
	{
		succ = item->Succ;
		if (succ != null)
		{
			m_Head->b3Remove(item);
			m_Head->b3Insert(succ,item);
			b3UpdateList(item);
			m_Changed = true;
		}
	}
}

void CDlgItemMaintain::OnItemLast() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
		m_Head->b3Remove(item);
		m_Head->b3Append(item);
		b3UpdateList(item);
		m_Changed = true;
	}
}

void CDlgItemMaintain::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	CB3GetApp()->WriteProfileInt(CB3ClientString(),"item maintain.creation source",m_CreationValue);
	if ((m_pDoc != null) && m_Changed)
	{
		m_pDoc->SetModifiedFlag();
	}
}

void CDlgItemMaintain::OnDoItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnItemEdit();
	*pResult = 0;
}

void CDlgItemMaintain::OnDoClassList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnItemNew();
	*pResult = 0;
}

BOOL CDlgItemMaintain::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (m_ItemListCtrl.m_hWnd == pMsg->hwnd)
			{
				OnItemEdit();
				return TRUE;
			}
			if (m_ClassListCtrl.m_hWnd == pMsg->hwnd)
			{
				OnItemNew();
				return TRUE;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
