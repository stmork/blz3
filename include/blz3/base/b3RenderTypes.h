/*
**
**	$Filename:	b3RenderTypes.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Render type definitions
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_RENDERTYPES_H
#define B3_BASE_RENDERTYPES_H

#include "blz3/b3Config.h"

#define B3_MAX_RENDER_SUBDIV 48

/**
 * This enumeration lists possible render modes.
 */
enum b3_render_mode
{
	B3_RENDER_UNDIFINED = -1,  //!< Undefined rendering.
	B3_RENDER_NOTHING   =  0,  //!< No rendering.
	B3_RENDER_LINE,            //!< Wire frame objects.
	B3_RENDER_FILLED           //!< Solid filled draing mode.
};

/**
 * This enumeration lists possible matrix modes.
 */
enum b3_matrix_mode
{
	B3_MATRIX_OBJECT,    //!< The matrix transforms objects.
	B3_MATRIX_PROJECTION //!< The matrix transforms the projection.
};

/**
 * This structure is for vertex array methods used in OpenGL.
 * for use with glInterleavedArrays(GL_T2F_N3F_V3F,0, b3_vertex *));
 */
struct b3_gl_vertex
{
	b3_gl_texture t; //!< The texture coordinate.
	b3_gl_vector  n; //!< The normal.
	b3_gl_vector  v; //!< The space coordinates.
};

/**
 * This structure specifies two indices into a vertex array which
 * marks the end points of a single line.
 */
struct b3_gl_line
{
#ifdef BLZ3_USE_OPENGL
	/**
	 * The line indices.
	 */
	GLuint   a,b;
#else
	/**
	 * The line indices.
	 */
	b3_u32   a,b;
#endif
};

/**
 * This structure specifies three indices into a vertex array which
 * marks the corner points of a single triangle. This is the reason
 * why a triangle is called a triangle ;-)
 */
struct b3_gl_polygon
{
#ifdef BLZ3_USE_OPENGL
	/**
	 * The triangle indices.
	 */
	GLuint   a,b,c;
#else
	/**
	 * The triangle indices.
	 */
	b3_u32   a,b,c;
#endif
};

#define B3_GL_LINIT(l,ai,bi)    { (l)->a = (ai); (l)->b = (bi); (l)++; }
#define B3_GL_PINIT(p,ai,bi,ci) { (p)->a = (ai); (p)->b = (bi); (p)->c = (ci); (p)++; }

#endif
