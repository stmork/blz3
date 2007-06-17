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

#include "blz3/system/b3Dir.h"
#include "blz3/image/b3Tx.h"

class CB3SelectLoadObject
{
public:
	static b3_bool b3Select(b3Path &name,const char *reg_entry);
};

class CB3SelectSaveObject
{
	static const char *m_RegEntry;

public:
	static b3_bool b3Select(b3Path &name,const char *box_name);
};

class CB3SelectLoadCOB
{
	static const char *m_RegEntry;

public:
	static b3_bool b3Select(b3Path &name);
};

class CB3SelectLoadArcon
{					
	static const char *m_RegEntry;

public:
	static b3_bool b3Select(b3Path &path);
};

#endif
