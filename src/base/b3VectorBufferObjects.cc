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

#include "blz3/base/b3Render.h"

#define USE_VBOS

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2004/09/24 13:45:36  sm
**	- Extracted OpenGL extension vector buffer objects into own files.
**	- Some cleanup for Lines.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_bool                   b3VectorBufferObjects::glHasVBO;

PFNGLGENBUFFERSARBPROC    b3VectorBufferObjects::glGenBuffersARB;
PFNGLDELETEBUFFERSARBPROC b3VectorBufferObjects::glDeleteBuffersARB;
PFNGLBINDBUFFERARBPROC    b3VectorBufferObjects::glBindBufferARB;
PFNGLBUFFERDATAARBPROC    b3VectorBufferObjects::glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC b3VectorBufferObjects::glBufferSubDataARB;
PFNGLMAPBUFFERARBPROC     b3VectorBufferObjects::glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC   b3VectorBufferObjects::glUnmapBufferARB;

void b3VectorBufferObjects::b3Init(char *extensions)
{
#ifdef BLZ3_USE_OPENGL
	glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)   b3Runtime::b3GetOpenGLExtension("glGenBuffersARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)b3Runtime::b3GetOpenGLExtension("glDeleteBuffersARB");
	glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)   b3Runtime::b3GetOpenGLExtension("glBindBufferARB");
	glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)   b3Runtime::b3GetOpenGLExtension("glBufferDataARB");
	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)b3Runtime::b3GetOpenGLExtension("glBufferSubDataARB");
	glMapBufferARB     = (PFNGLMAPBUFFERARBPROC)    b3Runtime::b3GetOpenGLExtension("glMapBufferARB");
	glUnmapBufferARB   = (PFNGLUNMAPBUFFERARBPROC)  b3Runtime::b3GetOpenGLExtension("glUnmapBufferARB");
#endif

#ifdef USE_VBOS
	glHasVBO = (strstr(extensions,"ARB_vertex_buffer_object") != 0) &&
		(glGenBuffersARB != null) &&
		(glDeleteBuffersARB != null) &&
		(glBindBufferARB != null) &&
		(glBufferDataARB != null) &&
		(glMapBufferARB  != null) &&
		(glUnmapBufferARB != null);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "glGenBuffersARB    = %p\n", glGenBuffersARB);
	b3PrintF(B3LOG_FULL, "glDeleteBuffersARB = %p\n", glDeleteBuffersARB);
	b3PrintF(B3LOG_FULL, "glBindBufferARB    = %p\n", glBindBufferARB);
	b3PrintF(B3LOG_FULL, "glBufferDataARB    = %p\n", glBufferDataARB);
	b3PrintF(B3LOG_FULL, "glMapBufferARB     = %p\n", glMapBufferARB);
	b3PrintF(B3LOG_FULL, "glUnmapBufferARB   = %p\n", glUnmapBufferARB);
#endif

#else // HAS_VBO
	glHasVBO = false;
#endif
}
