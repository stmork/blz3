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

#include "blz3/base/b3VectorBufferObjects.h"

#define USE_VBOS

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.10  2004/12/04 12:54:07  sm
**	- Disabling VBO check box if VBO not available.
**
**	Revision 1.9  2004/12/04 12:32:49  sm
**	- Disabling ATI VBOs.
**	
**	Revision 1.8  2004/11/28 20:20:17  sm
**	- Added support for switchable VBOs.
**	
**	Revision 1.7  2004/11/21 16:44:46  sm
**	- Corrected fulcrum drawing problem: The fulcrum was
**	  updated before first initialization. And even the initialization
**	  was before RenderContext init. So the fulcrum was not
**	  able to use VBOs and used vertex arrays as fallback.
**	  The vertex array drawing cannot be combined with
**	  VBOs due to binding problems. Its likely that any VBO
**	  is bound so a simple vertex array call should go wrong.
**	
**	Revision 1.6  2004/11/21 14:56:58  sm
**	- Merged VBO development into main trunk.
**	
**	Revision 1.5  2004/10/16 17:00:52  sm
**	- Moved lighting into own class to ensure light setup
**	  after view setup.
**	- Fixed lighting for scene and simple overview
**	- Fixed Light cutoff exponent deadloop.
**	- Corrected OpenGL define (BLZ3_USE_OPENGL)
**	
**	Revision 1.4  2004/09/24 20:22:05  sm
**	- Some VBO adjustments.
**	
**	Revision 1.3  2004/09/24 19:07:27  sm
**	- VBOs on ATI running - or better: crawling.
**	
**	Revision 1.2  2004/09/24 15:39:31  sm
**	- Added multisampling support which doesn't work.
**	
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

b3_bool                   b3VectorBufferObjects::glHasVBO   = false;
b3_bool                   b3VectorBufferObjects::glAllowVBO = true;

#ifdef BLZ3_USE_OPENGL
PFNGLGENBUFFERSARBPROC    b3VectorBufferObjects::glGenBuffersARB;
PFNGLDELETEBUFFERSARBPROC b3VectorBufferObjects::glDeleteBuffersARB;
PFNGLBINDBUFFERARBPROC    b3VectorBufferObjects::glBindBufferARB;
PFNGLBUFFERDATAARBPROC    b3VectorBufferObjects::glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC b3VectorBufferObjects::glBufferSubDataARB;
PFNGLMAPBUFFERARBPROC     b3VectorBufferObjects::glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC   b3VectorBufferObjects::glUnmapBufferARB;
#endif

void b3VectorBufferObjects::b3Init(const char *extensions)
{
#ifdef BLZ3_USE_OPENGL
	const char *vendor = (const char *)glGetString(GL_VENDOR);

	glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)   b3Runtime::b3GetOpenGLExtension("glGenBuffersARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)b3Runtime::b3GetOpenGLExtension("glDeleteBuffersARB");
	glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)   b3Runtime::b3GetOpenGLExtension("glBindBufferARB");
	glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)   b3Runtime::b3GetOpenGLExtension("glBufferDataARB");
	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)b3Runtime::b3GetOpenGLExtension("glBufferSubDataARB");
	glMapBufferARB     = (PFNGLMAPBUFFERARBPROC)    b3Runtime::b3GetOpenGLExtension("glMapBufferARB");
	glUnmapBufferARB   = (PFNGLUNMAPBUFFERARBPROC)  b3Runtime::b3GetOpenGLExtension("glUnmapBufferARB");

#ifdef USE_VBOS
	glHasVBO = (strncmp(vendor,"ATI",3) != 0) &&
		(strstr(extensions,"ARB_vertex_buffer_object") != 0) &&
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

#else // USE_VBOS
	glHasVBO = false;
#endif
#endif
}
