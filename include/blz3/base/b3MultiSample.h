/*
**
**	$Filename:	b3MultiSample.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Containter for OpenGL multisampling
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_MULTISAMPLE_H
#define B3_BASE_MULTISAMPLE_H

#include "blz3/b3Config.h"

class B3_PLUGIN b3MultiSample
{
	static        b3_bool glHasMS;

public:
	static        void    b3Init(const char *extensions);
	static        void    b3Enable(b3_bool enable = true);
	static        b3_bool b3IsEnabled();
	static inline b3_bool b3HasMS()
	{
		return glHasMS;
	}
};

#endif
