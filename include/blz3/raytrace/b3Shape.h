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
#include "blz3/raytrace/b3Condition.h"
#include "blz3/raytrace/b3PickInfo.h"

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

class B3_PLUGIN b3ShapeBaseTrans
{
protected:
	b3_vector         m_Normals[3];       // cross products
	b3_f64            m_Denom;            // denominator of lin. system
	b3_f64            m_DirLen[3];        // length of direction vectors

public:
	b3_vector         m_Base;
	b3_vector         m_Dir1,m_Dir2,m_Dir3;

public:
	b3_bool b3NormalDeriv(b3_ray *ray);

protected:
	b3_bool b3Prepare();
	void    b3BaseTrans(b3_line64 *in,b3_line64 *out);
};

class b3Material;
class b3Condition;
class b3Bump;

struct b3_surface;

// same structure entries for all shapes
class B3_PLUGIN b3Shape : public b3Item, public b3RenderObject, public b3Activation
{
protected:
	b3_count           xSize,ySize;
	b3_stencil_limit   m_Limit;

	b3_gl_line        *GridsCyl;
	b3_gl_polygon     *PolysCyl;
	b3_gl_line        *GridsCone;
	b3_gl_polygon     *PolysCone;

protected:
	B3_ITEM_BASE(b3Shape);

public:
	B3_ITEM_INIT(b3Shape);
	B3_ITEM_LOAD(b3Shape);

	static  void        b3Register();
	        void        b3Write();
	virtual void        b3StoreShape();
	        void        b3InitActivation();
	        b3Material *b3GetSurfaceValues(b3_surface *surface);
	        void        b3BumpNormal(b3_ray *ray);
	        void        b3ComputeBound(b3_stencil_limit *limit);
	virtual b3_bool     b3CheckStencil(b3_polar *polar);
	virtual void        b3Normal(b3_ray *ray);
	virtual void        b3Transform(b3_matrix *transformation,b3_bool is_affine);
	virtual b3_bool     b3Prepare();
	virtual void        b3GetStencilBoundInfo(b3_stencil_bound *info);
	virtual void        b3SetupPicking(b3PickInfo *pick_info);
	virtual void        b3SetupGrid(b3PickInfo *pick_info);

	inline b3Base<b3Item> *b3GetBumpHead()
	{
		return &m_Heads[0];
	}

	inline b3Base<b3Item> *b3GetConditionHead()
	{
		return &m_Heads[1];
	}

	inline b3Base<b3Item> *b3GetMaterialHead()
	{
		return &m_Heads[2];
	}

	virtual b3_bool        b3NormalDeriv(b3_ray *ray);

protected:
	b3_count        b3GetIndexOverhead(b3_f64 xl,b3_f64 yl);
	void            b3GetDiffuseColor(b3Color &color);
	b3_f64          b3GetColors(b3Color &ambient,b3Color &diffuse,b3Color &specular);
	b3_bool         b3GetChess(b3Color &bColor,b3Color &wColor,b3_res &xRepeat,b3_res &yRepeat);
	b3Tx           *b3GetTexture(b3_f64 &xTrans,b3_f64 &yTrans,b3_f64 &xScale,b3_f64 &yScale);
	b3_bool         b3GetImage(b3Tx *image);
	b3_render_mode  b3GetRenderMode();

	void            b3ComputeQuadricNormals(b3_bool normalize = true);

	// Sphere
	void            b3ComputeSphereVertices(b3_vector &base,b3_vector &dir);
	void            b3ComputeSphereNormals( b3_vector &base,b3_bool normalize);

	// Cylinder
	void            b3ComputeCylinderVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void            b3ComputeCylinderIndices();

	// Cone
	void            b3ComputeConeVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void            b3ComputeConeIndices();

	// El           lipsoid
	void b3ComputeEllipsoidVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void            b3ComputeEllipsoidIndices();

	// Box
	void            b3ComputeBoxVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void            b3ComputeBoxIndices();

	// Torus
	void            b3ComputeTorusVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3,
		b3_f64    aRad,
		b3_f64    bRad);
	void            b3ComputeTorusNormals();
	void            b3ComputeTorusIndices();

private:
	b3_index        b3FindVertex(b3_index vIndex);
};

class B3_PLUGIN b3SimpleShape : public b3Shape
{
protected:
	b3_bool             b3CheckStencil(b3_polar *polar);

protected:
	B3_ITEM_BASE(b3SimpleShape);

public:
	B3_ITEM_INIT(b3SimpleShape);
	B3_ITEM_LOAD(b3SimpleShape);

public:
	virtual b3_f64      b3Intersect(b3_ray *ray,b3_polar *polar);
};

// SPHERE
class B3_PLUGIN b3Sphere : public b3SimpleShape    // Kugel
{
	b3_f64               m_QuadRadius;   // Quadrat vom Radius

public:
	b3_vector       	 m_Base;         // Mittelpunkt
	b3_vector       	 m_Dir;          // Radius

public:
	B3_ITEM_INIT(b3Sphere);
	B3_ITEM_LOAD(b3Sphere);

	void    b3StoreShape();
	void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	b3_f64  b3Intersect(b3_ray *ray,b3_polar *polar);
	void    b3Normal(b3_ray *ray);
	b3_bool b3NormalDeriv(b3_ray *ray);
	void    b3SetupPicking(b3PickInfo *pick_info);

	b3_bool b3Prepare();
	void    b3Transform(b3_matrix *transformation,b3_bool is_affine);

protected:
	void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void    b3ComputeVertices();
	void    b3ComputeNormals(b3_bool normalize=true);
	void    b3ComputeIndices();
};

// AREA, DISK
class B3_PLUGIN b3Shape2 : public b3SimpleShape
{
protected:
	b3_vector           m_Normal;
	b3_f64              m_NormalLength;   // normal length

public:
	b3_vector           m_Base;           // basis of area, disk
	b3_vector           m_Dir1,m_Dir2;    // direction vectors

protected:
	B3_ITEM_BASE(b3Shape2);

public:
	B3_ITEM_INIT(b3Shape2);
	B3_ITEM_LOAD(b3Shape2);

	        void    b3StoreShape();
	        b3_bool b3Prepare();
	        void    b3Transform(b3_matrix *transformation,b3_bool is_affine);
	        void    b3Normal(b3_ray *ray);
	        b3_bool b3NormalDeriv(b3_ray *ray);
			void    b3SetupPicking(b3PickInfo *pick_info);
};

class B3_PLUGIN b3Area : public b3Shape2
{
	b3_gl_vertex  area_vertex[4];

public:
	B3_ITEM_INIT(b3Area);
	B3_ITEM_LOAD(b3Area);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
};

class B3_PLUGIN b3Disk : public b3Shape2
{
public:
	B3_ITEM_INIT(b3Disk);
	B3_ITEM_LOAD(b3Disk);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
};

// CYLINDER, CONE, ELLIPSOID, BOX
class B3_PLUGIN b3Shape3 : public b3SimpleShape, public b3ShapeBaseTrans
{
protected:
	B3_ITEM_BASE(b3Shape3);

public:
	B3_ITEM_INIT(b3Shape3);
	B3_ITEM_LOAD(b3Shape3);

	        void    b3StoreShape();
	        b3_bool b3Prepare();
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void    b3Transform(b3_matrix *transformation,b3_bool is_affine);
			void    b3SetupPicking(b3PickInfo *pick_info);

protected:
	virtual void    b3ComputeNormals(b3_bool normalize = true);
};

class B3_PLUGIN b3Cylinder : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cylinder);
	B3_ITEM_LOAD(b3Cylinder);

	b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	void   b3Normal(b3_ray *ray);

protected:
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3AllocVertexMemory(b3RenderContext *context);
	void   b3ComputeVertices();
	void   b3ComputeIndices();
};

class B3_PLUGIN b3Cone : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cone);
	B3_ITEM_LOAD(b3Cone);

	b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	void   b3Normal(b3_ray *ray);

protected:
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3AllocVertexMemory(b3RenderContext *context);
	void   b3ComputeVertices();
	void   b3ComputeIndices();
};

class B3_PLUGIN b3Ellipsoid : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Ellipsoid);
	B3_ITEM_LOAD(b3Ellipsoid);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	        void   b3Normal(b3_ray *ray);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
};

class B3_PLUGIN b3Box : public b3Shape3
{
	b3_gl_vertex   m_BoxVertex[8 * 3];

public:
	B3_ITEM_INIT(b3Box);
	B3_ITEM_LOAD(b3Box);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar *polar);
	        void   b3Normal(b3_ray *ray);

protected:
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeNormals(b3_bool normalize = true);
	        void   b3ComputeIndices();
};

// DOUGHNUT, TORUS
class B3_PLUGIN b3Torus : public b3SimpleShape, public b3ShapeBaseTrans
{
protected:
	b3_s32            m_lSize;
	b3_f64            m_aQuad,m_bQuad;    // squared lengths of aRad, bRad

public:
	b3_f64            m_aRad, m_bRad;     // radiuses of torus

public:
	B3_ITEM_INIT(b3Torus);
	B3_ITEM_LOAD(b3Torus);

	        void    b3StoreShape();

	        b3_f64  b3Intersect(b3_ray *ray,b3_polar *polar);
	        void    b3Normal(b3_ray *ray);
	        void    b3SetupPicking(b3PickInfo *pick_info);

	        b3_bool b3Prepare();
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void    b3Transform(b3_matrix *transformation,b3_bool is_affine);

protected:
	        void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void    b3ComputeVertices();
	        void    b3ComputeNormals(b3_bool normalize=true);
	        void    b3ComputeIndices();
};

// normal interpolation defines
#define PHONG                   (1 << b3TriangleShape::B3_PHONG_B)     // use Phong interpolation
#define NORMAL_VERTEX_VALID     (1 << b3TriangleShape::B3_NORMAL_VERTEX_VALID_B)     // normals of vertices valid, no auto computation
#define NORMAL_FACE_VALID       (1 << b3TriangleShape::B3_NORMAL_FACE_VALID_B)     // normals of triangles valid, no auto computation

// Voxel index inside grid
#define GRID_INDEX(x,y,z,g) (((z) * (g) + (y)) * (g) + (x))

struct b3_triainfo
{
	b3_vector32 O,R1,R2;
	b3_vector32 Normal;
};

class B3_PLUGIN b3TriangleShape : public b3SimpleShape
{
	b3Array<b3_index>     *m_GridList;       // list of grids
	b3Array<b3_triainfo>   m_TriaInfos;
	b3_bool                m_GridComputed;

protected:
	b3_vector              m_Base,m_Size;    // size of bounding box of all triangles
	b3_count               m_GridSize;       // num grid edges
	b3_count               m_GridCount;      // array size of m_GridList;

public:
	b3_count               m_VertexCount;    // num of verteces and triangles
	b3_count               m_TriaCount;
	b3_vertex             *m_Vertices;
	b3_triangle           *m_Triangles;
	b3_u32                 m_Flags;          // interpolation flags
	b3_res                 m_xSize,m_ySize;  // triangle order

	enum b3_triangle_flag
	{
		B3_PHONG_B               = 0,
		B3_NORMAL_VERTEX_VALID_B = 1,
		B3_NORMAL_FACE_VALID_B   = 2
	};


protected:
	B3_ITEM_BASE(b3TriangleShape);

public:
	B3_ITEM_INIT(b3TriangleShape);
	B3_ITEM_LOAD(b3TriangleShape);

	               ~b3TriangleShape();
	        b3_bool b3Init(b3_count vertex_count,b3_count tria_count,b3_res xSize,b3_res ySize);
	        b3_f64  b3Intersect(b3_ray *ray,b3_polar *polar);
	        void    b3Normal(b3_ray *ray);
	virtual b3_bool b3Prepare();
	virtual void    b3Transform(b3_matrix *transformation,b3_bool is_affine);

protected:
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
					b3_index max = m_GridList[grid].b3GetCount();

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

// TRIANGLES
class B3_PLUGIN b3Triangles : public b3TriangleShape
{
public:
	B3_ITEM_INIT(b3Triangles);
	B3_ITEM_LOAD(b3Triangles);

	void   b3StoreShape();
	void   b3SetupPicking(b3PickInfo *pick_info);

protected:
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3ComputeVertices();
	void   b3ComputeNormals(b3_bool normalize=true);
	void   b3ComputeIndices();
};

// SPLINE_ROT
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
	B3_ITEM_INIT(b3SplineRotShape);
	B3_ITEM_LOAD(b3SplineRotShape);

	void    b3Init(b3_count degree,b3_count control_num,b3_bool closed,b3_count subdiv);
	void    b3StoreShape();
	void    b3Transform(b3_matrix *transformation,b3_bool is_affine);
	b3_bool b3Prepare();
	void    b3SetupPicking(b3PickInfo *pick_info);
	void    b3SetupGrid(b3PickInfo *pick_info);

protected:
	void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void    b3ComputeVertices();
	void    b3ComputeIndices();
};

// SPLINES_AREA, SPLINES_CYL, SPLINES_RING
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
	B3_ITEM_BASE(b3SplineShape);

public:
	B3_ITEM_INIT(b3SplineShape);
	B3_ITEM_LOAD(b3SplineShape);

	void b3Transform(b3_matrix *transformation,b3_bool is_affine);
	void b3Init(b3_count hDegree,b3_count vDegree,b3_count hControlNum,b3_count vControlNum);
	void b3SetupPicking(b3PickInfo *pick_info);
	void b3SetupGrid(b3PickInfo *pick_info);

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

class B3_PLUGIN b3SplineArea : public b3SplineShape
{
public:
	B3_ITEM_INIT(b3SplineArea);
	B3_ITEM_LOAD(b3SplineArea);
};

class B3_PLUGIN b3SplineCylinder : public b3SplineShape 
{
public:
	B3_ITEM_INIT(b3SplineCylinder);
	B3_ITEM_LOAD(b3SplineCylinder);
};

class B3_PLUGIN b3SplineRing : public b3SplineShape 
{
public:
	B3_ITEM_INIT(b3SplineRing);
	B3_ITEM_LOAD(b3SplineRing);
};

/*************************************************************************
**                                                                      **
**                        csg shapes                                    **
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

class B3_PLUGIN b3CSGShape : public b3Shape
{
protected:
	b3_s32           m_Index;

public:
	       b3_csg_operation m_Operation;
	static b3_csg_operation m_CSGMode[];

protected:
	B3_ITEM_BASE(b3CSGShape);

public:
	B3_ITEM_INIT(b3CSGShape);
	B3_ITEM_LOAD(b3CSGShape);

public:
	virtual b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        int      b3GetOperationIndex(b3_csg_operation mode);
	        void     b3Operate(b3_shape_intervals *local,b3_bbox_intervals *list,b3_bbox_intervals *result);
	virtual void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	virtual b3_count b3GetMaxIntersections();
};

// CSG_SPHERE
class B3_PLUGIN b3CSGSphere : public b3CSGShape
{
	b3_f64            m_QuadRadius;       // squared radius

public:
	b3_vector         m_Base;             // mid of sphere
	b3_vector         m_Dir;              // direction

public:
	B3_ITEM_INIT(b3CSGSphere);
	B3_ITEM_LOAD(b3CSGSphere);

	void     b3StoreShape();
	void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	b3_count b3GetMaxIntersections();
	void     b3Normal(b3_ray *ray);
	b3_bool  b3Prepare();
	void     b3Transform(b3_matrix *transformation,b3_bool is_affine);
	void     b3SetupPicking(b3PickInfo *pick_info);

protected:
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeNormals(b3_bool normalize=true);
	void     b3ComputeIndices();
};

// CSG_CYLINDER, CSG_CONE, CSG_ELLIPSOID, CSG_BOX
class B3_PLUGIN b3CSGShape3 : public b3CSGShape, public b3ShapeBaseTrans
{
protected:
	B3_ITEM_BASE(b3CSGShape3);

public:
	B3_ITEM_INIT(b3CSGShape3);
	B3_ITEM_LOAD(b3CSGShape3);

	        void     b3StoreShape();
	        b3_bool  b3Prepare();
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void     b3Transform(b3_matrix *transformation,b3_bool is_affine);
	        void     b3SetupPicking(b3PickInfo *pick_info);
	        b3_count b3GetMaxIntersections();

protected:
	virtual void     b3ComputeNormals(b3_bool normalize = true);
};

class B3_PLUGIN b3CSGCylinder : public b3CSGShape3
{
public:
	B3_ITEM_INIT(b3CSGCylinder);
	B3_ITEM_LOAD(b3CSGCylinder);

	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	void     b3Normal(b3_ray *ray);

protected:
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
};

class B3_PLUGIN b3CSGCone : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCone);
	B3_ITEM_LOAD(b3CSGCone);

	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	void     b3Normal(b3_ray *ray);

protected:
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
};

class B3_PLUGIN b3CSGEllipsoid : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGEllipsoid);
	B3_ITEM_LOAD(b3CSGEllipsoid);

	        b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        void     b3Normal(b3_ray *ray);
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);

protected:
	        void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void     b3ComputeVertices();
	        void     b3ComputeIndices();
};

class B3_PLUGIN b3CSGBox : public b3CSGShape3
{
	b3_gl_vertex    m_BoxVertex[8 * 3];

public:
	B3_ITEM_INIT(b3CSGBox);
	B3_ITEM_LOAD(b3CSGBox);

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

// CSG_TORUS
class B3_PLUGIN b3CSGTorus : public b3CSGShape, public b3ShapeBaseTrans
{
	b3_f64             m_aQuad,m_bQuad;      // squared lengths of aRad, bRad

public:
	b3_f64             m_aRad, m_bRad;       // radiuses of torus

public:
	B3_ITEM_INIT(b3CSGTorus);
	B3_ITEM_LOAD(b3CSGTorus);

	        void     b3StoreShape();
	        b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        b3_count b3GetMaxIntersections();
	        void     b3Normal(b3_ray *ray);
	        void     b3SetupPicking(b3PickInfo *pick_info);

	        b3_bool  b3Prepare();
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void     b3Transform(b3_matrix *transformation,b3_bool is_affine);

protected:
	        void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void     b3ComputeVertices();
	        void     b3ComputeNormals(b3_bool normalize=true);
	        void     b3ComputeIndices();
};

#endif
