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

class b3Loader : public b3LoaderBase
{
protected:
	b3_bool       b3IsPlugin(b3Path &library);
	b3PluginBase *b3CreatePlugin(b3Path &library);

public:
	b3Item       *b3Create(b3_u32 class_type);
	b3_bool       b3Edit(b3Item *item);
};

typedef b3_bool (*b3_plugin_edit_func)(b3Item *item);

struct b3_plugin_info
{
	b3_u32               m_Version;
	b3_u32               m_ClassType;
	const char          *m_Description;
	b3_plugin_edit_func  m_EditFunc;
};

typedef b3_bool (*b3_plugin_info_func)(b3Array<b3_plugin_info> &array);

class b3Plugin : public b3PluginBase
{
	HINSTANCE               m_Handle;
	b3Array<b3_plugin_info> m_InfoArray;

public:
	         b3Plugin(b3Path &library);
	void     b3Load();
	void     b3Unload();

public:
	b3Item  *b3Create(b3_u32 class_type);
	b3_bool  b3Edit(b3Item *item);
};

#endif
