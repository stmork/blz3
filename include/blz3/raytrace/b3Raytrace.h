/*
**
**	$Filename:	b3Raytrace.h $ 
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing Structure Definitions
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_RAYTRACE_H
#define B3_RAYTRACE_RAYTRACE_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/base/b3Spline.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3Render.h"

#define B3_TEXSTRINGLEN  128
#define B3_BOXSTRINGLEN   96
#define B3_CAMERANAMELEN  96

/*************************************************************************
**                                                                      **
**                        textures                                      **
**                                                                      **
*************************************************************************/

#define CLASS_TEXTURE       0x20000000
#define TYPE_USUAL          0x00000001
#define USUAL_TEXTURE       (CLASS_TEXTURE|TYPE_USUAL)

class b3Tx : public b3Link<b3Tx>
{
	char       *Name;             // name reference 
	b3_u08     *Data;             // plane data
	b3_u32     *Palette;          // palette (if needed)
	b3_s32      Planes;           // number of planes
	b3_s32      Type;             // internal type 
	b3_s32      FileType;         // file type
	b3_s32      xSize,ySize;      // resolution
	b3_s32      LastAccess;       // last access count
public:
	b3Tx();
};

/*************************************************************************
**                                                                      **
**                        einschraenkende Bedingungen                   **
**                                                                      **
*************************************************************************/

#define CLASS_CONDITION     0x30000000
#define MODE_OR             0x00000000
#define MODE_NOT            0x00008000
#define MODE_AND            0x00004000
#define MODE_NAND           0x0000c000
#define MODE_MASK           0x0000f000
#define MODE_FUNC           0x00000fff
#define TYPE_RECTANGLE      0x00000001
#define TYPE_CIRCLE         0x00000002
#define TYPE_CHESS          0x00000003
#define TYPE_SEGMENT        0x00000004
#define TYPE_TRIANGLE       0x00000005
#define TYPE_PARALLELOGRAM  0x00000006
#define TYPE_CTEXTURE       0x00000007
#define TYPE_CTEXTUREWRAP   0x00000008
#define TYPE_ELLIPSE        0x00000009

#define COND_RECTANGLE          (CLASS_CONDITION|TYPE_RECTANGLE)
#define COND_CIRCLE             (CLASS_CONDITION|TYPE_CIRCLE)
#define COND_SEGMENT            (CLASS_CONDITION|TYPE_SEGMENT)
#define COND_TRIANGLE           (CLASS_CONDITION|TYPE_TRIANGLE)
#define COND_PARALLELOGRAM      (CLASS_CONDITION|TYPE_PARALLELOGRAM)
#define COND_TEXTURE            (CLASS_CONDITION|TYPE_CTEXTURE)
#define COND_TEXTUREWRAP        (CLASS_CONDITION|TYPE_CTEXTUREWRAP)
#define COND_ELLIPSE            (CLASS_CONDITION|TYPE_ELLIPSE)

#define COND_NRECTANGLE         (CLASS_CONDITION|MODE_NAND|TYPE_RECTANGLE)
#define COND_NCIRCLE            (CLASS_CONDITION|MODE_NAND|TYPE_CIRCLE)
#define COND_NSEGMENT           (CLASS_CONDITION|MODE_NAND|TYPE_SEGMENT)
#define COND_NTRIANGLE          (CLASS_CONDITION|MODE_NAND|TYPE_TRIANGLE)
#define COND_NPARALLELOGRAM     (CLASS_CONDITION|MODE_NAND|TYPE_PARALLELOGRAM)
#define COND_NTEXTURE           (CLASS_CONDITION|MODE_NAND|TYPE_CTEXTURE)
#define COND_NTEXTUREWRAP       (CLASS_CONDITION|MODE_NAND|TYPE_CTEXTUREWRAP)
#define COND_NELLIPSE           (CLASS_CONDITION|MODE_NAND|TYPE_ELLIPSE)

#define COND_NOR_RECTANGLE      (CLASS_CONDITION|MODE_NOT|TYPE_RECTANGLE)
#define COND_NOR_CIRCLE         (CLASS_CONDITION|MODE_NOT|TYPE_CIRCLE)
#define COND_NOR_SEGMENT        (CLASS_CONDITION|MODE_NOT|TYPE_SEGMENT)
#define COND_NOR_TRIANGLE       (CLASS_CONDITION|MODE_NOT|TYPE_TRIANGLE)
#define COND_NOR_PARALLELOGRAM  (CLASS_CONDITION|MODE_NOT|TYPE_PARALLELOGRAM)
#define COND_NOR_TEXTURE        (CLASS_CONDITION|MODE_NOT|TYPE_CTEXTURE)
#define COND_NOR_TEXTUREWRAP    (CLASS_CONDITION|MODE_NOT|TYPE_CTEXTUREWRAP)
#define COND_NOR_ELLIPSE        (CLASS_CONDITION|MODE_NOT|TYPE_ELLIPSE)

#define COND_ARECTANGLE         (CLASS_CONDITION|MODE_AND|TYPE_RECTANGLE)
#define COND_ACIRCLE            (CLASS_CONDITION|MODE_AND|TYPE_CIRCLE)
#define COND_ASEGMENT           (CLASS_CONDITION|MODE_AND|TYPE_SEGMENT)
#define COND_ATRIANGLE          (CLASS_CONDITION|MODE_AND|TYPE_TRIANGLE)
#define COND_APARALLELOGRAM     (CLASS_CONDITION|MODE_AND|TYPE_PARALLELOGRAM)
#define COND_ATEXTURE           (CLASS_CONDITION|MODE_AND|TYPE_CTEXTURE)
#define COND_ATEXTUREWRAP       (CLASS_CONDITION|MODE_AND|TYPE_CTEXTUREWRAP)
#define COND_AELLIPSE           (CLASS_CONDITION|MODE_AND|TYPE_ELLIPSE)


typedef struct b3_cond_limit
{
	b3_f32 x1,y1,x2,y2;
} b3CondLimit;

class b3InitCondition
{
protected:
	static void b3Init();
};

class b3Condition : public b3Item
{
protected:
	b3Condition(b3_size class_size,b3_u32  class_type);

public:

	B3_ITEM_INIT(b3Condition);
	B3_ITEM_LOAD(b3Condition);

	virtual void b3ComputeBound(b3CondLimit *limit);
protected:
	static void b3CheckInnerBound(b3CondLimit *limit,b3CondLimit *object);
	static void b3CheckOuterBound(b3CondLimit *limit,b3CondLimit *object);
};

// TYPE_RECTANGLE
class b3CondRectangle : public b3Condition
{
protected:
	b3_f32  xStart,yStart;    // rel. start coordinates
	b3_f32  xEnd,yEnd;        // rel. end koordinates
	b3_s32  Flags;

public:
	B3_ITEM_INIT(b3CondRectangle);
	B3_ITEM_LOAD(b3CondRectangle);

	void b3ComputeBound(b3CondLimit *limit);
};

#define RCB_ACTIVE  0
#define RCF_ACTIVE (1 << RCB_ACTIVE)

// TYPE_TRIANGLE, TYPE_PARALLELOGRAM
class b3Cond2 : public b3Condition
{
protected:
	b3_f32  xPos,yPos;        // base of triangle/ parallelogramme
	b3_f32  xDir1,yDir1;      // direction 1
	b3_f32  xDir2,yDir2;      // direction 2
	b3_f32  denom;

protected:
	b3Cond2(b3_size class_size,b3_u32  class_type);

public:
	B3_ITEM_INIT(b3Cond2);
	B3_ITEM_LOAD(b3Cond2);
};

class b3CondPara : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondPara);
	B3_ITEM_LOAD(b3CondPara);

	void b3ComputeBound(b3CondLimit *limit);
};

class b3CondTria : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondTria);
	B3_ITEM_LOAD(b3CondTria);

	void b3ComputeBound(b3CondLimit *limit);
};


// TYPE_CIRCLE
class b3CondCircle : public b3Condition
{
protected:
	b3_f32  xCenter,yCenter,radius;      // Mittelpunkt und Radius

public:
	B3_ITEM_INIT(b3CondCircle);
	B3_ITEM_LOAD(b3CondCircle);

	void b3ComputeBound(b3CondLimit *limit);
};

// TYPE_SEGMENT
class b3CondSegment : public b3Condition
{
protected:
	b3_f32  xCenter,yCenter;     // Mittelpunkt
	b3_f32  radStart,radEnd;     // gültiger Bereich im Ring
	b3_f32  angleStart,angleEnd; // Segment

public:
	B3_ITEM_INIT(b3CondSegment);
	B3_ITEM_LOAD(b3CondSegment);

	void b3ComputeBound(b3CondLimit *limit);
};

// TYPE_ELLIPSE
class b3CondEllipse : public b3Condition
{
protected:
	b3_f32  xCenter,yCenter;     // Mittelpunkt
	b3_f32  xRadius,yRadius;     // Radien
	b3_f32  radStart,radEnd;     // gültiger Bereich im Ring
	b3_f32  angleStart,angleEnd; // Segment

public:
	B3_ITEM_INIT(b3CondEllipse);
	B3_ITEM_LOAD(b3CondEllipse);

	void b3ComputeBound(b3CondLimit *limit);
};

// TYPE_TEXTURE
class b3CondTexture : public b3Condition
{
protected:
	b3Tx   *Texture;
	b3_s32  Flags;               // unused yet
	b3_f32  xStart,yStart;       // base of texture
	b3_f32  xScale,yScale;       // scale of texture
	b3_s32  xTimes,yTimes;       // repetion of texture in x- and y-direction
	b3Path  Name;  // title

public:
	B3_ITEM_INIT(b3CondTexture);
	B3_ITEM_LOAD(b3CondTexture);

	void b3ComputeBound(b3CondLimit *limit);
};

// TYPE_WRAP_TEXTURE
class b3CondWrapTexture : public b3Condition
{
protected:
	b3Tx   *Texture;
	b3_s32  Flags;               // unused yet
	b3_f32  xStart,yStart;       // borders in hor. direction
	b3_f32  xEnd,yEnd;           // borders in vert. direction
	b3Path  Name;                // title

public:
	B3_ITEM_INIT(b3CondWrapTexture);
	B3_ITEM_LOAD(b3CondWrapTexture);

	void b3ComputeBound(b3CondLimit *limit);
};

/*************************************************************************
**                                                                      **
**                        bump mapping                                  **
**                                                                      **
*************************************************************************/

#define CLASS_BUMP          0x28000000
#define TYPE_NOISE          0x00000001
#define TYPE_MARBLE         0x00000002
#define TYPE_TEXTURE        0x00000003
#define TYPE_WATER          0x00000004
#define TYPE_WAVE           0x00000005
#define TYPE_GLOSSY         0x00000006
#define BUMP_NOISE          (CLASS_BUMP|TYPE_NOISE)
#define BUMP_MARBLE         (CLASS_BUMP|TYPE_MARBLE)
#define BUMP_TEXTURE        (CLASS_BUMP|TYPE_TEXTURE)
#define BUMP_WATER          (CLASS_BUMP|TYPE_WATER)
#define BUMP_WAVE           (CLASS_BUMP|TYPE_WAVE)
#define BUMP_GLOSSY         (CLASS_BUMP|TYPE_GLOSSY)

class b3InitBump
{
protected:
	static void b3Init();
};

class b3Bump : public b3Item
{
protected:
	b3Bump(b3_size class_size,b3_u32 classtype);

public:
	B3_ITEM_INIT(b3Bump);
	B3_ITEM_LOAD(b3Bump);
};

// BUMP_NOISE, BUMP_MARBLE
class b3BumpNoise : public b3Bump
{
	b3_vector   Scale;
	b3_f32      Size;

public:
	B3_ITEM_INIT(b3BumpNoise);
	B3_ITEM_LOAD(b3BumpNoise);
};

// BUMP_TEXTURE
class b3BumpTexture : public b3Bump
{
	b3_f32      xStart,yStart;       // base of bump texture
	b3_f32      xScale,yScale;       // scale of bump texture
	b3_s32      xTimes,yTimes;       // repetition
	b3_f32      Intensity;           // non zero
	b3Tx       *Texture;
	b3_s32      Flags;
	b3Path      Name;

public:
	B3_ITEM_INIT(b3BumpTexture);
	B3_ITEM_LOAD(b3BumpTexture);
};

// BUMP_WATER
class b3BumpWater : public b3Bump
{
	b3_s32      ScaleFlag;           // use BBox coords or direct coord
	b3_vector   ScaleIPoint;         // intersection point scalar
	b3_f32      ScaleRad;            // radius scalar
	b3_f32      ScaleTime;           // time period for wave swing

public:
	B3_ITEM_INIT(b3BumpWater);
	B3_ITEM_LOAD(b3BumpWater);
};

// BUMP_WAVE
class b3BumpWave : public b3Bump
{
	b3_s32      Flags;               // use BBox coords or direct coord
	b3_vector   Scale;               // point scalar
	b3_f32      Amplitude;           // amplitude

public:
	B3_ITEM_INIT(b3BumpWave);
	B3_ITEM_LOAD(b3BumpWave);
};

// BUMP_GLOSSY */
class b3BumpGlossy : public b3Bump
{
	b3_s32      Flags;
	b3_f32      Intensity;

public:
	B3_ITEM_INIT(b3BumpGlossy);
	B3_ITEM_LOAD(b3BumpGlossy);
};


//flags for WaterBump, WaveBump
#define BUMP_IPOINT  1

/*************************************************************************
**                                                                      **
**                        materials                                     **
**                                                                      **
*************************************************************************/

#define CLASS_MATERIAL      0x40000000
#define TYPE_NORMMATERIAL   0x00000001
#define TYPE_TEXTUREMAT     0x00000002
#define TYPE_CHESSMAT       0x00000003
#define TYPE_WRAPTEXTUREMAT 0x00000004
#define TYPE_MARBLE_MAT     0x00000005
#define TYPE_SLIDE          0x00000006
#define TYPE_WOOD           0x00000007

// WARNING: GL uses define MATERIAL, too!
#define MAT_NORMAL          (CLASS_MATERIAL|TYPE_NORMMATERIAL)
#define MATERIAL            (CLASS_MATERIAL|TYPE_NORMMATERIAL)
#define TEXTURE             (CLASS_MATERIAL|TYPE_TEXTUREMAT)
#define CHESS               (CLASS_MATERIAL|TYPE_CHESSMAT)
#define WRAPTEXTURE         (CLASS_MATERIAL|TYPE_WRAPTEXTUREMAT)
#define MARBLE              (CLASS_MATERIAL|TYPE_MARBLE_MAT)
#define SLIDE               (CLASS_MATERIAL|TYPE_SLIDE)
#define WOOD                (CLASS_MATERIAL|TYPE_WOOD)

class b3InitMaterial
{
protected:
	static void b3Init();
};

class b3Material : public b3Item
{
protected:
	b3Material(b3_size class_size,b3_u32 classtype);

public:
	B3_ITEM_INIT(b3Material);
	B3_ITEM_LOAD(b3Material);

	virtual b3_bool b3GetColors(
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// MATERIAL or MAT_NORMAL
class b3MatNormal : public b3Material
{
	b3_color          DiffColor,AmbColor,SpecColor;
	b3_f32            Reflection;          // self explaining
	b3_f32            Refraction;
	b3_f32            RefrValue;
	b3_f32            HighLight;
	b3_s32            Flags;

public:
	B3_ITEM_INIT(b3MatNormal);
	B3_ITEM_LOAD(b3MatNormal);

	b3_bool b3GetColors(
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// CHESS
class b3MatChess : public b3Material 
{
	b3_color          DiffColor[2],AmbColor[2],SpecColor[2];
	b3_f32            Reflection[2];         // same like NormMaterial, but
	b3_f32            Refraction[2];
	b3_f32            RefrValue[2];
	b3_f32            HighLight[2];
	b3_s32            Flags;
	b3_s32            xTimes,yTimes;

public:
	B3_ITEM_INIT(b3MatChess);
	B3_ITEM_LOAD(b3MatChess);
};

// MARBLE
class b3MatMarble : public b3Material 
{
	b3_color          DiffColor,AmbColor,SpecColor;
	b3_vector         Scale;
	b3_f32            Reflection;
	b3_f32            Refraction;
	b3_f32            RefrValue;
	b3_f32            HighLight;
	b3_s32            Flags;
	b3_s32            xTimes,yTimes;

public:
	B3_ITEM_INIT(b3MatMarble);
	B3_ITEM_LOAD(b3MatMarble);
};

// WOOD
class b3MatWood : public b3Material 
{
	b3_color          DiffColor,AmbColor,SpecColor;
	b3_vector         Scale;
	b3_f32            Reflection;
	b3_f32            Refraction;
	b3_f32            RefrValue;
	b3_f32            HighLight;
	b3_s32            Flags;
	b3_s32            xTimes,yTimes;

public:
	B3_ITEM_INIT(b3MatWood);
	B3_ITEM_LOAD(b3MatWood);
};

// TEXTURE
class b3MatTexture : public b3Material 
{
	b3_f32            Reflection;
	b3_f32            Refraction;
	b3_f32            RefrValue;
	b3_f32            HighLight;
	b3_f32            xStart,yStart;    // surface coordinate start
	b3_f32            xScale,yScale;    // texture scale
	b3_s32            xTimes,yTimes;    // repetition in x- y-direction
	b3Tx             *Texture;
	b3_s32            Flags;
	b3Path            Name;

public:
	B3_ITEM_INIT(b3MatTexture);
	B3_ITEM_LOAD(b3MatTexture);
};

// WRAPTEXTURE
class b3MatWrapTexture : public b3Material 
{
	b3_f32            Reflection;
	b3_f32            Refraction;
	b3_f32            RefrValue;
	b3_f32            HighLight;
	b3_f32            xStart,yStart;    // surface coordinate start
	b3_f32            xEnd,yEnd;        // surface coordinate end
	b3Tx             *Texture;          // only one texture (compat. Dali)
	b3_s32            Flags;
	b3Path            Name;

public:
	B3_ITEM_INIT(b3MatWrapTexture);
	B3_ITEM_LOAD(b3MatWrapTexture);
};

// SLIDE
class b3MatSlide : public b3Material 
{
	b3_color          DiffColor[2],AmbColor[2],SpecColor[2]; // start color
	b3_f32            From,To;             // rel. polar values of start, end
	b3_f32            Reflection;          // normal material definitions
	b3_f32            Refraction;
	b3_f32            RefrValue;
	b3_f32            HighLight;
	b3_s32            ModeFlag;            // direction and cut flags, see below

public:
	B3_ITEM_INIT(b3MatSlide);
	B3_ITEM_LOAD(b3MatSlide);
};

#define DIR_SLIDE_BIT       0
#define CUT_SLIDE_BIT       1
#define XINVERT_BIT         2
#define YINVERT_BIT         3
#define MAT_INSIDE_BIT      4

#define SLIDE_CUT           (1 << CUT_SLIDE_BIT)
#define MAT_XINVERT			(1 << XINVERT_BIT)
#define MAT_YINVERT         (1 << YINVERT_BIT)
#define MAT_INSIDE          (1 << MAT_INSIDE_BIT)

#define XSLIDE              0x00000000
#define YSLIDE              0x00000001
#define XSLIDE_CUT          (XSLIDE|SLIDE_CUT)
#define YSLIDE_CUT          (YSLIDE|SLIDE_CUT)

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

class b3RenderShapeContext : public b3RenderContext
{
	b3_count   subdiv;
	b3_f64     Sin[B3_MAX_RENDER_SUBDIV + 1];
	b3_f64     Cos[B3_MAX_RENDER_SUBDIV + 1];
	b3_vector *Between;
#ifdef BLZ3_USE_OPENGL
	GLushort  *CylinderIndices;
	GLushort  *CylinderPolygons;
	GLushort  *ConeIndices;
	GLushort  *ConePolygons;
#endif

public:
	           b3RenderShapeContext(b3_count subdiv = 16);
	void       b3InitSubdiv(b3_count subdiv);
	b3_count   b3GetSubdiv();
	b3_f64    *b3GetCosTable();
	b3_f64    *b3GetSinTable();
	b3_vector *b3GetSplineAux();

#ifdef BLZ3_USE_OPENGL
	GLushort  *b3GetCylinderIndices();
	GLushort  *b3GetCylinderPolygons();
	GLushort  *b3GetConeIndices();
	GLushort  *b3GetConePolygons();
#endif
};

class b3RenderShapeObject : public b3RenderObject
{
protected:
	b3_count         SinCosSteps;
	b3_vector       *Between;
	b3_f64          *Cos;
	b3_f64          *Sin;
	b3_f64           Epsilon;

	b3CondLimit      Limit;

public:
	         b3RenderShapeObject();

protected:
	b3_count b3GetIndexOverhead(b3_f64 xl,b3_f64 yl);
};

class b3InitShape
{
protected:
	static void b3Init();
};

// same structure entries for all shapes
class b3Shape : public b3Item, public b3RenderShapeObject
{
protected:
	b3_vector        Normal;
	b3_polar         Polar;

	b3_count         xSize,ySize;

protected:
	     b3Shape(b3_size class_size,b3_u32 class_type);
	void b3GetDiffuseColor(b3_color *color);

public:
	B3_ITEM_INIT(b3Shape);
	B3_ITEM_LOAD(b3Shape);

	        void b3ComputeBound(b3CondLimit *limit);
	virtual void b3Intersect();
};

class b3RenderShape : public b3Shape
{
protected:
#ifdef BLZ3_USE_OPENGL
	GLushort *GridsCyl;
	GLushort *PolysCyl;
	GLushort *GridsCone;
	GLushort *PolysCone;
#endif

protected:
	b3RenderShape(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3RenderShape);
	B3_ITEM_LOAD(b3RenderShape);

protected:
	// Sphere
	void b3ComputeSphereVertices(
		b3_vector &base,
		b3_vector &dir);

	// Cylinder
	void b3ComputeCylinderVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void b3ComputeCylinderIndices();

	// Cone
	void b3ComputeConeVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void b3ComputeConeIndices();

	// Ellipsoid
	void b3ComputeEllipsoidVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void b3ComputeEllipsoidIndices();

	// Box
	void b3ComputeBoxVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3);
	void b3ComputeBoxIndices();

	// Torus
	void b3ComputeTorusVertices(
		b3_vector &base,
		b3_vector &dir1,
		b3_vector &dir2,
		b3_vector &dir3,
		b3_f64    aRad,
		b3_f64    bRad);
	void b3ComputeTorusIndices();
};


// SPHERE
class b3Sphere : public b3RenderShape        // Kugel
{
	b3_vector       	 Base;         // Mittelpunkt
	b3_vector       	 Dir;          // Radius
	b3_f32               QuadRadius;       // Quadrat vom Radius

public:
	B3_ITEM_INIT(b3Sphere);
	B3_ITEM_LOAD(b3Sphere);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// AREA, DISK
class b3Shape2 : public b3Shape
{
protected:
	b3_vector           Base;           // basis of area, disk
	b3_vector           Dir1,Dir2;      // direction vectors
	b3_f32              NormalLength;     // normal length

protected:
	b3Shape2(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3Shape2);
	B3_ITEM_LOAD(b3Shape2);
};

class b3Area : public b3Shape2
{
#ifdef BLZ3_USE_OPENGL
	GLfloat  area_vertices[4 * 3];
	GLfloat  area_normals[4 * 3];
#endif

public:
	B3_ITEM_INIT(b3Area);
	B3_ITEM_LOAD(b3Area);

	void b3AllocVertices(b3RenderContext *context);
	void b3FreeVertices();
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3Disk : public b3Shape2
{
public:
	B3_ITEM_INIT(b3Disk);
	B3_ITEM_LOAD(b3Disk);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// CYLINDER, CONE, ELLIPSOID, BOX
class b3Shape3 : public b3RenderShape
{
protected:
	b3_vector         Normals[3];       // cross products
	b3_vector         Base;             // size
	b3_vector         Dir1,Dir2,Dir3;
	b3_s32            lSize;
	b3_f32            Denom;            // denominator of lin. system
	b3_f32            DirLen[3];        // length of direction vectors

protected:
	b3Shape3(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3Shape3);
	B3_ITEM_LOAD(b3Shape3);
};

class b3Cylinder : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cylinder);
	B3_ITEM_LOAD(b3Cylinder);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3AllocVertices(b3RenderContext *context);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3Cone : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cone);
	B3_ITEM_LOAD(b3Cone);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3AllocVertices(b3RenderContext *context);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3Ellipsoid : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Ellipsoid);
	B3_ITEM_LOAD(b3Ellipsoid);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3Box : public b3Shape3
{
#ifdef BLZ3_USE_OPENGL
	GLfloat  box_vertices[8 * 3 * 3];
	GLfloat  box_normals[8 * 3 * 3];
#endif

public:
	B3_ITEM_INIT(b3Box);
	B3_ITEM_LOAD(b3Box);

	void b3AllocVertices(b3RenderContext *context);
	void b3FreeVertices();
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// DOUGHNUT, TORUS
class b3Torus : public b3RenderShape
{
protected:
	b3_vector         Normals[3];       // cross products, unused
	b3_vector         Base;             // size
	b3_vector         Dir1,Dir2,Dir3;
	b3_s32            lSize;
	b3_f32            Denom;            // denominator of lin. system
	b3_f32            DirLen[3];        // squared lengths of direction vectors
	b3_f32            aRad,bRad;        // radiuses of torus
	b3_f32            aQuad,bQuad;      // squared lengths of aRad, bRad

public:
	B3_ITEM_INIT(b3Torus);
	B3_ITEM_LOAD(b3Torus);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// TRIANGLES
class b3TriangleShape : public b3Shape
{
	struct BHead *GridList;              // list of grids
	b3_vector     Base,Size;             // size of bounding box of all triangles
	b3_count      GridSize;              // num grid edges
	b3_count      TriaCount,VertexCount; // num of verteces and triangles
//	b3_s32        xSize,ySize;           // triangle order
	b3_u32        Flags;                 // interpolation flags
	b3_index      IndexHit;              // index of hit triangle
	b3_f64        aValue,bValue;         // polarcoord. of triangles

	b3_triangle  *triangles;
	b3_vertex    *vertices;
public:
	B3_ITEM_INIT(b3TriangleShape);
	B3_ITEM_LOAD(b3TriangleShape);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeNormals(b3_bool normalize=true);
	void b3ComputeIndices();
	void b3Intersect();
};

// index calculation of triangle grid
#define GRID_INDEX(x,y,z,GridSize)  (((z)*(GridSize)+(y))*(GridSize)+(x))

// normal interpolation defines
#define PHONG                   1L     // use Phong interpolation
#define NORMAL_VERTEX_VALID     2L     // normals of vertices valid, no auto computation
#define NORMAL_FACE_VALID       4L     // normals of triangles valid, no auto computation

// SPLINE, SPLINE_ROT
class b3SplineCurve : public b3Shape
{
protected:
	b3_line          Axis;                // for rotation shapes
	b3_spline        Spline;              // spline curve
	b3_s32           rSubDiv;             // sub division for rotation
	b3_f32           Knots[B3_MAX_KNOTS]; // one knot vector
	b3_vector       *Controls;

protected:
	b3SplineCurve(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3SplineCurve);
	B3_ITEM_LOAD(b3SplineCurve);
};

class b3SplineCurveShape : b3SplineCurve
{
public:
	B3_ITEM_INIT(b3SplineCurveShape);
	B3_ITEM_LOAD(b3SplineCurveShape);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3SplineRotShape : b3SplineCurve
{

public:
	B3_ITEM_INIT(b3SplineRotShape);
	B3_ITEM_LOAD(b3SplineRotShape);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// SPLINES_AREA, SPLINES_CYL, SPLINES_RING
class b3SplineShape : public b3Shape
{
protected:
#ifdef BLZ3_USE_OPENGL
#ifdef GLU_NURBS
	GLUnurbsObj     *glNURBS;
#endif
	b3_count         GridVertexCount;
	b3_count         SolidVertexCount;
	b3_count         xSubDiv,ySubDiv;
#endif
	b3_line          Axis;             // for rotation shapes, unused
	b3_spline        Spline[2];        // horizontal spline definition, these control points are valid!
	b3_f32           Knots[2][B3_MAX_KNOTS];  // two knot vectors
	b3_vector       *Controls;

protected:
	b3SplineShape(b3_size class_size,b3_u32 class_type);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();

public:
	B3_ITEM_INIT(b3SplineShape);
	B3_ITEM_LOAD(b3SplineShape);

#ifdef GLU_NURBS
	void b3AllocVertices(b3RenderContext *context);
	void b3FreeVertices();
	void b3Draw();
#endif

private:
	void b3ComputeGridVertices();
	void b3ComputeSolidVertices();
	void b3ComputeGridIndices();
	void b3ComputeSolidIndices();
};

class b3SplineArea : public b3SplineShape
{
public:
	B3_ITEM_INIT(b3SplineArea);
	B3_ITEM_LOAD(b3SplineArea);

	void b3Intersect();
};

class b3SplineCylinder : public b3SplineShape 
{
public:
	B3_ITEM_INIT(b3SplineCylinder);
	B3_ITEM_LOAD(b3SplineCylinder);

	void b3Intersect();
};

class b3SplineRing : public b3SplineShape 
{
public:
	B3_ITEM_INIT(b3SplineRing);
	B3_ITEM_LOAD(b3SplineRing);

	void b3Intersect();
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

#define CSG_MODE_SUB	MODE_NOT
#define CSG_MODE_AND	MODE_AND
#define CSG_MODE_OR		MODE_OR

// structures for CSG use
typedef struct
{
	b3_f32      Q;                // distance to intersection points
	b3Shape    *Shape;            // shape which delivers the intersection points
	b3_s32      Index;            // surface index
} b3Point;


// interval of intersection points
typedef struct
{
	b3_s32   num;
	b3Point  x[4];
} b3Interval;

typedef struct
{
	b3Interval *local,*ThisBox1,*ThisBox2;
} b3Intervals;


// CSG_SPHERE
class b3CSGSphere : public b3RenderShape
{
protected:
	b3_vector         Base;             // mid of sphere
	b3_vector         Dir;              // direction
	b3_f32            QuadRadius;       // squared radius

	b3_s32            Index;
	b3_s32            Operation;

public:
	B3_ITEM_INIT(b3CSGSphere);
	B3_ITEM_LOAD(b3CSGSphere);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// CSG_CYLINDER, CSG_CONE, CSG_ELLIPSOID, CSG_BOX
class b3CSGShape3 : public b3RenderShape
{
protected:
	b3_vector          Normals[3];       // cross products
	b3_vector          Base;             // size
	b3_vector          Dir1,Dir2,Dir3;
	b3_s32             lSize;
	b3_f32             Denom;            // denominator of lin. system
	b3_f32             DirLen[3];        // length of direction vectors

	b3_s32             Index;
	b3_s32             Operation;
	b3_line            BTLine;

protected:
	b3CSGShape3(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3CSGShape3);
	B3_ITEM_LOAD(b3CSGShape3);
};

class b3CSGCylinder : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCylinder);
	B3_ITEM_LOAD(b3CSGCylinder);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3AllocVertices(b3RenderContext *context);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3CSGCone : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCone);
	B3_ITEM_LOAD(b3CSGCone);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3AllocVertices(b3RenderContext *context);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3CSGEllipsoid : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGEllipsoid);
	B3_ITEM_LOAD(b3CSGEllipsoid);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

class b3CSGBox : public b3CSGShape3
{
#ifdef BLZ3_USE_OPENGL
	GLfloat  box_vertices[8 * 3 * 3];
	GLfloat  box_normals[8 * 3 * 3];
#endif

public:
	B3_ITEM_INIT(b3CSGBox);
	B3_ITEM_LOAD(b3CSGBox);

	void b3AllocVertices(b3RenderContext *context);
	void b3FreeVertices();
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

// CSG_TORUS
class b3CSGTorus : public b3RenderShape
{
protected:
	b3_vector          Normals[3];       // cross products, unused
	b3_vector          Base;             // size
	b3_vector          Dir1,Dir2,Dir3;
	b3_s32             lSize;
	b3_f32             Denom;            // denominator of lin. system
	b3_f32             DirLen[3];        // squared lengths of direction vectors
	b3_f32             aRad,bRad;        // radiuses of torus
	b3_f32             aQuad,bQuad;      // squared lengths of aRad, bRad

	b3_s32             Index;
	b3_s32             Operation;
	b3_line            BTLine;

public:
	B3_ITEM_INIT(b3CSGTorus);
	B3_ITEM_LOAD(b3CSGTorus);

	void b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void b3ComputeVertices();
	void b3ComputeIndices();
	void b3Intersect();
};

/*************************************************************************
**                                                                      **
**                        bounding boxes                                **
**                                                                      **
*************************************************************************/

#define CLASS_BBOX  0x60000000
#define BBOX        CLASS_BBOX

class b3InitBBox
{
protected:
	static void b3Init();
};

class b3BBox : public b3Item, public b3RenderObject
{
	// Inherited from Blizzard II
	b3_u32           Type;               // texture type
	b3_vector        Base;
	b3_vector        Size;
	b3_matrix        Matrix;             // all composed transformations
	char             BoxName[B3_BOXSTRINGLEN];   // object name
	char             BoxURL[B3_BOXSTRINGLEN]; // HTML link

#ifdef BLZ3_USE_OPENGL
	GLfloat          bbox_vertices[8 * 3];
	GLfloat          bbox_normals[8 * 3];
#endif

public:
	B3_ITEM_INIT(b3BBox);
	B3_ITEM_LOAD(b3BBox);

	       void    b3Dump(b3_count level);
	       void    b3AllocVertices(b3RenderContext *context);
	       void    b3FreeVertices();
	       void    b3ComputeVertices();
	       void    b3Draw();
		   b3_bool b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance);
 	static void    b3Reorg(b3Base<b3Item> *depot,b3Base<b3Item> *base,b3_count level,b3_count rec);
};

#define BBB_HTML         0
#define BBB_TEXT         1
#define BBB_BOLD         2
#define BBB_ITALIC       3
#define BBF_HTML        (1 << BBB_HTML)
#define BBF_TEXT        (1 << BBB_TEXT)
#define BBF_BOLD        (1 << BBB_BOLD)
#define BBF_ITALIC      (1 << BBB_ITALIC)
#define BBF_SERIF        0
#define BBF_SANSSERIF   16
#define BBF_TYPEWRITER  32

#define BB_GETTEXTSIZE(BBox)      (((BBox)->Type & 0x0ff00) >>  8)
#define BB_SETTEXTSIZE(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xffff00ff) |  ((Size) << 8))
#define BB_GETTEXTTYPE(BBox)      (((BBox)->Type & 0x00030) >>  4)
#define BB_SETTEXTTYPE(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xffffffcf) |  ((Size) << 4))
#define BB_GETROTATION(BBox)     ((((long)((BBox)->Type) & 0x70000) << 13) >> 29)
#define BB_SETROTATION(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xfff8ffff) | (((long)(Size) << 29) >> 13))

/*************************************************************************
**                                                                      **
**                        light sources                                 **
**                                                                      **
*************************************************************************/

#define CLASS_LIGHT             0x72000000
#define TYPE_POINTLIGHT         0x00000002
#define TYPE_AREALIGHT          0x00000001
#define TYPE_SPOTLIGHT          0x00000003
#define POINT_LIGHT             (CLASS_LIGHT|TYPE_POINTLIGHT)
#define AREA_LIGHT              (CLASS_LIGHT|TYPE_AREALIGHT)
#define SPOT_LIGHT              (CLASS_LIGHT|TYPE_SPOTLIGHT)

#define LIGHT_OFF               1L
#define LIGHT_PENUMBRA          2L

#define LIGHT_NAMEOFF(Node) ((char *)(Node)+BINDEX_OFFSET(Node))
#define LIGHT_NAMELEN(Node) (BINDEX_OFFSET(Node) > 0 ? \
	BINDEX_LENGTH(Node) - BINDEX_OFFSET(Node) : 0)

class b3InitLight
{
protected:
	static void b3Init();
};

// POINT_LIGHT
class b3Light : public b3Item
{
	b3_vector        Position;     // Position
	b3_color         Color;        // Farbe
	b3_f32           Distance;     // rel. Reichweite
	b3_s32           Type;         // Lampentyp
	b3_f32           Size;         // Auffaecherung
	b3_s32           JitterEdge;   // Kantenaufspaltung
	b3_vector        Direction;    // Abstrahlrichtung
	b3_spline        Spline;       // Spline Kurve
	b3_f32           Knots[B3_MAX_KNOTS];
	b3_vector        Controls[B3_MAX_CONTROLS];
	char             Name[B3_BOXSTRINGLEN]; // Lampenname

public:
	B3_ITEM_INIT(b3Light);
	B3_ITEM_LOAD(b3Light);
};

// aux. structure for computing illumination
typedef struct
{
	b3_vector  IPoint;
	b3_vector  Normal;
	b3_ray     Refl;
	b3_ray     Refr;
	b3_color   Diff,Amb,Spec;
	b3_f64     Reflection,Refraction,RefrValue,HighLight;
} b3SurfaceInfo;

// aux. structure for JitterLight
typedef struct
{
	b3_line         LightDir;
	b3_vector       LightView,xDir,yDir;
	b3_f32          Size,LightFrac,LightDist,Q;
	b3_s32          Distr;
} b3JitterPrep;

/************************************************************************\
**                                                                      **
**                        animations                                    **
**                                                                      **
\************************************************************************/

#define CLASS_ANIMATION         0x75000000 // animation class
#define TYPE_MOVE               0x00000001
#define TYPE_ROTATE             0x00000002
#define TYPE_SCALE              0x00000003
#define ANIM_MOVE              (CLASS_ANIMATION|TYPE_MOVE)
#define ANIM_ROTATE            (CLASS_ANIMATION|TYPE_ROTATE)
#define ANIM_SCALE             (CLASS_ANIMATION|TYPE_SCALE)

#define ANIMFLAGB_X          0
#define ANIMFLAGB_Y          1
#define ANIMFLAGB_Z          2
#define ANIMFLAGB_DIRECT     4
#define ANIMFLAGB_ACTIVE     5
#define ANIMFLAGB_RECURSIVE  6
#define ANIMFLAGB_OBJECT     8
#define ANIMFLAGB_LIGHT      9
#define ANIMFLAGB_CAMERA    10
#define ANIMFLAGB_CENTER    11
#define ANIMFLAGB_ILLUM     12

#define ANIMFLAGF_X         (1 << ANIMFLAGB_X)
#define ANIMFLAGF_Y         (1 << ANIMFLAGB_Y)
#define ANIMFLAGF_Z         (1 << ANIMFLAGB_Z)
#define ANIMFLAGF_DIRECT    (1 << ANIMFLAGB_DIRECT)
#define ANIMFLAGF_ACTIVE    (1 << ANIMFLAGB_ACTIVE)
#define ANIMFLAGF_RECURSIVE (1 << ANIMFLAGB_RECURSIVE)
#define ANIMFLAGF_OBJECT    (1 << ANIMFLAGB_OBJECT)
#define ANIMFLAGF_LIGHT     (1 << ANIMFLAGB_LIGHT)
#define ANIMFLAGF_CAMERA    (1 << ANIMFLAGB_CAMERA)
#define ANIMFLAGF_CENTER    (1 << ANIMFLAGB_CENTER)
#define ANIMFLAGF_ILLUM     (1 << ANIMFLAGB_ILLUM)

#define ANIMMASK_AXIS       0x0007
#define ANIMMASK_ACTIVE		0x0060
#define ANIMMASK_DEST       0x1f00

#define B3_ANIMSTRINGLEN       32

class b3InitAnimation
{
protected:
	static void b3Init();
};

class b3AnimElement : b3Item
{
	b3_vector           center;               // rotation center
	b3_matrix           actual;               // actual transformation
	b3_matrix           neutralInverse;       // inverse of neutral position
	b3_f32              ratio;                // parametrisation ratio
	b3_s32              empty;                // empty entry
	b3_f32              start;                // start of action
	b3_f32              end;                  // end of action
	b3_s32              flags;                // ANIMF_xxx flags
	b3_s32              trackIndex;           // start track number
	b3_s32              curveUse;             // number of curves used
	b3_nurbs            param;                // param. curves
	b3_f32              knots[B3_MAX_KNOTS];     // knot vectors
	char                name[B3_ANIMSTRINGLEN];  // element name
	char                object[B3_BOXSTRINGLEN]; // name of destination object

public:
	B3_ITEM_INIT(b3AnimElement);
	B3_ITEM_LOAD(b3AnimElement);
};

/*************************************************************************
**                                                                      **
**                        specials                                      **
**                                                                      **
*************************************************************************/

#define CLASS_SPECIAL           0x7f000000 // for diverse elements
#define TYPE_SUPERSAMPLE4       0x00000001
#define TYPE_NEBULAR            0x00000002
#define TYPE_CAMERA             0x00000003
#define TYPE_MODELLERINFO       0x00000004
#define TYPE_ANIMATION          0x00000005
#define TYPE_DISTRIBUTE         0x00000006
#define TYPE_LENSFLARE          0x00000007
#define SUPERSAMPLE4            (CLASS_SPECIAL|TYPE_SUPERSAMPLE4)
#define NEBULAR                 (CLASS_SPECIAL|TYPE_NEBULAR)
#define CAMERA                  (CLASS_SPECIAL|TYPE_CAMERA)
#define LINES_INFO              (CLASS_SPECIAL|TYPE_MODELLERINFO)
#define ANIMATION               (CLASS_SPECIAL|TYPE_ANIMATION)
#define DISTRIBUTE              (CLASS_SPECIAL|TYPE_DISTRIBUTE)
#define LENSFLARE               (CLASS_SPECIAL|TYPE_LENSFLARE)

class b3InitSpecial
{
protected:
	static void b3Init();
};

class b3Special : public b3Item
{
protected:
	b3Special(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3Special);
	B3_ITEM_LOAD(b3Special);
};

// SUPERSAMPLE4
class b3SuperSample : public b3Special
{
	b3_color    Limit;

public:
	B3_ITEM_INIT(b3SuperSample);
	B3_ITEM_LOAD(b3SuperSample);
};

// CAMERA
class b3CameraPart : public b3Special
{
public:
	b3_vector        Width;
	b3_vector        Height;
	b3_vector        EyePoint;
	b3_vector        ViewPoint;
	b3_s32           Flags;
	char             CameraName[B3_CAMERANAMELEN];

public:
	B3_ITEM_INIT(b3CameraPart);
	B3_ITEM_LOAD(b3CameraPart);
};

#define CAMERA_TITLE  1
#define CAMERA_ACTIVE 2

// NEBULAR
class b3Nebular : public b3Special
{
	b3_color         NebularColor;
	b3_f32           NebularVal;

public:
	B3_ITEM_INIT(b3Nebular);
	B3_ITEM_LOAD(b3Nebular);
};

// LINES_INFO
class b3ModellerInfo : public b3Special
{
	b3_vector        Center;
	b3_f32           GridMove;
	b3_f32           GridRot;
	b3_bool          ResizeFlag;
	b3_bool          BBoxTitles;
	b3_bool          GridActive;
	b3_bool          CameraActive;
	b3_s32           Flags;
	b3_f32           Unit;

public:
	B3_ITEM_INIT(b3ModellerInfo);
	B3_ITEM_LOAD(b3ModellerInfo);
};

#define RULE_MM 0
#define RULE_CM 1
#define RULE_IN 2
#define RULE_DM 3
#define RULE_FT 4
#define RULE_M  5

// ANIMATION
class b3Animation : public b3Special
{
	b3_f32   start;           // start time (one unit per frame)
	b3_f32   end;             // end time (one unit per frame)
	b3_f32   time;            // time point
	b3_f32   neutral;         // neutral point
	b3_s32   framesPerSecond;
	b3_s32   flags;

	// OK, the following values are only for "Lines"
	b3_s32          frames;          // computed number of frames
	b3_s32          tracks;          // number of visible tracks
	b3_s32          trackIndex;      // start track in window 
	b3_s32          frameIndex;      // start frame in window
	b3_s32          WTracks;         // actual number of tracks
	b3_s32          WFrames;         // whole of frames
	b3AnimElement  *Element;         // actual animation element

public:
	B3_ITEM_INIT(b3Animation);
	B3_ITEM_LOAD(b3Animation);
};

#define ANIMB_ON     1

#define ANIMF_ON     (1<<ANIMB_ON)

// DISTRIBUTE
class b3Distribute : public b3Special
{
	b3_s32           Type;
	b3_s32           SamplesPerPixel;
	b3_s32           SamplesPerFrame;
	b3_f32           DepthOfField;
	b3_s32           PixelAperture;
	b3_s32           FrameAperture;

public:
	B3_ITEM_INIT(b3Distribute);
	B3_ITEM_LOAD(b3Distribute);
};

#define SAMPLE_MOTION_BLUR_B     0
#define SAMPLE_SUPERSAMPLE_B     1
#define SAMPLE_DEPTH_OF_FIELD_B  2

#define SAMPLE_MOTION_BLUR      (1 << SAMPLE_MOTION_BLUR_B)
#define SAMPLE_SUPERSAMPLE      (1 << SAMPLE_SUPERSAMPLE_B)
#define SAMPLE_DEPTH_OF_FIELD   (1 << SAMPLE_DEPTH_OF_FIELD_B)

#define SAMPLE_GET_FLAGS(d)      ((d)->Type & 0x00ff)
#define SAMPLE_GET_TYPE(d)       ((d)->Type & 0xff00)
#define SAMPLE_SET_FLAGS(d,v)   ((d)->Type = ((d)->Type & 0xffffff00) | (v))
#define SAMPLE_SET_TYPE(d,v)    ((d)->Type = ((d)->Type & 0xffff00ff) | (v))

#define SAMPLE_REGULAR          0x0000
#define SAMPLE_RANDOM           0x0100
#define SAMPLE_JITTER           0x0200
#define SAMPLE_SEMI_JITTER      0x0300
#define SAMPLE_SEPARATED        0x0400

#define FILTER_BOX              0
#define FILTER_GAUSS            1
#define FILTER_SHUTTER          2

// LENSFLARE
class b3LensFlare : public b3Special
{
	b3_s32         Flags;
	b3_color       Color;
	b3_f32         Expon;

public:
	B3_ITEM_INIT(b3LensFlare);
	B3_ITEM_LOAD(b3LensFlare);
};

/*************************************************************************
**                                                                      **
**                        global rendering informations                 **
**                                                                      **
*************************************************************************/

#define CLASS_GLOBAL            0x7ff00000
#define TYPE_TRACEANGLE         0x00000001      // raytracing (normal projection)
#define TYPE_TRACEPHOTO         0x00000002      // raytracing (obsolete)
#define TYPE_RADIOSITY          0x00000003      // radiosity
#define TYPE_REFLECTION         0x00000004      // reflection mapping
#define TYPE_GOROUD             0x00000005      // Gouraud shading
#define TYPE_GLOBAL_ILLUM       0x00000006      // global illumination

#define	MORK                    0x00001000      // Mork's illumination
#define	PHONG_ILLUM             0x00002000      // Phong's illumination
#define ALBRECHT                0x00003000      // Jens' coherence algo

#define TRACEANGLE_MORK         (CLASS_GLOBAL|TYPE_TRACEANGLE|MORK)
#define TRACEPHOTO_MORK         (CLASS_GLOBAL|TYPE_TRACEPHOTO|MORK)
#define TRACEANGLE_PHONG        (CLASS_GLOBAL|TYPE_TRACEANGLE|PHONG_ILLUM)
#define TRACEPHOTO_PHONG        (CLASS_GLOBAL|TYPE_TRACEPHOTO|PHONG_ILLUM)
#define TRACEPHOTO_ALBRECHT     (CLASS_GLOBAL|TYPE_TRACEPHOTO|ALBRECHT)
#define GLOBAL_ILLUM            (CLASS_GLOBAL|TYPE_GLOBAL_ILLUM|MORK)

#define TRACEPHOTO              TRACEPHOTO_MORK	// obsolete

class b3InitScene
{
protected:
	static void b3Init();
};

class b3Scene : public b3Item
{
public:
	// Camera
	b3_vector        EyePoint;
	b3_vector        ViewPoint;
	b3_vector        Width;                 // Bildschirmvektor X
	b3_vector        Height;                // Bildschirmvektor Y
	b3_s32           xSize,ySize;           // Rechenaufloesung
	b3_f32           xAngle,yAngle;         // Blickwinkel

	// Background
	b3_color         Top;
	b3_color         Bottom;
	b3Tx            *BackTexture;           //
	b3_s32           BackgroundType;        // Hintergrund: Farbe/Datei/...

	b3_s32           TraceDepth;            // Rekursionstiefe
	b3_s32           Flags;                 // beschreibt, welche Werte gueltig sind
	b3_f32           ShadowBrightness;      // Schattenhelligkeit

	// Some limits
	b3_f32           BBoxOverSize;          // BBox-Ueberziehun
	b3_f32           Limit;                 // Schwellenwert
	char             TextureName[B3_TEXSTRINGLEN]; // Name des Hintergrundbildes

public:
	                b3Scene(b3_u32  class_type);
	                b3Scene(b3_u32 *src);

	static b3Item  *b3Init(b3_u32  class_type);
	static b3Item  *b3Init(b3_u32 *src);

	       void     b3Reorg();
	       void     b3GetDisplaySize(b3_res &xSize,b3_res &ySize);
		   void     b3AllocVertices(b3RenderContext *context);
		   void     b3FreeVertices();
	       void     b3Draw();
		   b3_bool  b3ComputeBounds(b3_vector *lower,b3_vector *upper);
};

#define TP_TEXTURE       1L            // Hintergrundbild
#define TP_SLIDE         2L            // Hintergundfarbverlauf
#define TP_SKY_N_HELL    3L            // Himmel & Hoelle

#define TP_SIZEVALID     2L            // SizeX, SizeY gueltig
#define TP_NO_GFX        4L            // no rgb output

#define TP_FRAME         0L            // use frame size
#define TP_FIELD_EVEN    8L            // even field
#define TP_FIELD_ODD    16L            // odd field
#define TP_FRAME_MASK   24L            // mask

#define RAY_INSIDE       1
#define RAY_OUTSIDE      0

class b3InitRaytrace :
	public b3InitScene,
	public b3InitSpecial,
	public b3InitAnimation,
	public b3InitLight,
	public b3InitBBox,
	public b3InitShape,
	public b3InitMaterial,
	public b3InitBump,
	public b3InitCondition
{
public:
	static void b3Init();
};

#endif
