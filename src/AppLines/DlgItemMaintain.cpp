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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaintain message handlers

BOOL CDlgItemMaintain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	b3UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgItemMaintain::b3UpdateList(b3Item *select)
{
	b3Item *item;
	int     index;

	m_ItemList.ResetContent();
	B3_FOR_BASE(m_Head,item)
	{
		index = m_ItemList.AddString("Item");
		if (index != LB_ERR)
		{
			m_ItemList.SetItemDataPtr(index,item);
			if (item == select)
			{
				m_ItemList.SetCurSel(index);
			}
		}
	}
}

void CDlgItemMaintain::b3UpdateUI()
{
	b3Item *item  = b3GetSelectedItem();
	long    state = m_Head->b3State(item);

	GetDlgItem(IDC_ITEM_EDIT)->EnableWindow(  (state & B3_NODE_NULL) == 0);
	GetDlgItem(IDC_ITEM_DELETE)->EnableWindow((state & B3_NODE_NULL) == 0);
	GetDlgItem(IDC_ITEM_FIRST)->EnableWindow( (state & B3_NODE_FIRST) == 0);
	GetDlgItem(IDC_ITEM_UP)->EnableWindow(     state & B3_NODE_PREV);
	GetDlgItem(IDC_ITEM_DOWN)->EnableWindow(   state & B3_NODE_SUCC);
	GetDlgItem(IDC_ITEM_LAST)->EnableWindow(  (state & B3_NODE_LAST) == 0);
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
	
}

void CDlgItemMaintain::OnItemEdit() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
	}
}

void CDlgItemMaintain::OnDblclkItemList() 
{
	// TODO: Add your control notification handler code here
	OnItemEdit();
}

void CDlgItemMaintain::OnItemDelete() 
{
	// TODO: Add your control notification handler code here
	b3Item *item = b3GetSelectedItem();

	if (item != null)
	{
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
