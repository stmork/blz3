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

#include "AppLines.h"
#include "DlgItemMaintain.h"
#include "b3StaticPluginInfoInit.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2003/06/14 15:31:26  sm
**	- First successful plugin test including dialog. Debug version
**	  not working yet.
**
**	Revision 1.3  2003/06/14 14:44:59  sm
**	- Erster Plugintest mit Dialog und Raytracing
**	
**	Revision 1.2  2003/06/09 17:33:30  sm
**	- New item maintainance dialog added.
**	
**	Revision 1.1  2003/06/08 18:57:02  sm
**	- Added list editing to Lines
**	
*/

/*************************************************************************
**                                                                      **
**                        CDlgItemMaintain implementation               **
**                                                                      **
*************************************************************************/

CDlgItemMaintain::CDlgItemMaintain(b3Base<b3Item> *head,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemMaintain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgItemMaintain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Head = head;
	m_Plugins = &b3Loader::b3GetLoader();
}


void CDlgItemMaintain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgItemMaintain)
	DDX_Control(pDX, IDC_ITEM_LIST, m_ItemList);
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
	ON_LBN_DBLCLK(IDC_ITEM_LIST, OnDblclkItemList)
	ON_LBN_SELCHANGE(IDC_ITEM_LIST, OnSelectionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaintain message handlers

BOOL CDlgItemMaintain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(b3StaticPluginInfoInit::b3GetClassName(m_Head->b3GetClass()));
	b3UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgItemMaintain::b3UpdateList(b3Item *select)
{
	b3_plugin_info *info;
	CString     unknown;
	CString     text;
	const char *title;
	const char *desc;
	b3Item     *item;
	int         index;

	m_ItemList.ResetContent();
	unknown.LoadString(IDS_UNKNOWN);
	B3_FOR_BASE(m_Head,item)
	{
		info = m_Plugins->b3FindInfo(item);
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

		index = m_ItemList.AddString(text);
		if (index != LB_ERR)
		{
			m_ItemList.SetItemDataPtr(index,item);
			if (item == select)
			{
				m_ItemList.SetCurSel(index);
			}
		}
	}
	b3UpdateUI();
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
	b3Item *item = null;
	int     index;

	index = m_ItemList.GetCurSel();
	if (index != LB_ERR)
	{
		item = (b3Item *)m_ItemList.GetItemDataPtr(index);
	}
	return item;
}

void CDlgItemMaintain::OnItemNew() 
{
	// TODO: Add your control notification handler code here
	b3Item *selected = b3GetSelectedItem();
	b3Item *item = b3Loader::b3GetLoader().b3Create(0x7f000100);

	if (item != null)
	{
		m_Head->b3Insert(selected,item);
		b3UpdateList(item);
	}
}

void CDlgItemMaintain::OnItemEdit() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
		b3Loader::b3GetLoader().b3Edit(item);
		b3UpdateList(item);
	}
}

void CDlgItemMaintain::OnDblclkItemList() 
{
	// TODO: Add your control notification handler code here
	OnItemEdit();
}

void CDlgItemMaintain::OnSelectionChanged() 
{
	// TODO: Add your control notification handler code here
	b3UpdateUI();
}

void CDlgItemMaintain::OnItemDelete() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
		m_Head->b3Remove(item);
		delete item;
		b3UpdateList(item);
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
	}
}

void CDlgItemMaintain::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
