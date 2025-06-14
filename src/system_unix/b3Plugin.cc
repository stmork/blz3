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
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3FileList.h"

#include <dlfcn.h>

/*************************************************************************
**                                                                      **
**                        The Blizzard III loader functions             **
**                                                                      **
*************************************************************************/

b3_bool b3Loader::b3IsPlugin(b3Path & library)
{
	b3Path ext(library);

	ext.b3ExtractExt();
	return b3StringTool::b3CaseCompare(ext, "so") == 0;
}

b3PluginBase * b3Loader::b3CreatePlugin(b3Path & library)
{
	return new b3Plugin(library);
}

/*************************************************************************
**                                                                      **
**                        The Blizzard III plugin functions             **
**                                                                      **
*************************************************************************/

b3Plugin::b3Plugin(b3Path & library) : b3PluginBase(library)
{
	const char * path = m_PluginPath;

	m_Handle = dlopen(m_PluginPath, RTLD_LAZY | RTLD_GLOBAL);
	if (m_Handle != nullptr)
	{
		b3PrintF(B3LOG_DEBUG, "Plugin %s loaded.\n", path);

		// cppcheck-suppress cstyleCast
		b3_plugin_init_func init = (b3_plugin_init_func)dlsym(m_Handle, B3_PLUGIN_INIT_FUNC);

		if (init != nullptr)
		{
			init();
			b3PrintF(B3LOG_DEBUG, "Plugin %s initialized.\n", path);
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Plugin %s doesn't have init method!\n",
				path);
		}
	}
	else
	{
		b3PrintF(B3LOG_NORMAL, "Problems loading plugin %s:\n", path);
		b3PrintF(B3LOG_NORMAL, "  %s\n", dlerror());
	}
}

b3Plugin::~b3Plugin()
{
	if (m_Handle != nullptr)
	{
		const char * path = m_PluginPath;

		dlclose(m_Handle);
		b3PrintF(B3LOG_DEBUG, "Plugin %s unloaded.\n", path);
	}
}
