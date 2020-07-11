/*
**
**	$Filename:	b3StaticPluginInfoInit.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Adding static plugin infos
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_STATIC_PLUGIN_INFO_INIT_H
#define B3_STATIC_PLUGIN_INFO_INIT_H

#include "blz3/system/b3Plugin.h"
#include "blz3/base/b3Hash.h"

struct b3_class_name
{
	char name[128];
};

class b3StaticPluginInfoInit
{
	static b3HashMap<b3_u32, b3_class_name> m_ClassNames;

public:
	static void        b3Init();
	static const char * b3GetClassName(b3_u32 class_type);

private:
	static void b3AddClass(b3_u32 class_id,  int DescID);
};

#endif
