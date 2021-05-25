/*
**
**	$Filename:	b3VectorBufferObjects.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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
#include "blz3/base/b3VectorBufferObjects.h"

#define USE_VBOS

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

bool                      b3VectorBufferObjects::glHasVBO   = false;
bool                      b3VectorBufferObjects::glAllowVBO = true;

#ifdef BLZ3_USE_OPENGL
PFNGLGENBUFFERSARBPROC    b3VectorBufferObjects::glGenBuffersARB;
PFNGLDELETEBUFFERSARBPROC b3VectorBufferObjects::glDeleteBuffersARB;
PFNGLBINDBUFFERARBPROC    b3VectorBufferObjects::glBindBufferARB;
PFNGLBUFFERDATAARBPROC    b3VectorBufferObjects::glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC b3VectorBufferObjects::glBufferSubDataARB;
PFNGLMAPBUFFERARBPROC     b3VectorBufferObjects::glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC   b3VectorBufferObjects::glUnmapBufferARB;
#endif

void b3VectorBufferObjects::b3Init(const char * extensions)
{
#ifdef BLZ3_USE_OPENGL
	const char * vendor = (const char *)glGetString(GL_VENDOR);

	if (extensions == nullptr)
	{
		extensions = (const char *)glGetString(GL_EXTENSIONS);
	}

	glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)   b3Runtime::b3GetOpenGLExtension("glGenBuffersARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)b3Runtime::b3GetOpenGLExtension("glDeleteBuffersARB");
	glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)   b3Runtime::b3GetOpenGLExtension("glBindBufferARB");
	glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)   b3Runtime::b3GetOpenGLExtension("glBufferDataARB");
	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)b3Runtime::b3GetOpenGLExtension("glBufferSubDataARB");
	glMapBufferARB     = (PFNGLMAPBUFFERARBPROC)    b3Runtime::b3GetOpenGLExtension("glMapBufferARB");
	glUnmapBufferARB   = (PFNGLUNMAPBUFFERARBPROC)  b3Runtime::b3GetOpenGLExtension("glUnmapBufferARB");

#ifdef USE_VBOS
	bool is_not_ati       = strncmp(vendor, "ATI", 3) != 0;
	bool driver_has_vbo   = strstr(extensions, "ARB_vertex_buffer_object") != 0;
	bool methods_not_null =
		(glGenBuffersARB != nullptr) &&
		(glDeleteBuffersARB != nullptr) &&
		(glBindBufferARB != nullptr) &&
		(glBufferDataARB != nullptr) &&
		(glMapBufferARB  != nullptr) &&
		(glUnmapBufferARB != nullptr);

	glHasVBO = is_not_ati && driver_has_vbo && methods_not_null;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "glGenBuffersARB    = %p\n", glGenBuffersARB);
	b3PrintF(B3LOG_FULL, "glDeleteBuffersARB = %p\n", glDeleteBuffersARB);
	b3PrintF(B3LOG_FULL, "glBindBufferARB    = %p\n", glBindBufferARB);
	b3PrintF(B3LOG_FULL, "glBufferDataARB    = %p\n", glBufferDataARB);
	b3PrintF(B3LOG_FULL, "glMapBufferARB     = %p\n", glMapBufferARB);
	b3PrintF(B3LOG_FULL, "glUnmapBufferARB   = %p\n", glUnmapBufferARB);
#endif

#else // USE_VBOS
	glHasVBO = false;
#endif
#endif
}
