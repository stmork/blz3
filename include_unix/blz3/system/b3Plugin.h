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

#pragma once

#ifndef B3_SYSTEM_PLUGIN_H
#define B3_SYSTEM_PLUGIN_H

#include "blz3/system/b3PluginBase.h"

#define B3_PLUGIN_INIT_FUNC "b3Init"

/**
 * This class implements the b3LoaderBase.
 */
class b3Loader : public b3LoaderBase
{
protected:
	b3_bool       b3IsPlugin(b3Path &library);
	b3PluginBase *b3CreatePlugin(b3Path &library);
};

/**
 * This class references one valid plugin.
 */
class b3Plugin : public b3PluginBase
{
	void    *m_Handle;

public:
	/**
	 * This constructor initializes this plugin from the given path.
	 *
	 * @param library The plugin file.
	 */
	b3Plugin(b3Path &library);

	/**
	 * This destructor deinitializes the plugin.
	 */
	virtual ~b3Plugin();
};

#endif
