/*
**
**	$Filename:	b3PickInfo.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setup pick points depending on shapes
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_PICKINFO_H
#define B3_RAYTRACE_PICKINFO_H

#include "blz3/base/b3Array.h"
#include "blz3/base/b3Pick.h"
#include "blz3/base/b3Render.h"

class b3Shape;

/**
 * This enumeration lists the free degrees for pick moving.
 */
enum b3_pick_mode
{
	B3_PICK_UNABLE,      //!< The pick point is static.
	B3_PICK_ALL,         //!< The pick point is free movable in all directions.
	B3_PICK_HORIZONTAL,  //!< The pick point is horizontal free to move.
	B3_PICK_VERTICAL     //!< The pick point is vertical free to move.
};

/**
 * This class visualizes picks for one shape.
 */
class b3PickInfo : public b3PickBase, public b3RenderObject
{
	b3Array<b3_gl_vertex> m_Vertices;
	b3Array<b3_gl_line>   m_Grid;

protected:
	b3Shape     *    m_Shape;           //!< The shape for which the pick points should be created.

public:
	b3_pick_mode     m_AllowedMode;     //!< The move mode.
	static b3Color   m_GridColor;       //!< The color of the pick points.

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3PickInfo();

	/**
	 * The destructor deinitializes this instance.
	 */
	virtual         ~b3PickInfo();

	/**
	 * This method sets a new shape into the pick info instance.
	 *
	 * @param shape The new shape.
	 * @return True when the shape is not null.
	 */
	b3_bool  b3SetShape(b3Shape * shape);

	/**
	 * This method marks the pick info as modified. The vertex and index
	 * list is cleared. The shapes' render object is invalidated using the
	 * b3RenderObject::b3Recompute() method. The rendering object of this
	 * instance is invalidated in the same way.
	 *
	 * @see b3RenderObject.
	 */
	void     b3Modified();

	/**
	 * This method adds a pick point to this instance with a given title.
	 *
	 * @param point The position of the point.
	 * @param title The title to display.
	 */
	virtual void     b3AddPickPoint(
		b3_vector * point,
		const char * title = nullptr) = 0;

	/**
	 * This method adds a direction pick to this instance with a given title.
	 *
	 * @param point The start position of the direction.
	 * @param dir The direction vector which results in the pick point.
	 * @param title The title to display.
	 */
	virtual void     b3AddPickDir(
		b3_vector * point,
		b3_vector * dir,
		const char * title = nullptr) = 0;

	/**
	 * This method adds a vertex for drawing a line between two pick points.
	 *
	 * @param point The point to add.
	 */
	void     b3AddVertex(b3_vector * point);

	/**
	 * This method adds a line which connects to vertices.
	 *
	 * @param a The first vertex index.
	 * @param b The second vertex index.
	 */
	void     b3AddLine(b3_index a, b3_index b);

protected:
	void     b3ComputeVertices();
	void     b3ComputeIndices();
	void     b3GetGridColor(b3Color & color);
};

#endif
