/*
**
**	$Filename:	b3ShadeMork.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Mork shader
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
#include "blz3/base/b3Matrix.h"

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


b3SceneMork::b3SceneMork(b3_u32 class_type) : b3Scene(sizeof(b3SceneMork), class_type)
{
	b3PrintF(B3LOG_NORMAL,"Using Mork shading...\n");
}

b3SceneMork::b3SceneMork(b3_u32 *src) : b3Scene(src)
{
	b3PrintF(B3LOG_NORMAL,"Using Mork shading...\n");
}

void b3SceneMork::b3Illuminate(
	b3Light         *light,
	b3_light_info   *Jit,
	b3_illumination *surface,
	b3_color        *result)
{
	b3_f64   ShapeAngle,Factor;
	b3_color filter;

	// Real absorption
	result->a  = 0;
	result->r += (surface->diffuse.r * m_ShadowBrightness);
	result->g += (surface->diffuse.g * m_ShadowBrightness);
	result->b += (surface->diffuse.b * m_ShadowBrightness);

	filter.r =
	filter.g =
	filter.b = 0;

		   /***** no shadow => surface in light *****/
	if (Jit->shape == null)
	{


		// specular high light
		if ((ShapeAngle =
			surface->incoming->normal.x * Jit->dir.x +
			surface->incoming->normal.y * Jit->dir.y +
			surface->incoming->normal.z * Jit->dir.z) >= 0)
		{
			if (surface->se < 100000)
			{
				Factor = log ((
					surface->refl_ray.dir.x * Jit->dir.x +
					surface->refl_ray.dir.y * Jit->dir.y +
					surface->refl_ray.dir.z * Jit->dir.z + 1) * 0.5);

				Factor = exp (Factor * surface->se) * Jit->LightFrac;
				surface->specular_sum.r += (Factor * light->m_Color.r);
				surface->specular_sum.g += (Factor * light->m_Color.g);
				surface->specular_sum.b += (Factor * light->m_Color.b);
			}
		}
		else
		{
			ShapeAngle = 0;
		}

		// surface illumination (diffuse color)
		if ((Factor = ShapeAngle * Jit->LightFrac - m_ShadowBrightness) > 0)
		{
			filter.r = light->m_Color.r * Factor;
			filter.g = light->m_Color.g * Factor;
			filter.b = light->m_Color.b * Factor;
		}
	}

	result->r += (surface->diffuse.r * filter.r);
	result->g += (surface->diffuse.g * filter.g);
	result->b += (surface->diffuse.b * filter.b);
}


b3_bool b3SceneMork::b3IsPointLightBackground (
	b3Light     *Light,
	b3_ray_info *Ray)
{
	b3_vector LightDir;
	b3_f64    LightDist,Factor,ReflexAngle;

	if (!Light->m_LightActive)
	{
		return false;
	}

	LightDir.x = Light->m_Position.x - Ray->pos.x;
	LightDir.y = Light->m_Position.y - Ray->pos.y;
	LightDir.z = Light->m_Position.z - Ray->pos.z;
	LightDist  =
		LightDir.x * LightDir.x +
		LightDir.y * LightDir.y +
		LightDir.z * LightDir.z;
	if (LightDist == 0)
	{
		return false;
	}
 	LightDist   = 1.0 / sqrt(LightDist);
	LightDir.x *= LightDist;
	LightDir.y *= LightDist;
	LightDir.z *= LightDist;

	ReflexAngle =
		(LightDir.x * Ray->dir.x +
		 LightDir.y * Ray->dir.y +
		 LightDir.z * Ray->dir.z);
	Factor = ReflexAngle * Light->m_Distance * LightDist;
	if (Factor > 1)
	{
		Factor = 1;
	}

	Ray->color.r = Factor * Light->m_Color.r;
	Ray->color.g = Factor * Light->m_Color.g;
	Ray->color.b = Factor * Light->m_Color.b;
	return true;
}

void b3SceneMork::b3LightFlare (b3_ray_info *Ray)
{
	b3Item    *item;
	b3Light   *Light;
	b3_vector  toLight;
	b3_color   result;
	b3_f64     angle,reverse;

	B3_FOR_BASE(&heads[1],item)
	{
		Light = (b3Light *)item;

		toLight.x = Light->m_Position.x - m_EyePoint.x;
		toLight.y = Light->m_Position.y - m_EyePoint.y;
		toLight.z = Light->m_Position.z - m_EyePoint.z;

		// Ray->dir is already normalized so only
		// toLight is to be normalized for angle
		// computation between toLight and Ray->dir
		angle  = (
			toLight.x * Ray->dir.x +
			toLight.y * Ray->dir.y +
			toLight.z * Ray->dir.z) / sqrt(
			toLight.x * toLight.x +
			toLight.y * toLight.y +
			toLight.z * toLight.z);
		angle *= angle;
		angle *= angle;
		angle *= angle;
		angle *= angle;
		angle *= angle;

		reverse    = 1.0 - angle;
		result.r   = angle * Light->m_Color.r + reverse * Ray->color.r;
		result.g   = angle * Light->m_Color.g + reverse * Ray->color.g;
		result.b   = angle * Light->m_Color.b + reverse * Ray->color.b;
		Ray->color = result;
	}
}

b3_bool b3SceneMork::b3Shade(
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

	surface.specular_sum.a = ray->color.r =
	surface.specular_sum.r = ray->color.g =
	surface.specular_sum.g = ray->color.b =
	surface.specular_sum.b = ray->color.a = 0;

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
		return false;
	}

	if (b3Intersect(ray))
	{
		// Initialize some values
		// Compute intersection point "ipoint"
		// Get hit material
		// Compute normal at ipoint and bump normal
		// Get material values
		shape = ray->shape;
		surface.incoming = ray;
		ray->ipoint.x = ray->pos.x + ray->Q * ray->dir.x;
		ray->ipoint.y = ray->pos.y + ray->Q * ray->dir.y;
		ray->ipoint.z = ray->pos.z + ray->Q * ray->dir.z;
		shape->b3GetColors(ray,&surface);
		shape->b3BumpNormal(ray);

		// Where to shoot subsequent rays...
		transparent = b3ComputeOutputRays(&surface);
		if (transparent)
		{
			if (surface.ior == 1)
			{
				surface.refr_ray.inside = false;
				surface.refl_ray.inside = false;
			}
			refr = surface.refr;
		}
		else
		{
			refr = 0;
		}

		if (depth_count <= m_TraceDepth)
		{
			// Reflection
			if (((!ray->inside) || (!transparent)) && (surface.refl > 0))
			{
				refl = surface.refl;
				if (!b3Shade(&surface.refl_ray,depth_count + 1))
				{
					b3GetInfiniteColor(&surface.refl_ray.color);
				}
			}
			else
			{
				refl = 0;
			}

			if (transparent)
			{
				if (!b3Shade(&surface.refr_ray,depth_count + 1))
				{
					b3GetInfiniteColor(&surface.refr_ray.color);
				}
			}
		}
		else
		{
			refl = 0;
		}

		// For each light source
		B3_FOR_BASE(&heads[1],item)
		{
			light = (b3Light *)item;
			light->b3Illuminate(this,&surface);
		}

		// Mix colors
		factor        = (1.0 - refl - refr) * 0.5;
		ray->color.r *= factor;
		ray->color.g *= factor;
		ray->color.b *= factor;
		ray->color.a *= factor;

		if (refl > 0)
		{
			ray->color.r += (refl * surface.refl_ray.color.r);
			ray->color.g += (refl * surface.refl_ray.color.g);
			ray->color.b += (refl * surface.refl_ray.color.b);
			ray->color.a += (refl * surface.refl_ray.color.a);
		}
		if (refr > 0)
		{
			ray->color.r += (refr * surface.refr_ray.color.r);
			ray->color.g += (refr * surface.refr_ray.color.g);
			ray->color.b += (refr * surface.refr_ray.color.b);
			ray->color.a += (refr * surface.refr_ray.color.a);
		}

		ray->color.r += surface.specular_sum.r;
		ray->color.g += surface.specular_sum.g;
		ray->color.b += surface.specular_sum.b;
		ray->color.a += surface.specular_sum.a;

		if (m_Nebular != null)
		{
			m_Nebular->b3ComputeNebular(&ray->color,&ray->color,ray->Q);
		}
		result = true;
	}
	else
	{
		if (m_Nebular != null)
		{
			m_Nebular->b3GetNebularColor(&ray->color);
			result = true;
		}
		else
		{
			B3_FOR_BASE(&heads[1],item)
			{
				b3IsPointLightBackground((b3Light *)item,ray);
			}
			result = false;
		}
	}

	return result;
}
