/*
**
**	$Filename:	b3Shade.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - The default shader
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
**	Revision 1.7  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**
**	Revision 1.6  2001/10/05 20:30:46  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**	Revision 1.5  2001/10/03 20:17:56  sm
**	- Minor bugfixes
**	
**	Revision 1.4  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**	
**	Revision 1.3  2001/10/02 16:01:58  sm
**	- Moving b3Polar into b3Ray but that's not right at all. The
**	  result must be placed there but a simple result from one
**	  intersection must be placed into a temp instance. The same
**	  must be done for surface normals as they result from using
**	  the b3Polar class.
**	
**	Revision 1.2  2001/09/30 16:27:48  sm
**	- Raytracing with diffuse color without shading
**	- Sphere intersection fixed (now using normalized rays)
**	
**	Revision 1.1  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**
*/

b3_bool b3Scene::b3ComputeOutputRays(b3_illumination *surface)
{
	b3_vector  *Normal       = &surface->incoming->normal;
	b3_dVector *incoming_dir = &surface->incoming->dir;
	b3_dVector *refl_dir     = &surface->refl_ray.dir;
	b3_dVector *refr_dir;
	b3_f64      Factor,cos_a,d,ior;
	b3_bool     transparent = false;
 
	Factor = 2 * (cos_a =
		incoming_dir->x * Normal->x +
		incoming_dir->y * Normal->y +
		incoming_dir->z * Normal->z);
	refl_dir->x = incoming_dir->x - Factor * Normal->x;
	refl_dir->y = incoming_dir->y - Factor * Normal->y;
	refl_dir->z = incoming_dir->z - Factor * Normal->z;
	Factor = 1/sqrt(
		refl_dir->x * refl_dir->x +
		refl_dir->y * refl_dir->y +
		refl_dir->z * refl_dir->z);
	refl_dir->x *= Factor;
	refl_dir->y *= Factor;
	refl_dir->z *= Factor;
	surface->refl_ray.pos    =  surface->incoming->pos;
	surface->refl_ray.inside = !surface->incoming->inside;

	//Use only sharp angles
	if (cos_a >= 0)
	{
		Normal->x = -Normal->x;
		Normal->y = -Normal->y;
		Normal->z = -Normal->z;
		cos_a     = -cos_a;
	}

	if (surface->refr > 0)
	{
		ior      = surface->incoming->inside ? 1.0 / surface->ior : surface->ior;
		refr_dir = &surface->refr_ray.dir;

		if (fabs(cos_a) < 1)
		{
			d = ior * ior;
			Factor = 1 - d + d * cos_a * cos_a;
			if (Factor >= 0)	/* Test auf Totalreflexion */
			{
				Factor = sqrt(Factor) + ior * cos_a;
				refr_dir->x = Factor * Normal->x - incoming_dir->x * ior;
				refr_dir->y = Factor * Normal->y - incoming_dir->y * ior;
				refr_dir->z = Factor * Normal->z - incoming_dir->z * ior;

				Factor = -1/sqrt(
					refr_dir->x * refr_dir->x +
					refr_dir->y * refr_dir->y +
					refr_dir->z * refr_dir->z);
				refr_dir->x *= Factor;
				refr_dir->y *= Factor;
				refr_dir->z *= Factor;

				surface->refr_ray.pos    =  surface->incoming->pos;
				surface->refr_ray.inside = !surface->incoming->inside;
				transparent = true;
			}
		}
		else					/* Einfall senkrecht -> Richtung bleibt */
		{
			surface->refr_ray.pos    =  surface->incoming->pos;
			surface->refr_ray.dir    =  surface->incoming->dir;
			surface->refr_ray.inside = !surface->incoming->inside;
			transparent = true;
		}
	}
	return transparent;
}

void b3Scene::b3Illuminate(
	b3Light         *light,
	b3_light_info   *Jit,
	b3_illumination *surface)
{
	register double ShapeAngle,Factor;

	if ((ShapeAngle =
		surface->incoming->normal.x * Jit->dir.x +
		surface->incoming->normal.y * Jit->dir.y +
		surface->incoming->normal.z * Jit->dir.z) >= 0)
	{
		surface->incoming->color.r +=
			ShapeAngle * surface->diffuse.r * light->m_Color.r;
		surface->incoming->color.g +=
			ShapeAngle * surface->diffuse.g * light->m_Color.g;
		surface->incoming->color.b +=
			ShapeAngle * surface->diffuse.b * light->m_Color.b;
	}
}

void b3Scene::b3GetBackgroundColor(b3_color *bg)
{
	bg->r = (m_TopColor.r + m_BottomColor.r) * 0.5;
	bg->g = (m_TopColor.g + m_BottomColor.g) * 0.5;
	bg->b = (m_TopColor.b + m_BottomColor.b) * 0.5;
	bg->a = (m_TopColor.a + m_BottomColor.a) * 0.5;
}

void b3Scene::b3GetInfiniteColor(b3_color *inf)
{
	inf->r = 0.5f;
	inf->g = 0.5f;
	inf->b = 0.5f;
	inf->a = 0.0f;
}

b3_bool b3Scene::b3Shade(
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
