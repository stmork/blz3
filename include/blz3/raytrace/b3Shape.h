/*
**
**	$Filename:	b3Shape.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for shapes
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_SHAPE_H
#define B3_RAYTRACE_SHAPE_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/raytrace/b3Activation.h"
#include "blz3/raytrace/b3Condition.h"
#include "blz3/raytrace/b3PickInfo.h"
#include "blz3/raytrace/b3BaseTrans.h"

/*************************************************************************
**                                                                      **
**                        shapes                                        **
**                                                                      **
*************************************************************************/

#define CLASS_SHAPE         0x50000000
#define TYPE_PLATE          0x00000001
#define TYPE_SPHERE         0x00000002
#define TYPE_CYLINDER       0x00000003
#define TYPE_CONE           0x00000004
#define TYPE_ELLIPSOID      0x00000005
#define TYPE_BOX            0x00000006
#define TYPE_TORUS          0x00000007
#define TYPE_TRIANGLEFIELD  0x00000008
#define TYPE_SHUTUP         0x00000009
#define TYPE_DISK           0x0000000a
#define TYPE_SPLINE         0x0000000b
#define TYPE_SPLINE_ROT     0x0000000c
#define TYPE_SPLINES_AREA   0x0000000d
#define TYPE_SPLINES_CYL    0x0000000e
#define TYPE_SPLINES_RING   0x0000000f

#define AREA                (CLASS_SHAPE|TYPE_PLATE)
#define SPHERE              (CLASS_SHAPE|TYPE_SPHERE)
#define CYLINDER            (CLASS_SHAPE|TYPE_CYLINDER)
#define CONE                (CLASS_SHAPE|TYPE_CONE)
#define ELLIPSOID           (CLASS_SHAPE|TYPE_ELLIPSOID)
#define BOX                 (CLASS_SHAPE|TYPE_BOX)
#define TORUS               (CLASS_SHAPE|TYPE_TORUS)
#define TRIANGLES           (CLASS_SHAPE|TYPE_TRIANGLEFIELD)
#define SHUTUP              (CLASS_SHAPE|TYPE_SHUTUP)
#define DISK                (CLASS_SHAPE|TYPE_DISK)
#define SPLINE              (CLASS_SHAPE|TYPE_SPLINE)
#define SPLINE_ROT          (CLASS_SHAPE|TYPE_SPLINE_ROT)
#define SPLINES_AREA        (CLASS_SHAPE|TYPE_SPLINES_AREA)
#define SPLINES_CYL         (CLASS_SHAPE|TYPE_SPLINES_CYL)
#define SPLINES_RING        (CLASS_SHAPE|TYPE_SPLINES_RING)

/**
 * This class provides a base transformation including a normal derivation.
 */
class B3_PLUGIN b3ShapeBaseTransformation : public b3BaseTransformation
{
public:
	/**
	 * This method computes the derivates along the normal given by the
	 * ray structure.
	 *
	 * @param ray The ray which contains the normal.
	 * @return True on success.
	 */
	bool    b3NormalDeriv(b3_ray * ray) const;
};

class b3Material;
class b3Condition;
class b3Bump;

struct b3_surface;

/*************************************************************************
**                                                                      **
**                        Base class for all shapes                     **
**                                                                      **
*************************************************************************/

/**
 * This is a base class for all Blizzard shapes. It provides archive support
 * via the b3Item class, rendering support through the b3RenderObject class
 * and activation support from the b3Activation class.
 *
 * @see b3Item
 * @see b3RenderObject
 * @see b3Activation
 * @dotfile shape.dot "Class hierarchy of all shapes."
 */
class B3_PLUGIN b3Shape : public b3Item, public b3RenderObject, public b3Activation
{
	static const b3_gl_line    m_BoxGrids[];
	static const b3_gl_polygon m_BoxPolygons[];
	static const b3_f32        m_BoxTexcoord[];

	bool          m_EndLine = false;
	b3_count      m_Overhead, m_Heights, m_Widths;

protected:
	unsigned         m_xVertices = 0;  //!< The horizontal count of vertex subdivision.
	unsigned         m_yVertices = 0;  //!< The vertical count of vertex subdivision.
	b3_stencil_limit m_Limit {};       //!< The 2D bounding box of this shape in surface coordinates.

	b3_gl_line   *   GridsCyl    = nullptr;  //!< The cylinder line indices.
	b3_gl_polygon  * PolysCyl    = nullptr;  //!< The cylinder triangle indices.
	b3_gl_line   *   GridsCone   = nullptr;  //!< The cone line indices.
	b3_gl_polygon  * PolysCone   = nullptr;  //!< The cone triangle indices.

protected:
	B3_ITEM_BASE(b3Shape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3Shape); //!< These constructors handle default initialization.

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static  void        b3RegisterShapes();
	void                b3Write() override;
	virtual bool        b3Prepare(b3_preparation_info * prep_info) override;

	/**
	 * This method stores the basic shape values.
	 *
	 * @see b3Item::b3Write()
	 */
	virtual void        b3StoreShape();

	/**
	 * This method initializes animation activation.
	 */
	void        b3InitActivation();

	/**
	 * This method computes the material properties at the given surface point.
	 *
	 * @param surface The surface values which receives the material properties.
	 * @return The hit material.
	 * @see b3Material
	 * @see b3_surface
	 */
	b3Material * b3GetSurfaceValues(b3_surface * surface) const;

	/**
	 * This method computes the normal at the computed intersection point.
	 *
	 * @param ray The ray which contains information about the intersection point.
	 * @see b3Bump
	 */
	void        b3BumpNormal(b3_ray * ray) const;

	/**
	 * This method computes the stencil border in polar coordinates.
	 *
	 * @param limit The bounding box in polar coordinates.
	 */
	void        b3ComputeBound(b3_stencil_limit * limit) const;

	/**
	 * This method computes if the intersection point is valid. The intersection
	 * point is represented in the shapes' polar coordinates. If the intersection
	 * point due to stencelling is not valid False is returned and another
	 * intersection point is to be tried.
	 *
	 * @param polar The intersection point as polar coordinates.
	 * @return True if intersection point is valid.
	 * @see b3Condition
	 * @see b3Intersect()
	 * @see b3_polar
	 */
	virtual bool        b3CheckStencil(b3_polar * polar) const;

	/**
	 * This method transforms the shape with the given transformation matrix. The
	 * affine flag signals if the normals must be recomputed. Otherwise the
	 * normals can be simply transformed, too.
	 *
	 * @param transformation The transformation matrix.
	 * @param isAffine The flag if the transformation is affine.
	 */
	virtual void        b3Transform(b3_matrix * transformation, bool isAffine);

	/**
	 * This method returns the possible value range of the polar coordinates.
	 *
	 * @param info The shape bound info structure to fill.
	 * @see b3_stencil_bound
	 */
	virtual void        b3GetStencilBoundInfo(b3_stencil_bound * info) const;

	/**
	 * This method sets up picking information. This is for showing
	 * end points of direction vectors or positions.
	 *
	 * @param info The pick info class.
	 * @see b3PickInfo
	 */
	virtual void        b3SetupPicking(b3PickInfo * info);

	/**
	 * This method sets up grid information when picking. This is usable
	 * for drawing direction vectors.
	 *
	 * @param info The pick info class.
	 * @see b3PickInfo
	 */
	virtual void            b3SetupGrid(b3PickInfo * info);

	/**
	 * The implementation of this method computes the normal at the intersection point given in the
	 * ray structure.
	 *
	 * @param ray The ray structure which contains all information of the incident
	 *            ray and the intersection point.
	 */
	virtual void            b3Normal(b3_ray * ray B3_UNUSED) const
	{
	}

	/**
	 * This method returns the list base of the shapes bumps.
	 *
	 * @return The bump list.
	 */
	inline b3Base<b3Item> * b3GetBumpHead() const
	{
		return &m_Heads[0];
	}

	/**
	 * This method returns the list base of the shapes stencils.
	 *
	 * @return The stencil list.
	 */
	inline b3Base<b3Item> * b3GetConditionHead() const
	{
		return &m_Heads[1];
	}

	/**
	 * This method returns the list base of the shapes materials.
	 *
	 * @return The material list.
	 */
	inline b3Base<b3Item> * b3GetMaterialHead() const
	{
		return &m_Heads[2];
	}

	/**
	 * This method computes the derivates along the normal given by the
	 * ray structure.
	 *
	 * @param ray The ray which contains the normal.
	 * @return True on success.
	 */
	virtual bool           b3NormalDeriv(b3_ray * ray) const;

protected:
	/**
	 * This method computes the needed polygon faces in horizontal direction.
	 *
	 * @param xl The x coordinate of the lower bounding box corner in polar coordinates.
	 * @param yl The y coordinate of the lower bounding box corner in polar coordinates.
	 * @return The needed faces in horizontal direction.
	 */
	b3_count        b3GetIndexOverhead(b3_f64 xl, b3_f64 yl);
	void            b3GetDiffuseColor(b3Color & diffuse) const override;
	b3_f64          b3GetColors(b3Color & ambient, b3Color & diffuse, b3Color & specular) const override;
	bool            b3GetChess(b3Color & bColor, b3Color & wColor, b3_res & xRepeat, b3_res & yRepeat) const override;
	b3Tx      *     b3GetTexture(b3_f64 & xTrans, b3_f64 & yTrans, b3_f64 & xScale, b3_f64 & yScale) const override;
	bool            b3GetImage(b3Tx * image) const override;
	b3_render_mode  b3GetRenderMode() const override;

	/**
	 * This method computes the quadratic normals of the shape.
	 *
	 * @param normalize This flag signals if the normal should be normalized.
	 */
	void            b3ComputeQuadricNormals(bool normalize = true);

	/**
	 * This method computes vertices for a sphere.
	 *
	 * @param base The center of the sphere.
	 * @param dir The radius vector of the sphere.
	 */
	void            b3ComputeSphereVertices(const b3_vector & base, const b3_vector & dir);

	/**
	 * This method computes normals.
	 *
	 * @param base The center point of the sphere.
	 * @param normalize A flag to show if normalization is wanted.
	 */
	void            b3ComputeSphereNormals(const b3_vector & base, const bool normalize);

	/**
	 * This method computes the cylinder vertices.
	 *
	 * @param base The cylinder base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 */
	void            b3ComputeCylinderVertices(
		const b3_vector & base,
		const b3_vector & dir1,
		const b3_vector & dir2,
		const b3_vector & dir3);

	/**
	 * This method computes the cylinder indices for wire frame and polygons.
	 */
	void            b3ComputeCylinderIndices();

	/**
	 * This method computes the cone vertices.
	 *
	 * @param base The cone base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 */
	void            b3ComputeConeVertices(
		const b3_vector & base,
		const b3_vector & dir1,
		const b3_vector & dir2,
		const b3_vector & dir3);

	/**
	 * This method computes the cone indices for wire frame and polygons.
	 */
	void            b3ComputeConeIndices();

	/**
	 * This method computes the ellipsoid wireframe and polygon count.
	 *
	 * @param gridCount The resulting wireframe line count.
	 * @param polyCount The resulting polygon count
	 */
	void b3GetEllipsoidIndexCount(b3_count & gridCount, b3_count & polyCount);

	/**
	 * This method computes the ellipsoid vertices.
	 *
	 * @param base The cone base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 */
	void b3ComputeEllipsoidVertices(
		const b3_vector & base,
		const b3_vector & dir1,
		const b3_vector & dir2,
		const b3_vector & dir3);

	/**
	 * This method computes the ellipsoid indices for wire frame and polygons.
	 */
	void            b3ComputeEllipsoidIndices();

	/**
	 * This method computes the box vertices.
	 *
	 * @param base The cone base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 */
	void            b3ComputeBoxVertices(
		const b3_vector & base,
		const b3_vector & dir1,
		const b3_vector & dir2,
		const b3_vector & dir3);

	/**
	 * This method computes the box indices for wire frame and polygons.
	 */
	void            b3ComputeBoxIndices();

	/**
	 * This method computes the torus wireframe and polygon count.
	 *
	 * @param gridCount The resulting wireframe line count.
	 * @param polyCount The resulting polygon count
	 */
	void            b3GetTorusIndexCount(b3_count & gridCount, b3_count & polyCount);

	/**
	 * This method computes the box vertices.
	 *
	 * @param base The cone base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 * @param aRad The tube radius.
	 * @param bRad The ring radius.
	 */
	void            b3ComputeTorusVertices(
		const b3_vector & base,
		const b3_vector & dir1,
		const b3_vector & dir2,
		const b3_vector & dir3,
		const b3_f64 aRad,
		const b3_f64 bRad);

	/**
	 * This method computes the torus' normals.
	 */
	void            b3ComputeTorusNormals();

	/**
	 * This method computes the torus indices for wire frame and polygons.
	 */
	void            b3ComputeTorusIndices();

private:
	b3_index        b3FindVertex(b3_index vIndex);
};

/*************************************************************************
**                                                                      **
**                        Base class for all non CSG shapes             **
**                                                                      **
*************************************************************************/

/**
 * This class represents a simple shape. What we call simple can result
 * in several intersection ponts and does not involve other shapes against
 * to CSG shapes which may involve each intersection point from a whole
 * list of CSG shapes.
 *
 * @see b3CSGShape.
 */
class B3_PLUGIN b3SimpleShape : public b3Shape
{
protected:
	/**
	 * This method checks the given polar coordinates for validity. The
	 * stencil list is checked for that.
	 *
	 * @param polar The polar coordinates to check against the stencil list.
	 * @return True if the polar coordinates are valid.
	 */
	bool                b3CheckStencil(b3_polar * polar) const override;

protected:
	B3_ITEM_BASE(b3SimpleShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3SimpleShape); //!< These constructors handle default initialization.

public:
	/**
	 * This method intesects the given ray with this shape. If the result
	 * is positive there is a valid intersection point and marks the distance
	 * between the base point of the ray and the intersection point. If the
	 * result is negative it marks only that no intersection point was found.
	 * The implementation of this method should check the validity of the
	 * polar coordinates against the stencil list via the b3CheckStencil()
	 * method call and have to reject this intersection point.
	 *
	 * @param ray The ray to intersect with this shape.
	 * @param polar The polar coordinates of the resulting intersection point.
	 * @return The distance from the rays base point to the intersection point.
	 */
	virtual b3_f64 b3Intersect(
		b3_ray  *  ray   B3_UNUSED,
		b3_polar * polar B3_UNUSED)
	{
		return -1;
	}
};

/*************************************************************************
**                                                                      **
**                        Sphere                                        **
**                                                                      **
*************************************************************************/

/**
 * This class represents a simple sphere. It can create up to two
 * intersection points with a ray.
 */
class B3_PLUGIN b3Sphere : public b3SimpleShape    // Kugel
{
	b3_f64               m_QuadRadius;   // Quadrat vom Radius

public:
	b3_vector       	 m_Base;         // Mittelpunkt
	b3_vector       	 m_Dir;          // Radius

public:
	B3_ITEM_INIT(b3Sphere); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Sphere); //!< This constructor handles deserialization.

	void    b3StoreShape() override;
	bool    b3Prepare(b3_preparation_info * prep_info) override;
	void    b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	b3_f64  b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void    b3Normal(b3_ray * ray) const override;
	void    b3SetupPicking(b3PickInfo * info) override;
	void    b3Transform(b3_matrix * transformation, bool isAffine) override;

	/**
	 * This method computes the derivates along the normal given by the
	 * ray structure.
	 *
	 * @param ray The ray which contains the normal.
	 * @return True on success.
	 */
	bool    b3NormalDeriv(b3_ray * ray) const override;

protected:
	void    b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void    b3ComputeVertices() override;
	void    b3ComputeNormals(bool normalize = true) override;
	void    b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        2D shapes                                     **
**                                                                      **
*************************************************************************/

/**
 * This class represents 2d shapes with zero volume.
 */
class B3_PLUGIN b3Shape2 : public b3SimpleShape
{
protected:
	b3_vector           m_Normal;         //!< The precomputed normal.
	b3_f64              m_NormalLength;   //!< The length of the normal.

public:
	b3_vector           m_Base;           //!< Base point of 2d shapes.
	b3_vector           m_Dir1;           //!< The first direction vector
	b3_vector           m_Dir2;           //!< The second direction vector.

protected:
	B3_ITEM_BASE(b3Shape2); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3Shape2); //!< These constructors handle default initialization.

	void    b3StoreShape() override;
	bool    b3Prepare(b3_preparation_info * prep_info) override;
	void    b3Transform(b3_matrix * transformation, bool isAffine) override;
	void    b3Normal(b3_ray * ray) const override;
	void    b3SetupPicking(b3PickInfo * info) override;

	/**
	 * This method computes the derivates along the normal given by the
	 * ray structure.
	 *
	 * @param ray The ray which contains the normal.
	 * @return True on success.
	 */
	bool    b3NormalDeriv(b3_ray * ray) const override;
};

/*************************************************************************
**                                                                      **
**                        Area                                          **
**                                                                      **
*************************************************************************/

/**
 * This class represents a parallelogramme.
 */
class B3_PLUGIN b3Area : public b3Shape2
{
	b3_gl_vertex  m_AreaVertex[4];
	static b3_gl_line    m_AreaGrids[4];
	static b3_gl_polygon m_AreaPolygons[2];
	static b3_f32        m_AreaTexcoord[8];

public:
	B3_ITEM_INIT(b3Area); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Area); //!< This constructor handles deserialization.

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	b3_f64 b3Intersect(b3_ray * ray, b3_polar * polar) override;

protected:
	void   b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void   b3ComputeVertices() override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Disk                                          **
**                                                                      **
*************************************************************************/

/**
 * This class represents a 2d disk.
 */
class B3_PLUGIN b3Disk : public b3Shape2
{
public:
	B3_ITEM_INIT(b3Disk); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Disk); //!< This constructor handles deserialization.

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	b3_f64 b3Intersect(b3_ray * ray, b3_polar * polar) override;

protected:
	void   b3GetCount(
		b3RenderContext * context,
		b3_count     &    vertCount,
		b3_count     &    gridCount,
		b3_count     &    polyCount) override;
	void   b3ComputeVertices() override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        3D shapes                                     **
**                                                                      **
*************************************************************************/

/**
 * This ckass provides support for 3d shapes. It can create up to two intersection points
 * per ray. So this shape is also called a quadric.
 *
 * @see b3BaseTransformation
 */
class B3_PLUGIN b3Shape3 : public b3SimpleShape, public b3ShapeBaseTransformation
{
protected:
	B3_ITEM_BASE(b3Shape3); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3Shape3); //!< These constructors handle default initialization.

	void    b3StoreShape() override;
	bool    b3Prepare(b3_preparation_info * prep_info) override;
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	void    b3Transform(b3_matrix * transformation, bool isAffine) override;
	void    b3SetupPicking(b3PickInfo * info) override;

protected:
	virtual void    b3ComputeNormals(bool normalize = true) override;
};

/*************************************************************************
**                                                                      **
**                        Cylinder                                      **
**                                                                      **
*************************************************************************/

/**
 * This class represents a cylinder.
 *
 * @see b3Shape3
 */
class B3_PLUGIN b3Cylinder : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cylinder); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Cylinder); //!< This constructor handles deserialization.

	b3_f64 b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void   b3Normal(b3_ray * ray) const override;

protected:
	void   b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void   b3AllocVertexMemory(b3RenderContext * context) override;
	void   b3FreeVertexMemory() override;
	void   b3ComputeVertices() override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Cone                                          **
**                                                                      **
*************************************************************************/

/**
 * This class represents a cone.
 *
 * @see b3Shape3
 */
class B3_PLUGIN b3Cone : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cone); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Cone); //!< This constructor handles deserialization.

	b3_f64 b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void   b3Normal(b3_ray * ray) const override;

protected:
	void   b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void   b3AllocVertexMemory(b3RenderContext * context) override;
	void   b3ComputeVertices() override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Ellipsoid                                     **
**                                                                      **
*************************************************************************/

/**
 * This class represents an ellipsoid,
 *
 * @see b3Shape3
 */
class B3_PLUGIN b3Ellipsoid : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Ellipsoid); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Ellipsoid); //!< This constructor handles deserialization.

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	b3_f64 b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void   b3Normal(b3_ray * ray) const override;

protected:
	void   b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void   b3ComputeVertices() override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Box                                           **
**                                                                      **
*************************************************************************/

/**
 * This class represents a box,
 *
 * @see b3Shape3
 */
class B3_PLUGIN b3Box : public b3Shape3
{
	b3_gl_vertex   m_BoxVertex[6 * 4];

public:
	B3_ITEM_INIT(b3Box); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Box); //!< This constructor handles deserialization.

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	b3_f64 b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void   b3Normal(b3_ray * ray) const override;

protected:
	void   b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void   b3ComputeVertices() override;
	void   b3ComputeNormals(bool normalize = true) override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Torus (doughnut)                              **
**                                                                      **
*************************************************************************/

/**
 * This class represents a 3d torus. It can create up to four intersection points
 * per ray. So this shape is also called a quartic.
 */
class B3_PLUGIN b3Torus : public b3SimpleShape, public b3ShapeBaseTransformation
{
protected:
	b3_s32            m_lSize;
	b3_f64            m_aQuad, m_bQuad;   // squared lengths of aRad, bRad

public:
	b3_f64            m_aRad, m_bRad;     // radiuses of torus

public:
	B3_ITEM_INIT(b3Torus); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Torus); //!< This constructor handles deserialization.

	void    b3StoreShape() override;

	b3_f64  b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void    b3Normal(b3_ray * ray) const override;
	void    b3SetupPicking(b3PickInfo * info) override;

	bool    b3Prepare(b3_preparation_info * prep_info) override;
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	void    b3Transform(b3_matrix * transformation, bool isAffine) override;

protected:
	void    b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void    b3ComputeVertices() override;
	void    b3ComputeNormals(bool normalize = true) override;
	void    b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Shape containing triangles                    **
**                                                                      **
*************************************************************************/

// normal interpolation defines
#define B3_PHONG                   (1 << b3TriangleShape::B3_PHONG_B)     // use Phong interpolation
#define B3_NORMAL_VERTEX_VALID     (1 << b3TriangleShape::B3_NORMAL_VERTEX_VALID_B)     // normals of vertices valid, no auto computation
#define B3_NORMAL_FACE_VALID       (1 << b3TriangleShape::B3_NORMAL_FACE_VALID_B)     // normals of triangles valid, no auto computation

// Voxel index inside grid
#define GRID_INDEX(x,y,z,g) (((z) * (g) + (y)) * (g) + (x))

/**
 * This structure contains preparation information of one single triangle.
 */
struct b3_triainfo
{
	b3_vector base;     //!< The base point of the triangle.
	b3_vector dir1;     //!< The precomputed direction vector to the second triangle corner.
	b3_vector dir2;     //!< The precomputed direction vector to the third triangle corner.
	b3_vector Normal;   //!< The precomputed triangle normal.
};

/**
 * This class represents a triangle array which uses a vertex array. This class is
 * also been used for the internal spline representation. All spline shapes are
 * converted into triangles before rendering.
 */
class B3_PLUGIN b3TriangleShape : public b3SimpleShape
{
	b3Array<b3_index>   *  m_GridList     = nullptr;  // list of grids
	b3Array<b3_triainfo>   m_TriaInfos;
	bool                   m_GridComputed = false;

protected:
	b3_vector     m_Base;             //!< Base point of bounding box of all triangles.
	b3_vector     m_Size;             //!< Dimension of bounding box of all triangles.
	b3_count      m_GridSize    = 0;  //!< Number grid edges.
	b3_count      m_GridCount   = 0;  //!< Array size of m_GridList.

public:
	b3_count      m_VertexCount = 0;        //!< Number of vertices.
	b3_count      m_TriaCount   = 0;        //!< Number of triangles.
	b3_vertex  *  m_Vertices    = nullptr;  //!< Pointer to vertices.
	b3_triangle * m_Triangles   = nullptr;  //!< Pointer to triangle index list.
	b3_u32        m_Flags       = 0;        //!< Interpolation flags.
	b3_res        m_xTxSubDiv   = 0;        //!< Horizontal vertex distribution for texture mapping.
	b3_res        m_yTxSubDiv   = 0;        //!< Vertical vertex distribution for texture mapping.

	/**
	 * This enumeration lists the bit position of the triangle interpolation flags.
	 */
	enum b3_triangle_flag
	{
		B3_PHONG_B               = 0, //!< Use Phong interpolation.
		B3_NORMAL_VERTEX_VALID_B = 1, //!< Use the predefined normal list for each vertex.
		B3_NORMAL_FACE_VALID_B   = 2  //!< Use the predefined normal list for each triangle.
	};


protected:
	B3_ITEM_BASE(b3TriangleShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3TriangleShape); //!< These constructors handle default initialization.

	/**
	 * This destructor deinitializes the triangle shape.
	 */
	virtual ~b3TriangleShape();

	/**
	 * This method initializes a new triangle list. It initializes the
	 * vertex count, the triangle count and the texture mapping order.
	 *
	 * @param vertCount The new vertex count.
	 * @param triaCount The new triangle count.
	 * @param xSize     The horizontal vertex distribution for texture mapping.
	 * @param ySize     The vertical vertex distribution for texture mapping.
	 * @return True on success.
	 */
	bool            b3Init(b3_count vertCount, b3_count triaCount, b3_res xSize, b3_res ySize);
	b3_f64          b3Intersect(b3_ray * ray, b3_polar * polar) override;
	void            b3Normal(b3_ray * ray) const override;
	virtual bool    b3Prepare(b3_preparation_info * prep_info) override;
	virtual void    b3Transform(b3_matrix * transformation, bool isAffine) override;

	inline  bool    b3GetFlag(b3_u32 flag) const
	{
		return (m_Flags & flag) != 0;
	}

	inline void     b3SetFlag(b3_u32 flag, bool state)
	{
		m_Flags = (m_Flags & (~flag)) | (state ? flag : 0);
	}

protected:
	/**
	 * This method frees all prepared triangle references.
	 */
	void     b3FreeTriaRefs();

private:
	b3_f64   b3IntersectTriangleList(
		b3_ray     *     ray,
		b3_polar    *    polar,
		const b3_index   grid_index);

	void     b3Clear();
	void     b3PrepareGridList() const;

	void     b3SubdivideIntoGrid(
		b3_vector * P1,
		b3_vector * P2,
		b3_vector * P3,
		b3_index    triangle,
		b3_count    max) const;

	inline b3_index b3GridDistance(b3_f32 v1, b3_f32 v2, b3_f32 v3) const
	{
		const b3_index i1 = (b3_index)v1;
		const b3_index i2 = (b3_index)v2;
		const b3_index i3 = (b3_index)v3;
		const b3_index d12 = std::abs(i2 - i1);
		const b3_index d23 = std::abs(i3 - i2);
		const b3_index d31 = std::abs(i1 - i3);

		const b3_index result = std::max(d12, d23);
		return std::max(result, d31);
	}

	inline void b3ToGridSpace(const b3_vector * point, b3_vector * result) const
	{
		result->x = (point->x - m_Base.x) / m_Size.x;
		result->y = (point->y - m_Base.y) / m_Size.y;
		result->z = (point->z - m_Base.z) / m_Size.z;
	}

	inline b3_index b3GetGrid(b3_vector * p) const
	{
		return GRID_INDEX(
				b3_index(p->x), b3_index(p->y), b3_index(p->z), m_GridSize);
	}

	inline void b3AddTriangleToGrid(b3_index grid, b3_index triangle) const
	{
		if ((grid >= 0) && (grid < m_GridCount))
		{
			const b3_count max = m_GridList[grid].b3GetCount();

			if (max > 0)
			{
				if (m_GridList[grid][max - 1] == triangle)
				{
					return;
				}
			}
			m_GridList[grid].b3Add(triangle);
		}
	}
};

/*************************************************************************
**                                                                      **
**                        Triangle mesh                                 **
**                                                                      **
*************************************************************************/

/**
 * This class epresents a shape built from a triangle mesh.
 */
class B3_PLUGIN b3Triangles : public b3TriangleShape
{
public:
	B3_ITEM_INIT(b3Triangles); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Triangles); //!< This constructor handles deserialization.

	void   b3StoreShape() override;
	void   b3SetupPicking(b3PickInfo * info) override;

protected:
	void   b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void   b3ComputeVertices() override;
	void   b3ComputeNormals(bool normalize = true) override;
	void   b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        Rotating spline profile shape                 **
**                                                                      **
*************************************************************************/

/**
 * This class represents a rotation shape formed by a rotation axis and
 * a outline spline curve.
 */
class B3_PLUGIN b3SplineRotShape : public b3TriangleShape
{
	b3_count                  m_xSubDiv;
	b3_count                  m_ySubDiv;

public:
	b3_line                   m_Axis;       //!< The rotation axis.
	b3_u32                    m_rSubDiv;    //!< The rotation sub division.
	b3Spline                  m_Spline;     //!< The outline spline curve rotated around the rotation axis.
	b3Spline::b3_knot_vector  m_Knots;      //!< The knot vector vector of the outline spline curve.
	b3_vector        *        m_Controls;   //!< The control points of the outline spline curve.

public:
	B3_ITEM_INIT(b3SplineRotShape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SplineRotShape); //!< This constructor handles deserialization.

	/**
	 * This method initializes the outline spline curve with default values.
	 *
	 * @param degree The outline spline degree.
	 * @param controlNum The number of cotrol points.
	 * @param closed True if the spline curve is closed.
	 * @param subdiv The rotation sub division.
	 */
	void    b3Init(b3_count degree, b3_count controlNum, bool closed, b3_count subdiv);
	void    b3StoreShape() override;
	void    b3Transform(b3_matrix * transformation, bool isAffine) override;
	bool    b3Prepare(b3_preparation_info * prep_info) override;
	void    b3SetupPicking(b3PickInfo * info) override;
	void    b3SetupGrid(b3PickInfo * info) override;

protected:
	void    b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void    b3ComputeVertices() override;
	void    b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        2D spline shape                               **
**                                                                      **
*************************************************************************/

/**
 * This class represents the base class of a 2d spline shape.
 */
class B3_PLUGIN b3SplineShape : public b3TriangleShape
{
	unsigned         m_xSubDiv = 0, m_ySubDiv = 0;

protected:
	b3_count         m_GridVertexCount  = 0;  //!< The computed vertex count for wireframe shading.
	b3_count         m_SolidVertexCount = 0;  //!< The computed vertex count for solid shading.

public:
	b3Spline                  m_Spline[2];           //!< The two spline representations.
	b3Spline::b3_knot_vector  m_Knots[2];            //!< The two knot vectors.
	b3_vector        *        m_Controls = nullptr;  //!< The control point grid.

protected:
	B3_ITEM_BASE(b3SplineShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3SplineShape); //!< These constructors handle default initialization.

	/**
	 * This method initializes a spline shape and computes the appropriate shape type
	 * (area, cylinder or ring) depending on the control count and spline degrees.
	 *
	 * @param hDegree The horizontal spline degree.
	 * @param vDegree The vertical spline degree.
	 * @param hControlNum The horizontal control point dimension.
	 * @param vControlNum The vertical control point dimension.
	 */
	void b3Init(b3_count hDegree, b3_count vDegree, b3_count hControlNum, b3_count vControlNum);
	void b3Transform(b3_matrix * transformation, bool isAffine) override;
	void b3SetupPicking(b3PickInfo * info) override;
	void b3SetupGrid(b3PickInfo * info) override;

protected:
	void b3StoreShape() override;
	void b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void b3GetVertexRange(b3_index & start, b3_index & end) override;
	void b3ComputeVertices() override;
	void b3ComputeIndices() override;

private:
	void b3ComputeGridVertices();
	void b3ComputeSolidVertices();
	void b3ComputeGridIndices();
	void b3ComputeSolidIndices();

	bool b3Prepare(b3_preparation_info * prep_info) override;
};

/*************************************************************************
**                                                                      **
**                        Spline area                                   **
**                                                                      **
*************************************************************************/

/**
 * This class represents an piece of paper like shape formed by a 2d spline shape.
 *
 * @see b3SplineShape.
 */
class B3_PLUGIN b3SplineArea : public b3SplineShape
{
public:
	B3_ITEM_INIT(b3SplineArea); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SplineArea); //!< This constructor handles deserialization.
};

/*************************************************************************
**                                                                      **
**                        Spline tubus                                  **
**                                                                      **
*************************************************************************/

/**
 * This class represents a cylinder like shape formed by a 2d spline shape.
 *
 * @see b3SplineShape.
 */
class B3_PLUGIN b3SplineCylinder : public b3SplineShape
{
public:
	B3_ITEM_INIT(b3SplineCylinder); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SplineCylinder); //!< This constructor handles deserialization.
};

/*************************************************************************
**                                                                      **
**                        Spline ring                                   **
**                                                                      **
*************************************************************************/

/**
 * This class represents a torus like shape formed by a 2d spline shape.
 *
 * @see b3SplineShape.
 */
class B3_PLUGIN b3SplineRing : public b3SplineShape
{
public:
	B3_ITEM_INIT(b3SplineRing); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SplineRing); //!< This constructor handles deserialization.
};

/*************************************************************************
**                                                                      **
**                        CSG shapes                                    **
**                                                                      **
*************************************************************************/

#define CLASS_CSG           0x50500000
#define CSG_SPHERE          (CLASS_CSG | TYPE_SPHERE)
#define CSG_CONE            (CLASS_CSG | TYPE_CONE)
#define CSG_CYLINDER        (CLASS_CSG | TYPE_CYLINDER)
#define CSG_ELLIPSOID       (CLASS_CSG | TYPE_ELLIPSOID)
#define CSG_BOX             (CLASS_CSG | TYPE_BOX)
#define CSG_TORUS           (CLASS_CSG | TYPE_TORUS)

class b3CSGShape;

#define B3_MAX_CSG_SHAPES_PER_BBOX         80
#define B3_MAX_CSG_INTERSECTIONS_PER_BBOX 240

/**
 * This enumeration lists all CSG operation modes.
 */
enum b3_csg_operation
{
	B3_CSG_UNION     = MODE_OR,  //!< CSG union.
	B3_CSG_INTERSECT = MODE_AND, //!< CSG intersection.
	B3_CSG_SUB       = MODE_NOT  //!< CSG subtraction.
};

/**
 * This enumeration lists all possible intersection point types.
 */
enum b3_csg_index
{
	B3_CSG_SIDE = 0,   //!< Side of a CSG box.
	B3_CSG_FRONT,      //!< Front of a CSG box.
	B3_CSG_NORMAL,     //!< Every CSG shape.
	B3_CSG_BOTTOM,     //!< Bottom plate of cylinder or cone.
	B3_CSG_TOP         //!< Top plate of a CSG cylinder.
};

/**
 * This structure represents one intersection point a CSG shape can create.
 */
struct b3_csg_point
{
	b3_f64        m_Q;          //!< Distance to intersection points.
	b3CSGShape  * m_Shape;      //!< Shape which delivers the intersection points.
	b3_line64  *  m_BTLine;     //!< The base transformed ray.
	b3_csg_index  m_Index;      //!< The intersection point type.
};

/**
 * This structure collects all CSG intersection points on a ray.
 */
template<int count> struct b3_csg_intervals
{
	b3_count      m_Count;    //!< Number of collected CSG intersection points.
	b3_csg_point  m_x[count]; //!< All collected CSG intersection points of a bounding box.
};

typedef b3_csg_intervals<4>                                 b3_shape_intervals;
typedef b3_csg_intervals<B3_MAX_CSG_INTERSECTIONS_PER_BBOX> b3_bbox_intervals;

/*************************************************************************
**                                                                      **
**                        Base class for all CSG shapes                 **
**                                                                      **
*************************************************************************/

/**
 * This class represents one CSG shape.
 *
 * @note Stencelling does not work on CSG shapes.
 * @see b3SimpleShape
 */
class B3_PLUGIN b3CSGShape : public b3Shape
{
protected:
	b3_s32           m_Index; //!< This is for GUI purposes in Lines III.

public:
	b3_csg_operation m_Operation = B3_CSG_UNION;  //!< The CSG operation mode.

	static b3_csg_operation m_CSGMode[];  //!< The list of possible operation modes.

protected:
	B3_ITEM_BASE(b3CSGShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3CSGShape); //!< These constructors handle default initialization.

public:
	/**
	 * This method computes all intersection points created by this shape and the given ray.
	 *
	 * @param ray The shooted ray.
	 * @param interval The structure which collects the intersection points.
	 * @param BTLine The base transformed ray.
	 * @return True if any intersection point was found.
	 */
	virtual bool     b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine);

	/**
	 * This method gets the index of an operation mode table from the given operation mode.
	 *
	 * @param mode The operation mode.
	 * @return The resulting index.
	 */
	b3_size  b3GetOperationIndex(b3_csg_operation mode) const;

	/**
	 * This method sorts the intersection points by usage of the shapes CSG operation mode.
	 *
	 * @todo Document this more precisely.
	 * @param local The shape intersection points.
	 * @param list  The existing intervals.
	 * @param result The resulting intervals.
	 */
	void     b3Operate(
		const b3_shape_intervals * local,
		const b3_bbox_intervals  * list,
		b3_bbox_intervals     *    result);

	/**
	 * This method computes the polar coordinates for the found intersection point and
	 * puts the result into the given ray for further computation such as normal
	 * computation or material shading.
	 *
	 * @param ray The ray getting the polar coordinates.
	 * @param point The real computed intersection point.
	 * @see b3_ray
	 * @see b3_csg_point
	 */
	virtual void     b3InverseMap(b3_ray * ray, b3_csg_point * point);

	/**
	 * This method returns the maximal possible intersection count the shape can create.
	 * The result must be divdable by two.
	 *
	 * @return The maximal possible amount of intersection the shape can create.
	 */
	virtual b3_count b3GetMaxIntersections() const;
};

/*************************************************************************
**                                                                      **
**                        CSG sphere                                    **
**                                                                      **
*************************************************************************/

/**
 * This class represents a CSG sphere.
 */
class B3_PLUGIN b3CSGSphere : public b3CSGShape
{
	b3_f64            m_QuadRadius;       //!< The squared radius

public:
	b3_vector         m_Base;             //!< The center of the sphere
	b3_vector         m_Dir;              //!< The radius direction

public:
	B3_ITEM_INIT(b3CSGSphere); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGSphere); //!< This constructor handles deserialization.

	void     b3StoreShape() override;
	void     b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	bool     b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine) override;
	void     b3InverseMap(b3_ray * ray, b3_csg_point * point) override;
	b3_count b3GetMaxIntersections() const override;
	void     b3Normal(b3_ray * ray) const override;
	bool     b3Prepare(b3_preparation_info * prep_info) override;
	void     b3Transform(b3_matrix * transformation, bool isAffine) override;
	void     b3SetupPicking(b3PickInfo * info) override;

protected:
	void     b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void     b3ComputeVertices() override;
	void     b3ComputeNormals(bool normalize = true) override;
	void     b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        3D CSG shapes                                 **
**                                                                      **
*************************************************************************/

/**
 * This class represents CSG shapes which can produce two intersection points.
 */
class B3_PLUGIN b3CSGShape3 : public b3CSGShape, public b3ShapeBaseTransformation
{
protected:
	B3_ITEM_BASE(b3CSGShape3); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_ABSTRACT(b3CSGShape3); //!< These constructors handle default initialization.

	void     b3StoreShape() override;
	bool     b3Prepare(b3_preparation_info * prep_info) override;
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	void     b3Transform(b3_matrix * transformation, bool isAffine) override;
	void     b3SetupPicking(b3PickInfo * info) override;
	b3_count b3GetMaxIntersections() const override;

protected:
	virtual void     b3ComputeNormals(bool normalize = true) override;
};

/*************************************************************************
**                                                                      **
**                        CSG cylinder                                  **
**                                                                      **
*************************************************************************/

/**
 * This class represents a CSG cylinder.
 *
 * @note The cylinder is closed at the bottom and the top by disk plates.
 */
class B3_PLUGIN b3CSGCylinder : public b3CSGShape3
{
public:
	B3_ITEM_INIT(b3CSGCylinder); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGCylinder); //!< This constructor handles deserialization.

	bool     b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine) override;
	void     b3InverseMap(b3_ray * ray, b3_csg_point * point) override;
	void     b3Normal(b3_ray * ray) const override;

protected:
	void     b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void     b3ComputeVertices() override;
	void     b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        CSG cone                                      **
**                                                                      **
*************************************************************************/

/**
 * This class represents a CSG cone.
 *
 * @note The cone is closed at the bottom by a disk plate.
 */
class B3_PLUGIN b3CSGCone : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCone); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGCone); //!< This constructor handles deserialization.

	bool     b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine) override;
	void     b3InverseMap(b3_ray * ray, b3_csg_point * point) override;
	void     b3Normal(b3_ray * ray) const override;

protected:
	void     b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void     b3ComputeVertices() override;
	void     b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        CSG ellipsoid                                 **
**                                                                      **
*************************************************************************/

/**
 * This class represents a CSG ellipsoid.
 */
class B3_PLUGIN b3CSGEllipsoid : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGEllipsoid); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGEllipsoid); //!< This constructor handles deserialization.

	bool     b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine) override;
	void     b3InverseMap(b3_ray * ray, b3_csg_point * point) override;
	void     b3Normal(b3_ray * ray) const override;
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound * info) const override;

protected:
	void     b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void     b3ComputeVertices() override;
	void     b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        CSG box                                       **
**                                                                      **
*************************************************************************/

/**
 * This class represents a CSG box.
 */
class B3_PLUGIN b3CSGBox : public b3CSGShape3
{
	b3_gl_vertex    m_BoxVertex[6 * 4];

public:
	B3_ITEM_INIT(b3CSGBox); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGBox); //!< This constructor handles deserialization.

	bool    b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine) override;
	void    b3InverseMap(b3_ray * ray, b3_csg_point * point) override;
	void    b3Normal(b3_ray * ray) const override;
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound * info) const override;

protected:
	void    b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void    b3ComputeNormals(bool normalize = true) override;
	void    b3ComputeVertices() override;
	void    b3ComputeIndices() override;
};

/*************************************************************************
**                                                                      **
**                        CSG torus                                     **
**                                                                      **
*************************************************************************/

/**
 * This class represents a CSG torus. This shape can create four
 * intersection points.
 */
class B3_PLUGIN b3CSGTorus : public b3CSGShape, public b3ShapeBaseTransformation
{
	b3_f64             m_aQuad, m_bQuad;     // squared lengths of aRad, bRad

public:
	b3_f64             m_aRad, m_bRad;       // radiuses of torus

public:
	B3_ITEM_INIT(b3CSGTorus); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGTorus); //!< This constructor handles deserialization.

	void     b3StoreShape() override;
	bool     b3Intersect(b3_ray * ray, b3_shape_intervals * interval, b3_line64 * BTLine) override;
	void     b3InverseMap(b3_ray * ray, b3_csg_point * point) override;
	b3_count b3GetMaxIntersections() const override;
	void     b3Normal(b3_ray * ray) const override;
	void     b3SetupPicking(b3PickInfo * info) override;

	bool     b3Prepare(b3_preparation_info * prep_info) override;
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound * info) const override;
	void     b3Transform(b3_matrix * transformation, bool isAffine) override;

protected:
	void     b3GetCount(b3RenderContext * context, b3_count & vertCount, b3_count & gridCount, b3_count & polyCount) override;
	void     b3ComputeVertices() override;
	void     b3ComputeNormals(bool normalize = true) override;
	void     b3ComputeIndices() override;
};

#endif
