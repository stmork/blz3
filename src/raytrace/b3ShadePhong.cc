/*
**
**	$Filename:	b3ShadePhong.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Phong shader
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

b3ShaderPhong::b3ShaderPhong(b3Scene *scene) : b3Shader(scene)
{
}

#define MIX_REFLECTION 1
#define MIX_REFRACTION 2
#define MIX_BOTH       (MIX_REFLECTION | MIX_REFRACTION)

void b3ShaderPhong::b3ShadeLight(
	b3Light       *light,
	b3_light_info *Jit,
	b3_surface    *surface)
{
	// No shadow => surface in light
	if (Jit->shape == null)
	{
		b3_f32 ShapeAngle = b3Vector::b3SMul(&surface->m_Incoming->normal, &Jit->dir);

		if (ShapeAngle >= 0) // test for far side to light
		{
			b3_u32 spec_exp = (b3_u32)surface->m_SpecularExp;

			if (spec_exp < 100000) // test if surface if rough
			{
				b3_f64 lambda = b3Vector::b3SMul(&surface->m_ReflRay.dir,&Jit->dir);
				b3_f32 factor = b3Math::b3FastPow (lambda, spec_exp) * Jit->m_LightFrac;

				Jit->m_SpecularSum += (surface->m_Specular * light->m_Color * factor);
			}
			Jit->m_DiffuseSum += (surface->m_Diffuse * light->m_Color * ShapeAngle);
		}
	}
#ifdef TRANSPARENT_SHADING
	else
	{
		b3_surface  obsSurface;
		b3Material *obsMat;

		obsSurface.m_Incoming = Jit;
		obsMat = Jit->shape->b3GetSurfaceValues(&obsSurface);
		if (obsSurface.m_Refraction > 0)
		{
			Jit->m_DiffuseSum += (
									 light->m_Color * obsSurface.m_Diffuse * obsSurface.m_Refraction);
		}
	}
#endif
}

void b3ShaderPhong::b3ShadeSurface(
	b3_surface *surface,
	b3_count    depth_count)
{
	b3_ray      *ray = surface->m_Incoming;
	b3_f32       refl,refr,factor;

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

	refl = surface->m_Reflection;
	if (refl > 0)
	{
		b3Shade(&surface->m_ReflRay,depth_count);
	}
	else
	{
		surface->m_ReflRay.color.b3Init();
	}

	surface->m_DiffuseSum.b3Init();
	surface->m_SpecularSum.b3Init();
	b3Illuminate(surface);

	if (!b3Material::b3MixComponents(surface, refl, refr))
	{
		factor = (1.0 - refl - refr);
		ray->color =
			surface->m_Ambient +
			surface->m_DiffuseSum * factor +
			surface->m_ReflRay.color * refl +
			surface->m_RefrRay.color * refr +
			surface->m_SpecularSum;
	}
}
