/*
**
**	$Filename:	b3SearchPath.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Search path routines
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_SEARCHPATH_H
#define B3_BASE_SEARCHPATH_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"
#include "blz3/system/b3Dir.h"

class B3_PLUGIN b3PathEntry : public b3Link<b3PathEntry>, public b3Path
{
public:
	b3PathEntry(const b3PathEntry &src);
	b3PathEntry(const b3PathEntry *src);
	b3PathEntry(const char *path);
};

class B3_PLUGIN b3SearchPath
{
public:
	b3Base<b3PathEntry> m_SearchPath;

public:
	         b3SearchPath();
	virtual ~b3SearchPath();
	void     b3AddPath(const char *path);
	void     b3Empty();
	b3_bool  b3IsValid(const char *short_name,char *result_as_full_name);
	b3_bool  b3CutName(const char *file_name,char *result_as_short_name);
};

#endif
