/*
**
**	$Filename:	b3Plugin.h $
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

#ifndef B3_SYSTEM_PLUGIN_H
#define B3_SYSTEM_PLUGIN_H

#include "blz3/system/b3PluginBase.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3World.h"

#define B3_PLUGIN_INIT_FUNC "b3Init"
#define B3_PLUGIN_INFO_FUNC "b3Info"

#define B3_PLUGIN_INFO_VERSION   1
#define B3_PLUGIN_DESC_LEN     128

#define B3_PLUGIN_RANGE_CHECK(i,max) (((i) >= 0) && ((i) < (max)))

typedef b3_bool          (*b3_plugin_create_func)(b3Item *item,void *ptr);
typedef b3_bool          (*b3_plugin_edit_func)(b3Item *item,void *ptr);

struct b3_plugin_info
{
	b3_u32                 m_Version;
	b3_size                m_Size;
	b3_u32                 m_CheckSum;
	b3_u32                 m_ClassType;
	char                   m_Description[B3_PLUGIN_DESC_LEN];
	HICON                  m_Icon;
	b3_plugin_create_func  m_CreateFunc;
	b3_plugin_edit_func    m_EditFunc;
};

typedef b3_plugin_info * (*b3_plugin_info_func)(b3_index i);

class b3Loader : public b3LoaderBase
{
	       b3Array<b3_plugin_info> m_InfoArray;
	static b3Loader                m_Loader;
	static CString                 m_UnknownDesc;
	static HICON                   m_UnknownIcon;

	                b3Loader();
public:
	b3Item         *b3Create(b3_u32 class_type,void *ptr,b3_bool edit = true);
	b3Item         *b3Create(b3_u32 *class_buffer,void *ptr,b3_bool edit = true);
	b3_bool         b3Edit(b3Item *item,void *ptr);
	b3_bool         b3AddPluginInfo(b3_plugin_info *info);
	b3_plugin_info *b3FindInfo(b3Item *item);
	b3_plugin_info *b3FindInfo(b3_u32 class_type);
	b3_count        b3GetClassTypes(b3Array<b3_u32> &array,b3_u32 class_id);

	static void     b3SetUnknownIds(int unknown_desc_id,int unknown_icon_id);
	static void     b3AddClassType(b3_u32 class_type,
		int DescID = 0,
		int IconID = 0,
		b3_plugin_create_func create_func = null,
		b3_plugin_edit_func   edit_func = null);

	static inline b3Loader &b3GetLoader()
	{
		return m_Loader;
	}

protected:
	b3_bool         b3IsPlugin(b3Path &library);
	b3PluginBase   *b3CreatePlugin(b3Path &library);

private:
	b3Item         *b3EditCreation(b3Item *item,void *ptr,b3_bool edit);
};

class b3Plugin : public b3PluginBase
{
	HINSTANCE               m_Handle;

public:
	         b3Plugin(b3Loader *loader,b3Path &library);

public:
	virtual ~b3Plugin();
	b3_bool  b3Create(b3Item *item);
	b3_bool  b3Edit(b3Item *item);

public:
	static inline void b3InitPluginInfo(b3_plugin_info *info,b3_bool clear = true)
	{
		if (clear)
		{
			memset(info,0,sizeof(b3_plugin_info));
		}
		info->m_Version = B3_PLUGIN_INFO_VERSION;
		info->m_Size    = sizeof(b3_plugin_info);
	}

	static inline void b3SetChecksum(b3_plugin_info *info)
	{
		b3_u32    checksum = 0;
		b3_u08   *ptr = (b3_u08 *)info;
		b3_size   i;

		for (i = 0;i < info->m_Size;i++)
		{
			checksum += *ptr++;
		}
		info->m_CheckSum = checksum;
	}

	static inline b3_bool b3HasCreateFunc(b3_plugin_info *info)
	{
		return info != null ? info->m_CreateFunc != null : false;
	}

	static inline b3_bool b3HasEditFunc(b3_plugin_info *info)
	{
		return info != null ? info->m_EditFunc != null : false;
	}

private:
	static b3_bool b3IsValid(b3_plugin_info *info);

	friend class b3Loader;
};

#endif
