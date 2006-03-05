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

#include "b3BaseInclude.h"
#include "blz3/base/b3FileList.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 21:22:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2006/02/05 18:57:14  sm
**	- Added locale support.
**	
**	Revision 1.7  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.6  2003/05/24 13:46:49  sm
**	- Added plugin support
**	- Fixed b3FileList on non existing directory.
**	
**	Revision 1.5  2002/08/16 13:20:13  sm
**	- Removed some unused methods.
**	- Allocation bug found in brt3 - the Un*x version of the
**	  Blizzard III raytracer: It's necessary to use b3ShapeRenderContext
**	  rather than b3renderContext which doesn't initialize subdivision
**	  for shapes.
**	
**	Revision 1.4  2002/07/30 21:46:24  sm
**	- More powerful pixel format selection.
**	- Added b3Comparator class for sorting.
**	
**	Revision 1.3  2001/10/15 14:45:07  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
**	Revision 1.2  2001/07/01 20:29:21  sm
**	- Changing some comments
**	
**	Revision 1.1.1.1  2001/07/01 12:24:58  sm
**	Blizzard III is born
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
	name.b3Format("%s",new_name);
}

int b3FileEntry::b3Cmp(b3FileEntry *compare)
{
	return strcoll(b3Name(),compare->b3Name());
}

const char *b3FileEntry::b3Name()
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
	list.b3Free();
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

		switch (type)
		{
		case B3_TYPE_FILE:
			b3Path::b3LinkFileName (subdir,startDir,name);
			b3Add(subdir);
			break;

		case B3_TYPE_DIR:
			b3Path::b3LinkFileName (subdir,startDir,name);
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
		list.b3Append(entry);
	}
	catch(...)
	{
		entry = null;
	}

	return entry != null;
}

void b3FileList::b3Sort()
{
	list.b3Sort(&b3FileEntry::b3Sort);
}
