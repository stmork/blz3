/*
**
**	$Filename:	b3SelectColor.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting color
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SELECT_COLOR_H
#define B3_SELECT_COLOR_H

#include "blz3/system/b3ColorField.h"
#include "blz3/base/b3Color.h"

class CB3ColorFieldSelector : public CB3ColorField
{
	b3Color  *m_Color;
	CWnd     *m_Parent;

public:
	               CB3ColorFieldSelector();
	       void    b3Init(b3Color *color,CWnd *parent = null);
		   b3_bool b3Select();
	static b3_bool b3Select(b3Color &color,CWnd *parent = null);
};

#endif
