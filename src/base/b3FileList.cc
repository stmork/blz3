/*
**
**	$Filename:	b3FileList.cc $
**	$Release:	Dortmund 2001, 2016 $
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

#include "b3BaseInclude.h"
#include "blz3/base/b3FileList.h"

/*************************************************************************
**                                                                      **
**                        b3FileEntry                                   **
**                                                                      **
*************************************************************************/

b3FileEntry::b3FileEntry(const char * new_name) :
	b3Link<b3FileEntry>(sizeof(b3FileEntry))
{
	name.b3Format("%s", new_name);
}

int b3FileEntry::b3Cmp(const b3FileEntry * compare) const
{
	return strcoll(b3Name(), compare->b3Name());
}

const char * b3FileEntry::b3Name() const
{
	return name;
}

/*************************************************************************
**                                                                      **
**                        Blizzard III b3FileList                       **
**                                                                      **
*************************************************************************/

b3FileList::b3FileList()
{
}

b3FileList::b3FileList(const char * dir)
{
	b3CreateList(dir);
}

b3FileList::~b3FileList()
{
	b3DeleteList();
}

void b3FileList::b3CreateList(const char * dir)
{
	b3DeleteList();
	b3RecCreateList(dir);
	b3Sort();
}

void b3FileList::b3DeleteList()
{
	list.b3Free();
}

void b3FileList::b3RecCreateList(const char * startDir)
{
	b3Path       subdir, name;
	b3Dir        dir;
	b3_path_type type;
	b3_bool      loop = true;

	dir.b3OpenDir(startDir);
	do
	{
		type = dir.b3DirNext(name);

		switch (type)
		{
		case B3_TYPE_FILE:
			b3Path::b3LinkFileName(subdir, startDir, name);
			b3Add(subdir);
			break;

		case B3_TYPE_DIR:
			b3Path::b3LinkFileName(subdir, startDir, name);
			b3RecCreateList(subdir);
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

b3FileEntry * b3FileList::b3First() const
{
	return list.First;
}

bool b3FileList::b3Add(const char * name)
{
	b3FileEntry * entry;

	try
	{
		entry = new b3FileEntry(name);
		list.b3Append(entry);
	}
	catch (std::bad_alloc & e)
	{
		entry = nullptr;
	}

	return entry != nullptr;
}

void b3FileList::b3Sort()
{
	list.b3Sort(&b3FileEntry::b3Sort);
}
