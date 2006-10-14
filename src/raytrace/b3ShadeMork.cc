/*
**
**	$Filename:	b3ShadeMork.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Mork shader
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        b3ShaderMork implementation                   **
**                                                                      **
*************************************************************************/

b3ShaderMork::b3ShaderMork(b3Scene *scene) : b3Shader(scene)
{
}

void b3ShaderMork::b3Prepare(b3_preparation_info *prep_info)
{
	b3Shader::b3Prepare(prep_info);
	m_ShadowFactor = m_Scene->m_ShadowBrightness / m_Scene->b3GetLightCount();
}

void b3ShaderMork::b3ShadeLight(
	b3Light       *light,
	b3_light_info *Jit,
	b3_surface    *surface)
{
	b3_f64   ShapeAngle;
	b3_f32   Factor;

	// Real absorption
	Jit->m_AmbientSum += (surface->m_Diffuse * m_ShadowFactor);

	// No shadow => surface in light
	if (Jit->shape == null) // This shape is the obscurer not the intersection point shape!!!
	{
		// specular high light
		if ((ShapeAngle =
			surface->m_Incoming->normal.x * Jit->dir.x +
			surface->m_Incoming->normal.y * Jit->dir.y +
			surface->m_Incoming->normal.z * Jit->dir.z) >= 0)
		{
			b3_f64 lambda   = b3Vector::b3SMul(&surface->m_ReflRay.dir,&Jit->dir);
			b3_u32 spec_exp = (b3_u32)surface->m_SpecularExp;

			if (spec_exp < 100000)
			{
				Factor = b3Math::b3FastPow((lambda + 1.0) * 0.5, spec_exp) * Jit->m_LightFrac;
				Jit->m_SpecularSum += (light->m_Color * Factor);
			}

			// surface illumination (diffuse color)
			Factor = ShapeAngle * Jit->m_LightFrac - m_ShadowFactor;
			if (Factor > 0)
			{
				Jit->m_DiffuseSum += (surface->m_Diffuse * light->m_Color * Factor);
			}
		}
	}
	else
	{
		B3_ALIGN_16 b3_surface obsSurface;
		obsSurface.m_Incoming = Jit;
		b3Material *obsMat = Jit->shape->b3GetSurfaceValues(&obsSurface);

		if (obsSurface.m_Refraction > 0)
		{
			b3_f64 ShapeAngle = b3Vector::b3SMul(&surface->m_Incoming->normal, &Jit->dir);
			b3_f32 factor = (ShapeAngle * Jit->m_LightFrac - m_ShadowFactor) * obsSurface.m_Refraction;

			// specular high light
			if (ShapeAngle >= 0)
			{
				Jit->m_DiffuseSum += (light->m_Color * factor * obsSurface.m_Diffuse);
			}
		}
	}
}

void b3ShaderMork::b3ShadeSurface(
	b3_surface *surface,
	b3_count    depth_count)
{
	b3_ray  *ray = surface->m_Incoming;
	b3_f32   refl,refr,factor;

	// Refraction
	if (surface->m_Transparent)
	{
		if (surface->m_Ior == 1)
		{
			surface->m_RefrRay.inside = false;
			surface->m_ReflRay.inside = false;
		}
		refr = surface->m_Refraction;
		b3Shade(&surface->m_RefrRay,depth_count);
	}
	else
	{
		refr = 0;
		surface->m_RefrRay.color.b3Init();
	}

	// Reflection
	refl = surface->m_Reflection;
	if (((!ray->inside) || (!surface->m_Transparent)) && (refl > 0))
	{
		b3Shade(&surface->m_ReflRay,depth_count);
	}
	else
	{
		refl = 0;
		surface->m_ReflRay.color.b3Init();
	}

	factor = 1.0 - refl - refr;
	if (factor > 0)
	{
		surface->m_AmbientSum.b3Init();
		surface->m_DiffuseSum.b3Init();
		surface->m_SpecularSum.b3Init();

		// For each light source...
		b3Illuminate(surface);
		if (!b3Material::b3MixComponents(surface, refl, refr))
		{
			ray->color =
				(surface->m_AmbientSum + surface->m_DiffuseSum) * factor * 0.5 +
				surface->m_ReflRay.color * refl +
				surface->m_RefrRay.color * refr +
				surface->m_SpecularSum;
		}
	}
	else
	{
		if (!b3Material::b3MixComponents(surface, refl, refr))
		{
			ray->color =
				surface->m_ReflRay.color * refl +
				surface->m_RefrRay.color * refr;
		}
	}
}
