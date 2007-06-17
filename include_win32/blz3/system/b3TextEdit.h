/*
**
**	$Filename:	b3TextEdit.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
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
