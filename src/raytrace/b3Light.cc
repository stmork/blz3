/*
**
**	$Filename:      b3Light.cc $
**	$Release:       Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Raytracing light sources
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.25  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.24  2002/02/17 21:25:07  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.23  2001/12/31 11:05:18  sm
**      - Added TestData for testing Blizzard data structures for reading
**        and writing.
**      - Fixed bugs found with previous mentioned tool:
**        o Some b3AnimElement errors found to be fixed under Windows.
**        o b3TriangleShape destructor handled unchecked m_GridList pointer
**      - Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**        B3LOG_NORMAL to B3LOG_DEBUG.
**
**      Revision 1.22  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.21  2001/12/23 09:43:26  sm
**      - Inlining some methods.
**
**      Revision 1.20  2001/12/22 21:08:35  sm
**      - Tidied up some dialogs
**      - Designed new icons for document templates
**      - Toolbars got adjusted and beautified
**      - Introduced b3Scene::b3IsObscured() for faster Phong illumination
**      - Found and fixed some minor bugs
**
**      Revision 1.19  2001/12/02 17:38:17  sm
**      - Removing nasty CR/LF
**      - Added b3ExtractExt()
**      - Added stricmp() for Un*x
**      - Fixed some defines
**
**      Revision 1.18  2001/12/02 15:43:49  sm
**      - Creation/Deletion/Editing of lights
**      - Creation/Deletion of cameras
**      - New toolbars introduced.
**
**      Revision 1.17  2001/10/19 19:43:15  sm
**      - Searching for 5 percent performance lost...
**
**      Revision 1.16  2001/10/19 18:27:28  sm
**      - Fixing LDC bug
**      - Optimizing color routines
**
**      Revision 1.15  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.14  2001/10/17 21:09:06  sm
**      - Triangle support added for intersections, normal computations. So
**        Spline shapes can be computed, too. Now only CSG is missing.
**
**      Revision 1.13  2001/10/17 14:46:02  sm
**      - Adding triangle support.
**      - Renaming b3TriangleShape into b3Triangles and introducing
**        new b3TriangleShape as base class. This results in
**        source file renaming, too.
**      - Fixing soft shadow bug.
**      - Only scene loading background image when activated.
**      - Fixing LDC spline initialization.
**      - Converting Windows paths into right paths on Un*x
**
**      Revision 1.12  2001/10/15 19:07:12  sm
**      - Experimenting with soft shadows.
**
**      Revision 1.11  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.10  2001/10/07 20:41:32  sm
**      - Updating MSVC project status
**
**      Revision 1.9  2001/10/07 20:17:27  sm
**      - Prepared texture support.
**      - Noise procedures added.
**      - Added bump and material support.
**      - Added soft shadows.
**
**      Revision 1.8  2001/10/06 19:56:00  sm
**      - Fixing bugs concerning reflection and
**        refraction computation (both: direction
**        computation and shading)
**
**      Revision 1.7  2001/10/05 20:30:45  sm
**      - Introducing Mork and Phong shading.
**      - Using light source when shading
**
**      Revision 1.6  2001/10/03 18:46:45  sm
**      - Adding illumination and recursive raytracing
**
**      Revision 1.5  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.4  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
**      Revision 1.3  2001/08/20 14:16:48  sm
**      - Putting data into cmaera and light combobox.
**      - Selecting camera and light.
**
**      Revision 1.2  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

void b3InitLight::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering light sources...\n");

	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, POINT_LIGHT );
	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, AREA_LIGHT );
	b3Item::b3Register(&b3Light::b3Init, &b3Light::b3Init, SPOT_LIGHT );
}

b3Light::b3Light(b3_u32 class_type) : b3Item(sizeof(b3Light),class_type)
{
	b3Init();
}

b3Light::b3Light(b3_u32 *src) : b3Item(src)
{
	b3_index i;

	b3Init();
	b3InitVector(&m_Position);
	b3InitColor(&m_Color);
	m_Distance = b3InitFloat();
	m_Flags    = b3InitInt();

	if ((b3GetClassType() >= AREA_LIGHT) && B3_PARSE_INDEX_VALID)
	{
		m_Size       = b3InitFloat();
		m_JitterEdge = b3InitInt();
		if (b3GetClassType() >= SPOT_LIGHT)
		{
			b3InitVector(&m_Direction);

			b3InitSpline(&m_Spline,m_Controls,m_Knots);
			for (i = 0;i < B3_MAX_KNOTS;i++)    m_Knots[i] = b3InitFloat();
			for (i = 0;i < B3_MAX_CONTROLS;i++) b3InitVector(&m_Controls[i]);
			m_SpotActive = (m_Flags & LIGHT_SPOT_OFF ? false : true);
		}
		else
		{
			m_SpotActive = false;
		}
		m_SoftShadow = ((m_Flags & LIGHT_PENUMBRA) != 0);
	}
	else
	{
		m_SoftShadow = false;
	}
	m_LightActive = ((m_Flags & LIGHT_OFF) == 0);

	if (m_ItemOffset > 0)
	{
		b3InitString(m_Name,m_ItemSize - (m_ParseIndex << 2));
	}

	if (m_LightActive)
	{
		b3PrintF(B3LOG_DEBUG,"  Light \"%s\" is active",m_Name);
		if (m_SoftShadow)
		{
			b3PrintF(B3LOG_DEBUG," and uses soft shadows");
		}
		if (m_SpotActive)
		{
			b3PrintF(B3LOG_DEBUG," and uses light distribution curve");
		}
		b3PrintF(B3LOG_DEBUG,".\n");
	}
}

void b3Light::b3Write()
{
	b3_index i;

	m_Flags = 0;
	ClassType = SPOT_LIGHT;
	if (!m_LightActive) m_Flags |= LIGHT_OFF;
	if  (m_SoftShadow)  m_Flags |= LIGHT_PENUMBRA;
	if (!m_SpotActive)  m_Flags |= LIGHT_SPOT_OFF;

	b3StoreVector(&m_Position);
	b3StoreColor(&m_Color);
	b3StoreFloat(m_Distance);
	b3StoreInt(m_Flags);
	b3StoreFloat(m_Size);
	b3StoreInt(m_JitterEdge);
	b3StoreVector(&m_Direction);

	b3StoreSpline(&m_Spline);
	for (i = 0;i < B3_MAX_KNOTS;i++)    b3StoreFloat(m_Knots[i]);
	for (i = 0;i < B3_MAX_CONTROLS;i++) b3StoreVector(&m_Controls[i]);

	b3StoreString(m_Name,B3_BOXSTRINGLEN);
}

#define INIT_DEGREE      3
#define INIT_CONTROL_MAX 7

void b3Light::b3Init()
{
	b3_index i;

	m_Color.r     =      1.0;
	m_Color.g     =      1.0;
	m_Color.b     =      1.0;
	m_Distance    =  50000.0;
	m_Position.x  =  10000.0;
	m_Position.y  = -10000.0;
	m_Position.z  =  10000.0;
	m_Size        =      0.02f;
	m_JitterEdge  =      5;
	m_Flags       =      0;
	m_Direction.x =   -100.0;
	m_Direction.y =    100.0;
	m_Direction.z =   -100.0;

	m_Spline.offset      = 1;
	m_Spline.knots       = m_Knots;
	m_Spline.controls    = m_Controls;
	m_Spline.control_max = sizeof(m_Controls) / sizeof(m_Controls[0]);
	m_Spline.knot_max    = sizeof(m_Knots)    / sizeof(m_Knots[0]);
	b3BSplineInitCurve (&m_Spline,INIT_DEGREE,INIT_CONTROL_MAX,false);
	for (i = 0;i < INIT_CONTROL_MAX;i++)
	{
		m_Controls[i].x = (b3_f64)i / (INIT_CONTROL_MAX - 1);
		m_Controls[i].y = 1.0;
		m_Controls[i].z = 0;
	}
	m_Controls[INIT_CONTROL_MAX - 1].y = 0.5;

	for (i = INIT_DEGREE;i <= INIT_CONTROL_MAX;i++)
	{
		m_Knots[i] =
			(b3_f64)(i - INIT_DEGREE) /
			(b3_f64)(INIT_CONTROL_MAX - INIT_DEGREE);
	}
	m_Knots[INIT_CONTROL_MAX] += epsilon;
	b3BSplineThroughEndControl (&m_Spline);

	strcpy(m_Name,"Light");
	m_LightActive = true;
	m_SoftShadow  = false;
	m_SpotActive  = false;
}

b3_bool b3Light::b3Prepare()
{
	if (m_LightActive)
	{
		if (m_SoftShadow)
		{
			if (m_JitterEdge > 1)
			{
				m_HalfJitter = 0.5 / (b3_f64)(m_JitterEdge - 1);
				m_FullJitter = 1.0 / (b3_f64)(m_JitterEdge * m_JitterEdge);
			}
			else
			{
				m_SoftShadow = false;
			}
		}
		if (m_SpotActive)
		{
			b3_f64 length;

			length = b3Vector::b3Length(&m_Direction);
			if (length > 0)
			{
				m_SpotDir.x = m_Direction.x / length;
				m_SpotDir.y = m_Direction.y / length;
				m_SpotDir.z = m_Direction.z / length;
			}
			else
			{
				m_SpotActive = false;
			}
		}
	}
	return true;
}

char *b3Light::b3GetName()
{
	return m_Name;
}

b3_bool b3Light::b3Illuminate(
	b3Scene         *scene,
	b3_illumination *surface)
{
	if (!m_LightActive)
	{
		return false;
	}
	return (m_SoftShadow ?
		b3AreaIllumination(scene,surface) :
		b3PointIllumination(scene,surface));
}

inline b3_bool b3Light::b3PointIllumination(
	b3Scene         *scene,
	b3_illumination *surface)
{
	b3_light_info Jit;
	b3_vector     point;
	b3_f64        LightDist,LightFrac,q,UpperBound;

	if (!m_LightActive)
	{
		return false;
	}

	Jit.Distr = 1;
	Jit.Size  = 0;
	Jit.pos.x = surface->incoming->ipoint.x;
	Jit.pos.y = surface->incoming->ipoint.y;
	Jit.pos.z = surface->incoming->ipoint.z;
	Jit.dir.x = m_Position.x - surface->incoming->ipoint.x;
	Jit.dir.y = m_Position.y - surface->incoming->ipoint.y;
	Jit.dir.z = m_Position.z - surface->incoming->ipoint.z;

	if ((UpperBound =
		Jit.dir.x * Jit.dir.x +
		Jit.dir.y * Jit.dir.y +
		Jit.dir.z * Jit.dir.z) == 0)
	{
		return false;
	}
	LightDist = 1.0 / (UpperBound = sqrt(UpperBound));
	Jit.dir.x *= LightDist;
	Jit.dir.y *= LightDist;
	Jit.dir.z *= LightDist;

	// Compute relative brightness via LDC
	// (= light distribution curve)
	if (m_SpotActive)
	{
		LightFrac = -(
			Jit.dir.x * m_SpotDir.x +
			Jit.dir.y * m_SpotDir.y +
			Jit.dir.z * m_SpotDir.z);
		if (LightFrac <= 0)
		{
			q = 1.0 - epsilon;
		}
		else
		{
			q = (LightFrac >= 1 ? epsilon : acos(LightFrac) * 2.0 / M_PI);
		}

		m_Spline.b3DeBoorOpened (&point,0,q);
		Jit.LightFrac = LightDist * m_Distance * point.y;
	}
	else
	{
		Jit.LightFrac = LightDist * m_Distance;
	}
	Jit.LightDist = LightDist;

	scene->b3FindObscurer(&Jit,UpperBound - epsilon);
	scene->b3Illuminate(this,&Jit,surface,&surface->incoming->color);
	return true;
}

inline b3_bool b3Light::b3AreaIllumination (
	b3Scene         *scene,
	b3_illumination *surface)
{
	b3Shape       *Edge1,*Edge2,*LastEdge = null;
	b3_light_info  Jit;
	b3_vector      point;
	b3_f64         Factor,denomLightDist,q;
	b3_coord       x,y,xs;
	b3_count       max,Distr;
	b3_bool        equal;				

	Jit.Distr = m_JitterEdge;
	Jit.Size  = m_Distance * m_Size / (b3_f64)Jit.Distr;

	// computing light axis between ipoint and light
	// Changed by SAM 9-04-1993
	Jit.pos         = surface->incoming->ipoint;
	Jit.LightView.x = m_Position.x - surface->incoming->ipoint.x;
	Jit.LightView.y = m_Position.y - surface->incoming->ipoint.y;
	Jit.LightView.z = m_Position.z - surface->incoming->ipoint.z;

	// normalizing light axis
	if ((denomLightDist =
		Jit.LightView.x * Jit.LightView.x +
		Jit.LightView.y * Jit.LightView.y +
		Jit.LightView.z * Jit.LightView.z) == 0)
	{
		return false;
	}
	denomLightDist = 1.0 / (Jit.LightDist = sqrt(denomLightDist));
	Jit.LightView.x *= denomLightDist;
	Jit.LightView.y *= denomLightDist;
	Jit.LightView.z *= denomLightDist;

	// inserted Nov. 1994, SAM
	if (m_SpotActive)
	{
		Factor = -(
			Jit.LightView.x * m_SpotDir.x +
			Jit.LightView.y * m_SpotDir.y +
			Jit.LightView.z * m_SpotDir.z);
		if (Factor <= 0) q = 1.0 - epsilon;
		else
		{
			q = (Factor >= 1 ? epsilon : acos(Factor) * 2.0 / M_PI);
		}

		m_Spline.b3DeBoorOpened (&point,0,q);
		Jit.LightFrac  = denomLightDist * m_Distance * point.y;
	}
	else
	{
		Jit.LightFrac = denomLightDist * m_Distance;
	}

	Factor = denomLightDist / sqrt(
		Jit.LightView.x * Jit.LightView.x +
		Jit.LightView.y * Jit.LightView.y);
	Jit.xDir.x	= -Jit.LightView.y * Factor;
	Jit.xDir.y	=  Jit.LightView.x * Factor;
	Jit.xDir.z	=  0;

	b3Vector::b3CrossProduct(&Jit.LightView,&Jit.xDir,&Jit.yDir);
	Factor = denomLightDist / b3Vector::b3Length(&Jit.yDir);
	Jit.yDir.x *= Factor;
	Jit.yDir.y *= Factor;
	Jit.yDir.z *= Factor;

	Jit.Result.a =
	Jit.Result.r =
	Jit.Result.g =
	Jit.Result.b = 0;

	// The following loops check every second point of the outline
	// of the light raster. If every sample hits the same shape the
	// shadow computation is completed.
	equal = true;
	Distr = Jit.Distr - 1;
	xs    = B3_IRAN(2);
	for (x = xs;x <= Distr;x += 2)
	{
		Edge1 = b3CheckSinglePoint (scene,surface,&Jit,x,0);
		Edge2 =	b3CheckSinglePoint (scene,surface,&Jit,Distr,Distr - x);

		equal   &= (Edge1 == Edge2);
		if (x != xs) equal &= (Edge1 == LastEdge);
		LastEdge = Edge1;
	}
	for (y = 2 - xs;y < Distr;y += 2)
	{
		Edge1 = b3CheckSinglePoint (scene,surface,&Jit,0,y);
		Edge2 =	b3CheckSinglePoint (scene,surface,&Jit,Distr,Distr - y);

		equal   &= ((Edge1 == Edge2) && (Edge1 == LastEdge));
		LastEdge = Edge1;
	}


	// This part computes the untested points in the light raster.
	if (equal)
	{
		Factor = m_HalfJitter;
	}
	else
	{
		// fill top and bottom outline
		for (x = 1 - xs;x <= Distr;x += 2)
		{
			b3CheckSinglePoint (scene,surface,&Jit,x,0);
			b3CheckSinglePoint (scene,surface,&Jit,x,Distr);
		}

		for (y = 1;y < Distr;y++)
		{
			max = Distr + ((Jit.Distr + xs) & 1);
			for (x = xs;x < max;x++)
			{
				b3CheckSinglePoint (scene,surface,&Jit,x,y);
			}
			xs ^= 1;
		}

		Factor = m_FullJitter;
	}

	surface->incoming->color.r += Jit.Result.r * Factor;
	surface->incoming->color.g += Jit.Result.g * Factor;
	surface->incoming->color.b += Jit.Result.b * Factor;
	return true;
}

inline b3Shape *b3Light::b3CheckSinglePoint (
	b3Scene         *scene,                                
	b3_illumination *surface,
	b3_light_info   *Jit,
	b3_coord         x,
	b3_coord         y)
{
	b3_f64   jx,jy,UpperBound;

	jx = (((b3_f64)x + 0.25 + B3_FRAN(0.5)) - 0.5 * Jit->Distr) * Jit->Size;
	jy = (((b3_f64)y + 0.25 + B3_FRAN(0.5)) - 0.5 * Jit->Distr) * Jit->Size;

	Jit->dir.x = Jit->LightView.x + jx * Jit->xDir.x + jy * Jit->yDir.x;
	Jit->dir.y = Jit->LightView.y + jx * Jit->xDir.y + jy * Jit->yDir.y;
	Jit->dir.z = Jit->LightView.z + jx * Jit->xDir.z + jy * Jit->yDir.z;

	if ((UpperBound = b3Vector::b3Normalize(&Jit->dir)) != 0)
	{
		scene->b3FindObscurer(Jit,Jit->LightDist / UpperBound - epsilon);
		scene->b3Illuminate(this,Jit,surface,&Jit->Result);
	}
	else
	{
		Jit->shape = null;
	}

	return Jit->shape;
}
