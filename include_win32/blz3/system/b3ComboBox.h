/*
**
**	$Filename:	b3ComboBox.h $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Flat combo box
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_SYSTEM_COMBOBOX_H
#define B3_SYSTEM_COMBOBOX_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Toolbar.h"
#include "../smart_gui/CJFlatComboBox.h"
#include "blz3/system/b3App.h"

class CB3ComboBox : public CCJFlatComboBox
{
	b3_s32        m_ID;
	CB3Toolbar   *m_Toolbar;

public:
	        CB3ComboBox();
	b3_bool b3Create(CB3Toolbar *toolbar,b3_s32 id,b3_res width=200,b3_bool sort=true);

	// Access functions
	void    b3Clear     ();
	b3_s32  b3AddString (const char    *text,void *ptr);
	b3_s32  b3FindString(const char    *text);
	b3_s32  b3SetString (const char    *text,b3_bool notify);
	void    b3Sort      (const b3_bool  sort=true);
};

#endif
