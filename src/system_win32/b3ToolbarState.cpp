/*
**
**	$Filename:	b3ToolbarState.cpp $  
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Toolbar state handling
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3App.h"
#include "blz3/system/b3Toolbar.h"
							  
#include "afxpriv.h"
#include "afxext.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III - docking window collections     **
**                                                                      **
*************************************************************************/
									
CB3ToolbarState::CB3ToolbarState(const char *appName)
{
	m_MainFrame = null;
	m_MenuCount = 0;
	m_ToolCount = 0;
	snprintf (m_Code,sizeof(m_Code),"_ToolbarState of %s",appName);
}

void CB3ToolbarState::b3AddToolbar(
	CB3Toolbar *toolbar,
	long        id_bitmap,
	long        id_title)
{
	if (toolbar != null)
	{
		m_Toolbars.b3Append(toolbar);
		toolbar->b3SetID(id_bitmap,id_title,m_ToolCount == 0 ? AFX_IDW_TOOLBAR : id_bitmap);
		m_ToolCount++;
	}
}

void CB3ToolbarState::b3AddMenubar(
	CB3Menubar *menubar,
	long          id_menu)
{
	if (menubar != null)
	{
		m_Menubars.b3Append(menubar);
		menubar->b3SetID(id_menu,m_MenuCount == 0 ? AFX_IDW_MENUBAR : id_menu);
	}
}

void CB3ToolbarState::b3AddDialogbar(
	CB3Dialogbar *dialogbar,
	long          id_dialog,
	long          id_title)
{
	if (dialogbar != null)
	{
		m_Dialogbars.b3First(dialogbar);
		dialogbar->b3SetID(id_dialog,id_title,m_ToolCount == 0 ? AFX_IDW_DIALOGBAR : id_dialog);
	}
}

b3_bool CB3ToolbarState::b3CreateToolbars(CFrameWnd *parent)
{
	CB3Menubar   *mb;
	CB3Toolbar   *tb;
	CB3Dialogbar *db;

	m_MainFrame = parent;
	ASSERT(m_MainFrame != null);

	// Create menubars
	B3_FOR_BASE(&m_Menubars,mb)
	{
		if (!mb->b3Create(m_MainFrame))
		{
			return false;
		}
	}

	// Create toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		if (!tb->b3Create(m_MainFrame))
		{
			return false;
		}
	}

	// Create toolbars
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		if (!db->b3Create(m_MainFrame))
		{
			return false;
		}
	}
	return true;
}

void CB3ToolbarState::b3UpdateUI()
{
	CB3Menubar   *mb;
	CB3Toolbar   *tb;
	CB3Dialogbar *db;

	// Do nothing if no MainFrame was specified.
	if (m_MainFrame == null)
	{
		return;
	}

	// Update menubars
	B3_FOR_BASE(&m_Menubars,mb)
	{
		mb->OnUpdateCmdUI(m_MainFrame,TRUE);
	}

	// Update toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->OnUpdateCmdUI(m_MainFrame,TRUE);
	}

	// Update dialogbars
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->OnUpdateCmdUI(m_MainFrame,TRUE);
	}
}

void CB3ToolbarState::b3GetData()
{
	CB3Dialogbar *db;

	// Read data out of controls
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->b3GetData();
	}
}

void CB3ToolbarState::b3SetData()
{
	CB3Dialogbar *db;

	// Put data into controls
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->b3SetData();
	}
}

void CB3ToolbarState::b3Free()
{
	CB3Toolbar   *tb;

	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->b3Free();
	}
}

void CB3ToolbarState::b3LoadState(const char *name)
{
	CB3Menubar   *mb;
	CB3Toolbar   *tb;
	CB3Dialogbar *db;
	CControlBar  *last = null;

	// Do nothing if no MainFrame was specified.
	if (m_MainFrame == null)
	{
		return;
	}

	// Enable docking for menubars
	B3_FOR_BASE(&m_Menubars,mb)
	{
		mb->EnableDockingEx(CBRS_ALIGN_ANY);
	}

	// Enable docking for toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->EnableDocking(CBRS_ALIGN_ANY);
	}

	// Enable docking for dialogbars
	B3_FOR_BASE(&m_Dialogbars,db)
	{
		db->EnableDocking(CBRS_ALIGN_ANY);
	}

	// Enable docking for main frame window
	m_MainFrame->EnableDocking(CBRS_ALIGN_ANY);

	// Dock menus
	B3_FOR_BASE(&m_Menubars,mb)
	{
		m_MainFrame->DockControlBar(mb);
	}

	// Dock toolbars
	B3_FOR_BASE(&m_Toolbars,tb)
	{
		tb->b3InitCustomization();
		tb->b3RestoreState(name);
		last = tb->b3DockRight(last);
	}

	// Dock Dialogbars
	last = null;
	B3_FOR_BASE(&m_Dialogbars,db)
	{
//		db->b3InitCustomization();
		db->b3RestoreState(name);
		last = db->b3DockRight(last);
	}

	// Load bar state
	try
	{
		if (name == null)
		{
			name = m_Code;
		}
		m_MainFrame->LoadBarState(name);
	}
	catch (...)
	{
		b3PrintF (B3LOG_NORMAL,"### CLASS: b3TBar # b3LoadState() failed.\n");
	}
}

void CB3ToolbarState::b3SaveState(const char *name)
{
	CB3Toolbar   *tb;
	CB3Dialogbar *db;

	if (m_MainFrame != null)
	{
		try
		{
			// Write bar state
			B3_FOR_BASE(&m_Toolbars,tb)
			{
				tb->b3SaveState(name);
			}

			// Write bar state
			B3_FOR_BASE(&m_Dialogbars,db)
			{
				db->b3SaveState(name);
			}
			if (name == null)
			{
				name = m_Code;
			}
			m_MainFrame->SaveBarState(name);
		}
		catch (...)
		{
			b3PrintF (B3LOG_NORMAL,"### CLASS: b3TBar # b3SaveState() failed.\n");
		}
	}
}

b3_bool CB3ToolbarState::b3PreTranslateMsg(MSG *pMSG)
{
	CB3Menubar *mb;

	B3_FOR_BASE(&m_Menubars,mb)
	{
		if (mb->TranslateFrameMessage(pMSG))
		{
			return true;
		}
	}
	return false;
}
