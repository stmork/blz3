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

class b3Loader : public b3LoaderBase
{
protected:
	b3_bool b3IsPlugin(b3Path &library);
	b3PluginBase *b3CreatePlugin(b3Path &library);
};

class b3Plugin : public b3PluginBase
{
	HINSTANCE  m_Handle;

public:
	         b3Plugin(b3Path &library);
	virtual ~b3Plugin();
};

#endif
