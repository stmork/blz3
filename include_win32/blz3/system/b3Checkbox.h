/*
**
**	$Filename:	b3Checkbox.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1438 $
**	$Date: 2006-06-01 16:17:24 +0200 (Do, 01 Jun 2006) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Checkbox control
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_CHECKBOX_H
#define B3_SYSTEM_CHECKBOX_H

#pragma once

#include "blz3/b3Config.h"

class B3_PLUGIN CB3Checkbox :
	public CButton
{
public:
	CB3Checkbox(void);
	~CB3Checkbox(void);
	void    b3DDX(CDataExchange *pDX,b3_bool &value);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3Checkbox)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CB3Checkbox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
