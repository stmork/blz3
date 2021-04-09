/*
**
**	$Filename:	b3BBox.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for bounnding boxes
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_BBOX_H
#define B3_RAYTRACE_BBOX_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3PickInfo.h"
#include "blz3/raytrace/b3PrepareInfo.h"
#include "blz3/raytrace/b3CameraProjection.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/base/b3Render.h"

/*************************************************************************
**                                                                      **
**                        bounding boxes                                **
**                                                                      **
*************************************************************************/

#define CLASS_BBOX  0x60000000
#define BBOX        CLASS_BBOX

/**
 * This class is a representation for a bounding box. This bounding box can
 * be nested for hierarchical order. One bounding box can contain a list
 * of shapes and a list of sub bounding boxes. The dimensions are computed
 * via the b3ComputeBounds() method call.
 */
class B3_PLUGIN b3BBox : public b3Item, public b3RenderObject
{
	static const b3_gl_line m_BBoxIndices[12 * 2];

	enum b3_bbox_visibility
	{
		B3_BBOX_UNKNOWN,
		B3_BBOX_INVISIBLE,
		B3_BBOX_PARTIALLY_VISIBLE,
		B3_BBOX_VISIBLE
	}                m_Visibility;

	// Inherited from Blizzard II
	b3_u32           m_Type;                      // texture type
	b3_count         m_ShapeCount;
	b3_count         m_CSGIntersectionCount;
	b3_matrix        m_Inverse;

public:
	b3_vector        m_DimBase;                   //!< Lower base position of dimension size.
	b3_vector        m_DimSize;                   //!< Dimension size.
	b3_matrix        m_Matrix;                    //!< All composed transformations.
	char             m_BoxName[B3_BOXSTRINGLEN];  //!< Oject name.
	char             m_BoxURL[B3_BOXSTRINGLEN];   //!< HTML link (actually unused.)

	b3_gl_vertex     m_BBoxVertex[8];             //!< Bounding box vertex list (custom list in b3RenderObject).

	static b3Color   m_GridColor;                 //!< Grid color of bounding boxes.
	static bool      m_GridVisible;               //!< Flag if bounding box grid should be shown.

	static b3_count  m_Visible;                   //!< Statistic cound of completely camera visible bounding boxes.
	static b3_count  m_PartiallyVisible;          //!< Statistic cound of partially camera visible bounding boxes.
	static b3_count  m_Invisible;                 //!< Statistic cound of completely camera invisible bounding boxes

public:
	B3_ITEM_INIT(b3BBox); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3BBox); //!< This constructor handles deserialization.

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static void      b3Register();
	void             b3Write() override;
	void             b3Dump(b3_count level) const override;
	void             b3SetName(const char * name);
	void             b3SetupVertexMemory(b3RenderContext * context) override;
	void             b3FreeVertexMemory() override;
	void             b3Draw(b3RenderContext * context) override;
	bool             b3Transform(b3_matrix * transformation, bool is_affine, bool force_action = false);
	bool             b3Inverse(b3_matrix *);
	bool             b3Reverse(b3_matrix *);
	void             b3ResetTransformation();
	void             b3Activate(bool activate = true, bool recurse = true);
	void             b3Animate(b3Activation::b3_anim_activation animate = b3Activation::B3_ANIM_ACTIVE, bool recurse = true);
	bool             b3IsActive() const;
	void             b3Expand(bool expand = true);
	bool             b3IsExpanded() const;
	void             b3Update();
	void             b3UpdateMaterial();
	void             b3UpdateBBox();
	bool             b3ComputeBounds(b3_vector * lower, b3_vector * upper, b3_f64 tolerance);
	b3_count         b3Count() const;
	bool b3PrepareBBox(b3_scene_preparation * prep_info, bool recursive = false);
	const char   *   b3GetName() const override;
	bool             b3BacktraceRecompute(b3BBox * search);
	b3Base<b3Item> * b3FindBBoxHead(b3BBox * bbox);
	b3BBox     *     b3FindParentBBox(b3Shape * shape);
	bool             b3Intersect(b3_ray * ray, bool check_visibility);
	b3CSGShape   *   b3IntersectCSG(b3_ray * ray);
	void             b3CollectBBoxes(b3Array<b3BBoxReference> & array);
	void             b3CollectBBoxes(b3_ray * ray, b3Array<b3BBox *> * array);
	void             b3CollectBBoxes(b3_vector * lower, b3_vector * upper, b3Array<b3BBox *> * array);
	void             b3ComputeVisibility(b3CameraProjection * projection);

	void             b3CollectActiveBBoxes(b3Array<b3BBox *> * array, bool activation);
	static void      b3Reorg(b3Base<b3Item> * depot, b3Base<b3Item> * base, b3_count level, b3_count rec, b3Item * insert_after = nullptr);
	static void      b3Recount(b3Base<b3Item> * base, b3_count level = 1);
	static bool      b3FindBBox(b3Base<b3Item> * base, b3BBox * search);
	static b3BBox  * b3ReadCOB(const char * filename);
	static b3BBox  * b3ReadTGF(const char * filename);

	/**
	 * This method returns the list base of the bounding box shapes.
	 *
	 * @return List of bounding box shapes.
	 */
	inline b3Base<b3Item> * b3GetShapeHead() const
	{
		return &m_Heads[0];
	}

	/**
	 * This method returns the list base of the sub bounding boxes.
	 *
	 * @return List of sub bounding boxes.
	 */
	inline b3Base<b3Item> * b3GetBBoxHead() const
	{
		return &m_Heads[1];
	}

	/**
	 * This method computes the polar coordinates of the ray intersection point.
	 *
	 * @param ray The ray containing the intersection point.
	 */
	inline void         b3ComputeBoxPolar(b3_ray * ray) const
	{
		b3_f64 x = ray->ipoint.x;
		b3_f64 y = ray->ipoint.y;
		b3_f64 z = ray->ipoint.z;

		ray->polar.m_BoxPolar.x = (x - m_DimBase.x) / m_DimSize.x;
		ray->polar.m_BoxPolar.y = (y - m_DimBase.y) / m_DimSize.y;
		ray->polar.m_BoxPolar.z = (z - m_DimBase.z) / m_DimSize.z;

		ray->polar.m_BBoxOriginal.x = (b3_f32)(x * m_Inverse.m11 + y * m_Inverse.m12 + z * m_Inverse.m13 + m_Inverse.m14);
		ray->polar.m_BBoxOriginal.y = (b3_f32)(x * m_Inverse.m21 + y * m_Inverse.m22 + z * m_Inverse.m23 + m_Inverse.m24);
		ray->polar.m_BBoxOriginal.z = (b3_f32)(x * m_Inverse.m31 + y * m_Inverse.m32 + z * m_Inverse.m33 + m_Inverse.m34);
	}

protected:
	inline void b3GetGridColor(b3Color & color) const override
	{
		color = m_GridColor;
	}

	inline b3_render_mode  b3GetRenderMode() const override
	{
		return m_GridVisible ? B3_RENDER_LINE : B3_RENDER_NOTHING;
	}

	void    b3GetCount(
		b3RenderContext * context,
		b3_count     &    vertCount,
		b3_count     &    gridCount,
		b3_count     &    polyCount) override;
	void    b3AllocVertexMemory(b3RenderContext * context) override;
	void    b3ComputeVertices() override;
	void    b3ComputeNormals(bool normalize = true) override;
};

#define BBB_HTML         0
#define BBB_TEXT         1
#define BBB_BOLD         2
#define BBB_ITALIC       3
#define BBB_SANSSERIF    4
#define BBB_TYPEWRITER   5
#define BBB_ACTIVE       6
#define BBB_EXPANDED     7

#define BBF_HTML        (1 << BBB_HTML)
#define BBF_TEXT        (1 << BBB_TEXT)
#define BBF_BOLD        (1 << BBB_BOLD)
#define BBF_ITALIC      (1 << BBB_ITALIC)
#define BBF_SERIF        0
#define BBF_SANSSERIF   (1 << BBB_SANSSERIF)
#define BBF_TYPEWRITER  (1 << BBB_TYPEWRITER)
#define BBF_ACTIVE      (1 << BBB_ACTIVE)
#define BBF_EXPANDED    (1 << BBB_EXPANDED)

#define BB_GETTEXTSIZE(BBox)      (((BBox)->Type & 0x0ff00) >>  8)
#define BB_SETTEXTSIZE(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xffff00ff) |  ((Size) << 8))
#define BB_GETTEXTTYPE(BBox)      (((BBox)->Type & 0x00030) >>  4)
#define BB_SETTEXTTYPE(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xffffffcf) |  ((Size) << 4))
#define BB_GETROTATION(BBox)     ((((long)((BBox)->Type) & 0x70000) << 13) >> 29)
#define BB_SETROTATION(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xfff8ffff) | (((long)(Size) << 29) >> 13))

#endif
