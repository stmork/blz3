/*
**
**	$Filename:	b3ShapeRenderContext.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definition for shapes' render context
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_SHAPERENDERCONTEXT_H
#define B3_RAYTRACE_SHAPERENDERCONTEXT_H

#include "blz3/base/b3Render.h"

class B3_PLUGIN b3ShapeRenderContext : public b3RenderContext
{
	b3_vector       *m_Between;
	b3_gl_line      *m_CylinderIndices;
	b3_gl_polygon   *m_CylinderPolygons;
	b3_gl_line      *m_ConeIndices;
	b3_gl_polygon   *m_ConePolygons;

public:
	static b3_f64    m_Sin[B3_MAX_RENDER_SUBDIV + 1];
	static b3_f64    m_Cos[B3_MAX_RENDER_SUBDIV + 1];
	static b3_count  m_SubDiv;

public:
	                 b3ShapeRenderContext(b3_count subdiv = 16);
	void             b3InitSubdiv(b3_count subdiv);
	b3_gl_line      *b3GetCylinderIndices();
	b3_gl_polygon   *b3GetCylinderPolygons();
	b3_gl_line      *b3GetConeIndices();
	b3_gl_polygon   *b3GetConePolygons();
};

#endif
