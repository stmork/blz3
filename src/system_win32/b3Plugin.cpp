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
**	Revision 1.15  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**
**	Revision 1.14  2004/05/10 15:12:09  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.13  2004/04/25 19:28:21  sm
**	- Added available b3Items as list to maintain dialog.
**	- Preview is done only on auto refresh activated.
**	
**	Revision 1.12  2004/04/25 13:40:59  sm
**	- Added file saving into registry
**	- Added last b3Item state saving for cloned b3Item
**	  creation.
**	- Now saving refresh state per b3Item dialog
**	
**	Revision 1.11  2003/06/20 09:02:45  sm
**	- Added material dialog skeletons
**	- Fixed ticket no. 10 (camera dialog handled camera
**	  dimension wring)
**	
**	Revision 1.10  2003/06/15 09:24:25  sm
**	- Added item creation dialog
**	
**	Revision 1.9  2003/06/09 17:33:30  sm
**	- New item maintainance dialog added.
**	
**	Revision 1.8  2003/06/07 11:40:10  sm
**	- Changed plugin interface:
**	  o Added icons
**	  o Added checksum/size/version in b3_plugin_info
**	  o Changed plugin method name
**	  o Changed b3Info signature for use of only one b3_plugin_info
**	
**	Revision 1.7  2003/06/01 13:36:03  sm
**	- Added define for exported plugin methods.
**	
**	Revision 1.6  2003/06/01 13:03:45  sm
**	- Is this the final plugin version?
**	
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

b3Loader b3Loader::m_Loader;
CString  b3Loader::m_UnknownDesc;
HICON    b3Loader::m_UnknownIcon = null;

b3Loader::b3Loader()
{
}

b3_bool b3Loader::b3IsPlugin(b3Path &library)
{
	b3Path ext(library);

	ext.b3ExtractExt();
	return stricmp(ext,"dll") == 0;
}

b3PluginBase *b3Loader::b3CreatePlugin(b3Path &library)
{
	return new b3Plugin(this,library);
}

b3Item *b3Loader::b3Create(b3_u32 class_type,void *ptr,b3_bool edit)
{
	b3Item *item = b3World::b3AllocNode(class_type);

	return b3EditCreation(item,ptr,edit);
}

b3Item *b3Loader::b3Create(b3_u32 *class_buffer,void *ptr,b3_bool edit)
{
	b3Item *item = b3World::b3AllocNode(class_buffer);

	return b3EditCreation(item,ptr,edit);
}

b3Item *b3Loader::b3EditCreation(b3Item *item,void *ptr,b3_bool edit)
{
	b3_plugin_info *info;

	if ((item != null) && edit)
	{
		info = b3FindInfo(item->b3GetClassType());
		if (info != null)
		{
			if (info->m_CreateFunc != null)
			{
				if (!info->m_CreateFunc(item, ptr))
				{
					delete item;
					item = null;
				}
			}
		}
	}
	return item;
}

b3_bool b3Loader::b3Edit(b3Item *item,void *ptr)
{
	b3_bool         result = false;
	b3_plugin_info *info;

	if (item != null)
	{
		info = b3FindInfo(item->b3GetClassType());
		if (info != null)
		{
			if (info->m_EditFunc != null)
			{
				result = info->m_EditFunc(item, ptr);
			}
		}
	}

	return result;
}

b3_plugin_info *b3Loader::b3FindInfo(b3Item *item)
{
	return item != null ? b3FindInfo(item->b3GetClassType()) : null;
}

b3_plugin_info *b3Loader::b3FindInfo(b3_u32 class_type)
{
	b3_count i;

	for (i = 0;i < m_InfoArray.b3GetCount();i++)
	{
		if (m_InfoArray[i].m_ClassType == class_type)
		{
			return &m_InfoArray[i];
		}
	}
	return null;
}

b3_count b3Loader::b3GetClassTypes(b3Array<b3_u32> &array,b3_u32 class_id)
{
	b3_count i;

	array.b3Clear();
	for (i = 0;i < m_InfoArray.b3GetCount();i++)
	{
		if (b3Item::b3IsClass(m_InfoArray[i].m_ClassType,class_id))
		{
			array.b3Add(m_InfoArray[i].m_ClassType);
		}
	}
	return array.b3GetCount();
}

b3_bool b3Loader::b3AddPluginInfo(b3_plugin_info *info)
{
	b3_bool result = false;

	if (info != null)
	{
		if (b3Plugin::b3IsValid(info))
		{
			m_InfoArray.b3Add(*info);
			b3PrintF(B3LOG_DEBUG,"  class: %08x V%d \"%s\" %s create option and %s edit option\n",
				info->m_ClassType,
				info->m_Version,
				info->m_Description == null ? "(unnamed)" : info->m_Description,
				info->m_CreateFunc != null ? "with" : "without",
				info->m_EditFunc   != null ? "with" : "without");
			result = true;
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"  info class%08x/V%d invalid!\n",
				info->m_ClassType,
				info->m_Version);
		}
	}
	return result;
}

void b3Loader::b3SetUnknownIds(
	int                   DescID,
	int                   IconID)
{
	m_UnknownDesc.LoadString(DescID);
	m_UnknownIcon = AfxGetApp()->LoadIcon(IconID);
}

void b3Loader::b3AddClassType(
	b3_u32                class_type,
	int                   DescID,
	int                   IconID,
	b3_plugin_create_func CreateFunc,
	b3_plugin_edit_func   EditFunc)
{
	b3_plugin_info info;
	CString        description;
	
	b3Plugin::b3InitPluginInfo(&info);

	if (DescID != 0)
	{
		description.LoadString(DescID);
		strcpy(info.m_Description,description);
	}
	else
	{
		strcpy(info.m_Description,m_UnknownDesc);
	}

	info.m_Icon       = (IconID != 0 ? AfxGetApp()->LoadIcon(IconID) : m_UnknownIcon);
	info.m_CreateFunc = CreateFunc;
	info.m_EditFunc   = EditFunc;
	info.m_ClassType  = class_type;
	
	b3Plugin::b3SetChecksum(&info);
	
	b3GetLoader().b3AddPluginInfo(&info);
}

/*************************************************************************
**                                                                      **
**                        The Blizzard III plugin functions             **
**                                                                      **
*************************************************************************/

b3Plugin::b3Plugin(b3Loader *loader,b3Path &library) : b3PluginBase(library)
{
	b3_count        i = 0;
	b3_plugin_info *info;

	m_Handle    = AfxLoadLibrary(m_PluginPath);
	if (m_Handle != null)
	{
		b3_plugin_init_func b3Init = (b3_plugin_init_func)GetProcAddress(m_Handle,B3_PLUGIN_INIT_FUNC);
		b3_plugin_info_func b3Info = (b3_plugin_info_func)GetProcAddress(m_Handle,B3_PLUGIN_INFO_FUNC);

		b3PrintF(B3LOG_DEBUG,"Plugin %s loaded.\n",(const char *)m_PluginPath);

		if(b3Init != null)
		{
			b3Init();
			b3PrintF(B3LOG_DEBUG,"Plugin %s initialized.\n",(const char *)m_PluginPath);

			if (b3Info != null)
			{
				b3PrintF(B3LOG_DEBUG,"Got infos of plugin %s.\n",(const char *)m_PluginPath);
				do
				{
					info = b3Info(i++);
					loader->b3AddPluginInfo(info);
				}
				while(info != null);
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

b3Plugin::~b3Plugin()
{
	if (m_Handle != null)
	{
		AfxFreeLibrary(m_Handle);
		b3PrintF(B3LOG_DEBUG,"Plugin %s unloaded.\n",(const char *)m_PluginPath);
	}
}

b3_bool b3Plugin::b3IsValid(b3_plugin_info *info)
{
	b3_u32    checksum = 0,old;
	b3_u08   *ptr = (b3_u08 *)info;
	b3_size   i;

	// If the plugin is newer than the application: Don't use!
	if (info->m_Version > B3_PLUGIN_INFO_VERSION)
	{
		return false;
	}

	// If the plugin info is greater than of the application: Don't use!
	if (info->m_Size    > sizeof(b3_plugin_info))
	{
		return false;
	}

	// Compute negative checksum of itself
	old = info->m_CheckSum;
	for (i = 0; i < 4;i++)
	{
		checksum -= (old & 0xff);
		old = old >> 8;
	}

	// Add all info bytes
	for (i = 0;i < info->m_Size;i++)
	{
		checksum += *ptr++;
	}
	return checksum == info->m_CheckSum;
}
