/*
**
**	$Filename:	b3FileList.cc $  
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3FileList.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/01 12:24:58  sm
**	Initial revision
**
*/

/*************************************************************************
**                                                                      **
**                        b3FileEntry                                   **
**                                                                      **
*************************************************************************/

b3FileEntry::b3FileEntry (const char *new_name) :
	b3Link<b3FileEntry>(sizeof(b3FileEntry))
{
	strcpy (name,new_name);
}

int b3FileEntry::b3Cmp(b3FileEntry *compare)
{
	return strcmp(name,compare->name);
}

char *b3FileEntry::b3Name()
{
	return name;
}

/*************************************************************************
**                                                                      **
**                        MAUI b3FileList                             **
**                                                                      **
*************************************************************************/

static int b3SortFiles (
	b3FileEntry *a,
	b3FileEntry *b,
	void          *ptr)
{
	return a->b3Cmp(b);
}

b3FileList::b3FileList()
{
}

b3FileList::b3FileList(const char *dir)
{
	b3CreateList(dir);
}

b3FileList::~b3FileList()
{
	b3DeleteList();
}

void b3FileList::b3CreateList(const char *dir)
{
	b3DeleteList();
	b3RecCreateList(dir);
	b3Sort();
}

void b3FileList::b3DeleteList()
{
	b3FileEntry *entry;

	while ((entry = list.First) != null)
	{
		list.b3Remove(entry);
		delete entry;
	}
}

void b3FileList::b3RecCreateList(const char *startDir)
{
	b3Path       subdir,name;
	b3Dir        dir;
	b3_path_type type;
	b3_bool      loop = true;

	dir.b3OpenDir(startDir);
	do
	{
		type = dir.b3DirNext(name);
		b3Path::b3LinkFileName (subdir,startDir,name);

		switch (type)
		{
		case B3_TYPE_FILE:
			b3Add(subdir);
			break;

		case B3_TYPE_DIR:
			b3RecCreateList (subdir);
			break;

		case B3_NOT_EXISTANT:
		default :
			loop = false;
			break;
		}
	}
	while (loop);
	dir.b3CloseDir();
}

b3FileEntry *b3FileList::b3First()
{
	return list.First;
}

b3_bool b3FileList::b3Add(const char *name)
{
	b3FileEntry *entry;

	try
	{
		entry = new b3FileEntry(name);
	}
	catch(...)
	{
		entry = null;
	}
	if (entry)
	{
		list.b3Append(entry);
	}

	return entry != null;
}

void b3FileList::b3Sort()
{
	list.b3Sort(b3SortFiles,null);
}
