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

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/base/b3List.h"
#include "blz3/base/b3SearchPath.h"

class b3PluginBase;

class b3LoaderBase : public b3SearchPath
{
	b3Base<b3PluginBase> m_PluginList;

public:
	void     b3Load();
	void     b3Unload();

protected:
	virtual         ~b3LoaderBase();
	virtual b3_bool  b3IsPlugin(b3Path &library) = 0;
};

class b3Loader : public b3LoaderBase
{
protected:
	b3_bool b3IsPlugin(b3Path &library);
};


typedef void (*b3_plugin_init_func)();


class b3PluginBase : public b3Link<b3PluginBase>
{
protected:
	b3Path m_PluginPath;

public:
	b3PluginBase(b3Path &library);
};

class b3Plugin : public b3PluginBase
{
	HINSTANCE  m_Handle;

public:
	         b3Plugin(b3Path &library);
	virtual ~b3Plugin();
};

#endif
