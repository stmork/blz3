/*
**
**	$Filename:	b3ShadeMork2.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Enhanced Mork shader
**
**      (C) Copyright 2004  Steffen A. Mork
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

#include "blz3/raytrace/b3Light.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/raytrace/b3Shade.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2004/06/23 14:03:05  sm
**	- Reversed transparent computation.
**
**	Revision 1.7  2004/06/23 13:58:07  sm
**	- Changed Fresnel computation decision from real transparent
**	  to real refractive to force Fresnel computation even on
**	  total reflection.
**	
**	Revision 1.6  2004/06/23 13:50:49  sm
**	- Fixed ticket no. 26: Now the Fresnel coefficients were mixed correctly.
**	
**	Revision 1.5  2004/06/23 11:02:54  sm
**	- Fixed material shader problem in Mork shading model: The half factor
**	  moved into the lighting method.
**	
**	Revision 1.4  2004/06/05 08:07:05  sm
**	- Corrected b3Color for multiplying colors as filter.
**	
**	Revision 1.3  2004/05/29 13:38:11  sm
**	- Made shading model visible to material an bump dialogs.
**	
**	Revision 1.2  2004/05/28 20:33:05  sm
**	- Backported Mork shader
**	
**	Revision 1.1  2004/05/28 19:35:39  sm
**	- Added Mork shader enhancement as new extra shader.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3ShaderMork2 implementation                  **
**                                                                      **
*************************************************************************/

b3ShaderMork2::b3ShaderMork2(b3Scene *scene) : b3Shader(scene)
{
}

void b3ShaderMork2::b3Prepare()
{
	b3Shader::b3Prepare();
	m_ShadowFactor = m_Scene->m_ShadowBrightness / m_Scene->b3GetLightCount();
}

void b3ShaderMork2::b3ShadeLight(
	b3Light       *light,
	b3_light_info *Jit,
	b3_surface    *surface,
	b3Color       &result)
{
	b3Color illumination;

	illumination.b3InitFactor(m_ShadowFactor);

	// No shadow => surface in light
	if (Jit->shape == null)
	{
		b3_f64 ShapeAngle = b3Vector::b3SMul(&surface->incoming->normal, &Jit->dir);
		b3_f32 factor;

		// specular high light
		if (ShapeAngle >= 0)
		{
			b3_u32 spec_exp = (b3_u32)surface->m_SpecularExp;

			if (spec_exp < 100000) // test if surface if rough
			{
				b3_f64 lambda = b3Vector::b3SMul(&surface->refl_ray.dir,&Jit->dir);
				
				factor = b3Math::b3FastPow(lambda, spec_exp) * Jit->m_LightFrac;
				surface->m_SpecularSum += (light->m_Color * factor);
			}

			// surface illumination (diffuse color)
			factor = ShapeAngle * Jit->m_LightFrac * 0.5 - m_ShadowFactor;
			if (factor >= 0)
			{
				illumination += (light->m_Color * factor);
			}
		}
	}

	result += (surface->m_Diffuse * illumination);
}

void b3ShaderMork2::b3ComputeFresnelCoeffs(b3_surface *surface, b3_f64 &refl, b3_f64 &refr)
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
	b3_surface &surface,
	b3_count    depth_count)
{
	b3Color   result;
	b3Item   *item;
	b3Light  *light;
	b3_ray   *ray = surface.incoming;
	b3_f64    refl,refr,factor;

	// Refraction
	if (surface.m_Transparent)
	{
		if (surface.m_Ior == 1)
		{
			surface.refr_ray.inside = false;
			surface.refl_ray.inside = false;
		}
		b3ComputeFresnelCoeffs(&surface,refl,refr);

		b3Shade(&surface.refr_ray,depth_count);
		result = (surface.refr_ray.color * refr);
	}
	else
	{
		if (surface.m_Ior != 1.0)
		{
			// simulate dielectric metal
			b3ComputeFresnel(&surface);
			refl = surface.m_Reflection * (1 - surface.m_Reflection + surface.m_Fresnel);
		}
		else
		{
			// plastic reflection
			refl = surface.m_Reflection;
		}
		refr = 0;
		result.b3Init();
	}

	// Reflection
	if (refl > 0)
	{
		b3Shade(&surface.refl_ray,depth_count);
		result += (surface.refl_ray.color * refl);
	}
	else
	{
		refl = 0;
	}

	// Mix colors
	factor = (1.0 - refl - refr);
	if (factor > 0)
	{
		// For each light source
		ray->color.b3Init();
		surface.m_SpecularSum.b3Init();
		B3_FOR_BASE(m_Scene->b3GetLightHead(),item)
		{
			light = (b3Light *)item;
			light->b3Illuminate(this,&surface);
		}
		ray->color = ray->color * factor + result + surface.m_SpecularSum;
	}
	else
	{
		ray->color = result;
	}
}
