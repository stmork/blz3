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
	b3PrintF(B3LOG_NORMAL,"  using Phong shading...\n");
}

b3ScenePhong::b3ScenePhong(b3_u32 *src) : b3Scene(src)
{
	b3PrintF(B3LOG_NORMAL,"  using Phong shading...\n");
}

void b3ScenePhong::b3Illuminate(
	b3Light         *light,
	b3_light_info   *Jit,
	b3_illumination *surface,
	b3_color        *result)
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

			result->r += (
				Factor     * surface->specular.r +
				ShapeAngle * surface->diffuse.r) * light->m_Color.r;
			result->g += (
				Factor     * surface->specular.g +
				ShapeAngle * surface->diffuse.g) * light->m_Color.g;
			result->b += (
				Factor     * surface->specular.b +
				ShapeAngle * surface->diffuse.b) * light->m_Color.b;
		}
	}
}


b3_bool b3ScenePhong::b3Shade(
	b3_ray_info *ray,
	b3_count     depth_count)
{
	b3Item          *item;
	b3Light         *light;
	b3Shape         *shape;
	b3_illumination  surface;
	b3_f64           refl,refr,factor;
	b3_f64           denom;
	b3_index         formula = 0;
	b3_bool          transparent;
	b3_bool          result = false;

	// If max raytrace depth is reached leave!
	if (depth_count > m_TraceDepth)
	{
		ray->color.r =
		ray->color.g =
		ray->color.b = 0.0f;
		ray->color.a = 0.0f;
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
		ray->color.r =
		ray->color.g =
		ray->color.b = 0.0f;
		ray->color.a = 0.0f;
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
				b3GetInfiniteColor(&surface.refr_ray.color);
			}
			formula |= 1;
		}

		refl = surface.refl;
		if (refl > 0)
		{
			if (!b3Shade(&surface.refl_ray,depth_count + 1))
			{
				b3GetInfiniteColor(&surface.refl_ray.color);
			}
			formula |= 2;
		}

		switch(formula)
		{
		case 1:
			// Only refraction
			factor = (1.0 - refr);
			ray->color.r =
				surface.refr_ray.color.r * refr +
				            ray->color.r * factor;
			ray->color.g =
				surface.refr_ray.color.g * refr +
				            ray->color.g * factor;
			ray->color.b =
				surface.refr_ray.color.b * refr +
				            ray->color.b * factor;
			ray->color.a =
				surface.refr_ray.color.a * refr +
				            ray->color.a * factor;
			break;

		case 2:
			// Only reflection
			factor = (1.0 - refr);
			ray->color.r =
				surface.refl_ray.color.r * refl +
				            ray->color.r * factor;
			ray->color.g =
				surface.refl_ray.color.g * refl +
				            ray->color.g * factor;
			ray->color.b =
				surface.refl_ray.color.b * refl +
				            ray->color.b * factor;
			ray->color.a =
				surface.refl_ray.color.a * refl +
				            ray->color.a * factor;
			break;

		case 3:
			// Reflection and refraction
			factor = (1.0 - refl - refr);
			ray->color.r =
				surface.refl_ray.color.r * refl +
				surface.refr_ray.color.r * refr +
				            ray->color.r * factor;
			ray->color.g =
				surface.refl_ray.color.g * refl +
				surface.refr_ray.color.g * refr +
				            ray->color.g * factor;
			ray->color.b =
				surface.refl_ray.color.b * refl +
				surface.refr_ray.color.b * refr +
				            ray->color.b * factor;
			ray->color.a =
				surface.refl_ray.color.a * refl +
				surface.refr_ray.color.a * refr +
				            ray->color.a * factor;
			break;
		}

		B3_FOR_BASE(&heads[1],item)
		{
			light = (b3Light *)item;
			light->b3Illuminate(this,&surface);
		}

		if (m_Nebular != null)
		{
			m_Nebular->b3ComputeNebular(&ray->color,&ray->color,ray->Q);
		}
		result = true;
	}

	if (m_Nebular != null)
	{
		m_Nebular->b3GetNebularColor(&ray->color);
		result = true;
	}
	return result;
}
