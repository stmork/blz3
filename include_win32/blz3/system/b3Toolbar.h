/*
**
**	$Filename:	b3Toolbar.h $  
**	$Release:	Dortmund 2001 $
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

#define FLAT_TOOLBAR_HEIGHT 28

class CB3ToolbarInfo
{
public:
	TBBUTTON  tbButton;		// information regarding the button
	char     *btnText;		// text for the button
	long      width;

	inline CB3ToolbarInfo()
	{
		btnText = null;
	}

	virtual ~CB3ToolbarInfo()
	{
		b3FreeText();
	}

	inline void b3FreeText()
	{
		if (btnText != null)
		{
			delete [] btnText;
			btnText = null;
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
	DECLARE_DYNAMIC(CB3Toolbar)

	CFrameWnd        *m_MainFrame;
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
	virtual      ~CB3Toolbar();
	b3_bool       b3ToggleVisibility();
	b3_bool       b3SetVisibility(bool new_visibility=true);
	b3_bool       b3IsVisible();
	void          b3AddArrow(int id);
	void          b3Customize();
	void          b3Free();

protected:
	friend class  CB3ToolbarState;

	void          b3SetID(long id_bitmap,long id_title,long id_bar);
	b3_bool       b3Create(CFrameWnd *parent);
	CControlBar  *b3DockRight(CControlBar *Left=null);
	void          b3DockSimple();
	void          b3SaveState(const char *name = null);
	void          b3RestoreState(const char *name = null);
	b3_bool       b3InitCustomization();

protected:
	//{{AFX_MSG(CB3Toolbar)
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
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void          b3Print(CRect &rect,long dw);
	void          b3AdjustComboboxes();
	void          b3Reset();
	CString      &b3GetValueName(const char *name,CString &result);
};

class CB3Menubar : public CMenuBar, public b3Link<CB3Menubar>
{
	DECLARE_DYNAMIC(CB3Menubar)

	CFrameWnd    *m_MainFrame;
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

protected:
	// Generated message map functions
	//{{AFX_MSG(CB3Menubar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CB3Dialogbar : public CDialogBar, public b3Link<CB3Dialogbar>
{
	DECLARE_DYNAMIC(CB3Dialogbar)

	CFrameWnd    *m_MainFrame;
	b3_index      m_ID;
	b3_index      m_Bar;
	b3_bool       m_Visible;
	CString       m_Title;
	CString       m_Key;
	CString       m_Value;

public:
	              CB3Dialogbar();
	        BOOL  Create(CWnd * pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	        BOOL  Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID);
	virtual BOOL  OnInitDialog();
	b3_bool       b3ToggleVisibility();
	b3_bool       b3SetVisibility(bool new_visibility=true);
	b3_bool       b3IsVisible();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3Dialogbar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL


protected:
	// Generated message map functions
	//{{AFX_MSG(CB3Dialogbar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void  b3GetData();
	virtual void  b3SetData();

protected:
	friend class  CB3ToolbarState;

	void          b3SetID(long id_bitmap,long id_title,long id_bar);
	b3_bool       b3Create(CFrameWnd *parent);
	CControlBar  *b3DockRight(CControlBar *Left=null);
	void          b3DockSimple();
	void          b3SaveState(const char *name = null);
	void          b3RestoreState(const char *name = null);
};

class CB3ToolbarState
{
	b3Base<CB3Toolbar>    m_Toolbars;
	b3Base<CB3Menubar>    m_Menubars;
	b3Base<CB3Dialogbar>  m_Dialogbars;
	CFrameWnd            *m_MainFrame;
	b3_count              m_MenuCount;
	b3_count              m_ToolCount;
	char                  m_Code[128];
public:
	        CB3ToolbarState(const char *appName);
	void    b3AddToolbar(CB3Toolbar *toolbar,long id_bitmap,long id_title);
	void    b3AddMenubar(CB3Menubar *menubar,long id_menu);
	void    b3AddDialogbar(CB3Dialogbar *dialogbar,long id_dialog,long id_title);
	b3_bool b3CreateToolbars(CFrameWnd *parent);
	void    b3UpdateUI();
	void    b3LoadState(const char *name = null);
	void    b3SaveState(const char *name = null);
	void    b3GetData();
	void    b3SetData();
	void    b3Free();

protected:
	b3_bool b3PreTranslateMsg(MSG *pMSG);
};

#endif
