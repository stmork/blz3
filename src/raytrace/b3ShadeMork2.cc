/*
**
**	$Filename:	b3ShadeMork2.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Enhanced Mork shader
**
**	(C) Copyright 2004  Steffen A. Mork
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

#include "blz3/raytrace/b3Shade.h"

/*************************************************************************
**                                                                      **
**                        b3ShaderMork2 implementation                  **
**                                                                      **
*************************************************************************/

b3ShaderMork2::b3ShaderMork2(b3Scene * scene) : b3Shader(scene)
{
}

void b3ShaderMork2::b3Prepare(b3_preparation_info * prep_info)
{
	b3Shader::b3Prepare(prep_info);
	m_ShadowFactor = m_Scene->m_ShadowBrightness / m_Scene->b3GetLightCount();
}

void b3ShaderMork2::b3ShadeLight(
	const b3Light * light,
	b3_light_info * Jit,
	b3_surface   *  surface) const
{
	b3Color illumination;

	illumination.b3InitFactor(m_ShadowFactor);

	// No shadow => surface in light
	if (Jit->shape == nullptr)
	{
		b3_f64 ShapeAngle = b3Vector::b3SMul(&surface->m_Incoming->normal, &Jit->dir);
		b3_f32 factor;

		// specular high light
		if (ShapeAngle >= 0)
		{
			b3_u32 spec_exp = (b3_u32)surface->m_SpecularExp;

			if (spec_exp < 100000) // test if surface if rough
			{
				b3_f64 lambda = b3Vector::b3SMul(&surface->m_ReflRay.dir, &Jit->dir);

				factor = b3Math::b3FastPow(lambda, spec_exp) * Jit->m_LightFrac;
				Jit->m_SpecularSum += (light->m_Color * factor);
			}

			// surface illumination (diffuse color)
			factor = ShapeAngle * Jit->m_LightFrac * 0.5 - m_ShadowFactor;
			if (factor >= 0)
			{
				illumination += (light->m_Color * factor);
			}
		}
	}
#ifdef TRANSPARENT_SHADING
	else
	{
		b3_surface  obsSurface;

		obsSurface.m_Incoming = Jit;
		if (obsSurface.m_Refraction > 0)
		{
			Jit->m_DiffuseSum += (
					light->m_Color * obsSurface.m_Diffuse * obsSurface.m_Refraction);
		}
	}
#endif

	Jit->m_DiffuseSum += (surface->m_Diffuse * illumination);
}

void b3ShaderMork2::b3ComputeFresnelCoeffs(b3_surface * surface, b3_f32 & refl, b3_f32 & refr)
{
	b3_f64 alpha    = b3Math::b3Acos(surface->m_CosAlpha);
	b3_f64 sin_beta = sin(alpha) * surface->m_IorComputed;
	b3_f64 beta     = b3Math::b3Asin(sin_beta);
	b3_f64 apb      = alpha + beta;
	b3_f64 amb      = alpha - beta;
	b3_f64 s_apb    = sin(apb);
	b3_f64 c_apb    = cos(apb);
	b3_f64 s_amb    = sin(amb);
	b3_f64 c_amb    = cos(amb);

	// compute perpendicular (s = senkrecht) component of polarized light
	b3_f64 Ers   = s_amb / s_apb;
	b3_f64 Ets   = 2.0 * sin_beta * surface->m_CosAlpha / s_apb;

	// compute parallel component of polarized light
	b3_f64 Erp   = (s_amb * c_apb) / (c_amb * s_apb);
	b3_f64 Etp   = 2.0 * sin_beta * surface->m_CosAlpha / (s_apb * c_amb);

	b3ComputeFresnel(surface);

	// Mix to unpolarized light
	refl = (Ers + Erp) * surface->m_Reflection * 0.5;
	refr = (Ets + Etp) * surface->m_Refraction * 0.5;
}

void b3ShaderMork2::b3ShadeSurface(
	b3_surface * surface,
	b3_count    depth_count)
{
	b3_ray  * ray = surface->m_Incoming;
	b3_f32    refl, refr, factor;

	// Refraction
	if (surface->m_Transparent)
	{
		if (surface->m_Ior == 1)
		{
			[[unlikely]]
			surface->m_RefrRay.inside = false;
			surface->m_ReflRay.inside = false;
		}
		b3ComputeFresnelCoeffs(surface, refl, refr);
		b3Shade(&surface->m_RefrRay, depth_count);
	}
	else
	{
		if (surface->m_Ior != 1)
		{
			// simulate dielectric metal
			b3ComputeFresnel(surface);
			refl = surface->m_Reflection * (1.0 - surface->m_Reflection + surface->m_Fresnel);
		}
		else
		{
			// plastic reflection
			refl = surface->m_Reflection;
		}
		refr = 0;
		surface->m_RefrRay.color.b3Init();
	}

	// Reflection
	if (refl > 0)
	{
		[[likely]]
		b3Shade(&surface->m_ReflRay, depth_count);
	}
	else
	{
		[[unlikely]]
		refl = 0;
		surface->m_ReflRay.color.b3Init();
	}

	// Mix colors
	factor = (1.0 - refl - refr);
	if (factor > 0)
	{
		// For each light source
		surface->m_AmbientSum.b3Init();
		surface->m_DiffuseSum.b3Init();
		surface->m_SpecularSum.b3Init();

		// For each light source...
		b3Illuminate(surface);
		if (!b3Material::b3MixComponents(surface, refl, refr))
		{
			ray->color =
				surface->m_AmbientSum +
				surface->m_DiffuseSum * factor +
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
