/*
**
**	$Filename:	b3FileList.h $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - file list creation
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_FILELIST_H
#define B3_BASE_FILELIST_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/base/b3Compare.h"
#include "blz3/base/b3List.h"

class B3_PLUGIN b3FileEntry : public b3Link<b3FileEntry>, public b3Comparator<b3FileEntry>
{
	b3Path       name;
public:
	             b3FileEntry(const char *new_name);
	int          b3Cmp(b3FileEntry *compare);
	const char  *b3Name();
};

class b3FileList
{
	b3Base<b3FileEntry> list;
public:
	               b3FileList();
	               b3FileList(const char *dir_name);
	              ~b3FileList();
	void           b3CreateList(const char *dir_name);
	void           b3DeleteList();
	b3FileEntry   *b3First();
	b3_bool        b3Add(const char *file_name);
	void           b3Sort();

private:
	void           b3RecCreateList(const char *);
};

#endif
