/*
**
**	$Filename:	b3RenderTypes.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
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

typedef enum
{
	B3_RENDER_UNDIFINED = -1,
	B3_RENDER_NOTHING   =  0,
	B3_RENDER_LINE,
	B3_RENDER_FILLED
} b3_render_mode;

typedef enum
{
	B3_MATRIX_OBJECT,
	B3_MATRIX_PROJECTION
} b3_matrix_mode;

/*
** for use with glInterleavedArrays(GL_T2F_N3F_V3F,0, b3_vertex *));
*/

struct b3_gl_vertex
{
	b3_gl_texture t;
	b3_gl_vector  n;
	b3_gl_vector  v;
};

struct b3_gl_line
{
#ifdef BLZ3_USE_OPENGL
	GLuint   a,b;
#else
	b3_u32   a,b;
#endif
};

struct b3_gl_polygon
{
#ifdef BLZ3_USE_OPENGL
	GLuint   a,b,c;
#else
	b3_u32   a,b,c;
#endif
};

#define B3_GL_LINIT(l,ai,bi)    { (l)->a = (ai); (l)->b = (bi); (l)++; }
#define B3_GL_PINIT(p,ai,bi,ci) { (p)->a = (ai); (p)->b = (bi); (p)->c = (ci); (p)++; }

#endif
