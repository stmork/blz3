/*
**
**	$Filename:	b3PluginBase.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Plugin support
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemIndInclude.h"
#include "blz3/system/b3PluginBase.h"
#include "blz3/base/b3FileList.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2004/01/18 13:51:58  sm
**	- Done further security issues.
**	
**	Revision 1.1  2003/05/24 14:00:36  sm
**	- Added plugin base classes for system independend support
**	#
*/

/*************************************************************************
**                                                                      **
**                        The Blizzard III loader functions             **
**                                                                      **
*************************************************************************/

b3LoaderBase::~b3LoaderBase()
{
	b3Unload();
}

void b3LoaderBase::b3Load()
{
	b3PathEntry  *entry;
	b3FileList    list;
	b3FileEntry  *file;
	b3PluginBase *plugin;
	b3Path        name;

	B3_FOR_BASE(&m_SearchPath,entry)
	{
		list.b3CreateList(*entry);

		for (file = list.b3First();file != null;file = file->Succ)
		{
			strlcpy(name,file->b3Name(),B3_FILESTRINGLEN);
			if (b3IsPlugin(name))
			{
				plugin = b3CreatePlugin(name);
				m_PluginList.b3Append(plugin);
			}
		}
	}
}

void b3LoaderBase::b3Unload()
{
	m_PluginList.b3Free();
}

/*************************************************************************
**                                                                      **
**                        The Blizzard III plugin functions             **
**                                                                      **
*************************************************************************/

b3PluginBase::b3PluginBase(b3Path &library) :
	b3Link<b3PluginBase>(sizeof(b3PluginBase)),
	m_PluginPath(library)
{
}
