/*
**
**	$Filename:	b3Shape.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
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
	b3_bool b3NormalDeriv(b3_ray *ray);
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
 */
class B3_PLUGIN b3Shape : public b3Item, public b3RenderObject, public b3Activation
{
	static const b3_gl_line    m_BoxGrids[];
	static const b3_gl_polygon m_BoxPolygons[];
	static const b3_f32        m_BoxTexcoord[];

	             b3_bool       m_EndLine;
	             b3_count      m_Overhead,m_Heights,m_Widths;

protected:
	b3_count           xSize;         //!< The horizontal count of vertex subdivision.
	b3_count           ySize;         //!< The vertical count of vertex subdivision.
	b3_stencil_limit   m_Limit;       //!< The 2D bounding box of this shape in surface coordinates.

	b3_gl_line        *GridsCyl;      //!< The cylinder line indices.
	b3_gl_polygon     *PolysCyl;      //!< The cylinder triangle indices.
	b3_gl_line        *GridsCone;     //!< The cone line indices.
	b3_gl_polygon     *PolysCone;     //!< The cone triangle indices.

protected:
	B3_ITEM_BASE(b3Shape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3Shape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Shape); //!< This constructor handles deserialization.

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static  void        b3Register();
	        void        b3Write();
	virtual b3_bool     b3Prepare();

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
	b3Material *b3GetSurfaceValues(b3_surface *surface);

	/**
	 * This method computes the normal at the computed intersection point.
	 *
	 * @param ray The ray which contains information about the intersection point.
	 * @see b3Bump
	 */
	void        b3BumpNormal(b3_ray *ray);

	/**
	 * This method computes the stencil border in polar coordinates.
	 *
	 * @param limit The bounding box in polar coordinates.
	 */
	void        b3ComputeBound(b3_stencil_limit *limit);

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
	virtual b3_bool     b3CheckStencil(b3_polar *polar);

	/**
	 * This method transforms the shape with the given transformation matrix. The
	 * affine flag signals if the normals must be recomputed. Otherwise the
	 * normals can be simply transformed, too.
	 * 
	 * @param transformation The transformation matrix.
	 * @param isAffine The flag if the transformation is affine.
	 */
	virtual void        b3Transform(b3_matrix *transformation,b3_bool isAffine);

	/**
	 * This method returns the possible value range of the polar coordinates.
	 *
	 * @param info The shape bound info structure to fill.
	 * @see b3_stencil_bound
	 */
	virtual void        b3GetStencilBoundInfo(b3_stencil_bound *info);

	/**
	 * This method sets up picking information. This is for showing
	 * end points of direction vectors or positions.
	 *
	 * @param info The pick info class.
	 * @see b3PickInfo
	 */
	virtual void        b3SetupPicking(b3PickInfo *info);

	/**
	 * This method sets up grid information when picking. This is usable
	 * for drawing direction vectors.
	 *
	 * @param info The pick info class.
	 * @see b3PickInfo
	 */
	virtual void        b3SetupGrid(b3PickInfo *info);

	/**
	 * The implementation of this method computes the normal at the intersection point given in the
	 * ray structure.
	 *
	 * @param ray The ray structure which contains all information of the incident
	 *            ray and the intersection point.
	 */
	virtual inline void b3Normal(b3_ray *ray)
	{
	}

	/**
	 * This method returns the list base of the shapes bumps.
	 *
	 * @return The bump list.
	 */
	inline b3Base<b3Item> *b3GetBumpHead()
	{
		return &m_Heads[0];
	}

	/**
	 * This method returns the list base of the shapes stencils.
	 *
	 * @return The stencil list.
	 */
	inline b3Base<b3Item> *b3GetConditionHead()
	{
		return &m_Heads[1];
	}

	/**
	 * This method returns the list base of the shapes materials.
	 *
	 * @return The material list.
	 */
	inline b3Base<b3Item> *b3GetMaterialHead()
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
	virtual b3_bool        b3NormalDeriv(b3_ray *ray);

protected:
	/**
	 * This method computes the needed polygon faces in horizontal direction.
	 *
	 * @param xl The x coordinate of the lower bounding box corner in polar coordinates.
	 * @param yl The y coordinate of the lower bounding box corner in polar coordinates.
	 * @return The needed faces in horizontal direction.
	 */
	b3_count        b3GetIndexOverhead(b3_f64 xl,b3_f64 yl);
	void            b3GetDiffuseColor(b3Color &diffuse);
	b3_f64          b3GetColors(b3Color &ambient,b3Color &diffuse,b3Color &specular);
	b3_bool         b3GetChess(b3Color &bColor,b3Color &wColor,b3_res &xRepeat,b3_res &yRepeat);
	b3Tx           *b3GetTexture(b3_f64 &xTrans,b3_f64 &yTrans,b3_f64 &xScale,b3_f64 &yScale);
	b3_bool         b3GetImage(b3Tx *image);
	b3_render_mode  b3GetRenderMode();

	/**
	 * This method computes the quadratic normals of the shape.
	 *
	 * @param normalize This flag signals if the normal should be normalized.
	 */
	void            b3ComputeQuadricNormals(b3_bool normalize = true);

	/**
	 * This method computes vertices for a sphere.
	 *
	 * @param base The center of the sphere.
	 * @param dir The radius vector of the sphere.
	 */
	void            b3ComputeSphereVertices(b3_vector &base,b3_vector &dir);

	/**
	 * This method computes normals.
	 *
	 * @param base The center point of the sphere.
	 * @param normalize A flag to show if normalization is wanted.
	 */
	void            b3ComputeSphereNormals( b3_vector &base,b3_bool normalize);

	/**
	 * This method computes the cylinder vertices.
	 *
	 * @param base The cylinder base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 */
	void            b3ComputeCylinderVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);

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
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);

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
	void b3GetEllipsoidIndexCount(b3_count &gridCount,b3_count &polyCount);

	/**
	 * This method computes the ellipsoid vertices.
	 *
	 * @param base The cone base point.
	 * @param dir1 The first base plane direction vector.
	 * @param dir2 The second base plane direction vector.
	 * @param dir3 The axis direction vector.
	 */
	void b3ComputeEllipsoidVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);

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
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);

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
	void            b3GetTorusIndexCount(b3_count &gridCount,b3_count &polyCount);

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
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3,
		b3_f64    aRad,
		b3_f64    bRad);

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
	b3_bool             b3CheckStencil(b3_polar *polar);

protected:
	B3_ITEM_BASE(b3SimpleShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3SimpleShape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SimpleShape); //!< This constructor handles deserialization.

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
	virtual inline b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar)
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

	void    b3StoreShape();
	b3_bool b3Prepare();
	void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	b3_f64  b3Intersect(b3_ray *ray,b3_polar *polar);
	void    b3Normal(b3_ray *ray);
	void    b3SetupPicking(b3PickInfo *info);
	void    b3Transform(b3_matrix *transformation,b3_bool isAffine);

	/**
	 * This method computes the derivates along the normal given by the 
	 * ray structure.
	 *
	 * @param ray The ray which contains the normal.
	 * @return True on success.
	 */
	b3_bool b3NormalDeriv(b3_ray *ray);

protected:
	void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void    b3ComputeVertices();
	void    b3ComputeNormals(b3_bool normalize=true);
	void    b3ComputeIndices();
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
	B3_ITEM_INIT(b3Shape2); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Shape2); //!< This constructor handles deserialization.

	        void    b3StoreShape();
	        b3_bool b3Prepare();
	        void    b3Transform(b3_matrix *transformation,b3_bool isAffine);
	        void    b3Normal(b3_ray *ray);
			void    b3SetupPicking(b3PickInfo *info);

	/**
	 * This method computes the derivates along the normal given by the 
	 * ray structure.
	 *
	 * @param ray The ray which contains the normal.
	 * @return True on success.
	 */
	b3_bool b3NormalDeriv(b3_ray *ray);
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

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
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

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
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
	B3_ITEM_INIT(b3Shape3); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Shape3); //!< This constructor handles deserialization.

	        void    b3StoreShape();
	        b3_bool b3Prepare();
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void    b3Transform(b3_matrix *transformation,b3_bool isAffine);
			void    b3SetupPicking(b3PickInfo *info);

protected:
	virtual void    b3ComputeNormals(b3_bool normalize = true);
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

	b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	void   b3Normal(b3_ray *ray);

protected:
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3AllocVertexMemory(b3RenderContext *context);
	void   b3FreeVertexMemory();
	void   b3ComputeVertices();
	void   b3ComputeIndices();
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

	b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	void   b3Normal(b3_ray *ray);

protected:
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3AllocVertexMemory(b3RenderContext *context);
	void   b3ComputeVertices();
	void   b3ComputeIndices();
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

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	        void   b3Normal(b3_ray *ray);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
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

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	        void   b3Normal(b3_ray *ray);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeNormals(b3_bool normalize = true);
	        void   b3ComputeIndices();
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
	b3_f64            m_aQuad,m_bQuad;    // squared lengths of aRad, bRad

public:
	b3_f64            m_aRad, m_bRad;     // radiuses of torus

public:
	B3_ITEM_INIT(b3Torus); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Torus); //!< This constructor handles deserialization.

	        void    b3StoreShape();

	        b3_f64  b3Intersect(b3_ray *ray,b3_polar *polar);
	        void    b3Normal(b3_ray *ray);
	        void    b3SetupPicking(b3PickInfo *info);

	        b3_bool b3Prepare();
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void    b3Transform(b3_matrix *transformation,b3_bool isAffine);

protected:
	        void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void    b3ComputeVertices();
	        void    b3ComputeNormals(b3_bool normalize=true);
	        void    b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        Shape containing triangles                    **
**                                                                      **
*************************************************************************/

// normal interpolation defines
#define PHONG                   (1 << b3TriangleShape::B3_PHONG_B)     // use Phong interpolation
#define NORMAL_VERTEX_VALID     (1 << b3TriangleShape::B3_NORMAL_VERTEX_VALID_B)     // normals of vertices valid, no auto computation
#define NORMAL_FACE_VALID       (1 << b3TriangleShape::B3_NORMAL_FACE_VALID_B)     // normals of triangles valid, no auto computation

// Voxel index inside grid
#define GRID_INDEX(x,y,z,g) (((z) * (g) + (y)) * (g) + (x))

/**
 * This structure contains preparation information of one single triangle.
 */
struct b3_triainfo
{
	b3_vector32 base;     //!< The base point of the triangle.
	b3_vector32 dir1;     //!< The precomputed direction vector to the second triangle corner.
	b3_vector32 dir2;     //!< The precomputed direction vector to the third triangle corner.
	b3_vector32 Normal;   //!< The precomputed triangle normal.
};

/**
 * This class represents a triangle array which uses a vertex array. This class is
 * also been used for the internal spline representation. All spline shapes are
 * converted into triangles before rendering.
 */
class B3_PLUGIN b3TriangleShape : public b3SimpleShape
{
	b3Array<b3_index>     *m_GridList;       // list of grids
	b3Array<b3_triainfo>   m_TriaInfos;
	b3_bool                m_GridComputed;

protected:
	b3_vector              m_Base;           //!< Base point of bounding box of all triangles.
	b3_vector              m_Size;           //!< Dimension of bounding box of all triangles.
	b3_count               m_GridSize;       //!< Number grid edges.
	b3_count               m_GridCount;      //!< Array size of m_GridList.

public:
	b3_count               m_VertexCount;    //!< Number of vertices.
	b3_count               m_TriaCount;      //!< Number of triangles.
	b3_vertex             *m_Vertices;       //!< Pointer to vertices.
	b3_triangle           *m_Triangles;      //!< Pointer to triangle index list.
	b3_u32                 m_Flags;          //!< Interpolation flags.
	b3_res                 m_xSize;          //!< Horizontal vertex distribution for texture mapping.
	b3_res                 m_ySize;          //!< Vertical vertex distribution for texture mapping.

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
	B3_ITEM_INIT(b3TriangleShape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3TriangleShape); //!< This constructor handles deserialization.

	/**
	 * This destructor deinitializes the triangle shape.
	 */
	~b3TriangleShape();

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
	b3_bool b3Init(b3_count vertCount,b3_count triaCount,b3_res xSize,b3_res ySize);
	        b3_f64  b3Intersect(b3_ray *ray,b3_polar *polar);
	        void    b3Normal(b3_ray *ray);
	virtual b3_bool b3Prepare();
	virtual void    b3Transform(b3_matrix *transformation,b3_bool isAffine);

protected:
	/**
	 * This method frees all prepared triangle references.
	 */
	void    b3FreeTriaRefs();

private:
	        b3_f64   b3IntersectTriangleList(
				b3_ray    *ray,
				b3_polar  *polar,
				b3_index   index);

	        void     b3Clear();
	        void     b3PrepareGridList();

			void     b3SubdivideIntoGrid(
			    b3_vector *P1,
			    b3_vector *P2,
			    b3_vector *P3,
			    b3_index   triangle,
			    b3_count   max);

			inline b3_index b3GridDistance(b3_f32 v1,b3_f32 v2,b3_f32 v3)
			{
				b3_index i1 = (b3_index)v1;
				b3_index i2 = (b3_index)v2;
				b3_index i3 = (b3_index)v3;
				b3_index d12 = B3_ABS(i2 - i1);
				b3_index d23 = B3_ABS(i3 - i2);
				b3_index d31 = B3_ABS(i1 - i3);
				
				b3_index result = B3_MAX(d12,d23);
				return B3_MAX(result,d31);
			}
			
			inline void b3ToGridSpace(const b3_vector *point,b3_vector *result)
			{
				result->x = (point->x - m_Base.x) / m_Size.x;
				result->y = (point->y - m_Base.y) / m_Size.y;
				result->z = (point->z - m_Base.z) / m_Size.z;
			}

			inline b3_index b3GetGrid(b3_vector *p)
			{
				return GRID_INDEX(
					b3_index(p->x),b3_index(p->y),b3_index(p->z),m_GridSize);
			}

	        inline void b3AddTriangleToGrid(b3_index grid,b3_index triangle)
	        {
	        	if((grid >= 0) && (grid < m_GridCount))
	        	{
					b3_count max = m_GridList[grid].b3GetCount();

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

			static b3_count b3IntLog2(b3_count value);
			static b3_count b3IntLog3(b3_count value);
};

/*************************************************************************
**                                                                      **
**                        Triangle mesh                                 **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3Triangles : public b3TriangleShape
{
public:
	B3_ITEM_INIT(b3Triangles); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Triangles); //!< This constructor handles deserialization.

	void   b3StoreShape();
	void   b3SetupPicking(b3PickInfo *info);

protected:
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3ComputeVertices();
	void   b3ComputeNormals(b3_bool normalize=true);
	void   b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        Rotating spline profile shape                 **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3SplineRotShape : public b3TriangleShape
{
	b3_count         m_xSubDiv;
	b3_count         m_ySubDiv;

public:
	b3_line          m_Axis;                // for rotation shapes
	b3_s32           m_rSubDiv;             // sub division for rotation
	b3Spline         m_Spline;              // spline curve
	b3_knot_vector   m_Knots; // one knot vector
	b3_vector       *m_Controls;

public:
	B3_ITEM_INIT(b3SplineRotShape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SplineRotShape); //!< This constructor handles deserialization.

	void    b3Init(b3_count degree,b3_count control_num,b3_bool closed,b3_count subdiv);
	void    b3StoreShape();
	void    b3Transform(b3_matrix *transformation,b3_bool isAffine);
	b3_bool b3Prepare();
	void    b3SetupPicking(b3PickInfo *info);
	void    b3SetupGrid(b3PickInfo *info);

protected:
	void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void    b3ComputeVertices();
	void    b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        2D spline shape                               **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3SplineShape : public b3TriangleShape
{
	b3_count         m_xSubDiv,m_ySubDiv;

protected:
	b3_count         m_GridVertexCount;
	b3_count         m_SolidVertexCount;
	
public:
	b3Spline         m_Spline[2];
	b3_knot_vector   m_Knots[2];
	b3_vector       *m_Controls;

protected:
	B3_ITEM_BASE(b3SplineShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3SplineShape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SplineShape); //!< This constructor handles deserialization.

	void b3Transform(b3_matrix *transformation,b3_bool isAffine);
	void b3Init(b3_count hDegree,b3_count vDegree,b3_count hControlNum,b3_count vControlNum);
	void b3SetupPicking(b3PickInfo *info);
	void b3SetupGrid(b3PickInfo *info);

protected:
	void b3StoreShape();
	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3GetVertexRange(b3_index &start,b3_index &end);
	void b3ComputeVertices();
	void b3ComputeIndices();

private:
	void b3ComputeGridVertices();
	void b3ComputeSolidVertices();
	void b3ComputeGridIndices();
	void b3ComputeSolidIndices();

	b3_bool b3Prepare();
};

/*************************************************************************
**                                                                      **
**                        Spline area                                   **
**                                                                      **
*************************************************************************/

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

enum b3_csg_operation
{
	B3_CSG_UNION     = MODE_OR,
	B3_CSG_INTERSECT = MODE_AND,
	B3_CSG_SUB       = MODE_NOT
};

enum b3_csg_index
{
	B3_CSG_SIDE = 0,   // CSG box only
	B3_CSG_FRONT,      // CSG box only
	B3_CSG_NORMAL,     // every CSG shape
	B3_CSG_BOTTOM,     // CSG cylinder and cone
	B3_CSG_TOP         // CSG cylinder only
};

// structures for CSG use
struct b3_csg_point
{
	b3_f64        m_Q;          // distance to intersection points
	b3CSGShape   *m_Shape;      // shape which delivers the intersection points
	b3_line64    *m_BTLine;
	b3_csg_index  m_Index;      // surface index
};


// interval of intersection points
template<int count> struct b3_csg_intervals
{
	b3_count      m_Count;
	b3_csg_point  m_x[count];
};

typedef b3_csg_intervals<4>                                 b3_shape_intervals;
typedef b3_csg_intervals<B3_MAX_CSG_INTERSECTIONS_PER_BBOX> b3_bbox_intervals;

/*************************************************************************
**                                                                      **
**                        Base class for all CSG shapes                 **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGShape : public b3Shape
{
protected:
	b3_s32           m_Index;

public:
	       b3_csg_operation m_Operation;
	static b3_csg_operation m_CSGMode[];

protected:
	B3_ITEM_BASE(b3CSGShape); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3CSGShape); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGShape); //!< This constructor handles deserialization.

public:
	virtual b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        b3_size  b3GetOperationIndex(b3_csg_operation mode);
	        void     b3Operate(b3_shape_intervals *local,b3_bbox_intervals *list,b3_bbox_intervals *result);
	virtual void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	virtual b3_count b3GetMaxIntersections();
};

/*************************************************************************
**                                                                      **
**                        CSG sphere                                    **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGSphere : public b3CSGShape
{
	b3_f64            m_QuadRadius;       // squared radius

public:
	b3_vector         m_Base;             // mid of sphere
	b3_vector         m_Dir;              // direction

public:
	B3_ITEM_INIT(b3CSGSphere); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGSphere); //!< This constructor handles deserialization.

	void     b3StoreShape();
	void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	b3_count b3GetMaxIntersections();
	void     b3Normal(b3_ray *ray);
	b3_bool  b3Prepare();
	void     b3Transform(b3_matrix *transformation,b3_bool isAffine);
	void     b3SetupPicking(b3PickInfo *info);

protected:
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeNormals(b3_bool normalize=true);
	void     b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        3D CSG shapes                                 **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGShape3 : public b3CSGShape, public b3ShapeBaseTransformation
{
protected:
	B3_ITEM_BASE(b3CSGShape3); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3CSGShape3); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGShape3); //!< This constructor handles deserialization.

	        void     b3StoreShape();
	        b3_bool  b3Prepare();
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void     b3Transform(b3_matrix *transformation,b3_bool isAffine);
	        void     b3SetupPicking(b3PickInfo *info);
	        b3_count b3GetMaxIntersections();

protected:
	virtual void     b3ComputeNormals(b3_bool normalize = true);
};

/*************************************************************************
**                                                                      **
**                        CSG cylinder                                  **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGCylinder : public b3CSGShape3
{
public:
	B3_ITEM_INIT(b3CSGCylinder); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGCylinder); //!< This constructor handles deserialization.

	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	void     b3Normal(b3_ray *ray);

protected:
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        CSG cone                                      **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGCone : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCone); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGCone); //!< This constructor handles deserialization.

	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	void     b3Normal(b3_ray *ray);

protected:
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        CSG ellipsoid                                 **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGEllipsoid : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGEllipsoid); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGEllipsoid); //!< This constructor handles deserialization.

	        b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        void     b3Normal(b3_ray *ray);
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);

protected:
	        void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void     b3ComputeVertices();
	        void     b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        CSG box                                       **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGBox : public b3CSGShape3
{
	b3_gl_vertex    m_BoxVertex[6 * 4];

public:
	B3_ITEM_INIT(b3CSGBox); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGBox); //!< This constructor handles deserialization.

	        b3_bool b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void    b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        void    b3Normal(b3_ray *ray);
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);

protected:
	        void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void    b3ComputeVertices();
	        void    b3ComputeNormals(b3_bool normalize = true);
	        void    b3ComputeIndices();
};

/*************************************************************************
**                                                                      **
**                        CSG torus                                     **
**                                                                      **
*************************************************************************/

class B3_PLUGIN b3CSGTorus : public b3CSGShape, public b3ShapeBaseTransformation
{
	b3_f64             m_aQuad,m_bQuad;      // squared lengths of aRad, bRad

public:
	b3_f64             m_aRad, m_bRad;       // radiuses of torus

public:
	B3_ITEM_INIT(b3CSGTorus); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CSGTorus); //!< This constructor handles deserialization.

	        void     b3StoreShape();
	        b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        b3_count b3GetMaxIntersections();
	        void     b3Normal(b3_ray *ray);
	        void     b3SetupPicking(b3PickInfo *info);

	        b3_bool  b3Prepare();
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void     b3Transform(b3_matrix *transformation,b3_bool isAffine);

protected:
	        void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void     b3ComputeVertices();
	        void     b3ComputeNormals(b3_bool normalize=true);
	        void     b3ComputeIndices();
};

#endif
