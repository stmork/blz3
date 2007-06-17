/*
**
**	$Filename:	b3Plugin.h $
**	$Release:	Dortmund 2003 $
**	$Revision: 1241 $
**	$Date: 2005-08-12 11:59:28 +0200 (Fr, 12 Aug 2005) $
**	$Author: smork $
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
