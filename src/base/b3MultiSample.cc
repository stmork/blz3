/*
**
**	$Filename:	b3VectorBufferObjects.cc $ 
**	$Release:	Dortmund 2004 $
**	$Revision: 1450 $
**	$Date: 2006-06-28 19:48:22 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "b3BaseInclude.h"
#include "blz3/base/b3MultiSample.h"

#ifdef BLZ3_USE_OPENGL
#define HAS_MULTISAMPLE
#endif

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
			glEnable((GLenum)GL_MULTISAMPLE_ARB);
			glEnable((GLenum)GL_SAMPLE_BUFFERS_ARB);
			glEnable((GLenum)GL_SAMPLES_EXT);
		}
		else
		{
			glDisable((GLenum)GL_MULTISAMPLE_ARB);
			glDisable((GLenum)GL_SAMPLE_BUFFERS_ARB);
			glDisable((GLenum)GL_SAMPLES_EXT);
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
		
		multisample = glIsEnabled((GLenum)GL_MULTISAMPLE_ARB);
		glGetIntegerv((GLenum)GL_SAMPLE_BUFFERS_ARB,&sample_buffers);

		enabled = multisample && (sample_buffers == 1);
	}
#endif
	return enabled;
}
