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
**	Revision 1.5  2003/06/01 12:17:31  sm
**	- Some plugin movements
**
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

b3Item *b3Loader::b3Create(b3_u32 class_type)
{
	return b3World::b3AllocNode(class_type);
}

b3_bool b3Loader::b3Edit(b3Item *item)
{
	return false;
}

/*************************************************************************
**                                                                      **
**                        The Blizzard III plugin functions             **
**                                                                      **
*************************************************************************/

b3Plugin::b3Plugin(b3Path &library) : b3PluginBase(library)
{
}

void b3Plugin::b3Load()
{
	b3_count i;

	m_InfoArray = null;
	m_Handle    = AfxLoadLibrary(m_PluginPath);
	if (m_Handle != null)
	{
		b3_plugin_init_func init = (b3_plugin_init_func)GetProcAddress(m_Handle,"init");
		b3_plugin_info_func info = (b3_plugin_info_func)GetProcAddress(m_Handle,"info");

		b3PrintF(B3LOG_DEBUG,"Plugin %s loaded.\n",(const char *)m_PluginPath);

		if(init != null)
		{
			init();
			b3PrintF(B3LOG_DEBUG,"Plugin %s initialized.\n",(const char *)m_PluginPath);

			if (info != null)
			{
				info(m_InfoArray);
				b3PrintF(B3LOG_DEBUG,"Got infos of plugin %s.\n",(const char *)m_PluginPath);
				for(i = 0;i < m_InfoArray.b3GetCount();i++)
				{
					b3PrintF(B3LOG_DEBUG,"  class: %08x %s %s edit option\n",
						m_InfoArray[i].m_ClassType,
						m_InfoArray[i].m_Description == null ? "(unnamed)" : m_InfoArray[i].m_Description,
						m_InfoArray[i].m_EditFunc != null ? "with" : "without");
				}
			}
			else
			{
				b3PrintF(B3LOG_NORMAL,"No infos about plugin %s available!\n",(const char *)m_PluginPath);
			}
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

void b3Plugin::b3Unload()
{
	m_InfoArray.b3Clear(true);
	if (m_Handle != null)
	{
		AfxFreeLibrary(m_Handle);
		b3PrintF(B3LOG_DEBUG,"Plugin %s unloaded.\n",(const char *)m_PluginPath);
	}
}
