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

#include "b3BaseInclude.h"
#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Texture maintainance                          **
**                                                                      **
*************************************************************************/

b3PathEntry::b3PathEntry(const b3PathEntry &src) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	snprintf(m_Path,sizeof(m_Path),"%s",src.m_Path);
}

b3PathEntry::b3PathEntry(const b3PathEntry *src) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	snprintf(m_Path,sizeof(m_Path),"%s",src != null ? src->m_Path : "");
}

b3PathEntry::b3PathEntry(const char *new_path) : b3Link<b3PathEntry>(sizeof(b3PathEntry))
{
	snprintf(m_Path,sizeof(m_Path),"%s",new_path != null ? new_path : "");
}

void b3SearchPath::b3AddPath(const char *path)
{
	if (b3Dir::b3Exists(path) == B3_TYPE_DIR)
	{
		b3PathEntry *path_item = new b3PathEntry(path);

		b3PrintF(B3LOG_DEBUG,"Adding search path \"%s\"\n",
			(const char *)*path_item);
		m_SearchPath.b3Append(path_item);
	}
#ifdef _DEBUG
	else
	{
		b3PrintF(B3LOG_DEBUG,"Path %s not added.\n",path);
	}
#endif
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
	m_SearchPath.b3Free();
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
