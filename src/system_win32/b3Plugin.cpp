/*
**
**	$Filename:	b3Plugin.cpp $
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

#include "blz3/system/b3Plugin.h"
#include "blz3/base/b3FileList.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/05/24 14:00:36  sm
**	- Added plugin base classes for system independend support
**
**	Revision 1.1  2003/05/24 13:46:49  sm
**	- Added plugin support
**	- Fixed b3FileList on non existing directory.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        The Blizzard III loader functions             **
**                                                                      **
*************************************************************************/

b3_bool b3Loader::b3IsPlugin(b3Path &library)
{
	b3Path ext(library);

	ext.b3ExtractExt();
	return stricmp(ext,"dll") == 0;
}

/*************************************************************************
**                                                                      **
**                        The Blizzard III plugin functions             **
**                                                                      **
*************************************************************************/

b3Plugin::b3Plugin(b3Path &library) : b3PluginBase(library)
{
	m_Handle = LoadLibrary(library);
	if (m_Handle != null)
	{
		b3_plugin_init_func init;

		b3PrintF(B3LOG_DEBUG,"Plugin %s loaded.\n",(const char *)m_PluginPath);

		init = (b3_plugin_init_func)GetProcAddress(m_Handle,"init");
		if(init != null)
		{
			init();
			b3PrintF(B3LOG_DEBUG,"Plugin %s initialized.\n",(const char *)m_PluginPath);
		}
	}
}

b3Plugin::~b3Plugin()
{
	if (m_Handle != null)
	{
		FreeLibrary(m_Handle);
		b3PrintF(B3LOG_DEBUG,"Plugin %s unloaded.\n",(const char *)m_PluginPath);
	}
}
