/*
**
**	$Filename:	b3SearchPath.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Search path routines
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

#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2002/11/16 15:24:17  sm
**	- Include cleanups in RemCR
**
**	Revision 1.8  2002/11/01 12:49:07  sm
**	- Some b3SearchPath constructor refinements.
**	- Fixed texture path configuration.
**	
**	Revision 1.7  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.6  2002/02/14 16:51:49  sm
**	- Done some cleanups concernig doubly linked lists. Moved
**	  obsolete b3Node/b3Head into their own include.
**	
**	Revision 1.5  2002/01/01 13:50:21  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.4  2001/12/06 16:21:56  sm
**	- Finished CB3ControlLDC - very nice!
**	
**	Revision 1.3  2001/11/11 14:07:15  sm
**	- Adjusted b3Path to Un*x port.
**	
**	Revision 1.2  2001/11/11 11:51:21  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**	Revision 1.1  2001/10/26 18:37:14  sm
**	- Creating search path support
**	- Splitting image pool support and image loading into
**	  their own area.
**	- Fixed JPEG to support b3Tx::b3AllocTx()
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Texture maintainance                          **
**                                                                      **
*************************************************************************/

b3PathEntry::b3PathEntry(const b3PathEntry &src) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	strcpy(m_Path,src.m_Path);
}

b3PathEntry::b3PathEntry(const b3PathEntry *src) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	strcpy(m_Path,src != null ? src->m_Path : "");
}

b3PathEntry::b3PathEntry(const char *new_path) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	strcpy(m_Path,new_path != null ? new_path : "");
}

void b3SearchPath::b3AddPath(const char *path)
{
	b3PathEntry *path_item = new b3PathEntry(path);

	b3PrintF(B3LOG_DEBUG,"Adding search path \"%s\"\n",
		(const char *)*path_item);
	m_SearchPath.b3Append(path_item);
}

b3SearchPath::b3SearchPath()
{
}

b3SearchPath::~b3SearchPath()
{
	b3Empty();
}

void b3SearchPath::b3Empty()
{
	b3PathEntry *path_item;

	while((path_item = m_SearchPath.First) != null)
	{
		m_SearchPath.b3Remove(path_item);
		delete path_item;
	}
}

b3_bool b3SearchPath::b3IsValid(const char *Name,char *FullName)
{
	b3PathEntry *path;

	if ((Name != null) && (strlen(Name) > 0))
	{
		b3PrintF(B3LOG_FULL,"Trying \"%s\"...\n",(char *)Name);
		if (b3Dir::b3Exists(Name) == B3_TYPE_FILE)
		{
			strcpy(FullName,Name);
			return true;
		}
		else
		{
			B3_FOR_BASE(&m_SearchPath,path)
			{
				b3Path::b3LinkFileName(
					(char *)FullName,
					(const char *)*path,Name);
				b3PrintF(B3LOG_FULL,"Trying \"%s\"...",(const char *)FullName);
				if (b3Dir::b3Exists(FullName) == B3_TYPE_FILE)
				{
					b3PrintF(B3LOG_FULL," OK!\n");
					return true;
				}
				else
				{
					b3PrintF(B3LOG_FULL,"\n");
				}
			}
		}
	}

	return false;
}

b3_bool b3SearchPath::b3CutName(const char *fullname,char *result)
{
	b3PathEntry *path;
	b3_size      fullLen,shortLen,diff;

	B3_FOR_BASE(&m_SearchPath,path)
	{
		fullLen  = strlen(fullname);
		shortLen = strlen(*path);
		diff    = fullLen - shortLen;
		if (strncmp(fullname,*path,shortLen) == 0)
		{
			while(((fullname[shortLen] == '/') ||
				   (fullname[shortLen] == '\\')) &&
				   (fullname[shortLen] != 0))
			{
				shortLen++;
			}
			strcpy (result,&fullname[shortLen]);
			return true;
		}
	}
	strcpy (result,fullname);
	return false;
}
