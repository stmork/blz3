/*
**
**      $Filename:      b3Intersect.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Intersection routines
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/base/b3Cubic.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.38  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**
**	Revision 1.37  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.36  2003/11/25 13:31:43  sm
**	- Changed b3_loop to int (best performance)
**	
**	Revision 1.35  2003/03/22 14:38:41  sm
**	- Some optimizations continued concerning triangles.
**	
**	Revision 1.34  2003/03/20 21:04:58  sm
**	- Made some triangle intersection optimizations.
**	
**	Revision 1.33  2003/03/19 20:33:07  sm
**	- Triangles intersection optimized.
**	
**	Revision 1.32  2003/02/22 17:21:34  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.31  2003/02/17 16:57:46  sm
**	- Inlining head pointer computation.
**	
**	Revision 1.30  2003/01/18 14:13:49  sm
**	- Added move/rotate stepper operations
**	- Cleaned up resource IDs
**	
**	Revision 1.29  2003/01/06 19:16:03  sm
**	- Removed use of b3TriangleRef into an b3Array<b3_index>.
**	- Camera transformations are now matrix transformations.
**	
**	Revision 1.28  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**	
**	Revision 1.27  2002/08/03 18:05:10  sm
**	- Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**	- Moved b3PrepareInfo into b3Scene class as member. This
**	  saves memory allocation calls and is an investment into
**	  faster Lines III object transformation.
**	
**	Revision 1.26  2002/03/02 19:52:39  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.25  2002/02/28 16:58:46  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.24  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**	Revision 1.23  2002/02/19 16:26:49  sm
**	- Further CSG interval computing cleanup done.
**	
**	Revision 1.22  2002/02/18 17:50:32  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.21  2002/02/17 21:58:11  sm
**	- Done UnCR
**	- Modified makefiles
**	
**	Revision 1.20  2002/02/17 21:25:07  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.19  2002/02/14 16:32:33  sm
**	- Added activation via mouse selection
**	
**	Revision 1.18  2002/02/12 18:39:03  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.17  2001/12/23 08:57:21  sm
**	- Fixed recursive calling bug in b3IsObscured(...)
**	- Minor intersection optimazations done.
**	
**	Revision 1.16  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.15  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.14  2001/10/17 21:09:06  sm
**	- Triangle support added for intersections, normal computations. So
**	  Spline shapes can be computed, too. Now only CSG is missing.
**	
**	Revision 1.13  2001/10/17 14:46:02  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**	
**	Revision 1.12  2001/10/11 16:06:33  sm
**	- Cleaning up b3BSpline with including isolated methods.
**	- Cleaning up endian conversion routines and collecting into
**	  b3Endian
**	- Cleaning up some datatypes for proper display in Together.
**	
**	Revision 1.11  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	Revision 1.10  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.9  2001/10/05 20:30:45  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**	Revision 1.8  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**	
**	Revision 1.7  2001/10/02 16:01:58  sm
**	- Moving b3Polar into b3Ray but that's not right at all. The
**	  result must be placed there but a simple result from one
**	  intersection must be placed into a temp instance. The same
**	  must be done for surface normals as they result from using
**	  the b3Polar class.
**	
**	Revision 1.6  2001/09/30 16:27:48  sm
**	- Raytracing with diffuse color without shading
**	- Sphere intersection fixed (now using normalized rays)
**	
**	Revision 1.5  2001/09/30 15:53:19  sm
**	- Removing nasty CR/LF
**	
**	Revision 1.4  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**	
**	Revision 1.3  2001/09/23 15:37:15  sm
**	- Introducing raytracing for Lines III. There is much work
**	  for a b3Display-CScrollView.
**	
**	Revision 1.2  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.1  2001/09/22 16:19:52  sm
**	- Adding basic shape intersection routines
**	
*/

/*************************************************************************
**                                                                      **
**                        Normal shape intersections                    **
**                                                                      **
*************************************************************************/

b3_f64 b3SimpleShape::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	return -1;
}

b3_f64 b3Area::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_f64    Denom,aValue,bValue,lValue,result = -1;
	b3_vector Dir,Product;
 
	if ((Denom =
		-m_Normal.x * ray->dir.x
		-m_Normal.y * ray->dir.y
		-m_Normal.z * ray->dir.z) != 0)
	{
		Denom = 1 / Denom;
		if (((lValue = (
			m_Normal.x * (Dir.x = ray->pos.x - m_Base.x) +
			m_Normal.y * (Dir.y = ray->pos.y - m_Base.y) +
			m_Normal.z * (Dir.z = ray->pos.z - m_Base.z))
				* Denom) >= b3Scene::epsilon) && (lValue < ray->Q))
		{
			aValue = (
				m_Dir2.x * (Product.x = Dir.y * ray->dir.z - Dir.z * ray->dir.y) +
				m_Dir2.y * (Product.y = Dir.z * ray->dir.x - Dir.x * ray->dir.z) +
				m_Dir2.z * (Product.z = Dir.x * ray->dir.y - Dir.y * ray->dir.x))
					* Denom;
			if (fabs(aValue) <= 1)
			{
				bValue = (
					-m_Dir1.x * Product.x
					-m_Dir1.y * Product.y
					-m_Dir1.z * Product.z) * Denom;
				if (fabs(bValue) <= 1)
				{
					polar->m_Polar.x = polar->m_ObjectPolar.x = aValue;
					polar->m_Polar.y = polar->m_ObjectPolar.y = bValue;
					polar->m_Polar.z = polar->m_ObjectPolar.z = 0;

					if(b3CheckStencil(polar))
					{
						result = lValue;
					}
				}
			}
		}
	}
	return result;
}

b3_f64 b3Disk::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_f64     Denom,aValue,bValue,lValue;
	b3_vector  Dir,Product;
 
	if ((Denom =
		-m_Normal.x * ray->dir.x
		-m_Normal.y * ray->dir.y
		-m_Normal.z * ray->dir.z) == 0)
	{
		return -1;
	}
	Denom = 1 / Denom;

	// check distance
	if ((lValue = (
		m_Normal.x * (Dir.x = ray->pos.x - m_Base.x) +
		m_Normal.y * (Dir.y = ray->pos.y - m_Base.y) +
		m_Normal.z * (Dir.z = ray->pos.z - m_Base.z))
			* Denom) < b3Scene::epsilon)
	{
		return -1;
	}
	if (lValue > ray->Q)
	{
		return -1;
	}

	// check x axis
	aValue = (
		m_Dir2.x * (Product.x = Dir.y * ray->dir.z - Dir.z * ray->dir.y) +
		m_Dir2.y * (Product.y = Dir.z * ray->dir.x - Dir.x * ray->dir.z) +
		m_Dir2.z * (Product.z = Dir.x * ray->dir.y - Dir.y * ray->dir.x))
			* Denom;
	if (fabs(aValue) > 1)
	{
		return -1;
	}

	// check y axis
	bValue = (
		-m_Dir1.x * Product.x
		-m_Dir1.y * Product.y
		-m_Dir1.z * Product.z) * Denom;
	if (fabs(bValue) > 1)
	{
		return -1;
	}

	// check radius
	Denom = aValue * aValue + bValue * bValue;
	if (Denom > 1)
	{
		return -1;
	}

	// compute surface coordinates
	polar->m_Polar.x	= b3RelAngleOfScalars(aValue,bValue);
	polar->m_Polar.y	= sqrt (Denom);
	polar->m_Polar.z	= 0;
	polar->m_ObjectPolar.x = aValue;
	polar->m_ObjectPolar.y = bValue;
	polar->m_ObjectPolar.z = 0;

	return b3CheckStencil(polar) ? lValue : -1;
}

b3_f64 b3Sphere::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_f64    l1,Discriminant,p,l2;
	b3_f64    xDiff,yDiff,zDiff;
	b3_vector n,aux,pole;

	p = ray->dir.x * (xDiff = ray->pos.x - m_Base.x) +
	    ray->dir.y * (yDiff = ray->pos.y - m_Base.y) +
	    ray->dir.z * (zDiff = ray->pos.z - m_Base.z);
	if ((Discriminant = p * p + m_QuadRadius -
		xDiff * xDiff -
		yDiff * yDiff -
		zDiff * zDiff) < 0)
	{
		return -1;
	}

	Discriminant = sqrt(Discriminant);

	// l1 <= l2, always!
	l1 = -p - Discriminant;
	l2 = -p + Discriminant;

	// check against limit
	if (l1  < b3Scene::epsilon) l1 = -2;
	if (l2  < b3Scene::epsilon) l2 = -1;
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	// now compute "north pole" - radius is point on equator
	if ((m_Dir.x == 0) && (m_Dir.y == 0))
	{
		// special case: point on equator is north pole
		aux.x   = m_Dir.z;
		aux.y   = 0;
		aux.z   = 0;
		pole.x  = 0;
		pole.y  = m_Dir.z;
		pole.z  = 0;
	}
	else
	{
		aux.x  = -m_Dir.y;
		aux.y  =  m_Dir.x;
		aux.z  =  0;
		b3Vector::b3CrossProduct(&m_Dir,&aux,&pole);
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		n.x = ray->pos.x + l1 * ray->dir.x - m_Base.x;
		n.y = ray->pos.y + l1 * ray->dir.y - m_Base.y;
		n.z = ray->pos.z + l1 * ray->dir.z - m_Base.z;

		p = acos(-b3Vector::b3AngleOfVectors(&n,&pole));
		polar->m_Polar.x = acos(b3Vector::b3AngleOfVectors (&m_Dir,&n) /
			sin(p)) * 0.5 / M_PI;
		polar->m_Polar.y = p * 2.0 / M_PI - 1.0;
		polar->m_Polar.z = 0;
		if (b3Vector::b3AngleOfVectors(&aux,&n) < 0)
		{
			polar->m_Polar.x = 1.0 - polar->m_Polar.x;
		}

		if (b3CheckStencil(polar))
		{
			p = 1.0 / sqrt(m_QuadRadius);
			polar->m_ObjectPolar.x = n.x * p;
			polar->m_ObjectPolar.y = n.y * p;
			polar->m_ObjectPolar.z = n.z * p;
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		n.x = ray->pos.x + l2 * ray->dir.x - m_Base.x;
		n.y = ray->pos.y + l2 * ray->dir.y - m_Base.y;
		n.z = ray->pos.z + l2 * ray->dir.z - m_Base.z;

		p = acos(-b3Vector::b3AngleOfVectors(&n,&pole));
		polar->m_Polar.x = acos(b3Vector::b3AngleOfVectors (&m_Dir,&n) /
			sin(p)) * 0.5 / M_PI;
		polar->m_Polar.y = p * 2.0 / M_PI - 1.0;
		polar->m_Polar.z = 0;
		if (b3Vector::b3AngleOfVectors(&aux,&n) < 0)
		{
			polar->m_Polar.x = 1.0 - polar->m_Polar.x;
		}

		if (b3CheckStencil(polar))
		{
			p = 1.0 / sqrt(m_QuadRadius);
			polar->m_ObjectPolar.x = n.x * p;
			polar->m_ObjectPolar.y = n.y * p;
			polar->m_ObjectPolar.z = n.z * p;
			return l2;
		}
	}

	return l2;
}

b3_f64 b3Cylinder::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_line64 BTLine;
	b3_f64    l1,l2,z,Discriminant,a,p;

	b3BaseTrans (ray,&BTLine);
	if ((a =
		BTLine.dir.x * BTLine.dir.x +
		BTLine.dir.y * BTLine.dir.y) == 0)
	{
		return -1;
	}
	a = 1.0 / a;

	p = (
		BTLine.dir.x * BTLine.pos.x +
		BTLine.dir.y * BTLine.pos.y) * a;
	if ((Discriminant =	p * p - (
		BTLine.pos.x * BTLine.pos.x +
		BTLine.pos.y * BTLine.pos.y - 1) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -p - z;
	l2 = -p + z;

	if (l1 < b3Scene::epsilon)
	{
		l1 = -2;
	}
	else
	{
		z = BTLine.pos.z + l1 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l1 = -2;
		}
 	}
	if (l2 < b3Scene::epsilon)
	{
		l2 = -1;
	}
	else
	{
		z = BTLine.pos.z + l2 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l2 = -1;
		}
	}

	// Not found any intersection
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		polar->m_ObjectPolar.x = BTLine.pos.x + l1 * BTLine.dir.x;
		polar->m_ObjectPolar.y = BTLine.pos.y + l1 * BTLine.dir.y;
		polar->m_ObjectPolar.z = BTLine.pos.z + l1 * BTLine.dir.z;

		polar->m_Polar.x = b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y = polar->m_ObjectPolar.z;
		polar->m_Polar.z = 0;
		if (b3CheckStencil(polar))
		{
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		polar->m_ObjectPolar.x = BTLine.pos.x + l2 * BTLine.dir.x;
		polar->m_ObjectPolar.y = BTLine.pos.y + l2 * BTLine.dir.y;
		polar->m_ObjectPolar.z = BTLine.pos.z + l2 * BTLine.dir.z;

		polar->m_Polar.x	= b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y	= polar->m_ObjectPolar.z;
		polar->m_Polar.z	= 0;
		if (!b3CheckStencil(polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Cone::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_line64 BTLine;
	b3_f64    l1,l2,z,Discriminant,a,p;

	b3BaseTrans (ray,&BTLine);
	if ((a =
		BTLine.dir.x * BTLine.dir.x +
		BTLine.dir.y * BTLine.dir.y -
		BTLine.dir.z * BTLine.dir.z) == 0)
	{
		return -1;
	}

	a = 1.0 / a;
	p = (
		BTLine.dir.x  *      BTLine.pos.x +
		BTLine.dir.y  *      BTLine.pos.y +
		BTLine.dir.z  * (1 - BTLine.pos.z)) * a;
	if ((Discriminant = p * p - (
		   BTLine.pos.x  *    BTLine.pos.x +
		   BTLine.pos.y  *    BTLine.pos.y -
		(1-BTLine.pos.z) * (1-BTLine.pos.z)) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -p - z;
	l2 = -p + z;


	if (l1 < b3Scene::epsilon)
	{
		l1 = -2;
	}
	else
	{
		z = BTLine.pos.z + l1 * BTLine.dir.z;
		if ((z < 0) || (z > 1))
		{
			l1 = -2;
		}
 	}
	if (l2 < b3Scene::epsilon)
	{
		l2 = -1;
	}
	else
	{
		z = BTLine.pos.z + l2 * BTLine.dir.z;
		if ((z < 0)||(z > 1))
		{
			l2 = -1;
		}
	}
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		polar->m_ObjectPolar.x = BTLine.pos.x + l1 * BTLine.dir.x;
		polar->m_ObjectPolar.y = BTLine.pos.y + l1 * BTLine.dir.y;
		polar->m_ObjectPolar.z = BTLine.pos.z + l1 * BTLine.dir.z;

		polar->m_Polar.x	= b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y	= polar->m_ObjectPolar.z;
		polar->m_Polar.z	= 0;
		if (b3CheckStencil(polar))
		{
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		polar->m_ObjectPolar.x = BTLine.pos.x + l2 * BTLine.dir.x;
		polar->m_ObjectPolar.y = BTLine.pos.y + l2 * BTLine.dir.y;
		polar->m_ObjectPolar.z = BTLine.pos.z + l2 * BTLine.dir.z;
		polar->m_Polar.x	= b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y	= polar->m_ObjectPolar.z;
		polar->m_Polar.z	= 0;
		if (!b3CheckStencil(polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Ellipsoid::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_line64  BTLine;
	b3_f64     l1,l2,z,Discriminant,a,p;

	b3BaseTrans (ray,&BTLine);
	a = BTLine.dir.x * BTLine.dir.x +
		BTLine.dir.y * BTLine.dir.y +
		BTLine.dir.z * BTLine.dir.z;
	if (a == 0)
	{
		return -1;
	}

	a = 1 / a;
	p = (BTLine.dir.x * BTLine.pos.x +
		 BTLine.dir.y * BTLine.pos.y +
		 BTLine.dir.z * BTLine.pos.z) * a;
	if ((Discriminant = p * p -
		(BTLine.pos.x * BTLine.pos.x +
		 BTLine.pos.y * BTLine.pos.y +
		 BTLine.pos.z * BTLine.pos.z - 1) * a) < 0)
	{
		return -1;
	}
	z  = sqrt(Discriminant);
	l1 = -z - p;
	l2 =  z - p;

	if (l1  < b3Scene::epsilon) l1 = -2;
	if (l2  < b3Scene::epsilon) l2 = -1;
	if ((l1 < 0) && (l2 < 0))
	{
		return -3;
	}

	if (l1 > 0)
	{
		if (l1 > ray->Q)
		{
			return -2;
		}
		polar->m_ObjectPolar.x = BTLine.pos.x + l1 * BTLine.dir.x;
		polar->m_ObjectPolar.y = BTLine.pos.y + l1 * BTLine.dir.y;
		polar->m_ObjectPolar.z = BTLine.pos.z + l1 * BTLine.dir.z;

		polar->m_Polar.x	= b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y	= asin(polar->m_ObjectPolar.z) * 2.0 / M_PI;
		polar->m_Polar.z	= 0;
		if (b3CheckStencil(polar))
		{
			return l1;
		}
	}

	if (l2 > 0)
	{
		if (l2 > ray->Q)
		{
			return -1;
		}
		polar->m_ObjectPolar.x = BTLine.pos.x + l2 * BTLine.dir.x;
		polar->m_ObjectPolar.y = BTLine.pos.y + l2 * BTLine.dir.y;
		polar->m_ObjectPolar.z = BTLine.pos.z + l2 * BTLine.dir.z;

		polar->m_Polar.x	= b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y	= asin(polar->m_ObjectPolar.z) * 2.0 / M_PI;
		polar->m_Polar.z	= 0;
		if (!b3CheckStencil(polar))
		{
			l2 = -1;
		}
	}
	return l2;
}

b3_f64 b3Box::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_line64    BTLine;
	b3_vector64  BasePoint,EndPoint;
	b3_f64       l[2];
	b3_f64       x,y,z,m,l1;
	b3_index     Index;
	b3_index     n[2];

	Index = n[0] = n[1] = 0;
	b3BaseTrans (ray,&BTLine);

	EndPoint.x = (BasePoint.x = -BTLine.pos.x) + 1;
	EndPoint.y = (BasePoint.y = -BTLine.pos.y) + 1;
	EndPoint.z = (BasePoint.z = -BTLine.pos.z) + 1;

	if (BTLine.dir.x != 0)
	{
		if ((m = BasePoint.x / BTLine.dir.x) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			z = m * BTLine.dir.z;
			if ((y >= BasePoint.y) &&
			    (y <= EndPoint.y)  &&
			    (z >= BasePoint.z) &&
				(z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 0;
				Index++;
			}
		}

		if ((m = EndPoint.x / BTLine.dir.x) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			z = m * BTLine.dir.z;
			if ((y >= BasePoint.y) &&
			    (y <= EndPoint.y)  &&
			    (z >= BasePoint.z) &&
			    (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 0;
				Index++;
			}
		}
	}

	if (BTLine.dir.y != 0)
	{
		if ((m = BasePoint.y / BTLine.dir.y) >= b3Scene::epsilon)
		{
			x = m * BTLine.dir.x;
			z = m * BTLine.dir.z;
			if ((x >= BasePoint.x) &&
			    (x <= EndPoint.x)  &&
			    (z >= BasePoint.z) &&
			    (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 1;
				Index++;
			}
		}

		if ((m = EndPoint.y / BTLine.dir.y) >= b3Scene::epsilon)
		{
			x = m * BTLine.dir.x;
			z = m * BTLine.dir.z;
			if ((x >= BasePoint.x) &&
			    (x <= EndPoint.x)  &&
			    (z >= BasePoint.z) &&
			    (z <= EndPoint.z))
			{
				l[Index] = m;
				n[Index] = 1;
				Index++;
			}
		}
	}

	if (BTLine.dir.z != 0)
	{
		if ((m = BasePoint.z / BTLine.dir.z) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			x = m * BTLine.dir.x;
			if ((x >= BasePoint.x) &&
			    (x <= EndPoint.x)  &&
			    (y >= BasePoint.y) &&
			    (y <= EndPoint.y))
			{
				l[Index] = m;
				n[Index] = 2;
				Index++;
			}
		}

		if ((m = EndPoint.z / BTLine.dir.z) >= b3Scene::epsilon)
		{
			y = m * BTLine.dir.y;
			x = m * BTLine.dir.x;
			if ((x >= BasePoint.x) &&
			    (x <= EndPoint.x)  &&
			    (y >= BasePoint.y) &&
			    (y <= EndPoint.y))
			{
				l[Index] = m;
				n[Index] = 2;
				Index++;
			}
		}
	}

	switch (Index)
	{
	case 0:
		// No result
		return -1;
		
	case 1:
		// One result
		l1 = l[0];
		break;
		
	case 2:
		if (l[1] < l[0])
		{
			l1   = l[1];
			n[0] = n[1];
		}
		else
		{
			l1 = l[0];
		}
		break;

	default:
		// Shouldn't be!
		B3_ASSERT(false);
		return -1;
	}

	x = BTLine.pos.x + l1 * BTLine.dir.x;
	y = BTLine.pos.y + l1 * BTLine.dir.y;
	if (x < 0)               x = 0;
	else if (x >= 0.9999999) x = 0.9999999;
	if (y < 0)               y = 0;
	else if (y >= 0.9999999) y = 0.9999999;

	polar->m_Polar.x = polar->m_ObjectPolar.x = x;
	polar->m_Polar.y = polar->m_ObjectPolar.y = y;
	polar->m_Polar.z = polar->m_ObjectPolar.z = BTLine.pos.z + l1 * BTLine.dir.z;
	polar->m_NormalIndex = n[0];

	return l1;
}

b3_f64 b3Torus::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_loop   NumOfX,i,k;
	b3_f64    Val1,Val2,pQuad,dQuad,pdQuad;
	b3_f64    xp,yp;
	b3_f64    Coeff[5],x[4];
	b3_line64 BTLine;

	b3BaseTrans (ray,&BTLine);
	pQuad	= BTLine.pos.z * BTLine.pos.z;
	dQuad	= BTLine.dir.z * BTLine.dir.z;
	pdQuad	= BTLine.pos.z * BTLine.dir.z;
	Val1	= BTLine.pos.x * BTLine.pos.x +
	          BTLine.pos.y * BTLine.pos.y + pQuad - m_aQuad - m_bQuad;
	Val2	= BTLine.pos.x * BTLine.dir.x +
	          BTLine.pos.y * BTLine.dir.y + pdQuad;

	Coeff[4]	= 1;
	Coeff[3]	= 4 *  Val2;
	Coeff[2]	= 2 * (Val1        + 2 * Val2    * Val2 + 2 * m_aQuad * dQuad);
	Coeff[1]	= 4 * (Val1 * Val2 + 2 * m_aQuad * pdQuad);
	Coeff[0]	=      Val1 * Val1 + 4 * m_aQuad * (pQuad - m_bQuad);

	if ((NumOfX = b3Cubic::b3SolveOrd4 (Coeff,x)) == 0)
	{
		return -1;
	}
	for (i = 0;i < NumOfX;)
	{
		if ((x[i] > b3Scene::epsilon) && (x[i] < ray->Q)) i++;
		else x[i] = x[--NumOfX];
	}

	for (i = 0;i < NumOfX;i++)
	{
		Val1 = x[i];
		for (k = NumOfX - 1;k > i;k--)
		{
			if (x[k] < Val1)
			{
				Val2 = x[k];
				x[k] = Val1;
				Val1 = Val2;
			}
		}
		xp = polar->m_ObjectPolar.x = BTLine.pos.x + Val1 * BTLine.dir.x;
		yp = polar->m_ObjectPolar.y = BTLine.pos.y + Val1 * BTLine.dir.y;
		     polar->m_ObjectPolar.z = BTLine.pos.z + Val1 * BTLine.dir.z;

		Val2 = m_aRad - m_aQuad / sqrt(xp * xp + yp * yp);
		polar->m_Polar.x = b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
		polar->m_Polar.y = b3RelAngleOfScalars(
			Val2,
			polar->m_ObjectPolar.z);
		polar->m_Polar.z = 0;

		if (b3CheckStencil (polar))
		{
			return Val1;
		}
	}
	return -1;
}

#define INCREMENT(x)  ((x) >= 0 ? 1 : -1)
#define FSWAP(a,b,m)  { m = a; a = b; b = m; }

#define _TEST

b3_f64 b3TriangleShape::b3IntersectTriangleList (
	b3_ray                *ray,
	b3_polar_precompute   *polar,
	b3_index               TriaField)
{
	b3_triangle    *Triangle;
	b3_index        Index,i,max;
	b3_index       *buffer;
	b3_triainfo    *infos,*info;
	b3_res          dxSize;
	b3_vector32     aux,product;
	b3_vector32     dir,pos;
	b3_f32          denominator;
	b3_f32          nominator;
	b3_f32          aValue,bValue;
	b3_f32          e = b3Scene::epsilon;
	b3_f32          OldValue = -1,lValue;

	 xSize   = m_xSize;
	 ySize   = m_ySize;
	dxSize   = m_xSize << 1;

	pos.x    = ray->pos.x;
	pos.y    = ray->pos.y;
	pos.z    = ray->pos.z;

	dir.x    = ray->dir.x;
	dir.y    = ray->dir.y;
	dir.z    = ray->dir.z;

	B3_ASSERT(TriaField < m_GridCount);
	infos  = m_TriaInfos.b3GetBuffer();
	buffer = m_GridList[TriaField].b3GetBuffer();
	max    = m_GridList[TriaField].b3GetCount();
	for (i = 0;i < max;i++)
	{
		Index    = buffer[i];
		Triangle = &m_Triangles[Index];
		info     = &infos[Index];

		if ((nominator = 
			info->Normal.x * ray->dir.x +
			info->Normal.y * ray->dir.y +
			info->Normal.z * ray->dir.z) != 0)
		{
			lValue = (denominator = -1.0 / nominator) * (
				info->Normal.x * (aux.x = pos.x - info->O.x) +
				info->Normal.y * (aux.y = pos.y - info->O.y) +
				info->Normal.z * (aux.z = pos.z - info->O.z));
			if ((lValue >= e) && (lValue < ray->Q))
			{
				if ((aValue = denominator * (
					info->R2.x * (product.x = aux.y * dir.z - aux.z * dir.y) +
					info->R2.y * (product.y = aux.z * dir.x - aux.x * dir.z) +
					info->R2.z * (product.z = aux.x * dir.y - aux.y * dir.x))) >= 0)
				{
					bValue = -denominator * (
						info->R1.x * product.x +
						info->R1.y * product.y +
						info->R1.z * product.z);
					if ((bValue >= 0) && ((aValue + bValue) <= 1))
					{
						if (Index & 1)
						{
							polar->m_Polar.x =
								((((Index % dxSize) >> 1) + 1) - aValue) / xSize;
							polar->m_Polar.y =
								(  (Index / dxSize        + 1) - bValue) / ySize;
						}
						else
						{
							polar->m_Polar.x =
								(((Index % dxSize) >> 1) + aValue) / xSize;
							polar->m_Polar.y =
								  (Index / dxSize        + bValue) / ySize;
						}
						polar->m_Polar.z      = 0;
						polar->m_ObjectPolar = polar->m_Polar;
						if (b3CheckStencil (polar))
						{
							ray->Q = OldValue = lValue;
							ray->TriaIndex    = Index;
							ray->aTriaValue   = aValue;
							ray->bTriaValue   = bValue;
						}
					}
				}
			}
		}
	}
	return OldValue;
}

b3_f64 b3TriangleShape::b3Intersect(b3_ray *ray,b3_polar_precompute *polar)
{
	b3_index     gx,gy,gz,sx,sy,sz,index;
	b3_f64       start,end,tn,tf,m,result = -1;
	b3_vector64  pos;
	b3_vector64  d;
	b3_vector64  denom;
	b3_vector64  dmax;
#ifdef _DEBUG
	b3_count     GridMax;
#endif

	B3_ASSERT (m_GridList != null);

	if (m_GridSize <= 1)
	{
		return b3IntersectTriangleList (ray,polar,0);
	}
#ifdef _DEBUG
	GridMax = m_GridSize * m_GridSize * m_GridSize;
#endif

	denom.x = 1.0 / ray->dir.x;
	pos.x   = (ray->pos.x - m_Base.x);
	start   = (                      - pos.x) * denom.x;
	end     = (m_GridSize * m_Size.x - pos.x) * denom.x;
	if (start > end) FSWAP(start,end,m);

	denom.y = 1.0 / ray->dir.y;
	pos.y   = (ray->pos.y - m_Base.y);
	tn      = (                      - pos.y) * denom.y;
	tf      = (m_GridSize * m_Size.y - pos.y) * denom.y;
	if (tn    > tf)    FSWAP(tn,tf,m);
	if (tn    > start) start = tn;
	if (tf    < end)   end   = tf;
	if (start > end) return result;

	denom.z = 1.0 / ray->dir.z;
	pos.z   = (ray->pos.z - m_Base.z);
	tn      = (                      - pos.z) * denom.z;
	tf      = (m_GridSize * m_Size.z - pos.z) * denom.z;
	if (tn    > tf)    FSWAP(tn,tf,m);
	if (tn    > start) start = tn;
	if (tf    < end)   end   = tf;
	if (start > end) return result;

	if (end   < 0)      return result;
	if (start > ray->Q) return result;

	if (start < 0)   start = 0;
	end    -= b3Scene::epsilon;
	if (ray->Q < end) end = ray->Q;


	// start position
	pos.x = (pos.x + start * ray->dir.x) / m_Size.x;
	pos.y = (pos.y + start * ray->dir.y) / m_Size.y;
	pos.z = (pos.z + start * ray->dir.z) / m_Size.z;


	// start indizes
	gx    = (b3_index)pos.x;
	gy    = (b3_index)pos.y;
	gz    = (b3_index)pos.z;


	// compute deltas
	dmax.x = fabs(m_Size.x * denom.x);
	dmax.y = fabs(m_Size.y * denom.y);
	dmax.z = fabs(m_Size.z * denom.z);
	d.x    = (pos.x - gx);
	d.y    = (pos.y - gy);
	d.z    = (pos.z - gz);


	// correct start point
	if (d.x > b3Scene::epsilon)
	{
		if (ray->dir.x >= 0) d.x = (1.0 - d.x) * dmax.x;
		else                 d.x *= dmax.x;
	}
	else
	{
		d.x = dmax.x;
		if (gx == m_GridSize) gx--;
	}
	if (d.y > b3Scene::epsilon)
	{
		if (ray->dir.y >= 0) d.y = (1.0 - d.y) * dmax.y;
		else                  d.y *= dmax.y;
	}
	else
	{
		d.y = dmax.y;
		if (gy == m_GridSize) gy--;
	}
	if (d.z > b3Scene::epsilon)
	{
		if (ray->dir.z >= 0) d.z = (1.0 - d.z) * dmax.z;
		else                  d.z *= dmax.z;
	}
	else
	{
		d.z = dmax.z;
		if (gz == m_GridSize) gz--;
	}


		/* compute increments */
	sx  = INCREMENT(ray->dir.x);
	sy  = INCREMENT(ray->dir.y);
	sz  = INCREMENT(ray->dir.z);

	do
	{
		index = GRID_INDEX(gx,gy,gz,m_GridSize);
#ifdef _DEBUG
		if (index >= GridMax)
		{
			b3PrintF (B3LOG_NORMAL,"index error: (%2ld,%2ld,%2ld) = %ld\n",
				gx,gy,gz,index);
		}
		else
#endif
		{
			if (m_GridList[index].b3GetCount() > 0)
			{
				m = b3IntersectTriangleList (ray,polar,index);
				if (m > 0)
				{
					result = m;  /* ray->Q is modified by IntersectTriangleList */
					if (ray->Q < end)
					{
						end = ray->Q;
					}
				}
			}
			if (start >= end) return result;
		}

		if      ((d.x <= d.y) && (d.x <= d.z))
		{
			gx    +=     sx;
			if ((gx < 0) || (gx >= m_GridSize)) return result;
			start +=    d.x;
			d.y   -=    d.x;
			d.z   -=    d.x;
			d.x    = dmax.x;
		}
		else if ((d.y <= d.x) && (d.y <= d.z))
		{
			gy    +=     sy;
			if ((gy < 0) || (gy >= m_GridSize)) return result;
			start +=    d.y;
			d.x   -=    d.y;
			d.z   -=    d.y;
			d.y    = dmax.y;
		}
		else if ((d.z <= d.x) && (d.z <= d.y))
		{
			gz    +=     sz;
			if ((gz < 0) || (gz >= m_GridSize)) return result;
			start +=    d.z;
			d.x   -=    d.z;
			d.y   -=    d.z;
			d.z    = dmax.z;
		}
		else B3_BEEP;
	}
	while (true);

	return result;
}

/*************************************************************************
**                                                                      **
**                        CSG intersections                             **
**                                                                      **
*************************************************************************/

b3_bool b3CSGShape::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	return false;
}

b3_bool b3CSGSphere::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	b3_f64 Discriminant,p;
	b3_f64 xDiff,yDiff,zDiff;

	p = ray->dir.x * (xDiff = ray->pos.x - m_Base.x) +
	    ray->dir.y * (yDiff = ray->pos.y - m_Base.y) +
	    ray->dir.z * (zDiff = ray->pos.z - m_Base.z);
	if ((Discriminant = p * p + m_QuadRadius -
		xDiff * xDiff -
		yDiff * yDiff -
		zDiff * zDiff) >= 0)
	{
		Discriminant = sqrt(Discriminant);

		interval->m_x[0].m_Q     = -p - Discriminant;
		interval->m_x[0].m_Shape =  this;
		interval->m_x[1].m_Q     = -p + Discriminant;
		interval->m_x[1].m_Shape =  this;
		interval->m_Count        = 2;
	}
	else
	{
		interval->m_Count = 0;
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGCylinder::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	register   double l1,l2,z,Discriminant,a,p,x,y;
	register   bool   check;

	interval->m_Count = 0;

	b3BaseTrans (ray,BTLine);

	// Compute normal cylinder intersection
	a = BTLine->dir.x * BTLine->dir.x +
	    BTLine->dir.y * BTLine->dir.y;
	if (a != 0)
	{
		a = 1 / a;
		p = (BTLine->dir.x * BTLine->pos.x +
			 BTLine->dir.y * BTLine->pos.y) * a;
		if ((Discriminant = p * p - (
			BTLine->pos.x * BTLine->pos.x +
			BTLine->pos.y * BTLine->pos.y - 1) * a) >= 0)
		{
			z  = sqrt(Discriminant);
			l1 = -p - z;
			l2 = -p + z;

			// check near intersection
			z = BTLine->pos.z + l1 * BTLine->dir.z;
			if (z < 0)
			{
				l1 =  -BTLine->pos.z      / BTLine->dir.z;
				x  =   BTLine->pos.x + l1 * BTLine->dir.x;
				y  =   BTLine->pos.y + l1 * BTLine->dir.y;
				check = (x * x + y * y) <= 1;
				interval->m_x[0].m_Index = B3_CSG_BOTTOM;
			}
			else
			{
				if (z > 1)
				{
					l1 = (-BTLine->pos.z + 1) / BTLine->dir.z;
					x  =   BTLine->pos.x + l1 * BTLine->dir.x;
					y  =   BTLine->pos.y + l1 * BTLine->dir.y;
					check = (x * x + y * y) <= 1;
					interval->m_x[0].m_Index = B3_CSG_TOP;
				}
				else
				{
					check = true;
					interval->m_x[0].m_Index = B3_CSG_NORMAL;
				}
			}
			if (check)
			{
				interval->m_x[0].m_Q       = l1;
				interval->m_x[0].m_Shape   = this;
				interval->m_x[0].m_BTLine = BTLine;
				interval->m_Count++;
			}


			// check far intersection
			z = BTLine->pos.z + l2 * BTLine->dir.z;
			if (z < 0)
			{
				l2 =  -BTLine->pos.z      / BTLine->dir.z;
				x  =   BTLine->pos.x + l2 * BTLine->dir.x;
				y  =   BTLine->pos.y + l2 * BTLine->dir.y;
				check = (x * x + y * y) <= 1;
				interval->m_x[interval->m_Count].m_Index = B3_CSG_BOTTOM;	/* Zylinderboden */
			}
			else
			{
				if (z > 1)
				{
					l2 = (-BTLine->pos.z + 1) / BTLine->dir.z;
					x  =   BTLine->pos.x + l2 * BTLine->dir.x;
					y  =   BTLine->pos.y + l2 * BTLine->dir.y;
					check = (x * x + y * y) <= 1;
					interval->m_x[interval->m_Count].m_Index = B3_CSG_TOP;	/* Zylinderdeckel */
				}
				else
				{
					check = true;
					interval->m_x[interval->m_Count].m_Index = B3_CSG_NORMAL;	/* Zylindermantel */
				}
			}
			if (check)
			{
				interval->m_x[interval->m_Count].m_Q       = l2;
				interval->m_x[interval->m_Count].m_Shape   = this;
				interval->m_x[interval->m_Count].m_BTLine = BTLine;
				interval->m_Count++;
			}
			B3_ASSERT(interval->m_Count != 1);
		}
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGCone::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	b3_f64     l1,l2,z1,z2,Discriminant,a,p,x,y;
	b3_index   Index = 0;

	interval->m_Count = 0;
	b3BaseTrans (ray,BTLine);
	a = BTLine->dir.x * BTLine->dir.x +
		BTLine->dir.y * BTLine->dir.y -
		BTLine->dir.z * BTLine->dir.z;
	if (a != 0)
	{
		a = 1 / a;
		p = (
			BTLine->dir.x  *      BTLine->pos.x +
			BTLine->dir.y  *      BTLine->pos.y +
			BTLine->dir.z  * (1 - BTLine->pos.z)) * a;
		if ((Discriminant = p * p - (
			      BTLine->pos.x  *      BTLine->pos.x +
				  BTLine->pos.y  *      BTLine->pos.y -
			 (1 - BTLine->pos.z) * (1 - BTLine->pos.z)) * a) >= 0)
		{
			Discriminant = sqrt(Discriminant);
			z1           = BTLine->pos.z + (l1 = -p - Discriminant) * BTLine->dir.z;
			z2           = BTLine->pos.z + (l2 = -p + Discriminant) * BTLine->dir.z;

			if ((z1 >= 0) && (z1 <= 1))
			{
				interval->m_x[Index].m_Q      = l1;
				interval->m_x[Index].m_Shape  = this;
				interval->m_x[Index].m_BTLine = BTLine;
				interval->m_x[Index].m_Index  = B3_CSG_NORMAL;
				Index++;
			}

			if ((z2 >= 0) && (z2 <= 1))
			{
				interval->m_x[Index].m_Q      = l2;
				interval->m_x[Index].m_Shape  = this;
				interval->m_x[Index].m_BTLine = BTLine;
				interval->m_x[Index].m_Index  = B3_CSG_NORMAL;
				Index++;
			}

			if (Index < 2)
			{
				l1 = -BTLine->pos.z /      BTLine->dir.z;
				x  =  BTLine->pos.x + l1 * BTLine->dir.x;
				y  =  BTLine->pos.y + l1 * BTLine->dir.y;
				if ((x * x + y * y) <= 1)
				{
					if (l1 < interval->m_x[0].m_Q)
					{
						interval->m_x[1] = interval->m_x[0];
						interval->m_x[0].m_Q      = l1;
						interval->m_x[0].m_Index  = B3_CSG_BOTTOM;
					}
					else
		            {
						interval->m_x[1].m_Q      = l1;
						interval->m_x[1].m_Index  = B3_CSG_BOTTOM;
						interval->m_x[1].m_Shape  = this;
						interval->m_x[1].m_BTLine = BTLine;
					}

					Index++;
				}
			}
			interval->m_Count = Index;
		}
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGEllipsoid::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	b3_f64  z,Discriminant,a,p;

	interval->m_Count = 0;
	b3BaseTrans (ray,BTLine);
	a = BTLine->dir.x * BTLine->dir.x +
		BTLine->dir.y * BTLine->dir.y +
		BTLine->dir.z * BTLine->dir.z;
	if (a != 0)
	{
		a = 1 / a;
		p = (BTLine->dir.x * BTLine->pos.x +
			 BTLine->dir.y * BTLine->pos.y +
			 BTLine->dir.z * BTLine->pos.z) * a;
		if ((Discriminant = p * p -
			(BTLine->pos.x * BTLine->pos.x +
			 BTLine->pos.y * BTLine->pos.y +
			 BTLine->pos.z * BTLine->pos.z - 1) * a) >= 0)
		{
			z  = sqrt(Discriminant);

			interval->m_x[0].m_Q      = -z - p;
			interval->m_x[0].m_Shape  = this;
			interval->m_x[0].m_BTLine = BTLine;
			interval->m_x[1].m_Q      =  z - p;
			interval->m_x[1].m_Shape  = this;
			interval->m_x[1].m_BTLine = BTLine;
			interval->m_Count         = 2;
		}
	}
	return interval->m_Count > 0;
}

b3_bool b3CSGBox::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	b3_vector64   BasePoint,EndPoint;
	b3_f64        l[2];
	b3_csg_index  n[2];
	b3_f64        x,y,z,m;
	b3_index      Index = 0;

	interval->m_Count = 0;
	b3BaseTrans (ray,BTLine);

	EndPoint.x = (BasePoint.x = -BTLine->pos.x) + 1;
	EndPoint.y = (BasePoint.y = -BTLine->pos.y) + 1;
	EndPoint.z = (BasePoint.z = -BTLine->pos.z) + 1;

	if (BTLine->dir.x != 0)
	{
		m = BasePoint.x / BTLine->dir.x;
		y = m * BTLine->dir.y;
		z = m * BTLine->dir.z;
		if ((y >= BasePoint.y) &&
		    (y <= EndPoint.y)  &&
		    (z >= BasePoint.z) &&
			(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_SIDE;
			Index++;
		}

		m = EndPoint.x / BTLine->dir.x;
		y = m * BTLine->dir.y;
		z = m * BTLine->dir.z;
		if ((y >= BasePoint.y) &&
		    (y <= EndPoint.y)  &&
		    (z >= BasePoint.z) &&
		    (z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_SIDE;
			Index++;
		}
	}

	if (BTLine->dir.y != 0)
	{
		m = BasePoint.y / BTLine->dir.y;
		x = m * BTLine->dir.x;
		z = m * BTLine->dir.z;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (z >= BasePoint.z) &&
		    (z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_FRONT;
			Index++;
		}

		m = EndPoint.y / BTLine->dir.y;
		x = m * BTLine->dir.x;
		z = m * BTLine->dir.z;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (z >= BasePoint.z) &&
	    	(z <= EndPoint.z))
		{
			l[Index] = m;
			n[Index] = B3_CSG_FRONT;
			Index++;
		}
	}

	if (BTLine->dir.z != 0)
	{
		m = BasePoint.z / BTLine->dir.z;
		y = m * BTLine->dir.y;
		x = m * BTLine->dir.x;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (y >= BasePoint.y) &&
		    (y <= EndPoint.y))
		{
			l[Index] = m;
			n[Index] = B3_CSG_NORMAL;
			Index++;
		}

		m = EndPoint.z / BTLine->dir.z;
		y = m * BTLine->dir.y;
		x = m * BTLine->dir.x;
		if ((x >= BasePoint.x) &&
		    (x <= EndPoint.x)  &&
		    (y >= BasePoint.y) &&
		    (y <= EndPoint.y))
		{
			l[Index] = m;
			n[Index] = B3_CSG_NORMAL;
			Index++;
		}
	}

	B3_ASSERT(Index <= 2);
	if (Index == 2)
	{
		interval->m_Count = Index;

		// Index knows where lower one is...
		Index = (l[0] < l[1] ? 0 : 1);

		// Lower one
		interval->m_x[0].m_Q      = l[Index];
		interval->m_x[0].m_Shape  = this;
		interval->m_x[0].m_BTLine = BTLine;
		interval->m_x[0].m_Index  = n[Index];

		// Higher one
		Index ^= 1;
		interval->m_x[1].m_Q      = l[Index];
		interval->m_x[1].m_Shape  = this;
		interval->m_x[1].m_BTLine = BTLine;
		interval->m_x[1].m_Index  = n[Index];
	}

	return interval->m_Count > 0;
}

b3_bool b3CSGTorus::b3Intersect(
	b3_ray             *ray,
	b3_shape_intervals *interval,
	b3_line64          *BTLine)
{
	b3_loop       NumOfX;
	b3_index      i,k,t = 0;
	b3_f64        Val1,Val2,pQuad,dQuad,pdQuad;
	b3_f64        Coeff[5],x[4];

	interval->m_Count = 0;
	b3BaseTrans (ray,BTLine);
	pQuad  = BTLine->pos.z * BTLine->pos.z;
	dQuad  = BTLine->dir.z * BTLine->dir.z;
	pdQuad = BTLine->pos.z * BTLine->dir.z;
	Val1   =
		BTLine->pos.x * BTLine->pos.x +
		BTLine->pos.y * BTLine->pos.y + pQuad - m_aQuad - m_bQuad;
	Val2   =
		BTLine->pos.x * BTLine->dir.x +
		BTLine->pos.y * BTLine->dir.y + pdQuad;

	Coeff[4] = 1;
	Coeff[3] = 4 *  Val2;
	Coeff[2] = 2 * (Val1        + 2 * Val2  * Val2 + 2 * m_aQuad * dQuad);
	Coeff[1] = 4 * (Val1 * Val2 + 2 * m_aQuad * pdQuad);
	Coeff[0] =      Val1 * Val1 + 4 * m_aQuad * (pQuad - m_bQuad);

	NumOfX = b3Cubic::b3SolveOrd4 (Coeff,x);
	if ((NumOfX == 2) || (NumOfX == 4))
	{
		// Insert sorted
		for (i = 0; i < NumOfX; i++)
		{
			// Simple bubble sort - with NumOfX <= 4
			// it's much faster.
			for (k = i; k < NumOfX; k++)
			{
				if (x[k] <= x[i])
				{
					t = i;
				}
			}

			interval->m_x[i].m_Q      = x[t];
			interval->m_x[i].m_Shape  = this;
			interval->m_x[i].m_BTLine = BTLine;

			x[t] = x[i];
		}
		interval->m_Count = NumOfX;
	}

	return interval->m_Count > 0;
}

/*************************************************************************
**                                                                      **
**                        BBox intersection                             **
**                                                                      **
*************************************************************************/

b3_bool b3BBox::b3Intersect(b3_ray *ray)
{
	b3_f64 start,end,t_near,t_far,m,pos;

	m       = 1.0 / ray->dir.x;
	pos     = (ray->pos.x  - m_DimBase.x);
	start   = (            - pos) * m;
	end     = (m_DimSize.x - pos) * m;
	if (start > end)
	{
		m     = start;
		start = end;
		end   = m;
	}

	m       = 1.0 / ray->dir.y;
	pos     = (ray->pos.y  - m_DimBase.y);
	t_near  = (            - pos) * m;
	t_far   = (m_DimSize.y - pos) * m;
	if (t_near > t_far)
	{
		m      = t_near;
		t_near = t_far;
		t_far  = m;
	}
	if (t_near > start) start = t_near;
	if (t_far  < end)   end   = t_far;
	if (start  > end)
	{
		return false;
	}

	// Is it OK?
	if (end    < 0)
	{
		return false;
	}

	m       = 1.0 / ray->dir.z;
	pos     = (ray->pos.z  - m_DimBase.z);
	t_near  = (            - pos) * m;
	t_far   = (m_DimSize.z - pos) * m;
	if (t_near > t_far)
	{
		m      = t_near;
		t_near = t_far;
		t_far  = m;
	}
	if (t_near > start) start = t_near;
	if (t_far  < end)   end   = t_far;

	return (start <= end) && (end >= 0) && (start <= ray->Q);
}

b3CSGShape *b3BBox::b3IntersectCSG(b3_ray *ray)
{
	b3Item             *item;
	b3CSGShape         *shape;
	b3_line64           lines[B3_MAX_CSG_SHAPES_PER_BBOX]; // This is a hack! To be fixed!
	b3_shape_intervals  local;
	b3_bbox_intervals   intervals[2];
	b3_bbox_intervals  *result = intervals;
	b3_csg_point       *point;
	b3_index            t = 0,index = 0;

	intervals[0].m_Count = 0;
	intervals[1].m_Count = 0;
	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		result    = &intervals[index];
		index    ^= 1;
		shape     = (b3CSGShape *)item;
		shape->b3Intersect(ray,&local,&lines[t++]);
		shape->b3Operate(&local,&intervals[index],result);
	}

	point = result->m_x;
	for (t = 0; t < result->m_Count; t++)
	{
		if ((point->m_Q >= b3Scene::epsilon) && (point->m_Q <= ray->Q))
		{
			ray->Q = point->m_Q;
			shape  = point->m_Shape;
			shape->b3InverseMap(ray,point);
			return shape;
		}
		point++;
	}
	return null;
}

b3Shape *b3Scene::b3Intersect(
	b3BBox      *BBox,
	b3_ray_info *ray)
{
	b3Base<b3Item>      *Shapes;
	b3Base<b3Item>      *BBoxes;
	b3SimpleShape       *Shape;
	b3Shape             *ResultShape = null,*aux;
	b3Item              *item;
	b3_polar_precompute  polar;
	b3_f64               Result;

	while (BBox != null)
	{
		if (BBox->b3Intersect(ray))
		{
			//Check recursively
			BBoxes = BBox->b3GetBBoxHead();
			if (BBoxes->First)
			{
				aux = b3Intersect ((b3BBox *)BBoxes->First,ray);
				if (aux != null)
				{
					ResultShape = aux;
				}
			}

			// Check shapes inside this object
			Shapes = BBox->b3GetShapeHead();
			switch (Shapes->b3GetClass())
			{
			case CLASS_SHAPE:
				B3_FOR_BASE(Shapes,item)
				{
					Shape  = (b3SimpleShape *)item;
					Result = Shape->b3Intersect(ray,&polar);
					if ((Result > 0) && (Result <= ray->Q))
					{
						ResultShape = Shape;
						ray->bbox   = BBox;
						ray->Q      = Result;

						*((b3_polar_precompute *)&ray->polar) = polar;
					}
				}
				break;

			case CLASS_CSG:
				aux = BBox->b3IntersectCSG(ray);
				if (aux != null)
				{
					ResultShape = aux;
					ray->bbox   = BBox;
				}
				break;

			default:
				// We don't know any other kind of shapes, yet.
				break;
			}
		}
		BBox = (b3BBox *)BBox->Succ;
	}
	return ResultShape;
}

b3_bool b3Scene::b3Intersect(b3_ray_info *ray,b3_f64 max)
{
	b3BBox  *bbox;
	b3_bool  found;

	ray->Q     = max;
	ray->shape = b3Intersect(b3GetFirstBBox(),ray);

	found = (ray->shape != null);
	if (found)
	{
		bbox = ray->bbox;
	}

	return found;
}

b3Shape *b3Scene::b3IsObscured(
	b3BBox      *BBox,
	b3_ray_info *ray)
{
	b3Base<b3Item> *Shapes;
	b3Base<b3Item> *BBoxes;
	b3SimpleShape  *Shape;
	b3Shape        *ResultShape;
	b3Item         *item;
	b3_polar        polar;
	b3_f64          Result;

	while (BBox != null)
	{
		if (BBox->b3Intersect(ray))
		{
			Shapes = BBox->b3GetShapeHead();
			switch (Shapes->b3GetClass())
			{
			case CLASS_SHAPE:
				B3_FOR_BASE(Shapes,item)
				{
					Shape  = (b3SimpleShape *)item;
					Result = Shape->b3Intersect(ray,&polar);
					if ((Result > 0) && (Result <= ray->Q))
					{
						return Shape;
					}
				}
				break;

			case CLASS_CSG:
				ResultShape = BBox->b3IntersectCSG(ray);
				if (ResultShape != null)
				{
					ray->shape = ResultShape;
					ray->bbox  = BBox;
					return ResultShape;
				}
				break;

			default:
				// We don't know any other kind of shapes, yet.
				break;
			}

			// Check recursively
			BBoxes = BBox->b3GetBBoxHead();
			if (BBoxes->First != null)
			{
				ResultShape = b3IsObscured ((b3BBox *)BBoxes->First,ray);
				if (ResultShape != null)
				{
					return ResultShape;
				}
			}

		}
		BBox = (b3BBox *)BBox->Succ;
	}
	return null;
}

b3_bool b3Scene::b3IsObscured(b3_ray_info *ray,b3_f64 max)
{
	ray->Q     = max;
	ray->shape = b3IsObscured(b3GetFirstBBox(),ray);
	return ray->shape != null;
}

b3_bool b3Scene::b3FindObscurer(b3_ray_info *ray,b3_f64 max)
{
	return b3Intersect(ray,max);
}

/*************************************************************************
**                                                                      **
**                        Find bboxes by ray                            **
**                                                                      **
*************************************************************************/

void b3BBox::b3CollectBBoxes(b3_ray *ray,b3Array<b3BBox *> *array)
{
	b3Item         *item;
	b3Base<b3Item> *Shapes;
	b3SimpleShape  *shape;
	b3BBox         *bbox;
	b3_polar        polar;

	if (b3Intersect(ray))
	{
		// Is any shape hit?
		Shapes = b3GetShapeHead();
		switch (Shapes->b3GetClass())
		{
		case CLASS_SHAPE:
			B3_FOR_BASE(Shapes,item)
			{
				shape = (b3SimpleShape *)item;
				if (shape->b3Intersect(ray,&polar) > 0)
				{
					array->b3Add(this);

					// Leave loop
					break;
				}
			}
			break;

		case CLASS_CSG:
			if(b3IntersectCSG(ray) != null)
			{
				array->b3Add(this);
			}
			break;
		}

		// Collect sub bboxes
		B3_FOR_BASE(b3GetBBoxHead(),item)
		{
			bbox = (b3BBox *)item;
			bbox->b3CollectBBoxes(ray,array);
		}
	}
}

void b3Scene::b3CollectBBoxes(
	b3_line64         *line,
	b3Array<b3BBox *> *array,
	b3_f64             max)
{
	b3Item *item;
	b3BBox *bbox;
	b3_ray  ray;

	array->b3Clear();
	ray.pos = line->pos;
	ray.dir = line->dir;
	ray.Q   = max;
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(&ray,array);
	}
}

/*************************************************************************
**                                                                      **
**                        Find bboxes by area                           **
**                                                                      **
*************************************************************************/

void b3BBox::b3CollectBBoxes(
	b3_vector         *lower,
	b3_vector         *upper,
	b3Array<b3BBox *> *array)
{
	b3Item    *item;
	b3BBox    *bbox;
	b3_vector  lLower,lUpper;

	// Compute local dimensions
	lLower = m_DimBase;
	b3Vector::b3Add(&m_DimBase,&m_DimSize,&lUpper);

	// Are we completely inside the defined search area?
	// And do we contain any shape?
	if ((lower->x <= lLower.x) && (lUpper.x <= upper->x) &&
	    (lower->y <= lLower.y) && (lUpper.y <= upper->y) &&
        (lower->z <= lLower.z) && (lUpper.z <= upper->z) &&
		(b3GetShapeHead()->First != null))
	{
		// Yes!
		array->b3Add(this);
	}

	// Search for sub bboxes
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(lower,upper,array);
	}
}

void b3Scene::b3CollectBBoxes(
	b3_vector         *lower,
	b3_vector         *upper,
	b3Array<b3BBox *> *array)
{
	b3Item *item;
	b3BBox *bbox;

	array->b3Clear();
	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		bbox->b3CollectBBoxes(lower,upper,array);
	}
}
