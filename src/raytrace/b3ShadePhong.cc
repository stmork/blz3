/*
**
**	$Filename:	b3ShadePhong.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Phong shader
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
  
#include "blz3/b3Config.h" 
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.13  2003/03/04 20:37:39  sm
**	- Introducing new b3Color which brings some
**	  performance!
**
**	Revision 1.12  2002/02/28 16:58:46  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.11  2002/02/18 17:50:32  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.10  2002/02/17 21:25:07  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.9  2001/12/31 11:05:18  sm
**	- Added TestData for testing Blizzard data structures for reading
**	  and writing.
**	- Fixed bugs found with previous mentioned tool:
**	  o Some b3AnimElement errors found to be fixed under Windows.
**	  o b3TriangleShape destructor handled unchecked m_GridList pointer
**	- Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**	  B3LOG_NORMAL to B3LOG_DEBUG.
**	
**	Revision 1.8  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.7  2001/12/23 08:57:21  sm
**	- Fixed recursive calling bug in b3IsObscured(...)
**	- Minor intersection optimazations done.
**	
**	Revision 1.6  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.5  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.4  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.3  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	Revision 1.2  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.1  2001/10/05 20:30:46  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**
*/


b3ScenePhong::b3ScenePhong(b3_u32 class_type) : b3Scene(sizeof(b3ScenePhong), class_type)
{
	b3PrintF(B3LOG_DEBUG,"  using Phong shading...\n");
}

b3ScenePhong::b3ScenePhong(b3_u32 *src) : b3Scene(src)
{
	b3PrintF(B3LOG_DEBUG,"  using Phong shading...\n");
}

b3_bool b3ScenePhong::b3FindObscurer(b3_ray_info *ray,b3_f64 max)
{
#if 0
	return b3Scene::b3IsObscured(ray,max);
#else
	return b3IsObscured(ray,max);
#endif
}

void b3ScenePhong::b3Illuminate(
	b3Light       *light,
	b3_light_info *Jit,
	b3_ray_fork   *surface,
	b3Color       &result)
{
	b3_f64 ShapeAngle,Factor;

	if (Jit->shape == null)
	{
		if ((ShapeAngle =
			surface->incoming->normal.x * Jit->dir.x +
			surface->incoming->normal.y * Jit->dir.y +
			surface->incoming->normal.z * Jit->dir.z) >= 0)
		{
			Factor = log ((
				surface->refl_ray.dir.x * Jit->dir.x +
				surface->refl_ray.dir.y * Jit->dir.y +
				surface->refl_ray.dir.z * Jit->dir.z + 1) * 0.5);
			Factor = exp(Factor * surface->se) * Jit->LightFrac;

			result += (surface->specular * Factor +	surface->diffuse * ShapeAngle) * light->m_Color;
		}
	}
}


b3_bool b3ScenePhong::b3Shade(
	b3_ray_info *ray,
	b3_count     depth_count)
{
	b3Item      *item;
	b3Light     *light;
	b3Shape     *shape;
	b3_ray_fork  surface;
	b3_f64       refl,refr,factor;
	b3_f64       denom;
	b3_index     formula = 0;
	b3_bool      transparent;
	b3_bool      result = false;

	// If max raytrace depth is reached leave!
	if (depth_count > m_TraceDepth)
	{
		ray->color.b3Init();
		return false;
	}

	// Normalize incoming ray
	denom =
		ray->dir.x * ray->dir.x +
		ray->dir.y * ray->dir.y +
		ray->dir.z * ray->dir.z;
	if (denom != 0)
	{
		denom       = 1.0 / sqrt(denom);
		ray->dir.x *= denom;
		ray->dir.y *= denom;
		ray->dir.z *= denom;
	}
	else
	{
		ray->color.b3Init();
		return false;
	}

	if (b3Intersect(ray))
	{
		shape = ray->shape;
		surface.incoming = ray;
		ray->ipoint.x = ray->pos.x + ray->Q * ray->dir.x;
		ray->ipoint.y = ray->pos.y + ray->Q * ray->dir.y;
		ray->ipoint.z = ray->pos.z + ray->Q * ray->dir.z;
		shape->b3GetColors(ray,&surface);
		shape->b3BumpNormal(ray);

		ray->color = surface.ambient;

		transparent = b3ComputeOutputRays(&surface);
		if (transparent)
		{
			if (surface.ior == 1)
			{
				surface.refr_ray.inside = false;
				surface.refl_ray.inside = false;
			}
			refr = surface.refr;
			if (!b3Shade(&surface.refr_ray,depth_count + 1))
			{
				b3GetInfiniteColor(&surface.refr_ray);
			}
			formula |= 1;
		}
		else
		{
			refr = 0;
		}

		refl = surface.refl;
		if (refl > 0)
		{
			if (!b3Shade(&surface.refl_ray,depth_count + 1))
			{
				b3GetInfiniteColor(&surface.refl_ray);
			}
			formula |= 2;
		}

		switch(formula)
		{
		case 1:
			// Only refraction
			factor = (1.0 - refr);
			ray->color = surface.refr_ray.color * refr + ray->color * factor;
			break;

		case 2:
			// Only reflection
			factor = (1.0 - refr);
			ray->color = surface.refl_ray.color * refl + ray->color * factor;
			break;

		case 3:
			// Reflection and refraction
			factor = (1.0 - refl - refr);
			ray->color =
				surface.refl_ray.color * refl +
				surface.refr_ray.color * refr +
				            ray->color * factor;
			break;
		}

		B3_FOR_BASE(b3GetLightHead(),item)
		{
			light = (b3Light *)item;
			light->b3Illuminate(this,&surface);
		}

		if (m_Nebular != null)
		{
			m_Nebular->b3ComputeNebular(ray->color,ray->color,ray->Q);
		}
		result = true;
	}

	if (m_Nebular != null)
	{
		m_Nebular->b3GetNebularColor(ray->color);
		result = true;
	}
	return result;
}
