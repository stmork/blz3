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

b3PathEntry::b3PathEntry(const char *new_path) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	strcpy(path,new_path != null ? new_path : "");
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
	m_SearchPath.b3InitBase();
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
				b3PrintF(B3LOG_FULL,"Trying \"%s\"...\n",(const char *)FullName);
				if (b3Dir::b3Exists(FullName) == B3_TYPE_FILE)
				{
					return true;
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
