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

/**
 * This class handles the existance and the usablility
 * of vector buffer objects.
 */
class B3_PLUGIN b3VectorBufferObjects
{
	static b3_bool                   glHasVBO;

#ifdef BLZ3_USE_OPENGL
protected:
	static PFNGLGENBUFFERSARBPROC    glGenBuffersARB;     //!< The corresponding OpenGL procedure.
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;  //!< The corresponding OpenGL procedure.
	static PFNGLBINDBUFFERARBPROC    glBindBufferARB;     //!< The corresponding OpenGL procedure.
	static PFNGLBUFFERDATAARBPROC    glBufferDataARB;     //!< The corresponding OpenGL procedure.
	static PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;  //!< The corresponding OpenGL procedure.
	static PFNGLMAPBUFFERARBPROC     glMapBufferARB;      //!< The corresponding OpenGL procedure.
	static PFNGLUNMAPBUFFERARBPROC   glUnmapBufferARB;    //!< The corresponding OpenGL procedure.
#endif

public:
	static b3_bool                   glAllowVBO; //!< This flags shows if VBOs are allowed to use.

public:
	/**
	 * This method checks for the existance of VBOs in the given OpenGL extension string.
	 *
	 * @param extension The OpenGL extension string.
	 */
	static        void    b3Init(const char *extension);

	/**
	 * This method returns true if VBOs are available.
	 *
	 * @return True if VBOs are available.
	 */
	static inline b3_bool b3HasVBO()
	{
		return glHasVBO;
	}

	/**
	 * This method returns true if VBOs are available and allowed to use.
	 *
	 * @return True if VBOs may be usable.
	 */
	static inline b3_bool b3AllowVBO()
	{
		return glHasVBO && glAllowVBO;
	}
};

#endif
