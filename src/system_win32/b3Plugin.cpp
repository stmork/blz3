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
**	Revision 1.4  2003/05/29 12:31:46  sm
**	- Added error messages
**
**	Revision 1.3  2003/05/24 14:10:45  sm
**	- Updated platform independend support
**	
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

b3PluginBase *b3Loader::b3CreatePlugin(b3Path &library)
{
	return new b3Plugin(library);
}

/*************************************************************************
**                                                                      **
**                        The Blizzard III plugin functions             **
**                                                                      **
*************************************************************************/

b3Plugin::b3Plugin(b3Path &library) : b3PluginBase(library)
{
	m_Handle = AfxLoadLibrary(library);
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
		else
		{
			b3PrintF(B3LOG_NORMAL,"Init method for plugin %s failed to load!\n",(const char *)m_PluginPath);
		}
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Plugin %s failed to load!\n",(const char *)m_PluginPath);
	}
}

b3Plugin::~b3Plugin()
{
	if (m_Handle != null)
	{
		AfxFreeLibrary(m_Handle);
		b3PrintF(B3LOG_DEBUG,"Plugin %s unloaded.\n",(const char *)m_PluginPath);
	}
}
