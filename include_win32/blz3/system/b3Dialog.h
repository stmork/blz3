/*
**
**	$Filename:	b3Dialog.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple dialog
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DIALOG_H
#define B3_SYSTEM_DIALOG_H

#pragma once
#include "blz3/b3Config.h"

class B3_PLUGIN CB3Dialog : public CDialog
{
public:
	CB3Dialog(int dlgId,CWnd* pParent = NULL);   // standard constructor
	virtual ~CB3Dialog(void);

// Dialog Data
	//{{AFX_DATA(CB3Dialog)
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3Dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CB3Dialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void b3PreInitDialog() = 0;
	virtual void b3PostInitDialog() = 0;
	virtual void b3UpdateUI() = 0;
};

#endif
