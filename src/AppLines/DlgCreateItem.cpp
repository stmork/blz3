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

#include "AppLinesInclude.h"

#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2006/06/01 14:17:24  smork
**	- Added frame work controls for simpler plugin generation:
**	  o CB3Checkbox
**	  o CB3TextEdit
**
**	Revision 1.5  2006/03/05 22:12:31  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.4  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.3  2002/01/04 17:53:53  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.2  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
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
