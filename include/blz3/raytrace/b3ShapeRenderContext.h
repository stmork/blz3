/*
**
**	$Filename:	b3ShapeRenderContext.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

#ifndef B3_RAYTRACE_SHAPERENDERCONTEXT_H
#define B3_RAYTRACE_SHAPERENDERCONTEXT_H

#include "blz3/base/b3Render.h"

/**
 * This class is a drerived render contest to supply some support for shapes.
 */
class B3_PLUGIN b3ShapeRenderContext : public b3RenderContext
{
	b3_vector    *   m_Between          = nullptr;
	b3_gl_line   *   m_CylinderIndices  = nullptr;
	b3_gl_polygon  * m_CylinderPolygons = nullptr;
	b3_gl_line   *   m_ConeIndices      = nullptr;
	b3_gl_polygon  * m_ConePolygons     = nullptr;

public:
	static b3_f64    m_Sin[B3_MAX_RENDER_SUBDIV + 1]; //!< This is a list of precomputed sine values.
	static b3_f64    m_Cos[B3_MAX_RENDER_SUBDIV + 1]; //!< This is a list of precomputed cosine values.
	static b3_count  m_SubDiv;                        //!< Sub division count.

public:
	/**
	 * This constructor initializes the render context with the given sub division value.
	 *
	 * @param subdiv The new sub division value.
	 */
	explicit b3ShapeRenderContext(const b3_count subdiv = 16);

	/**
	 * This method initializes the sub division for render object indices.
	 *
	 * @note All render objects should be recomputed after this call.
	 * @param subdiv The new sub division value.
	 * @throws b3WorldException
	 */
	void             b3InitSubdiv(const b3_count subdiv);

	/**
	 * This method returns precomputed line indices for cylinder shapes.
	 *
	 * @return The precomputed cylinder line indices.
	 */
	b3_gl_line   *   b3GetCylinderIndices() const;

	/**
	 * This method returns precomputed triangle indices for cylinder shapes.
	 *
	 * @return The precomputed cylinder triangle indices.
	 */
	b3_gl_polygon  * b3GetCylinderPolygons() const;

	/**
	 * This method returns precomputed line indices for cone shapes.
	 *
	 * @return The precomputed cone line indices.
	 */
	b3_gl_line   *   b3GetConeIndices() const;

	/**
	 * This method returns precomputed triangle indices for cone shapes.
	 *
	 * @return The precomputed cone triangle indices.
	 */
	b3_gl_polygon  * b3GetConePolygons() const;
};

#endif
