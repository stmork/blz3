/*
**
**	$Filename:	b3Raytrace.h $ 
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing Structure Definitions
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_RAYTRACE_H
#define B3_RAYTRACE_RAYTRACE_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Display.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Spline.h"
#include "blz3/base/b3Filter.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3Render.h"
#include "blz3/image/b3Tx.h"
#include "blz3/image/b3TxPool.h"
#include "blz3/raytrace/b3PickInfo.h"
#include "blz3/raytrace/b3PrepareInfo.h"

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

struct b3_polar_precompute
{
	b3_vector polar;
	b3_vector object_polar;
	b3_index  normal_index;
};

struct b3_polar : b3_polar_precompute
{
	b3_vector box_polar;
};

struct b3_ray : public b3_line64
{
	b3_vector64 ipoint;
	b3_vector64 normal;
	b3_vector64 xDeriv;
	b3_vector64 yDeriv;
	b3_polar    polar;
	b3_f64      Q;
	b3_bool     inside;
	b3_index    TriaIndex;
	b3_f64      aTriaValue,bTriaValue;
	b3_f64      t;
};


// aux. structure for computing illumination
struct b3_surface
{
	b3_color  diffuse,ambient,specular,specular_sum;
	b3_f64    refl,refr,ior,se;
};

class b3Activation
{
public:
	enum b3_anim_activation
	{
		B3_ANIM_DISABLED,
		B3_ANIM_DEACTIVE,
		B3_ANIM_ACTIVE
	};
private:
	b3_bool            m_Active;
	b3_anim_activation m_AnimActive;

public:
	        b3Activation();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate = true);
	void    b3Animate(b3_bool activate = true);
	void    b3Animate(b3_anim_activation activate);
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

struct b3_triangle
{
	b3_index  P1;
	b3_index  P2;
	b3_index  P3;
	b3_vector Normal;
};

struct b3_vertex
{
	b3_vector Point;
	b3_vector Normal;
};

/*************************************************************************
**                                                                      **
**                        Stencil                                       **
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

enum b3_stencil_unit
{
	B3_STENCIL_UNIT,
	B3_STENCIL_LENGTH,
	B3_STENCIL_ANGLE
};

struct b3_stencil_limit
{
	b3_f64 x1,y1,x2,y2;
};

struct b3_stencil_bound
{
	b3_f64          xMin,xMax,xFactor;
	b3_f64          yMin,yMax,yFactor;
	b3_stencil_unit xUnit;
	b3_stencil_unit yUnit;
};

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

	virtual b3_bool b3Prepare();
	virtual void    b3ComputeBound(b3_stencil_limit *limit);
	virtual b3_bool b3CheckStencil(b3_polar_precompute *polar);
	        b3_bool b3Conditionate(b3_bool input,b3_bool operation);

protected:
	static  void    b3CheckInnerBound(b3_stencil_limit *limit,b3_stencil_limit *object);
	static  void    b3CheckOuterBound(b3_stencil_limit *limit,b3_stencil_limit *object);
};

// TYPE_RECTANGLE
class b3CondRectangle : public b3Condition
{
public:
	b3_f32  m_xStart,m_yStart;    // rel. start coordinates
	b3_f32  m_xEnd,  m_yEnd;      // rel. end koordinates
	b3_s32  m_Flags;

public:
	B3_ITEM_INIT(b3CondRectangle);
	B3_ITEM_LOAD(b3CondRectangle);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};

#define RCB_ACTIVE  0
#define RCF_ACTIVE (1 << RCB_ACTIVE)

// TYPE_TRIANGLE, TYPE_PARALLELOGRAM
class b3Cond2 : public b3Condition
{
public:
	b3_f32  m_xPos, m_yPos;       // base of triangle/ parallelogramme
	b3_f32  m_xDir1,m_yDir1;      // direction 1
	b3_f32  m_xDir2,m_yDir2;      // direction 2
	b3_f32  m_Denom;

protected:
	b3Cond2(b3_size class_size,b3_u32  class_type);

public:
	B3_ITEM_INIT(b3Cond2);
	B3_ITEM_LOAD(b3Cond2);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3ComputeBound(b3_stencil_limit *limit);
};

class b3CondPara : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondPara);
	B3_ITEM_LOAD(b3CondPara);

	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};

class b3CondTria : public b3Cond2
{
public:
	B3_ITEM_INIT(b3CondTria);
	B3_ITEM_LOAD(b3CondTria);

	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};


// TYPE_CIRCLE
class b3CondCircle : public b3Condition
{
protected:
	b3_f32  m_xCenter,m_yCenter,m_Radius;      // Mittelpunkt und Radius

public:
	B3_ITEM_INIT(b3CondCircle);
	B3_ITEM_LOAD(b3CondCircle);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};

// TYPE_SEGMENT
class b3CondSegment : public b3Condition
{
protected:
	b3_f32  m_xCenter,   m_yCenter;  // Mittelpunkt
	b3_f32  m_RadStart,  m_RadEnd;   // gültiger Bereich im Ring
	b3_f32  m_AngleStart,m_AngleEnd; // Segment

public:
	B3_ITEM_INIT(b3CondSegment);
	B3_ITEM_LOAD(b3CondSegment);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};

// TYPE_ELLIPSE
class b3CondEllipse : public b3Condition
{
protected:
	b3_f32  m_xCenter,   m_yCenter;  // Mittelpunkt
	b3_f32  m_xRadius,   m_yRadius;  // Radien
	b3_f32  m_RadStart,  m_RadEnd;   // gültiger Bereich im Ring
	b3_f32  m_AngleStart,m_AngleEnd; // Segment

public:
	B3_ITEM_INIT(b3CondEllipse);
	B3_ITEM_LOAD(b3CondEllipse);

	void    b3Write();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};

// TYPE_TEXTURE
class b3CondTexture : public b3Condition
{
protected:
	b3Tx   *m_Texture;
	b3_s32  m_Flags;               // unused yet
	b3_f32  m_xStart,m_yStart;     // base of texture
	b3_f32  m_xScale,m_yScale;     // scale of texture
	b3_s32  m_xTimes,m_yTimes;     // repetion of texture in x- and y-direction
	b3Path  m_Name;  // title

public:
	B3_ITEM_INIT(b3CondTexture);
	B3_ITEM_LOAD(b3CondTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar_precompute *polar);
};

// TYPE_WRAP_TEXTURE
class b3CondWrapTexture : public b3Condition
{
protected:
	b3Tx   *m_Texture;
	b3_s32  m_Flags;               // unused yet
	b3_f32  m_xStart,m_yStart;     // borders in hor. direction
	b3_f32  m_xEnd,m_yEnd;         // borders in vert. direction
	b3Path  m_Name;                // title

public:
	B3_ITEM_INIT(b3CondWrapTexture);
	B3_ITEM_LOAD(b3CondWrapTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3ComputeBound(b3_stencil_limit *limit);
	b3_bool b3CheckStencil(b3_polar_precompute *polar);
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
#define TYPE_GROOVE         0x00000007
#define BUMP_NOISE          (CLASS_BUMP|TYPE_NOISE)
#define BUMP_MARBLE         (CLASS_BUMP|TYPE_MARBLE)
#define BUMP_TEXTURE        (CLASS_BUMP|TYPE_TEXTURE)
#define BUMP_WATER          (CLASS_BUMP|TYPE_WATER)
#define BUMP_WAVE           (CLASS_BUMP|TYPE_WAVE)
#define BUMP_GLOSSY         (CLASS_BUMP|TYPE_GLOSSY)
#define BUMP_GROOVE         (CLASS_BUMP|TYPE_GROOVE)

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

	virtual        b3_bool b3Prepare();
	virtual        void    b3BumpNormal(b3_ray *ray);
	virtual inline b3_bool b3NeedDeriv()
	{
		return false;
	}
};

// BUMP_NOISE
class b3BumpNoise : public b3Bump
{
	b3_vector   m_Scale;
	b3_f32      m_Size;

public:
	B3_ITEM_INIT(b3BumpNoise);
	B3_ITEM_LOAD(b3BumpNoise);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

// BUMP_MARBLE
class b3BumpMarble : public b3Bump
{
	b3_vector   m_Scale;
	b3_f32      m_Size;

public:
	B3_ITEM_INIT(b3BumpMarble);
	B3_ITEM_LOAD(b3BumpMarble);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

// BUMP_TEXTURE
class b3BumpTexture : public b3Bump
{
	b3_f32      m_xStart,m_yStart;     // base of bump texture
	b3_f32      m_xScale,m_yScale;     // scale of bump texture
	b3_s32      m_xTimes,m_yTimes;     // repetition
	b3_f32      m_Intensity;           // non zero
	b3Tx       *m_Texture;
	b3_s32      m_Flags;
	b3Path      m_Name;

public:
	B3_ITEM_INIT(b3BumpTexture);
	B3_ITEM_LOAD(b3BumpTexture);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3BumpNormal(b3_ray *ray);
	b3_bool b3NeedDeriv();

private:
	b3_bool b3GetNormalDeriv(b3_f64 lx,b3_f64 ly,b3_vector *deriv);
};

// BUMP_WATER
class b3BumpWater : public b3Bump
{
	b3_s32      m_ScaleFlag;           // use BBox coords or direct coord
	b3_vector   m_ScaleIPoint;         // intersection point scalar
	b3_f32      m_ScaleRad;            // radius scalar
	b3_f32      m_ScaleTime;           // time period for wave swing

public:
	B3_ITEM_INIT(b3BumpWater);
	B3_ITEM_LOAD(b3BumpWater);

	void    b3Write();
	void    b3BumpNormal(b3_ray *ray);
};

// BUMP_WAVE
class b3BumpWave : public b3Bump
{
	b3_s32      m_Flags;               // use BBox coords or direct coord
	b3_vector   m_Scale;               // point scalar
	b3_f32      m_Amplitude;           // amplitude

public:
	B3_ITEM_INIT(b3BumpWave);
	B3_ITEM_LOAD(b3BumpWave);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

// BUMP_GROOVE
class b3BumpGroove : public b3Bump
{
	b3_s32      m_Flags;               // use BBox coords or direct coord
	b3_vector   m_Scale;               // point scalar
	b3_f32      m_Amplitude;           // amplitude

public:
	B3_ITEM_INIT(b3BumpGroove);
	B3_ITEM_LOAD(b3BumpGroove);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

// BUMP_GLOSSY */
class b3BumpGlossy : public b3Bump
{
	b3_s32      m_Flags;
	b3_f32      m_Intensity;

public:
	B3_ITEM_INIT(b3BumpGlossy);
	B3_ITEM_LOAD(b3BumpGlossy);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};


//flags for WaterBump, WaveBump
#define BUMP_IPOINT          1
#define BUMP_U_SUPPRESS_WAVE 2
#define BUMP_V_SUPPRESS_WAVE 4

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

	virtual b3_bool b3Prepare();
	virtual b3_f64  b3GetReflection(b3_polar *polar);
	virtual b3_f64  b3GetRefraction(b3_polar *polar);
	virtual b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	virtual b3_f64  b3GetSpecularExponent(b3_polar *polar);
	virtual b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// MATERIAL or MAT_NORMAL
class b3MatNormal : public b3Material
{
public:
	b3_color          m_DiffColor;
	b3_color          m_AmbColor;
	b3_color          m_SpecColor;
	b3_f32            m_Reflection;          // self explaining
	b3_f32            m_Refraction;
	b3_f32            m_RefrValue;
	b3_f32            m_HighLight;
	b3_s32            m_Flags;

public:
	B3_ITEM_INIT(b3MatNormal);
	B3_ITEM_LOAD(b3MatNormal);

	void    b3Write();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// CHESS
class b3MatChess : public b3Material 
{
public:
	b3_color m_DiffColor[2];
	b3_color m_AmbColor[2];
	b3_color m_SpecColor[2];
	b3_f32   m_Reflection[2];         // same like NormMaterial, but
	b3_f32   m_Refraction[2];
	b3_f32   m_RefrValue[2];
	b3_f32   m_HighLight[2];
	b3_s32   m_Flags;
	b3_s32   m_xTimes;
	b3_s32   m_yTimes;

public:
	B3_ITEM_INIT(b3MatChess);
	B3_ITEM_LOAD(b3MatChess);

	void    b3Write();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// MARBLE
class b3MatMarble : public b3Material 
{
	b3_color          m_DiffColor;
	b3_color          m_AmbColor;
	b3_color          m_SpecColor;
	b3_vector         m_Scale;
	b3_f32            m_Reflection;
	b3_f32            m_Refraction;
	b3_f32            m_RefrValue;
	b3_f32            m_HighLight;
	b3_s32            m_Flags;
	b3_s32            m_xTimes,m_yTimes;

public:
	B3_ITEM_INIT(b3MatMarble);
	B3_ITEM_LOAD(b3MatMarble);

	void    b3Write();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// WOOD
class b3MatWood : public b3Material 
{
	b3_color          m_DiffColor;
	b3_color          m_AmbColor;
	b3_color          m_SpecColor;
	b3_vector         m_Scale;
	b3_f32            m_Reflection;
	b3_f32            m_Refraction;
	b3_f32            m_RefrValue;
	b3_f32            m_HighLight;
	b3_s32            m_Flags;
	b3_s32            m_xTimes,m_yTimes;

public:
	B3_ITEM_INIT(b3MatWood);
	B3_ITEM_LOAD(b3MatWood);

	void    b3Write();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// TEXTURE
class b3MatTexture : public b3Material 
{
public:
	b3_f32            m_Reflection;
	b3_f32            m_Refraction;
	b3_f32            m_RefrValue;
	b3_f32            m_HighLight;
	b3_f32            m_xStart,m_yStart;    // surface coordinate start
	b3_f32            m_xScale,m_yScale;    // texture scale
	b3_s32            m_xTimes,m_yTimes;    // repetition in x- y-direction
	b3_s32            m_Flags;
	b3Path            m_Name;
	b3Tx             *m_Texture;

public:
	B3_ITEM_INIT(b3MatTexture);
	B3_ITEM_LOAD(b3MatTexture);

	void    b3Write();
	b3_bool b3Prepare();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// WRAPTEXTURE
class b3MatWrapTexture : public b3Material 
{
	b3Tx             *m_Texture;          // only one texture (compat. Dali)
public:
	b3_f32            m_Reflection;
	b3_f32            m_Refraction;
	b3_f32            m_RefrValue;
	b3_f32            m_HighLight;
	b3_f32            m_xStart,m_yStart;  // surface coordinate start
	b3_f32            m_xEnd,m_yEnd;      // surface coordinate end
	b3_s32            m_Flags;
	b3Path            m_Name;

public:
	B3_ITEM_INIT(b3MatWrapTexture);
	B3_ITEM_LOAD(b3MatWrapTexture);

	void    b3Write();
	b3_bool b3Prepare();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
};

// SLIDE
class b3MatSlide : public b3Material 
{
	b3_color          m_Diffuse[2];        // colors for start/end
	b3_color          m_Ambient[2];
	b3_color          m_Specular[2];
	b3_f32            m_From,m_To;           // rel. polar values of start, end
	b3_f32            m_Reflection;          // normal material definitions
	b3_f32            m_Refraction;
	b3_f32            m_RefrValue;
	b3_f32            m_HighLight;
	b3_s32            m_ModeFlag;            // direction and cut flags, see below

public:
	B3_ITEM_INIT(b3MatSlide);
	B3_ITEM_LOAD(b3MatSlide);

	void    b3Write();
	b3_f64  b3GetReflection(b3_polar *polar);
	b3_f64  b3GetRefraction(b3_polar *polar);
	b3_f64  b3GetIndexOfRefraction(b3_polar *polar);
	b3_f64  b3GetSpecularExponent(b3_polar *polar);
	b3_bool b3GetColors(
		b3_polar *polar,
		b3_color *diff,
		b3_color *amb,
		b3_color *spec);
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

class b3InitShape
{
protected:
	static void b3Init();
};

class b3ShapeBaseTrans
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

class b3ShapeRenderContext : public b3RenderContext
{
	static b3_f64         m_Sin[B3_MAX_RENDER_SUBDIV + 1];
	static b3_f64         m_Cos[B3_MAX_RENDER_SUBDIV + 1];
	static b3_count       m_SubDiv;

	b3_vector     *m_Between;
	b3_gl_line    *m_CylinderIndices;
	b3_gl_polygon *m_CylinderPolygons;
	b3_gl_line    *m_ConeIndices;
	b3_gl_polygon *m_ConePolygons;

public:
	                 b3ShapeRenderContext(b3_count subdiv = 16);
	void             b3InitSubdiv(b3_count subdiv);
	b3_gl_line      *b3GetCylinderIndices();
	b3_gl_polygon   *b3GetCylinderPolygons();
	b3_gl_line      *b3GetConeIndices();
	b3_gl_polygon   *b3GetConePolygons();

	static inline b3_count  b3GetSubdiv()
	{
		return m_SubDiv;
	}

	static inline b3_f64   *b3GetCosTable()
	{
		return m_Cos;
	}
	
	static inline b3_f64   *b3GetSinTable()
	{
		return m_Sin;
	}
};

// same structure entries for all shapes
class b3Shape : public b3Item, public b3RenderObject, public b3Activation
{
protected:
	b3_count           xSize,ySize;
	b3_count           SinCosSteps;
	b3_f64            *Cos;
	b3_f64            *Sin;
	b3_stencil_limit   Limit;

	b3_gl_line        *GridsCyl;
	b3_gl_polygon     *PolysCyl;
	b3_gl_line        *GridsCone;
	b3_gl_polygon     *PolysCone;

protected:
	                    b3Shape(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3Shape);
	B3_ITEM_LOAD(b3Shape);

	        void        b3Write();
	virtual void        b3StoreShape();
	        void        b3InitActivation();
	        b3Material *b3GetColors(b3_ray *ray,b3_surface *surface);
	        void        b3BumpNormal(b3_ray *ray);
	        void        b3ComputeBound(b3_stencil_limit *limit);
	virtual b3_bool     b3CheckStencil(b3_polar_precompute *polar);
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

	virtual inline b3_bool b3NormalDeriv(b3_ray *ray)
	{
		return false;
	}

protected:
	b3_count        b3GetIndexOverhead(b3_f64 xl,b3_f64 yl);
	void            b3GetDiffuseColor(b3_color *color);
	b3_f64          b3GetColors(b3_color *ambient,b3_color *diffuse,b3_color *specular);
	b3_bool         b3GetChess(b3_color *bColor,b3_color *wColor,b3_res &xRepeat,b3_res &yRepeat);
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

class b3SimpleShape : public b3Shape
{
protected:
	b3_bool             b3CheckStencil(b3_polar_precompute *polar);

protected:
	b3SimpleShape(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3SimpleShape);
	B3_ITEM_LOAD(b3SimpleShape);

public:
	virtual b3_f64      b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
};

// SPHERE
class b3Sphere : public b3SimpleShape    // Kugel
{
	b3_f64               m_QuadRadius;   // Quadrat vom Radius

public:
	b3_vector       	 m_Base;         // Mittelpunkt
	b3_vector       	 m_Dir;          // Radius

public:
	B3_ITEM_INIT(b3Sphere);
	B3_ITEM_LOAD(b3Sphere);

	void    b3StoreShape();
	void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	void    b3ComputeVertices();
	void    b3ComputeNormals(b3_bool normalize=true);
	void    b3ComputeIndices();
	b3_f64  b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	void    b3Normal(b3_ray *ray);
	void    b3SetupPicking(b3PickInfo *pick_info);

	b3_bool b3Prepare();
	void    b3Transform(b3_matrix *transformation,b3_bool is_affine);
};

// AREA, DISK
class b3Shape2 : public b3SimpleShape
{
protected:
	b3_vector           m_Normal;
	b3_f64              m_NormalLength;   // normal length

public:
	b3_vector           m_Base;           // basis of area, disk
	b3_vector           m_Dir1,m_Dir2;    // direction vectors

protected:
	b3Shape2(b3_size class_size,b3_u32 class_type);

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

class b3Area : public b3Shape2
{
	b3_gl_vertex  area_vertex[4];

public:
	B3_ITEM_INIT(b3Area);
	B3_ITEM_LOAD(b3Area);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void   b3AllocVertices(b3RenderContext *context);
	        void   b3FreeVertices();
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
};

class b3Disk : public b3Shape2
{
public:
	B3_ITEM_INIT(b3Disk);
	B3_ITEM_LOAD(b3Disk);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
};

// CYLINDER, CONE, ELLIPSOID, BOX
class b3Shape3 : public b3SimpleShape, public b3ShapeBaseTrans
{
protected:
	     b3Shape3(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3Shape3);
	B3_ITEM_LOAD(b3Shape3);

	        void    b3StoreShape();
	        b3_bool b3Prepare();
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	virtual void    b3ComputeNormals(b3_bool normalize = true);
	        void    b3Transform(b3_matrix *transformation,b3_bool is_affine);
			void    b3SetupPicking(b3PickInfo *pick_info);
};

class b3Cylinder : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cylinder);
	B3_ITEM_LOAD(b3Cylinder);

	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3AllocVertices(b3RenderContext *context);
	void   b3ComputeVertices();
	void   b3ComputeIndices();
	b3_f64 b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	void   b3Normal(b3_ray *ray);
};

class b3Cone : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Cone);
	B3_ITEM_LOAD(b3Cone);

	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3AllocVertices(b3RenderContext *context);
	void   b3ComputeVertices();
	void   b3ComputeIndices();
	b3_f64 b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	void   b3Normal(b3_ray *ray);
};

class b3Ellipsoid : public b3Shape3
{
public:
	B3_ITEM_INIT(b3Ellipsoid);
	B3_ITEM_LOAD(b3Ellipsoid);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void   b3ComputeVertices();
	        void   b3ComputeIndices();
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	        void   b3Normal(b3_ray *ray);
};

class b3Box : public b3Shape3
{
	b3_gl_vertex  box_vertex[8 * 3];

public:
	B3_ITEM_INIT(b3Box);
	B3_ITEM_LOAD(b3Box);

	virtual void   b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void   b3AllocVertices(b3RenderContext *context);
	        void   b3FreeVertices();
	        void   b3ComputeVertices();
	        void   b3ComputeNormals(b3_bool normalize = true);
	        void   b3ComputeIndices();
	        b3_f64 b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	        void   b3Normal(b3_ray *ray);
};

// DOUGHNUT, TORUS
class b3Torus : public b3SimpleShape, public b3ShapeBaseTrans
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

	        void    b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void    b3ComputeVertices();
	        void    b3ComputeNormals(b3_bool normalize=true);
	        void    b3ComputeIndices();
	        b3_f64  b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	        void    b3Normal(b3_ray *ray);
	        void    b3SetupPicking(b3PickInfo *pick_info);

	        b3_bool b3Prepare();
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void    b3Transform(b3_matrix *transformation,b3_bool is_affine);
};

// normal interpolation defines
#define PHONG                   (1 << b3TriangleShape::B3_PHONG_B)     // use Phong interpolation
#define NORMAL_VERTEX_VALID     (1 << b3TriangleShape::B3_NORMAL_VERTEX_VALID_B)     // normals of vertices valid, no auto computation
#define NORMAL_FACE_VALID       (1 << b3TriangleShape::B3_NORMAL_FACE_VALID_B)     // normals of triangles valid, no auto computation

class b3TriangleShape : public b3SimpleShape
{
	b3Array<b3_index>     *m_GridList;       // list of grids
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
	b3TriangleShape(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3TriangleShape);
	B3_ITEM_LOAD(b3TriangleShape);

	               ~b3TriangleShape();
	        b3_bool b3Init(b3_count vertex_count,b3_count tria_count,b3_res xSize,b3_res ySize);
	        b3_f64  b3Intersect(b3_ray *ray,b3_polar_precompute *polar);
	        void    b3Normal(b3_ray *ray);
	        b3_bool b3NormalDeriv(b3_ray *ray);
	virtual b3_bool b3Prepare();
	virtual void    b3Transform(b3_matrix *transformation,b3_bool is_affine);

protected:
	        void    b3FreeTriaRefs();
private:
	        void    b3PrepareGridList();
	        void    b3AddCubicItem(b3_count trianum,b3_index index);
	        void    b3SearchCubicItem(
				b3_vector *P1,b3_vector *P2,b3_vector *P3,
				b3_index   index,b3_index   rec,b3_count   MaxRec);
	        b3_f64  b3IntersectTriangleList(
				b3_ray                *ray,
				b3_polar_precompute   *polar,
				b3_index               index);
};

// index calculation of triangle grid
#define GRID_INDEX(x,y,z,GridSize)  (((z)*(GridSize)+(y))*(GridSize)+(x))

// TRIANGLES
class b3Triangles : public b3TriangleShape
{
public:
	B3_ITEM_INIT(b3Triangles);
	B3_ITEM_LOAD(b3Triangles);

	void   b3StoreShape();
	void   b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void   b3ComputeVertices();
	void   b3ComputeNormals(b3_bool normalize=true);
	void   b3ComputeIndices();
	void   b3SetupPicking(b3PickInfo *pick_info);
};

// SPLINE_ROT
class b3SplineRotShape : public b3TriangleShape
{
	b3_count         m_xSubDiv;
	b3_count         m_ySubDiv;
public:
	b3_line          m_Axis;                // for rotation shapes
	b3_s32           m_rSubDiv;             // sub division for rotation
	b3Spline         m_Spline;              // spline curve
	b3_f32           m_Knots[B3_MAX_KNOTS]; // one knot vector
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
class b3SplineShape : public b3TriangleShape
{
	b3_count         m_xSubDiv,m_ySubDiv;

protected:
	b3_count         m_GridVertexCount;
	b3_count         m_SolidVertexCount;
	
public:
	b3Spline         m_Spline[2];
	b3_f32           m_Knots[2][B3_MAX_KNOTS];
	b3_vector       *m_Controls;

protected:
	     b3SplineShape(b3_size class_size,b3_u32 class_type);

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

class b3SplineArea : public b3SplineShape
{
public:
	B3_ITEM_INIT(b3SplineArea);
	B3_ITEM_LOAD(b3SplineArea);
};

class b3SplineCylinder : public b3SplineShape 
{
public:
	B3_ITEM_INIT(b3SplineCylinder);
	B3_ITEM_LOAD(b3SplineCylinder);
};

class b3SplineRing : public b3SplineShape 
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

class b3CSGShape : public b3Shape
{
protected:
	b3_s32           m_Index;

public:
	       b3_csg_operation m_Operation;
	static b3_csg_operation m_CSGMode[];

protected:
	b3CSGShape(b3_size class_size,b3_u32 class_type);

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
class b3CSGSphere : public b3CSGShape
{
	b3_f64            m_QuadRadius;       // squared radius

public:
	b3_vector         m_Base;             // mid of sphere
	b3_vector         m_Dir;              // direction

public:
	B3_ITEM_INIT(b3CSGSphere);
	B3_ITEM_LOAD(b3CSGSphere);

	void     b3StoreShape();
	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	void     b3ComputeVertices();
	void     b3ComputeNormals(b3_bool normalize=true);
	void     b3ComputeIndices();
	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	b3_count b3GetMaxIntersections();
	void     b3Normal(b3_ray *ray);
	b3_bool  b3Prepare();
	void     b3Transform(b3_matrix *transformation,b3_bool is_affine);
	void     b3SetupPicking(b3PickInfo *pick_info);
};

// CSG_CYLINDER, CSG_CONE, CSG_ELLIPSOID, CSG_BOX
class b3CSGShape3 : public b3CSGShape, public b3ShapeBaseTrans
{
protected:
	b3CSGShape3(b3_size class_size,b3_u32 class_type);

public:
	B3_ITEM_INIT(b3CSGShape3);
	B3_ITEM_LOAD(b3CSGShape3);

	        void     b3StoreShape();
	        b3_bool  b3Prepare();
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	virtual void     b3ComputeNormals(b3_bool normalize = true);
	        void     b3Transform(b3_matrix *transformation,b3_bool is_affine);
	        void     b3SetupPicking(b3PickInfo *pick_info);
	        b3_count b3GetMaxIntersections();
};

class b3CSGCylinder : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCylinder);
	B3_ITEM_LOAD(b3CSGCylinder);

	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	void     b3Normal(b3_ray *ray);
};

class b3CSGCone : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGCone);
	B3_ITEM_LOAD(b3CSGCone);

	void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	void     b3ComputeVertices();
	void     b3ComputeIndices();
	b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	void     b3Normal(b3_ray *ray);
};

class b3CSGEllipsoid : public b3CSGShape3
{

public:
	B3_ITEM_INIT(b3CSGEllipsoid);
	B3_ITEM_LOAD(b3CSGEllipsoid);

	        void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void     b3ComputeVertices();
	        void     b3ComputeIndices();
	        b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        void     b3Normal(b3_ray *ray);
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
};

class b3CSGBox : public b3CSGShape3
{
	b3_gl_vertex  box_vertex[8 * 3];

public:
	B3_ITEM_INIT(b3CSGBox);
	B3_ITEM_LOAD(b3CSGBox);

	        void    b3AllocVertices(b3RenderContext *context);
	        void    b3FreeVertices();
	        void    b3ComputeVertices();
	        void    b3ComputeNormals(b3_bool normalize = true);
	        void    b3ComputeIndices();
	        b3_bool b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void    b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        void    b3Normal(b3_ray *ray);
	virtual void    b3GetStencilBoundInfo(b3_stencil_bound *info);
};

// CSG_TORUS
class b3CSGTorus : public b3CSGShape, public b3ShapeBaseTrans
{
	b3_f64             m_aQuad,m_bQuad;      // squared lengths of aRad, bRad

public:
	b3_f64             m_aRad, m_bRad;       // radiuses of torus

public:
	B3_ITEM_INIT(b3CSGTorus);
	B3_ITEM_LOAD(b3CSGTorus);

	        void     b3StoreShape();
	        void     b3GetCount(b3RenderContext *context,b3_count &vertCount,b3_count &gridCount,b3_count &polyCount);
	        void     b3ComputeVertices();
	        void     b3ComputeNormals(b3_bool normalize=true);
	        void     b3ComputeIndices();
	        b3_bool  b3Intersect(b3_ray *ray,b3_shape_intervals *interval,b3_line64 *BTLine);
	        void     b3InverseMap(b3_ray *ray,b3_csg_point *point);
	        b3_count b3GetMaxIntersections();
	        void     b3Normal(b3_ray *ray);
	        void     b3SetupPicking(b3PickInfo *pick_info);

	        b3_bool  b3Prepare();
	virtual void     b3GetStencilBoundInfo(b3_stencil_bound *info);
	        void     b3Transform(b3_matrix *transformation,b3_bool is_affine);
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
	b3_u32           m_Type;               // texture type
	b3_count         m_ShapeCount;
	b3_count         m_CSGIntersectionCount;

public:
	b3_vector        m_DimBase;
	b3_vector        m_DimSize;
	b3_matrix        m_Matrix;             // all composed transformations
	char             m_BoxName[B3_BOXSTRINGLEN];   // object name
	char             m_BoxURL[B3_BOXSTRINGLEN]; // HTML link

	b3_gl_vertex     m_BBoxVertex[8];

	static b3_color  m_GridColor;
	static b3_bool   m_GridVisible;

public:
	B3_ITEM_INIT(b3BBox);
	B3_ITEM_LOAD(b3BBox);

	       void            b3Write();
	       void            b3Dump(b3_count level);
	       void            b3AllocVertices(b3RenderContext *context);
	       void            b3FreeVertices();
	       void            b3ComputeVertices();
		   void            b3ComputeNormals(b3_bool normalize = true);
	       void            b3Draw(b3RenderContext *context);
		   b3_bool         b3Transform(b3_matrix *transformation,b3_bool is_affine,b3_bool force_action = false);
		   void            b3Activate(b3_bool activate=true,b3_bool recurse=true);
		   void            b3Animate(b3Activation::b3_anim_activation animate = b3Activation::B3_ANIM_ACTIVE,b3_bool recurse=true);
		   b3_bool         b3IsActive();
		   void            b3Expand(b3_bool expand=true);
		   b3_bool         b3IsExpanded();
		   void            b3Update();
		   b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance);
		   b3_count        b3Count();
		   b3_bool         b3Prepare(b3_bool recursive = false);
		   char           *b3GetName();
		   b3_bool         b3BacktraceRecompute(b3BBox *search);
		   b3Base<b3Item> *b3FindBBoxHead(b3BBox  *bbox);
		   b3BBox         *b3FindParentBBox(b3Shape *shape);
	       b3_bool         b3Intersect(b3_ray *ray);
		   b3CSGShape     *b3IntersectCSG(b3_ray *ray);
		   void            b3CollectBBoxes(b3Array<b3BBoxReference> &array);
		   void            b3CollectBBoxes(b3_ray *ray,b3Array<b3BBox *> *array);
		   void            b3CollectBBoxes(b3_vector *lower,b3_vector *upper,b3Array<b3BBox *> *array);

		   void            b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation);
 	static void            b3Reorg(b3Base<b3Item> *depot,b3Base<b3Item> *base,b3_count level,b3_count rec,b3Item *insert_after=null);
	static void            b3Recount(b3Base<b3Item> *base,b3_count level = 1);
	static b3_bool         b3FindBBox(b3Base<b3Item> *base,b3BBox *search);
	static b3BBox         *b3ReadCOB(const char *filename);
	static b3BBox         *b3ReadTGF(const char *filename);

	inline b3Base<b3Item> *b3GetShapeHead()
	{
		return &m_Heads[0];
	}

	inline b3Base<b3Item> *b3GetBBoxHead()
	{
		return &m_Heads[1];
	}

protected:
	inline void            b3GetGridColor(b3_color *color)
	{
		*color = m_GridColor;
	}

	inline b3_render_mode  b3GetRenderMode()
	{
		return m_GridVisible ? B3_RENDER_LINE : B3_RENDER_NOTHING;
	}
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
#define LIGHT_SPOT_OFF          4L

#define LIGHT_NAMEOFF(Node) ((char *)(Node)+BINDEX_OFFSET(Node))
#define LIGHT_NAMELEN(Node) (BINDEX_OFFSET(Node) > 0 ? \
	BINDEX_LENGTH(Node) - BINDEX_OFFSET(Node) : 0)


struct b3_ray_info : public b3_ray
{
	b3_index     depth;
	b3_color     color;
	b3Shape     *shape;
	b3BBox      *bbox;
};

struct b3_ray_fork : public b3_surface
{
	b3_ray_info *incoming;
	b3_ray_info  refl_ray;
	b3_ray_info  refr_ray;
};

// aux. structure for JitterLight
struct b3_light_info : public b3_ray_info
{
	b3_vector LightView,xDir,yDir;
	b3_color  Result;
	b3_f64    Size,LightFrac,LightDist;
	b3_s32    Distr;
};

class b3InitLight
{
protected:
	static void b3Init();
};

// POINT_LIGHT
class b3Scene;
class b3Light : public b3Item
{
	b3_f64            m_HalfJitter;
	b3_f64            m_FullJitter;
	b3_vector         m_SpotDir;

public:
	b3_vector        m_Position;     // Position
	b3_color         m_Color;        // Farbe
	b3_f32           m_Distance;     // rel. Reichweite
	b3_s32           m_Flags;        // Lampentyp
	b3_f32           m_Size;         // Auffaecherung
	b3_s32           m_JitterEdge;   // Kantenaufspaltung
	b3_vector        m_Direction;    // Abstrahlrichtung
	b3Spline         m_Spline;       // Spline Kurve
	b3_f32           m_Knots[B3_MAX_KNOTS];
	b3_vector        m_Controls[B3_MAX_CONTROLS];
	b3_bool          m_LightActive;
	b3_bool          m_SoftShadow;
	b3_bool          m_SpotActive;
	char             m_Name[B3_LIGHTNAMELEN]; // Lampenname

public:
	B3_ITEM_INIT(b3Light);
	B3_ITEM_LOAD(b3Light);

	void     b3Write();
	b3_bool  b3Illuminate(b3Scene *scene,b3_ray_fork *surface);
	b3_bool  b3Prepare();
	b3_bool  b3IsActive();
	char    *b3GetName();

	inline b3_f64 b3GetSpotFactor(b3_f64 angle) // angle inside [0..1]
	{
		if (m_SpotActive)
		{
			b3_vector point;

			m_Spline.b3DeBoorOpened(&point,0,angle);
			return point.y;
		}
		else
		{
			return 1;
		}
	}

private:
	void         b3Init();
	b3_bool      b3PointIllumination(b3Scene *scene,b3_ray_fork *surface);
	b3_bool      b3AreaIllumination(b3Scene  *scene,b3_ray_fork *surface);
	b3Shape     *b3CheckSinglePoint (b3Scene *scene,b3_ray_fork *surface,
		b3_light_info *Jit,b3_coord x,b3_coord y);
};

/*************************************************************************
**                                                                      **
**                        animations                                    **
**                                                                      **
*************************************************************************/

#define CLASS_ANIMATION         0x75000000 // animation class
#define CLASS_VERTEX            0x00010000
#define CLASS_TRIANGLE	        0x00020000
#define TYPE_MOVE               0x00000001
#define TYPE_ROTATE             0x00000002
#define TYPE_SCALE              0x00000003
#define TYPE_VERTEXNODE         0x00000001
#define TYPE_TRIA               0x00000001
#define TYPE_TRIAREF            0x00000002
#define TYPE_CPOINT_3D          0x00000003
#define TYPE_CPOINT_4D          0x00000004
#define ANIM_MOVE              (CLASS_ANIMATION|TYPE_MOVE)
#define ANIM_ROTATE            (CLASS_ANIMATION|TYPE_ROTATE)
#define ANIM_SCALE             (CLASS_ANIMATION|TYPE_SCALE)
#define TRIANGLE               (CLASS_TRIANGLE|TYPE_TRIA)
#define TRIANGLEREF            (CLASS_TRIANGLE|TYPE_TRIAREF)
#define VERTEXNODE             (CLASS_VERTEX|TYPE_VERTEXNODE)
#define CPOINT_3D              (CLASS_VERTEX|TYPE_CPOINT_3D)
#define CPOINT_4D              (CLASS_VERTEX|TYPE_CPOINT_4D)

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

class b3AnimControl : public b3Item
{
	b3_size       m_Dimension;           // vector dimension
	b3_count      m_Used;                // used number of vectors
	b3_count      m_Max;                 // maximum number of vectors
	b3_vector4D  *m_Controls;

public:
	B3_ITEM_INIT(b3AnimControl);
	B3_ITEM_LOAD(b3AnimControl);

	void b3Write();
	void b3InitNurbs(b3Nurbs &nurbs);
};

class b3Animation;

class b3AnimElement : public b3Item
{
	b3_s32              m_Empty;                   // empty entry
public:
	b3_vector           m_Center;                  // rotation center
	b3_matrix           m_Actual;                  // actual transformation
	b3_matrix           m_NeutralInverse;          // inverse of neutral position
	b3_f64              m_Ratio;                   // parametrisation ratio
	b3_f64              m_Start;                   // start of action
	b3_f64              m_End;                     // end of action
	b3_u32              m_Flags;                   // ANIMF_xxx flags
	b3_index            m_TrackIndex;              // start track number
	b3_bool             m_CurveUse;                // number of curves used
	b3Nurbs             m_Param;                   // param. curves
	b3_f32              m_Knots[B3_MAX_KNOTS];     // knot vectors
	char                m_Name[B3_ANIMSTRINGLEN];  // element name
	char                m_Object[B3_BOXSTRINGLEN]; // name of destination object

public:
	B3_ITEM_INIT(b3AnimElement);
	B3_ITEM_LOAD(b3AnimElement);

	void b3Write();

public:
	       char           *b3GetName();
	       void            b3GetPosition(b3_vector32_4D *position,b3_f64 t);
	       void            b3GetPosition(b3_vector      *position,b3_f64 t);
	       void            b3ComputeTransformationMatrix(b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);

	       b3_bool         b3SelectAnimElement (b3Scene *scene);
	       void            b3SelectObjects (b3BBox *BBox);
private:
	       void            b3AnimateMove  (b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);
	       void            b3AnimateRotate(b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);
	       void            b3AnimateScale (b3Animation *AnimRoot,b3_matrix *transform,b3_f64 t);
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
#define TYPE_CAUSTIC            0x00000008
#define SUPERSAMPLE4            (CLASS_SPECIAL|TYPE_SUPERSAMPLE4)
#define NEBULAR                 (CLASS_SPECIAL|TYPE_NEBULAR)
#define CAMERA                  (CLASS_SPECIAL|TYPE_CAMERA)
#define LINES_INFO              (CLASS_SPECIAL|TYPE_MODELLERINFO)
#define ANIMATION               (CLASS_SPECIAL|TYPE_ANIMATION)
#define DISTRIBUTE              (CLASS_SPECIAL|TYPE_DISTRIBUTE)
#define LENSFLARE               (CLASS_SPECIAL|TYPE_LENSFLARE)
#define CAUSTIC                 (CLASS_SPECIAL|TYPE_CAUSTIC)

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
	b3_bool     m_Active;

public:
	b3_color    m_Limit;

public:
	B3_ITEM_INIT(b3SuperSample);
	B3_ITEM_LOAD(b3SuperSample);

	void    b3Write();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate=true);
};

// CAMERA
class b3CameraPart : public b3Special
{
public:
	b3_vector        m_Width;
	b3_vector        m_Height;
	b3_vector        m_EyePoint;
	b3_vector        m_ViewPoint;
	b3_s32           m_Flags;
	char             m_CameraName[B3_CAMERANAMELEN];

public:
	B3_ITEM_INIT(b3CameraPart);
	B3_ITEM_LOAD(b3CameraPart);

	void     b3Write();
	void     b3Orientate(b3_vector *eye,b3_vector *view,b3_f64 focal_length,b3_f64 width,b3_f64 height);
	void     b3Overview(b3_vector *center,b3_vector *size,b3_f64 xAngle,b3_f64 yAngle);
	void     b3ComputeFocalLength(b3_f64 length);
	b3_f64   b3GetFocalLength();
	b3_f64   b3GetTwirl();
	void     b3SetTwirl(b3_f64 twirl);
	void     b3ScaleFocalLength(b3_f64 factor);
	void     b3Transform(b3_matrix *transformation);
	char    *b3GetName();
	b3_bool  b3IsActive();
	void     b3Activate(b3_bool activate = true);
};

#define CAMERA_TITLE  1
#define CAMERA_ACTIVE 2

// NEBULAR
class b3Nebular : public b3Special
{
	b3_f64           m_NebularDenom;
public:
	b3_color         m_NebularColor;
	b3_f32           m_NebularVal;

public:
	B3_ITEM_INIT(b3Nebular);
	B3_ITEM_LOAD(b3Nebular);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate=true);
	void    b3GetNebularColor(b3_color *result);
	void    b3ComputeNebular(b3_color *input,b3_color *result,b3_f64 distance);
};

// LINES_INFO
enum b3_unit
{
	B3_UNIT_MM = 0,
	B3_UNIT_CM,
	B3_UNIT_IN,
	B3_UNIT_DM,
	B3_UNIT_FT,
	B3_UNIT_M,
	B3_UNIT_MAX
};

enum b3_measure
{
	B3_MEASURE_1 = 0,
	B3_MEASURE_10,
	B3_MEASURE_20,
	B3_MEASURE_50,
	B3_MEASURE_100,
	B3_MEASURE_200,
	B3_MEASURE_500,
	B3_MEASURE_1000,
	B3_MEASURE_CUSTOM,
	B3_MEASURE_MAX
};

class b3ModellerInfo : public b3Special
{
	b3_u32           m_Flags;

public:
	b3_vector        m_Center;
	b3_vector        m_StepMove;
	b3_vector        m_StepRotate;
	b3_f32           m_GridMove;
	b3_f32           m_GridRot;
	b3_bool          m_ResizeFlag;
	b3_bool          m_BBoxTitles;
	b3_bool          m_GridActive;
	b3_bool          m_AngleActive;
	b3_bool          m_CameraActive;
	b3_bool          m_UseSceneLights;
	b3_unit          m_Unit;
	b3_measure       m_Measure;
	b3_u32           m_CustomMeasure;

public:
	B3_ITEM_INIT(b3ModellerInfo);
	B3_ITEM_LOAD(b3ModellerInfo);

	void        b3Write();
	void        b3SnapToGrid(b3_vector *translation);
	void        b3SnapToAngle(b3_f64 &angle);
	b3_f64      b3ScaleUnitToMM();
	const char *b3GetUnitDescr();
	void        b3SetMeasure(b3_u32 measure);
	void        b3SetMeasure(b3_measure measure);
	b3_u32      b3GetMeasure(b3_bool force_custom_value=true);
};

#define B3_UNIT_MASK           0x0000f
#define B3_MEASURE_MASK        0x000f0
#define B3_CUSTOM_MEASURE_MASK 0x3ff00
#define B3_USE_SCENE_LIGHTS    0x40000

#define B3_UNIT_SHIFT                0
#define B3_MEASURE_SHIFT             4
#define B3_CUSTOM_MEASURE_SHIFT      8

// ANIMATION
class b3Animation : public b3Special
{
	// OK, the following values are only for "Lines"
	b3_count        m_Frames;          // computed number of frames
	b3_count        m_Tracks;          // number of visible tracks
	b3_index        m_TrackIndex;      // start track in window 
	b3_index        m_FrameIndex;      // start frame in window
	b3_count        m_WTracks;         // actual number of tracks
	b3_count        m_WFrames;         // whole of frames
	b3AnimElement  *m_Element;         // actual animation element
	b3_vector       m_AnimCenter;

public:
	b3_f64   m_Start;           // start time (one unit per frame)
	b3_f64   m_End;             // end time (one unit per frame)
	b3_f64   m_Time;            // time point
	b3_f64   m_Neutral;         // neutral point
	b3_count m_FramesPerSecond;
	b3_u32   m_Flags;

public:
	B3_ITEM_INIT(b3Animation);
	B3_ITEM_LOAD(b3Animation);

	       void            b3Write();

public:
	       void            b3SetAnimElement (b3AnimElement *Element);
	       b3_bool         b3IsActive();
	       void            b3Activate(b3_bool activate = true);

	       void            b3SetAnimation (b3Scene *Global,b3_f64 t);
	       void            b3ResetAnimation (b3Scene *Global);
	       b3_bool         b3ActivateAnimation(b3Scene *scene,b3_bool activate = true);
	       b3_f64          b3AnimTimeCode (b3_index index);
	       b3_index        b3AnimFrameIndex (b3_f64 t);
	static b3_f64          b3ClipTimePoint(b3_f64 val,b3_f64 min,b3_f64 max);
	       b3_f64          b3ClipTimePoint(b3_f64);
		   void            b3RecomputeCenter (b3AnimElement *Element,b3_vector *center,b3_f64 t);

	inline b3Base<b3Item> *b3GetAnimElementHead()
	{
		return &m_Heads[0];
	}

private:
	       void            b3RecomputeNeutralInverse (b3AnimElement *Element);
		   void            b3GetNeutralPosition(b3AnimElement *Element,b3_vector *neutral);
		   void            b3ApplyTransformation (b3Scene *Global,b3AnimElement *Anim,b3_matrix *transform,b3_f64 t);
	static b3AnimElement  *b3FindSameTrack(b3AnimElement *Element);
};

#define ANIMB_ON     1

#define ANIMF_ON     (1<<ANIMB_ON)

// DISTRIBUTE

enum b3_sample
{
	SAMPLE_REGULAR     = 0x0000,
	SAMPLE_RANDOM      = 0x0100,
	SAMPLE_JITTER      = 0x0200,
	SAMPLE_SEMI_JITTER = 0x0300,
	SAMPLE_SEPARATED   = 0x0400
};

class b3Distribute : public b3Special
{
public:
	b3Array<b3_f64>    m_MotionBlur;
	b3Array<b3_index>  m_TimeIndex;
	b3_u32             m_Type;
	b3_count           m_SamplesPerPixel;
	b3_count           m_SamplesPerFrame;
	b3_f32             m_DepthOfField;
	b3_filter          m_PixelAperture;
	b3_filter          m_FrameAperture;
	b3Filter          *m_FilterPixel;
	b3Filter          *m_FilterFrame;
	b3_f32            *m_Samples;
	b3_count           m_SPP;

public:
	B3_ITEM_INIT(b3Distribute);
	B3_ITEM_LOAD(b3Distribute);

	virtual ~b3Distribute();
	void     b3Write();
	b3_bool  b3IsActive();
	b3_bool  b3IsMotionBlur();
	void     b3Prepare(b3_res xSize,b3Animation *animation=null);
};

#define SAMPLE_MOTION_BLUR_B     0
#define SAMPLE_SUPERSAMPLE_B     1
#define SAMPLE_DEPTH_OF_FIELD_B  2

#define SAMPLE_MOTION_BLUR      (1 << SAMPLE_MOTION_BLUR_B)
#define SAMPLE_SUPERSAMPLE      (1 << SAMPLE_SUPERSAMPLE_B)
#define SAMPLE_DEPTH_OF_FIELD   (1 << SAMPLE_DEPTH_OF_FIELD_B)

#define SAMPLE_GET_FLAGS(d)     ((d)->m_Type & 0x00ff)
#define SAMPLE_GET_TYPE(d)      (b3_sample)((d)->m_Type & 0xff00)
#define SAMPLE_SET_FLAGS(d,v)   ((d)->m_Type = ((d)->m_Type & 0xffffff00) | (v))
#define SAMPLE_SET_TYPE(d,v)    ((d)->m_Type = ((d)->m_Type & 0xffff00ff) | (v))

// LENSFLARE
class b3LensFlare : public b3Special
{
public:
	b3_s32         m_Flags;
	b3_color       m_Color;
	b3_f32         m_Expon;

public:
	B3_ITEM_INIT(b3LensFlare);
	B3_ITEM_LOAD(b3LensFlare);

	void    b3Write();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate=true);
};

#define LENSFLARE_ACTIVE 1

// CAUSTIC
class b3Caustic : public b3Special
{
public:
	b3_u32   m_Flags;
	b3_count m_NumPhotons;
	b3_count m_TraceDepth;

public:
	B3_ITEM_INIT(b3Caustic);
	B3_ITEM_LOAD(b3Caustic);

	void b3Write();
};

#define CAUSTIC_ENABLE_B 0
#define CAUSTIC_ENABLE   (1 << CAUSTIC_ENABLE_B)

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

// m_BackgroundType
enum b3_bg_type
{
	TP_NOTHING    = 0,            // Lightning background
	TP_TEXTURE    = 1,            // Background image
	TP_SLIDE      = 2,            // Background slide
	TP_SKY_N_HELL = 3             // Sky & hell
};

class b3RayRow;

class b3Scene : public b3Item
{
	b3Base<b3Row>    m_RowPool;
	b3Base<b3Row>    m_TrashPool;
	b3PrepareInfo    m_PrepareInfo;
	b3_vector        m_NormWidth;
	b3_vector        m_NormHeight;

protected:
	b3Path           m_Filename;
	b3Mutex          m_PoolMutex;
	b3Mutex          m_TrashMutex;
	b3Mutex          m_SamplingMutex;
	b3Distribute    *m_Distributed;
	b3Nebular       *m_Nebular;
	b3SuperSample   *m_SuperSample;
	b3LensFlare     *m_LensFlare;
	b3CameraPart    *m_ActualCamera;
	b3_f64           m_ShadowFactor;        // Schattenhelligkeit
	b3_color         m_AvrgColor;
	b3_color         m_DiffColor;
	b3_vector64      m_xHalfDir;
	b3_vector64      m_yHalfDir;
	b3_vector64      m_xStepDir;
	b3_count         m_LightCount;

public:
	// Camera
	b3_vector        m_EyePoint;
	b3_vector        m_ViewPoint;
	b3_vector        m_Width;               // Bildschirmvektor X
	b3_vector        m_Height;              // Bildschirmvektor Y
	b3_res           m_xSize,m_ySize;       // Rechenaufloesung
	b3_f32           m_xAngle,m_yAngle;     // Blickwinkel

	// Background
	b3_color         m_TopColor;
	b3_color         m_BottomColor;
	b3Tx            *m_BackTexture;         //
	b3_bg_type       m_BackgroundType;      // Hintergrund: Farbe/Datei/...

	b3_count         m_TraceDepth;          // Rekursionstiefe
	b3_u32           m_Flags;               // beschreibt, welche Werte gueltig sind
	b3_f32           m_ShadowBrightness;

	// Some limits
	b3_f32           m_BBoxOverSize;        // BBox-Ueberziehung
	b3_f32           m_Epsilon;             // Schwellenwert
	char             m_TextureName[B3_TEXSTRINGLEN]; // Name des Hintergrundbildes

	static b3TxPool  m_TexturePool;
	static b3_f64    epsilon;

protected:
	b3Scene(b3_size class_size,b3_u32  class_type);

public:
	B3_ITEM_INIT(b3Scene);
	B3_ITEM_LOAD(b3Scene);

	        void            b3Write();
	        void            b3Reorg();
	        b3_bool         b3GetDisplaySize(b3_res &xSize,b3_res &ySize);

			// Drawing routines
		    void            b3AllocVertices(b3RenderContext *context);
		    void            b3FreeVertices();
	virtual void            b3SetLights(b3RenderContext *context);
	        void            b3Draw(b3RenderContext *context);

			char           *b3GetName();
			void            b3Update();
		    b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper);
	        b3_bool         b3BacktraceRecompute(b3BBox *search);
		    b3Base<b3Item> *b3FindBBoxHead(b3BBox  *bbox);
		    b3BBox         *b3FindParentBBox(b3Shape *shape);
			b3Animation    *b3GetAnimation(b3_bool force = false);
			b3_f64          b3GetTimePoint();
		    b3ModellerInfo *b3GetModellerInfo();
			b3Distribute   *b3GetDistributed(b3_bool force = true);
		    b3Nebular      *b3GetNebular    (b3_bool force = true);
		    b3SuperSample  *b3GetSuperSample(b3_bool force = true);
		    b3LensFlare    *b3GetLensFlare  (b3_bool force = false);
		    b3CameraPart   *b3GetCamera(b3_bool must_active = false);
	        b3CameraPart   *b3GetActualCamera();
			b3CameraPart   *b3GetCameraByName(const char *camera_name);
		    b3CameraPart   *b3GetNextCamera(b3CameraPart *act);
		    b3CameraPart   *b3UpdateCamera();
			void            b3SetFilename(const char *filename);
			b3_bool         b3GetTitle(char *title);
			void            b3SetCamera(b3CameraPart *camera,b3_bool reorder=false);
		    b3Light        *b3GetLight(b3_bool must_active = false);
			b3Light        *b3GetLightByName(const char *light_name);
		    b3BBox         *b3GetFirstBBox();
		    b3_count        b3GetBBoxCount();
		    void            b3Activate(b3_bool activate=true);
		    void            b3Transform(b3_matrix *transformation,b3_bool is_affine = true,b3_bool force_action = false);
		    b3_bool         b3Prepare(b3_res xSize,b3_res ySize);
		    void            b3Raytrace(b3Display *display);
		    void            b3AbortRaytrace();
		    b3_bool         b3Intersect(b3_ray_info *ray,b3_f64 max = DBL_MAX);
			b3_bool         b3IsObscured(b3_ray_info *ray,b3_f64 max = DBL_MAX);
	virtual b3_bool         b3Shade(b3_ray_info *ray,b3_count depth = 0);
	virtual void            b3Illuminate(b3Light *light,b3_light_info *jit,b3_ray_fork *surface,b3_color *result);
	virtual b3_bool         b3FindObscurer(b3_ray_info *ray,b3_f64 max = DBL_MAX);
	        void            b3CollectBBoxes(b3_line64 *line,b3Array<b3BBox *> *array,b3_f64 max = DBL_MAX);
	        void            b3CollectBBoxes(b3_vector *lower,b3_vector *upper,b3Array<b3BBox *> *array);
		    void            b3GetBackgroundColor(b3_ray_info *ray,b3_f64 fx,b3_f64 fy);

			void            b3SetAnimation (b3_f64 t);
	        void            b3ResetAnimation();
			void            b3Animate(b3Activation::b3_anim_activation activation);

			void            b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation);
	static  b3Scene        *b3ReadTGF(const char *filename);

	inline b3Base<b3Item> *b3GetBBoxHead()
	{
		return &m_Heads[0];
	}

	inline b3Base<b3Item> *b3GetLightHead()
	{
		return &m_Heads[1];
	}

	inline b3Base<b3Item> *b3GetSpecialHead()
	{
		return &m_Heads[2];
	}

	static b3_bool   b3CheckTexture(b3Tx **tx,const char *name);
	static b3_bool   b3CutTextureName(const char *full_name,char *short_name);

protected:
		    b3_bool         b3ComputeOutputRays(b3_ray_fork *surface);
		    b3_f64          b3ComputeSpotExponent(b3Light *light);
		    void            b3GetInfiniteColor(b3_ray_info *ray);

private:
	        void            b3DoRaytrace(b3Display *display,b3_count CPUs);
	        void            b3DoRaytraceMotionBlur(b3Display *display,b3_count CPUs);
	static  b3_u32          b3RaytraceThread(void *ptr);
	static  b3_u32          b3RaytraceMotionBlurThread(void *ptr);
	static  b3_u32          b3PrepareThread(b3BBox *bbox,void *ptr);
	static  b3_u32          b3UpdateThread( b3BBox *bbox,void *ptr);
		    b3Shape        *b3Intersect(    b3BBox *bbox,b3_ray_info *ray);
		    b3Shape        *b3IsObscured(   b3BBox *bbox,b3_ray_info *ray);
		    void            b3MixLensFlare(b3_ray_info *ray);

	friend class b3RayRow;
	friend class b3SupersamplingRayRow;
	friend class b3DistributedRayRow;
	friend class b3MotionBlurRayRow;
};

class b3ScenePhong : public b3Scene
{
public:
	B3_ITEM_INIT(b3ScenePhong);
	B3_ITEM_LOAD(b3ScenePhong);

	b3_bool b3Shade(b3_ray_info *ray,b3_count depth = 0);
	void    b3Illuminate(b3Light *light,b3_light_info *jit,b3_ray_fork *surface,b3_color *result);
	b3_bool b3FindObscurer(b3_ray_info *ray,b3_f64 max = DBL_MAX);
};

class b3SceneMork : public b3Scene
{
public:
	B3_ITEM_INIT(b3SceneMork);
	B3_ITEM_LOAD(b3SceneMork);

	virtual void     b3SetLights(b3RenderContext *context);
	        b3_bool  b3Shade(b3_ray_info *ray,b3_count depth = 0);
	        void     b3Illuminate(b3Light *light,b3_light_info *jit,b3_ray_fork *surface,b3_color *result);

private:
	b3_bool b3IsPointLightBackground(b3Light *light,b3_ray_info *ray);
	void    b3LightFlare(b3_ray_info *ray);
};

class b3RayRow : public b3Row
{
protected:
	b3Display   *m_Display;
	b3Scene     *m_Scene;
	b3_color    *m_LastResult;
	b3_color    *m_ThisResult;
	b3_vector64  m_preDir;
	b3_coord     m_y;
	b3_res       m_xSize;
	b3_res       m_ySize;
	b3_f64       m_fxStep;
	b3_f64       m_fy;
	b3_f64       m_t;

public:
	              b3RayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);
	virtual      ~b3RayRow() {}
	virtual void  b3Raytrace();
};

enum b3_row_state
{
	B3_STATE_NOT_STARTED,
	B3_STATE_COMPUTING,
	B3_STATE_CHECK,
	B3_STATE_REFINING,
	B3_STATE_READY
};

class b3SupersamplingRayRow : public b3RayRow
{
	b3SupersamplingRayRow *m_PrevRow;
	b3SupersamplingRayRow *m_SuccRow;
	b3_color              *m_Limit;
	b3_row_state           m_RowState;
public:
	              b3SupersamplingRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize,b3SupersamplingRayRow *previous);
	virtual      ~b3SupersamplingRayRow() {}
	virtual void  b3Raytrace();

private:
	b3_bool b3Test(b3_res x);
	void    b3Refine(b3_bool this_row);
	void    b3Convert();
};

class b3DistributedRayRow : public b3RayRow
{
protected:
	b3Distribute *m_Distr;
	b3_count      m_SPP;
	b3_count      m_SPF;
	b3_vector64  *m_xHalfDir;
	b3_vector64  *m_yHalfDir;
	b3_f32       *m_Samples;

public:
	                b3DistributedRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);
	virtual        ~b3DistributedRayRow();
	virtual void    b3Raytrace();
};

class b3MotionBlurRayRow : public b3DistributedRayRow
{
	b3_index       *m_TimeIndex;
	b3_index        m_Index;
	b3_index        m_Modulo;
	b3_index        m_Start;
	b3_color       *m_Color;
	b3_vector64     m_BackupDir;

public:
	                b3MotionBlurRayRow(b3Scene *scene,b3Display *display,b3_coord y,b3_res xSize,b3_res ySize);
	virtual        ~b3MotionBlurRayRow();
	virtual void    b3Raytrace();
	        void    b3SetTimePoint(b3_f64 t);
};

// m_Flags
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
