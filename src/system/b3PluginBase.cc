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
**                        The Blizzard III loader functions             **
**                                                                      **
*************************************************************************/

b3LoaderBase::~b3LoaderBase()
{
	b3Unload();
}

void b3LoaderBase::b3Load()
{
	b3PathEntry * entry;
	b3FileList    list;
	b3FileEntry * file;
	b3PluginBase * plugin;
	b3Path        name;

	B3_FOR_BASE(&m_SearchPath, entry)
	{
		list.b3CreateList(*entry);

		for (file = list.b3First(); file != null; file = file->Succ)
		{
			strlcpy(name, file->b3Name(), B3_FILESTRINGLEN);
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

b3PluginBase::b3PluginBase(b3Path & library) :
	b3Link<b3PluginBase>(sizeof(b3PluginBase)),
	m_PluginPath(library)
{
}
