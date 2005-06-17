/*
**
**	$Filename:      b3Light.cc $
**	$Release:       Dortmund 2001, 2002, 2003 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Raytracing light sources
**
**	(C) Copyright 2001, 2002, 2003  Steffen A. Mork
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

#include "blz3/raytrace/b3Light.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/raytrace/b3Shade.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.54  2005/06/17 10:29:05  smork
**      - Made some inlining.
**      - Removed some unnecessary tests.
**      - Printing CPU message only once.
**
**      Revision 1.53  2005/06/07 08:56:48  smork
**      - Some further optimizations.
**
**      Revision 1.52  2005/06/06 14:59:41  smork
**      - More vectorization- More vectorization.
**
**      Revision 1.51  2005/01/03 10:34:30  smork
**      - Rebalanced some floating point comparisons:
**        a == 0  -> b3Math::b3NearZero
**        a == b  -> b3Math::b3IsEqual
**      - Removed some very inlikely fp comparisons
**        in intersection methods.
**
**      Revision 1.50  2004/10/16 17:00:52  sm
**      - Moved lighting into own class to ensure light setup
**        after view setup.
**      - Fixed lighting for scene and simple overview
**      - Fixed Light cutoff exponent deadloop.
**      - Corrected OpenGL define (BLZ3_USE_OPENGL)
**
**      Revision 1.49  2004/10/13 15:33:14  smork
**      - Optimized OpenGL lights.
**
**      Revision 1.48  2004/10/05 09:29:22  sm
**      - Donw some documentations.
**
**      Revision 1.47  2004/09/28 15:07:40  sm
**      - Support for car paint is complete.
**      - Made some optimizations concerning light.
**      - Added material dependend possibility for color
**        mixing instead of mixing inside shader.
**
**      Revision 1.46  2004/09/17 14:48:12  sm
**      - I have forgotten the area lights. Now sampling is correct by moving
**        the color sum from surface to Jit (light info).
**
**      Revision 1.45  2004/09/17 12:53:55  sm
**      - Changed chader signatures to sum on different color
**        channels (ambient, diffuse and specular). I wanted
**        to do this for a long time, puh!
**
**      Revision 1.44  2004/05/28 13:15:39  sm
**      - Major optimizations inside shader. But why is the intel brt3
**        5 minutes slower than the unoptimized version?
**
**      Revision 1.43  2004/05/22 17:02:56  sm
**      - Decoupled material shader.
**
**      Revision 1.42  2004/05/22 14:17:31  sm
**      - Merging some basic raytracing structures and gave them some
**        self explaining names. Also cleaned up some parameter lists.
**
**      Revision 1.41  2004/05/20 19:10:30  sm
**      - Separated shader from scene. this is easier
**        to handle.
**
**      Revision 1.40  2004/05/19 15:35:03  sm
**      - Hope of having fixed ticket no. 13.
**
**      Revision 1.39  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.38  2004/02/28 19:10:13  sm
**      - Cook/Torrance is applicable by use through material
**        shader.
**
**      Revision 1.37  2003/08/31 10:44:07  sm
**      - Further buffer overflow avoidments.
**
**      Revision 1.36  2003/07/25 07:24:04  sm
**      - Classical refactoring error: The effects of ticket no. 15 are
**        caused due to refactoring b3Light. An old method b3Init was
**        renamed to b3InitValues and an additional new method b3Init
**        was introduced not doing the same than before. This caused
**        the m_Spline member not to be initialized.
**
**      Revision 1.35  2003/07/24 16:21:51  sm
**      - Fixed Ticketno. 15: A simple point light wasn't initialized
**        properly.
**
**      Revision 1.34  2003/07/20 09:25:11  sm
**      - Init light name in every case
**
**      Revision 1.33  2003/07/12 17:44:47  sm
**      - Cleaned up raytracing b3Item registration
**
**      Revision 1.32  2003/03/04 20:37:37  sm
**      - Introducing new b3Color which brings some
**        performance!
**
**      Revision 1.31  2003/02/22 17:21:34  sm
**      - Changed some global variables into static class members:
**        o b3Scene::epsilon
**        o b3Scene::m_TexturePool et. al.
**        o b3SplineTemplate<class VECTOR>::bspline_errno
**
**      Revision 1.30  2003/01/03 15:47:09  sm
**      - Changed area light optimization.
**      - Fixed some errors in the light dialog.
**
**      Revision 1.29  2002/07/20 10:49:34  sm
**      - Added custom light support (not finished yet)
**      - Added b3Light::b3IsActive() for compatibility.
**      - Added texture search path support like in brt3.
**
**      Revision 1.28  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.27  2002/03/02 15:24:35  sm
**      - Templetized splines (uhff).
**      - Prepared spline shapes for their creation.
**        *** And now: Testing! Testing! Testing! ***
**
**      Revision 1.26  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
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

/*************************************************************************
**                                                                      **
**                        b3Light implementation                        **
**                                                                      **
*************************************************************************/

void b3Light::b3Register()
{
	b3PrintF (B3LOG_DEBUG,"Registering light sources...\n");

	b3Item::b3Register(&b3Light::b3StaticInit, &b3Light::b3StaticInit, POINT_LIGHT );
	b3Item::b3Register(&b3Light::b3StaticInit, &b3Light::b3StaticInit, AREA_LIGHT );
	b3Item::b3Register(&b3Light::b3StaticInit, &b3Light::b3StaticInit, SPOT_LIGHT );
}

b3Light::b3Light(b3_u32 class_type) : b3Item(sizeof(b3Light),class_type)
{
	b3InitValues();
}

b3Light::b3Light(b3_u32 *src) : b3Item(src)
{
	b3_index i;

	b3InitValues();
	b3InitVector(&m_Position);
	b3InitColor(m_Color);
	m_Distance = b3InitFloat();
	m_Flags    = b3InitInt();

	if ((b3GetClassType() >= AREA_LIGHT) && B3_PARSE_INDEX_VALID)
	{
		m_Size       = b3InitFloat();
		m_JitterEdge = b3InitInt();
		if (b3GetClassType() >= SPOT_LIGHT && B3_PARSE_INDEX_VALID)
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
		m_SpotActive = false;
	}
	m_LightActive = ((m_Flags & LIGHT_OFF) == 0);

	if (m_ItemOffset > 0)
	{
		b3InitString(m_Name,m_ItemSize - (m_ParseIndex << 2));
	}
	else
	{
		m_Name[0] = 0;
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
	b3StoreColor(m_Color);
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

void b3Light::b3InitValues()
{
	b3_index i;

	m_Color.b3Init(1.0,1.0,1.0);
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
	m_Spline.b3InitCurve (INIT_DEGREE,INIT_CONTROL_MAX,false);
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
	m_Knots[INIT_CONTROL_MAX] += b3Scene::epsilon;
	m_Spline.b3ThroughEndControl ();

	b3SetName("Light");
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
			m_SpotDir = m_Direction;
			m_SpotActive = b3Vector::b3Normalize(&m_SpotDir) != 0;
		}
	}
	return true;
}

void b3Light::b3Dump(b3_count level)
{
	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL,"Light %s, switched %s\n",b3GetName(),b3IsActive() ? "on" : "off");
}

char *b3Light::b3GetName()
{
	return m_Name;
}

void b3Light::b3SetName(const char *name)
{
	b3Item::b3SetString(m_Name,sizeof(m_Name),name);
}

b3_bool b3Light::b3Illuminate(
	b3Shader   *shader,
	b3_surface *surface)
{
	return (m_SoftShadow ?
		b3AreaIllumination(shader,surface) :
		b3PointIllumination(shader,surface));
}

inline b3_bool b3Light::b3PointIllumination(
	b3Shader   *shader,
	b3_surface *surface)
{
	b3_light_info Jit;
	b3_vector     point;
	b3_f64        RecLightDist,SpotAngle,q,LightDist;

	Jit.m_Distr = 1;
	Jit.m_Size  = 0;
	Jit.pos.x = surface->m_Incoming->ipoint.x;
	Jit.pos.y = surface->m_Incoming->ipoint.y;
	Jit.pos.z = surface->m_Incoming->ipoint.z;
	Jit.dir.x = m_Position.x - surface->m_Incoming->ipoint.x;
	Jit.dir.y = m_Position.y - surface->m_Incoming->ipoint.y;
	Jit.dir.z = m_Position.z - surface->m_Incoming->ipoint.z;

	LightDist    = b3Vector::b3QuadLength(&Jit.dir);
	RecLightDist = 1.0 / (LightDist = sqrt(LightDist));
	b3Vector::b3Scale(&Jit.dir, RecLightDist);

	// Compute relative brightness via LDC
	// (= light distribution curve)
	if (m_SpotActive)
	{
		SpotAngle = -b3Vector::b3SMul(&Jit.dir, &m_SpotDir);
		if (SpotAngle <= 0)
		{
			q = 1.0 - b3Scene::epsilon;
		}
		else
		{
			q = (SpotAngle >= 1 ? b3Scene::epsilon : acos(SpotAngle) * 2.0 / M_PI);
		}

		m_Spline.b3DeBoorOpened (&point,0,q);
		Jit.m_LightFrac = m_Distance * RecLightDist * point.y;
	}
	else
	{
		Jit.m_LightFrac = m_Distance * RecLightDist;
	}
	Jit.m_LightDist = RecLightDist;

	Jit.m_AmbientSum.b3Init();
	Jit.m_DiffuseSum.b3Init();
	Jit.m_SpecularSum.b3Init();

	shader->b3FindObscurer(&Jit,LightDist - b3Scene::epsilon);
	shader->b3Shade(this,&Jit,surface);

	surface->m_AmbientSum  += Jit.m_AmbientSum;
	surface->m_DiffuseSum  += Jit.m_DiffuseSum;
	surface->m_SpecularSum += Jit.m_SpecularSum;

	return true;
}

inline b3_bool b3Light::b3AreaIllumination (
	b3Shader   *shader,
	b3_surface *surface)
{
	b3_bool        Edge1, Edge2, LastEdge = false,first = true;
	b3_light_info  Jit;
	b3_vector      point;
	b3_f64         Factor,denomLightDist,q;
	b3_coord       x,y,xs;
	b3_count       max,Distr;
	b3_bool        equal;				

	Jit.m_Distr = m_JitterEdge;
	Jit.m_Size  = m_Distance * m_Size / (b3_f64)Jit.m_Distr;

	Jit.pos         = surface->m_Incoming->ipoint;
	Jit.m_LightView.x = m_Position.x - surface->m_Incoming->ipoint.x;
	Jit.m_LightView.y = m_Position.y - surface->m_Incoming->ipoint.y;
	Jit.m_LightView.z = m_Position.z - surface->m_Incoming->ipoint.z;

	// normalizing light axis
	denomLightDist = b3Vector::b3QuadLength(&Jit.m_LightView);
	denomLightDist = 1.0 / (Jit.m_LightDist = sqrt(denomLightDist));
	b3Vector::b3Scale(&Jit.m_LightView, denomLightDist);

	if (m_SpotActive)
	{
		Factor = -b3Vector::b3SMul(&Jit.m_LightView,&m_SpotDir);
		if (Factor <= 0)
		{
			q = 1.0 - b3Scene::epsilon;
		}
		else
		{
			q = (Factor >= 1 ? b3Scene::epsilon : acos(Factor) * 2.0 / M_PI);
		}

		m_Spline.b3DeBoorOpened (&point,0,q);
		Jit.m_LightFrac = denomLightDist * m_Distance * point.y;
	}
	else
	{
		Jit.m_LightFrac = denomLightDist * m_Distance;
	}

	Factor = denomLightDist / sqrt(
		Jit.m_LightView.x * Jit.m_LightView.x +
		Jit.m_LightView.y * Jit.m_LightView.y);
	Jit.m_xDir.x	= -Jit.m_LightView.y * Factor;
	Jit.m_xDir.y	=  Jit.m_LightView.x * Factor;
	Jit.m_xDir.z	=  0;

	b3Vector::b3CrossProduct(&Jit.m_LightView,&Jit.m_xDir,&Jit.m_yDir);
	Factor = denomLightDist / b3Vector::b3Length(&Jit.m_yDir);
	b3Vector::b3Scale(&Jit.m_yDir, Factor);

	Jit.m_AmbientSum.b3Init();
	Jit.m_DiffuseSum.b3Init();
	Jit.m_SpecularSum.b3Init();

	// The following loops check every second point of the outline
	// of the light raster. If every sample hits the same shape the
	// shadow computation is completed.
	equal = true;
	Distr = Jit.m_Distr - 1;
	xs    = 1;
	for (x = xs;x <= Distr;x += 2)
	{
		Edge1 = b3CheckSinglePoint (shader,surface,&Jit,x,0) != null;
		Edge2 =	b3CheckSinglePoint (shader,surface,&Jit,Distr,Distr - x) != null;

		equal   &= (Edge1 == Edge2);
		if ((x != xs) && (!first))
		{
			equal &= (Edge1 == LastEdge);
		}
		LastEdge = Edge1;
		first = false;
	}

	for (y = 2 - xs;y < Distr;y += 2)
	{
		Edge1 = b3CheckSinglePoint (shader,surface,&Jit,0,y) != null;
		Edge2 =	b3CheckSinglePoint (shader,surface,&Jit,Distr,Distr - y) != null;

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
			b3CheckSinglePoint (shader,surface,&Jit,x,0);
			b3CheckSinglePoint (shader,surface,&Jit,x,Distr);
		}

		for (y = 1;y < Distr;y++)
		{
			max = Distr + ((Jit.m_Distr + xs) & 1);
			for (x = xs;x < max;x++)
			{
				b3CheckSinglePoint (shader,surface,&Jit,x,y);
			}
			xs ^= 1;
		}

		Factor = m_FullJitter;
	}

	surface->m_AmbientSum  += Jit.m_AmbientSum  * Factor;
	surface->m_DiffuseSum  += Jit.m_DiffuseSum  * Factor;
	surface->m_SpecularSum += Jit.m_SpecularSum * Factor;

	return true;
}

inline b3Shape *b3Light::b3CheckSinglePoint (
	b3Shader      *shader,
	b3_surface    *surface,
	b3_light_info *Jit,
	b3_coord       x,
	b3_coord       y)
{
	b3_f64   jx,jy,LightDist;

	jx = ((b3_f32)x - 0.5 * Jit->m_Distr + B3_FRAN(1.0)) * Jit->m_Size;
	jy = ((b3_f32)y - 0.5 * Jit->m_Distr + B3_FRAN(1.0)) * Jit->m_Size;

	b3Vector::b3LinearCombine(&Jit->m_LightView, &Jit->m_xDir, &Jit->m_yDir, x, y, &Jit->dir);

	LightDist = b3Vector::b3Normalize(&Jit->dir);
	shader->b3FindObscurer(Jit,Jit->m_LightDist / LightDist - b3Scene::epsilon);
	shader->b3Shade(this,Jit,surface);

	return Jit->shape;
}

b3_f64 b3Light::b3ComputeSpotExponent()
{
	b3_index i,max = 20;
	b3_f64   p     = 0,angle;
	b3_bool  loop  = true;

	if (m_SpotActive)
	{
		i = 0;

		do
		{
			angle = (double)i++ / (double)max;
			loop  = b3GetSpotFactor(angle) > 0.25;
		}
		while ((i < max) && loop);
		p = - 1.0 / log10(cos(angle * 0.5 * M_PI));
		b3PrintF(B3LOG_FULL,"b3Light::b3ComputeSpotExponent(%s) = %3.2f lambda=%3.2f\n",
			b3GetName(),p,angle);
	}
	return p;
}
