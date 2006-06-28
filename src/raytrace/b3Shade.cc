/*
**
**	$Filename:	b3Shade.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - The shading dispatcher
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Some global shading helpers                   **
**                                                                      **
*************************************************************************/

b3Shader::b3Shader(b3Scene *scene) : m_Scene(scene)
{
	b3_scene_preparation info;

	info.m_Scene = scene;
	info.m_t     = scene->b3GetTimePoint();
	b3Prepare(&info);
}

void b3Shader::b3Prepare(b3_preparation_info *prep_info)
{
	m_Nebular    = m_Scene->m_Nebular;
	m_TraceDepth = m_Scene->m_TraceDepth;
}

void b3Shader::b3ComputeOutputRays(b3_surface *surface)
{
	b3_vector64 *Normal       = &surface->m_Incoming->normal;
	b3_vector64 *incoming_dir = &surface->m_Incoming->dir;
	b3_vector64 *refl_dir     = &surface->m_ReflRay.dir;
	b3_vector64 *refr_dir     = &surface->m_RefrRay.dir;
	b3_f64       Factor,cos_a,ior,ior_sqr;

	Factor = 2 * (cos_a =
		incoming_dir->x * Normal->x +
		incoming_dir->y * Normal->y +
		incoming_dir->z * Normal->z);
	b3Vector::b3LinearCombine(incoming_dir, Normal, -Factor, refl_dir);
	b3Vector::b3Normalize(refl_dir);

	surface->m_ReflRay.pos    = surface->m_Incoming->ipoint;
	surface->m_ReflRay.inside = surface->m_Incoming->inside;
	surface->m_Transparent    = false;

	// Use only sharp angles
	if (cos_a >= 0)
	{
		Normal->x = -Normal->x;
		Normal->y = -Normal->y;
		Normal->z = -Normal->z;
		cos_a     = -cos_a;
	}
	surface->m_CosAlpha = -cos_a;

	surface->m_IorComputed = ior = surface->m_Incoming->inside ? surface->m_Ior : 1.0 / surface->m_Ior;

	if (surface->m_Refraction > 0)
	{
		if (fabs(cos_a) < 1)
		{
			ior_sqr = ior * ior;
			Factor = 1 - ior_sqr + ior_sqr * cos_a * cos_a;

			// Test total reflection
			if (Factor >= 0)
			{
				b3Vector::b3LinearCombine(incoming_dir, Normal, ior, -sqrt(Factor) - ior * cos_a, refr_dir);
				b3Vector::b3Normalize(refr_dir);

				surface->m_RefrRay.pos    =  surface->m_Incoming->ipoint;
				surface->m_RefrRay.inside = !surface->m_Incoming->inside;
				surface->m_Transparent    = true;
			}
		}
		else
		{
			// Perpendicular incoming ray so refracting ray is
			// upside down.
			surface->m_RefrRay.pos    =  surface->m_Incoming->ipoint;
			surface->m_RefrRay.dir    =  surface->m_Incoming->dir;
			surface->m_RefrRay.inside = !surface->m_Incoming->inside;
			surface->m_Transparent    = true;
		}
	}

}

b3_f64 b3Shader::b3ComputeFresnel(b3_surface *surface)
{
	b3_f64 ica,ica_sqr,ica_pow5,R0;

	// Compute Fresnel factor for unpolarized light using
	// Christphe Schlick's hack.
	ica      = 1.0 - surface->m_CosAlpha;
	ica_sqr  = ica * ica;
	ica_pow5 = ica * ica_sqr * ica_sqr;
	R0       = (surface->m_IorComputed - 1.0) / (surface->m_IorComputed + 1);
	R0      *= R0;

	return surface->m_Fresnel = R0 + (1.0 - R0) * ica_pow5;
}

b3_bool b3Shader::b3Shade(
	b3_ray   *ray,
	b3_count  depth_count)
{
	b3BBox      *bbox;
	b3Shape     *shape;
	b3_surface   surface;
	b3_bool      finite;

	// Normalize incoming ray
	b3Vector::b3Normalize(&ray->dir);

	if ((depth_count < m_TraceDepth) && m_Scene->b3Intersect(ray, (b3_bool)(depth_count == 0)))
	{
		bbox  = ray->bbox;
		shape = ray->shape;
		surface.m_Incoming = ray;

		// Compute intersection point
		b3Vector::b3LinearCombine(&ray->pos, &ray->dir, ray->Q, &ray->ipoint);

		// Compute rel. box coordinates
		bbox->b3ComputeBoxPolar(ray);

		// Do bump mapping
		shape->b3BumpNormal(ray);

		// Get material values
		ray->material = shape->b3GetSurfaceValues(&surface);

		// Compute geometry
		b3ComputeOutputRays(&surface);

		// This does the shading
		b3ShadeSurface(&surface,depth_count + 1);

		// Post process nebular
		if (m_Nebular != null)
		{
			m_Nebular->b3ComputeNebular(ray->color,ray->color,ray->Q);
		}
		finite = true;
	}
	else
	{
		// Post process nebular
		if (m_Nebular != null)
		{
			m_Nebular->b3GetNebularColor(ray->color);
			finite = true;
		}
		else if (depth_count > 0)
		{
			m_Scene->b3GetInfiniteColor(ray);
			finite = true;
		}
		else
		{
			ray->color.b3Init();
			finite = false;
		}
	}
	return finite;
}

void b3Shader::b3Shade(b3Light *light,b3_light_info *jit,b3_surface *surface)
{
	b3Material *material = surface->m_Incoming->material;

	if (!((material != null) && material->b3Illuminate(surface,jit)))
	{
		b3ShadeLight(light, jit, surface);
	}
}
