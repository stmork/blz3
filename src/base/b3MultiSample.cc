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
**	Revision 1.4  2004/12/06 15:14:56  smork
**	- Minor changes
**
**	Revision 1.3  2004/11/21 14:56:57  sm
**	- Merged VBO development into main trunk.
**	
**	Revision 1.2.2.2  2004/11/21 14:13:29  sm
**	- Multisample test corrected.
**	
**	Revision 1.2.2.1  2004/11/20 11:37:15  sm
**	- Added Windows VBO support
**	
**	Revision 1.2  2004/09/24 20:22:05  sm
**	- Some VBO adjustments.
**	
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

void b3MultiSample::b3Init(const char *extensions)
{
#ifdef HAS_MULTISAMPLE
	if (extensions == null)
	{
		extensions = (const char *)glGetString(GL_EXTENSIONS);
	}

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
			glEnable(GL_SAMPLE_BUFFERS_ARB);
			glEnable(GL_SAMPLES_EXT);
		}
		else
		{
			glDisable(GL_MULTISAMPLE_ARB);
			glDisable(GL_SAMPLE_BUFFERS_ARB);
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
		GLint     sample_buffers = 0;
		GLboolean multisample;
		
		multisample = glIsEnabled(GL_MULTISAMPLE_ARB);
		glGetIntegerv(GL_SAMPLE_BUFFERS_ARB,&sample_buffers);

		enabled = multisample && (sample_buffers == 1);
	}
#endif
	return enabled;
}
