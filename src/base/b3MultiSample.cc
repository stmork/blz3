/*
**
**	$Filename:	b3VectorBufferObjects.cc $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Containter for OpenGL vector buffer objects
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/


/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3MultiSample.h"

#define HAS_MULTISAMPLE

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/09/24 15:39:31  sm
**	- Added multisampling support which doesn't work.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_bool                   b3MultiSample::glHasMS;

void b3MultiSample::b3Init(char *extensions)
{
#ifdef HAS_MULTISAMPLE
	glHasMS = strstr(extensions,"ARB_multisample") !=  null;
#else
	glHasMS = false;
#endif
}

void b3MultiSample::b3Enable(b3_bool enable)
{
	if (b3HasMS())
	{
#ifdef BLZ3_USE_OPENGL
		if (enable)
		{
			glEnable(GL_MULTISAMPLE_ARB);
			glEnable(GL_SAMPLE_BUFFERS_EXT);
			glEnable(GL_SAMPLES_EXT);
		}
		else
		{
			glDisable(GL_MULTISAMPLE_ARB);
			glDisable(GL_SAMPLE_BUFFERS_EXT);
			glDisable(GL_SAMPLES_EXT);
		}
#endif
	}
}

b3_bool b3MultiSample::b3IsEnabled()
{
	b3_bool enabled = false;

#ifdef BLZ3_USE_OPENGL
	if (b3HasMS())
	{
		enabled = glIsEnabled(GL_MULTISAMPLE_ARB);
	}
#endif
	return enabled;
}
