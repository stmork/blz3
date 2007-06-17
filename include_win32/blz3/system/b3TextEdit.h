/*
**
**	$Filename:	b3TextEdit.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1438 $
**	$Date: 2006-06-01 16:17:24 +0200 (Do, 01 Jun 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Editing text
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_TEXT_H
#define B3_SYSTEM_TEXT_H

#pragma once

#include "blz3/b3Config.h"

class B3_PLUGIN CB3TextEdit : public CEdit
{
public:
	CB3TextEdit(void);
	~CB3TextEdit(void);
	void    b3DDX(CDataExchange *pDX, char *value, int size);
	void    b3DDX(CDataExchange *pDX, CString &value, int size);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3TextEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CB3TextEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
