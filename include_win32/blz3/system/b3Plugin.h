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

typedef b3_bool (*b3_plugin_create_func)(b3Item *item);
typedef b3_bool (*b3_plugin_edit_func)(b3Item *item);

#define B3_PLUGIN_INFO_VERSION 1

struct b3_plugin_info
{
	b3_u32                 m_Version;
	b3_size                m_Size;
	b3_u32                 m_CheckSum;
	b3_u32                 m_ClassType;
	const char            *m_Description;
	HICON                  m_Icon;
	b3_plugin_create_func  m_CreateFunc;
	b3_plugin_edit_func    m_EditFunc;
};

class b3Loader : public b3LoaderBase
{
protected:
	b3_bool       b3IsPlugin(b3Path &library);
	b3PluginBase *b3CreatePlugin(b3Path &library);

public:
	b3Item       *b3Create(b3_u32 class_type);
	b3_bool       b3Edit(b3Item *item);

private:
	b3_plugin_info *b3FindInfo(b3_u32 class_type);
};

typedef b3_plugin_info * (*b3_plugin_info_func)(b3_index i);

class b3Plugin : public b3PluginBase
{
	HINSTANCE               m_Handle;

protected:
	b3Array<b3_plugin_info> m_InfoArray;

public:
	         b3Plugin(b3Path &library);

public:
	virtual ~b3Plugin();
	b3_bool  b3Create(b3Item *item);
	b3_bool  b3Edit(b3Item *item);

public:
	static void    b3InitPluginInfo(b3_plugin_info *info)
	{
		memset(info,0,sizeof(b3_plugin_info));
		info->m_Version = B3_PLUGIN_INFO_VERSION;
		info->m_Size    = sizeof(b3_plugin_info);
	}

	static void    b3SetChecksum(b3_plugin_info *info)
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

private:
	static b3_bool b3IsValid(b3_plugin_info *info);

	friend class b3Loader;
};

#endif
