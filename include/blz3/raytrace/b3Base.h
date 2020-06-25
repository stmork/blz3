/*
**
**	$Filename:	b3Base.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base structures for raytracing
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_BASE_H
#define B3_RAYTRACE_BASE_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3World.h"

#include <float.h>

#define B3_BOXSTRINGLEN  128
#define B3_TEXSTRINGLEN   96
#define B3_LIGHTNAMELEN  B3_BOXSTRINGLEN
#define B3_CAMERANAMELEN  96

/*************************************************************************
**                                                                      **
**                        Surface descriptions for intersections        **
**                                                                      **
*************************************************************************/

/**
 * This structure contains polar coordinates in different interpretations.
 */
struct b3_polar
{
	b3_vector64   m_Polar;        //!< Surface coordinates of shape (transform invariant!).
	b3_vector64   m_ObjectPolar;  //!< Rel. shape coordinates (transform invariant!).
	b3_vector64   m_BoxPolar;     //!< Rel. bbox coordinates which contains the shape (This is not transform invariant!).
	b3_vector64   m_BBoxOriginal; //!< Coordinates in original coordinate space (transform invariant!).
	b3_index      m_NormalIndex;  //!< Which triangle in case of triangle mesh.
};

class b3Shape;
class b3BBox;
class b3Material;

/**
 * This structure collects all information a ray can produce at an intersection point.
 */
struct B3_PLUGIN b3_ray : public b3_line64
{
	b3_f64      Q;                     //!< Backward limit.
	b3_vector64 ipoint;                //!< Intersection point.
	b3_vector64 normal;                //!< Normal at intersection point.
	b3_vector64 xDeriv;                //!< Surface x derivative.
	b3_vector64 yDeriv;                //!< Surface y derivative.
	b3_polar    polar;                 //!< Diverse polar coordinates.
	b3Shape    *shape;                 //!< Intersected shape.
	b3BBox     *bbox;                  //!< Bounding box which contains intersected shape.
	b3Material *material;              //!< Hit material of intersected shape.
	b3_bool     inside;                //!< Inside or outside flag.
	b3Color     color;                 //!< Result color.
	b3_index    TriaIndex;             //!< Triangle index at intersection point.
	b3_f64      aTriaValue,bTriaValue; //!< Polar coordinates of that triangle.
};

/*************************************************************************
**                                                                      **
**                        Some triangle definitions                     **
**                                                                      **
*************************************************************************/

#define CLASS_VERTEX        0x00010000
#define CLASS_TRIANGLE      0x00020000
#define TYPE_VERTEXNODE     0x00000001
#define TYPE_TRIA           0x00000001
#define TYPE_TRIAREF        0x00000002
#define TYPE_CPOINT_3D      0x00000003
#define TYPE_CPOINT_4D      0x00000004

#define TRIANGLE            (CLASS_TRIANGLE|TYPE_TRIA)
#define TRIANGLEREF         (CLASS_TRIANGLE|TYPE_TRIAREF)
#define VERTEXNODE          (CLASS_VERTEX|TYPE_VERTEXNODE)
#define CPOINT_3D           (CLASS_VERTEX|TYPE_CPOINT_3D)
#define CPOINT_4D           (CLASS_VERTEX|TYPE_CPOINT_4D)

/**
 * This structure contains all information about a triangle.
 */
struct b3_triangle
{
	b3_index  P1;      //!< The first vertex index.
	b3_index  P2;      //!< The second vertex index.
	b3_index  P3;      //!< The third vertex index.
	b3_vector Normal;  //!< The plane normal.
};

/**
 * This structure contains information about a single vertex.
 */
struct b3_vertex
{
	b3_vector Point;   //!< The space coordinate.
	b3_vector Normal;  //!< The normal at this point.
};

#endif
