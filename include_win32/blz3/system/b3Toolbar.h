/*
**
**	$Filename:	b3Toolbar.h $  
**	$Release:	Dortmund 2000 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Toolbar handling
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_TOOLBAR_H
#define B3_SYSTEM_TOOLBAR_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"

#include "../smart_gui/MenuBar.h"

class CB3ToolbarInfo
{
public:
	TBBUTTON  tbButton;		// information regarding the button
	char     *btnText;		// text for the button
	long      width;

	CB3ToolbarInfo()
	{
		btnText = null;
	}
	void b3FreeText()
	{
		if (btnText != null)
		{
			delete [] btnText;
		}
	}
};

struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount]

	WORD* items()
		{ return (WORD*)(this+1); }
};

class CB3Toolbar : public CToolBar, public b3Link<CB3Toolbar>
{
	CFrameWnd        *m_ToolbarMain;
	CString           m_Title;
	CString           m_Key;
	CString           m_Value;
	CB3ToolbarInfo   *m_ToolbarInfo;		// table of buttons and text
	b3_bool           m_Visible;
	b3_index          m_ID;
	b3_index          m_Bar;
	b3_count          m_ButtonCount;
public:
	              CB3Toolbar();
				 ~CB3Toolbar();
	b3_bool       b3ToggleVisibility();
	b3_bool       b3SetVisibility(bool new_visibility=true);
	b3_bool       b3IsVisible();
	void          b3Customize();

protected:
	friend class  CB3ToolbarState;

	void          b3SetID(long id_bitmap,long id_title,long id_bar);
	b3_bool       b3Create(CFrameWnd *parent);
	void          b3SaveState();
	CB3Toolbar   *b3DockRight(CB3Toolbar *Left=null);
	void          b3DockSimple();
	void          b3RestoreState();
	b3_bool       b3InitCustomization();

protected:
	//{{AFX_MSG(b3Toolbar)
	//}}AFX_MSG
	afx_msg void OnToolBarQueryDelete(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarQueryInsert(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarChange(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarBeginDrag(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarEndDrag(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarBeginAdjust(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarCustomHelp(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarEndAdjust(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarGetButtonInfo(NMHDR *notify, LRESULT *result);
	afx_msg void OnToolBarReset(NMHDR *notify, LRESULT *result);

	DECLARE_MESSAGE_MAP()
private:
	void          b3Print(CRect &rect,long dw);
	void          b3AdjustComboboxes();
	void          b3Reset();
};

class CB3Menubar : public CMenuBar, public b3Link<CB3Menubar>
{
	CFrameWnd    *m_MenubarMain;
	b3_index      m_ID;
	b3_index      m_Bar;
	b3_bool       m_Visible;

public:
	              CB3Menubar();
	b3_bool       b3ToggleVisibility();
	b3_bool       b3SetVisibility(bool new_visibility=true);
	b3_bool       b3IsVisible();	 

protected:
	friend class  CB3ToolbarState;

	void          b3SetID(long id_menu,long id_bar);
	b3_bool       b3Create(CFrameWnd *parent);
};

class CB3ToolbarState
{
	b3Base<CB3Toolbar>  m_Toolbars;
	b3Base<CB3Menubar>  m_Menubars;
	CFrameWnd          *m_ToolbarMain;
	b3_count            m_MenuCount;
	b3_count            m_ToolCount;
	char                m_Code[128];
public:
	        CB3ToolbarState();
	void    b3AddToolbar(CB3Toolbar *toolbar,long id_bitmap,long id_title);
	void    b3AddMenubar(CB3Menubar *menubar,long id_menu);
	b3_bool b3CreateToolbars(CFrameWnd *parent);
	void    b3UpdateUI();
	void    b3LoadState();
	void    b3SaveState();

protected:
	b3_bool b3PreTranslateMsg(MSG *pMSG);
};

#endif
