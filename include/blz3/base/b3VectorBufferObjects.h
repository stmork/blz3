/*
**
**	$Filename:	b3VectorBufferObjects.h $ 
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

#ifndef B3_BASE_VECTORBUFFEROBJECTS_H
#define B3_BASE_VECTORBUFFEROBJECTS_H

#include "blz3/b3Config.h"

class B3_PLUGIN b3VectorBufferObjects
{
	static b3_bool                   glAllowVBO;
	static b3_bool                   glHasVBO;

#ifdef BLZ3_USE_OPENGL
protected:
	static PFNGLGENBUFFERSARBPROC    glGenBuffersARB;
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
	static PFNGLBINDBUFFERARBPROC    glBindBufferARB;
	static PFNGLBUFFERDATAARBPROC    glBufferDataARB;
	static PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
	static PFNGLMAPBUFFERARBPROC     glMapBufferARB;
	static PFNGLUNMAPBUFFERARBPROC   glUnmapBufferARB;
#endif

public:
	static        void    b3Init(const char *extension);

	static inline b3_bool b3HasVBO()
	{
		return glHasVBO;
	}

	static inline void b3AllowVBO(b3_bool allow = true)
	{
		glAllowVBO = allow;
	}
};

#endif
