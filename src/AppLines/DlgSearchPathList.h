/*
**
**	$Filename:	DlgSearchPathList.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Changing path list
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_DLGSEARCHPATHLIST_H__E8D477C3_9420_11D3_A435_0080AD1927F8__INCLUDED_)
#define AFX_DLGSEARCHPATHLIST_H__E8D477C3_9420_11D3_A435_0080AD1927F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectList.h : header file
//

#include "AppLines.h"
#include "blz3/b3Config.h"
#include "blz3/base/b3SearchPath.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectList dialog

class CDlgSearchPathList : public CDialog
{
	// Construction
public:
	b3SearchPath * m_SearchPath;
	b3_bool       m_IsList;
	CDlgSearchPathList(CWnd * pParent = NULL);  // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgSearchPathList)
	enum { IDD = IDD_SEARCH_PATH_LIST };
	CComboBox	m_List;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSearchPathList)
protected:
	virtual void DoDataExchange(CDataExchange * pDX);   // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSearchPathList)
	virtual BOOL OnInitDialog();
	afx_msg void OnPathFirst();
	afx_msg void OnPathPrev();
	afx_msg void OnPathSucc();
	afx_msg void OnPathLast();
	afx_msg void OnPathNew();
	afx_msg void OnPathDelete();
	afx_msg void OnSelchangePath();
	afx_msg void OnKillfocusPath();
	afx_msg void OnPathEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void b3UpdateUI();
	void b3SetSelection(int index);
	void b3RefreshList();
	int m_Index;
	b3PathEntry * m_PathEntry;
	b3Base<b3PathEntry> * m_ListBase;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTLIST_H__E8D477C3_9420_11D3_A435_0080AD1927F8__INCLUDED_)
