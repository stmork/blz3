/*
**
**	$Filename:	DlgCreateItem.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        CDlgCreateItem implementation                 **
**                                                                      **
*************************************************************************/

CDlgCreateItem::CDlgCreateItem(CWnd* pParent /*=NULL*/)
	: CB3Dialog(CDlgCreateItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCreateItem)
	m_Label       = _T("");
	//}}AFX_DATA_INIT

	m_ClassType   = 0;
	m_ItemBase    = null;
	m_Suggest     = _T("");
	m_NewName     = _T("");
	m_MaxNameLen  = 0;
	m_NoNameCheck = false;
}


void CDlgCreateItem::DoDataExchange(CDataExchange* pDX)
{
	CB3Dialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateItem)
	DDX_Text(pDX, IDC_ITEM_CREATE_LABEL, m_Label);
	DDX_Control(pDX, IDC_NEW_ITEM_NAME, m_NewNameCtrl);
	//}}AFX_DATA_MAP
	m_NewNameCtrl.b3DDX(pDX, m_NewName, m_MaxNameLen);
}


BEGIN_MESSAGE_MAP(CDlgCreateItem, CB3Dialog)
	//{{AFX_MSG_MAP(CDlgCreateItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateItem message handlers

void CDlgCreateItem::b3PreInitDialog() 
{
	b3Suggest();
}

void CDlgCreateItem::b3PostInitDialog()
{
	CEdit *edit;

	B3_ASSERT(m_ItemBase != null);
	if (m_MaxNameLen > 0)
	{
		edit = (CEdit *)GetDlgItem(IDC_NEW_ITEM_NAME);
		edit->LimitText(m_MaxNameLen);
	}
}

b3_bool CDlgCreateItem::b3IsNameOK(const char *suggest)
{
	b3Item *item;

	if (!m_NoNameCheck)
	{
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

void CDlgCreateItem::b3UpdateUI()
{
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
	CB3Dialog::OnOK();
}

void CDlgCreateItem::b3GetText(char *buffer)
{
	strncpy(buffer, m_NewName, m_MaxNameLen);
}

const char *CDlgCreateItem::b3GetText()
{
	return m_NewName.Left(m_MaxNameLen - 1);
}
