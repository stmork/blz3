/*
**
**	$Filename:	b3Base.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
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

struct b3_polar_precompute
{
	b3_vector m_Polar; // surface coordinates of shape
	b3_vector m_ObjectPolar; // rel. shape coordinates
	b3_index  m_NormalIndex; // which triangle in case of triangle mesh
};

struct b3_polar : b3_polar_precompute
{
	b3_vector m_BoxPolar; // rel. bbox coordinates which contains the shape
};

class b3Shape;
class b3BBox;

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
	b3Shape    *shape;
	b3BBox     *bbox;
};

// aux. structure for computing illumination
struct b3_surface
{
	b3Color   m_Diffuse;
	b3Color   m_Ambient;
	b3Color   m_Specular;
	b3Color   m_SpecularSum;
	b3_f64    m_Reflection;
	b3_f64    m_Refraction;
	b3_f64    m_Ior;
	b3_f64    m_SpecularExp;
};

struct b3_ray_info : public b3_ray
{
	b3_index     depth;
	b3Color      color;
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
	b3Color   Result;
	b3_f64    Size,LightFrac,LightDist;
	b3_s32    Distr;
};

class B3_PLUGIN b3Activation
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


#endif
