/*
**
**	$Filename:	DlgCreateItem.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Adding a new name for a new item.
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

#include "AppLines.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CDlgCreateItem implementation                 **
**                                                                      **
*************************************************************************/

CDlgCreateItem::CDlgCreateItem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreateItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCreateItem)
	m_Label = _T("");
	m_NewName = _T("");
	//}}AFX_DATA_INIT

	m_ClassType  = 0;
	m_ItemBase   = null;
	m_MaxNameLen = 0;
	if (m_Suggest == null)
	{
		m_Suggest = "";
	}
}


void CDlgCreateItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateItem)
	DDX_Text(pDX, IDC_ITEM_CREATE_LABEL, m_Label);
	DDX_Text(pDX, IDC_NEW_ITEM_NAME, m_NewName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateItem, CDialog)
	//{{AFX_MSG_MAP(CDlgCreateItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateItem message handlers

BOOL CDlgCreateItem::OnInitDialog() 
{
	b3Suggest();
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit *edit;

	B3_ASSERT(m_ItemBase != null);
	if (m_MaxNameLen > 0)
	{
		edit = (CEdit *)GetDlgItem(IDC_NEW_ITEM_NAME);
		edit->LimitText(m_MaxNameLen);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

b3_bool CDlgCreateItem::b3IsNameOK(const char *suggest)
{
	b3Item *item;

	if(m_ClassType != 0)
	{
		B3_FOR_BASE(m_ItemBase,item)
		{
			if (item->b3GetClassType() == m_ClassType)
			{
				if (stricmp(suggest,item->b3GetName()) == 0)
				{
					return false;
				}
			}
		}
	}
	else
	{
		B3_FOR_BASE(m_ItemBase,item)
		{
			if (stricmp(suggest,item->b3GetName()) == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void CDlgCreateItem::b3Suggest()
{
	int count = 1;

	if (b3IsNameOK(m_Suggest))
	{
		m_NewName = m_Suggest;
	}
	else
	{
		do
		{
			m_NewName.Format("%s_%d",m_Suggest,count++);
		}
		while(!b3IsNameOK(m_NewName));
	}
}

void CDlgCreateItem::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (!b3IsNameOK(m_NewName))
	{
		AfxMessageBox(IDS_NAME_EXISTS,MB_ICONSTOP);
		GetDlgItem(IDC_NEW_ITEM_NAME)->SetFocus();
		return;
	}
	if (m_NewName.IsEmpty())
	{
		AfxMessageBox(IDS_EMPTY_NAME,MB_ICONSTOP);
		GetDlgItem(IDC_NEW_ITEM_NAME)->SetFocus();
		return;
	}
	CDialog::OnOK();
}
