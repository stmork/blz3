/*
**
**	$Filename:	b3SelectObject.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Selecting an object
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SELECT_OBJECT_H
#define B3_SELECT_OBJECT_H

#include "blz3/image/b3Tx.h"

class CB3SelectObject
{
public:
	static b3_bool b3Select(char *name);
};

class CB3SelectCOB
{
public:
	static b3_bool b3Select(char *name);
};

class CB3SelectArcon
{
public:
	static b3_bool b3Select(char *name);
};

#endif
