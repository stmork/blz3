/*
**
**	$Filename:	b3Plugin.cc $
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

#include <dlfcn.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2003/06/07 11:58:07  sm
**	- Changed plugin init method name to b3Init()
**
**	Revision 1.4  2003/06/01 13:03:45  sm
**	- Is this the final plugin version?
**	
**	Revision 1.3  2003/06/01 12:07:01  sm
**	- Minor plugin changes
**	
**	Revision 1.2  2003/05/25 16:47:31  sm
**	- Added error messages in case a plugin fails.
**	
**	Revision 1.1  2003/05/24 16:37:06  sm
**	- Added plugin support for Un*x
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
	return stricmp(ext,"so") == 0;
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
	m_Handle = dlopen(m_PluginPath,RTLD_LAZY | RTLD_GLOBAL);
	if (m_Handle != null)
	{
		b3_plugin_init_func init;

		b3PrintF(B3LOG_DEBUG,"Plugin %s loaded.\n",(const char *)m_PluginPath);

		init = (b3_plugin_init_func)dlsym(m_Handle,B3_PLUGIN_INIT_FUNC);
		if(init != null)
		{
			init();
			b3PrintF(B3LOG_DEBUG,"Plugin %s initialized.\n",
				(const char *)m_PluginPath);
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"Plugin %s doesn't have init method!\n",
				(const char *)m_PluginPath);
		}
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Problems loading plugin %s:\n",(const char *)m_PluginPath);
		b3PrintF(B3LOG_NORMAL,"  %s\n",dlerror());
	}
}

b3Plugin::~b3Plugin()
{
	if (m_Handle != null)
	{
		dlclose(m_Handle);
		b3PrintF(B3LOG_DEBUG,"Plugin %s unloaded.\n",(const char *)m_PluginPath);
	}
}