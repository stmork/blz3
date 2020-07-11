/*
**
**	$Filename:	b3Shade.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for root scene
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_SHADE_H
#define B3_RAYTRACE_SHADE_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Scene.h"

class b3Light;
class b3Nebular;

#define NO_TRANSPARENT_SHADING

/**
 * This base class describes the shading process.
 *
 * @see b3Material.
 * @see b3Scene.
 * @see b3Light.
 * @see b3Nebular.
 */
class B3_PLUGIN b3Shader
{
protected:
	b3Scene        *        m_Scene;      //!< The scene to shade through the raytracer.
	b3Nebular       *       m_Nebular;    //!< The nebular definition if any.
	b3_count                m_TraceDepth; //!< The maximum recursion depth.

public:
	/**
	 * This constructor initializes the shader with the given scene.
	 *
	 * @param scene The scene to shade.
	 */
	b3Shader(b3Scene * scene);

	virtual ~b3Shader() = default;

	/**
	 * This method prepares the shader for use.
	 */
	virtual void            b3Prepare(b3_preparation_info * prep_info);

	/**
	 * This method collects all necessary values for shading. These components are:
	 * #- The intersection point if any.
	 * #- material
	 * #- surface normal incl. bump mapping.
	 * #- reflective result.
	 * #- refractive result.
	 * These components are collected in a b3_surface structure and combined
	 * by the b3ShadeSurface() method.
	 *
	 * @param ray The tracing ray.
	 * @param depth The maximal raytrace recursion depth for reflection or refraction.
	 * @return True if an intersection point was found.
	 * @see b3RayRow
	 */
	b3_bool         b3Shade(b3_ray * ray, b3_count depth = 0);

	/**
	 * This method decides between material shading and light shading. If material
	 * shading is selected the b3Material::b3Illuminate() method is called
	 * otherwise light shading is called through the b3ShadeLight() method.
	 *
	 * The material pointer in the b3_surface structure must be non null. Material
	 * shading is provided if the b3Material::b3Illuminate() method of this material computes
	 * the shading and returns true for that as signal.
	 *
	 * @param light The light source for lighting
	 * @param jit The lighting info.
	 * @param surface The surface values needed for shading.
	 */
	void            b3Shade(b3Light * light, b3_light_info * jit, b3_surface * surface);

	/**
	 * This method find any obscurer so that an intersection point may be
	 * not lighted.
	 *
	 * @param ray A ray from the intersection point to the light source.
	 * @param max The distance between the intersection point and the light source as limit.
	 * This is because shapes behind the light source doesn't matter.
	 * @return True if an obscurer was found.
	 */
	inline  b3_bool         b3FindObscurer(b3_ray * ray, b3_f64 max = DBL_MAX)
	{
		return m_Scene->b3Intersect(ray, false, max);
	}

protected:
	/**
	 * This method combines all surface values for shading. These components are:
	 * #- The intersection point.
	 * #- material
	 * #- surface normal incl. bump mapping.
	 * #- reflective result.
	 * #- refractive result.
	 * These components are collected in the b3_surface structure and combined
	 * by this method.
	 *
	 * @param surface The collected surface values.
	 * @param depth The maximal raytrace recursion depth for reflection or refraction.
	 */
	virtual void            b3ShadeSurface(b3_surface * surface, b3_count depth) = 0;

	/**
	 * This method mixes the surface lighting for one light source. This may
	 * be done multiple times for one intersection point if more light sources
	 * are enabled.
	 *
	 * @param light The light source for lighting.
	 * @param jit The position information of the light source.
	 * @param surface The surface values to mix.
	 */
	virtual void            b3ShadeLight(b3Light * light, b3_light_info * jit, b3_surface * surface) = 0;

protected:
	/**
	 * This method computes the output rays depending on the surface values.
	 * The reflection ray is computed if the reflectance is higher than zero.
	 * The refraction ray is computed if the refractance is higher than zero
	 * and no total reflection occurs.
	 */
	void                    b3ComputeOutputRays(b3_surface * surface);

	/**
	 * This method computes the Fresnel terms of the given surface.
	 *
	 * @param surface The surface values.
	 * @return The resulting intensity level.
	 */
	b3_f64                  b3ComputeFresnel(b3_surface * surface);

	/**
	 * This method illuminates the given surface by all activated
	 * light sources.
	 *
	 * @param surface The surface to illuminate. The surface also contains the intersection point.
	 * @see b3Light::b3Illuminate().
	 */
	inline void b3Illuminate(b3_surface * surface)
	{
		b3Item   *   item;
		b3Light   *  light;

		B3_FOR_BASE(m_Scene->b3GetLightHead(), item)
		{
			light = (b3Light *)item;
			if(light->b3IsActive())
			{
				light->b3Illuminate(this, surface);
			}
		}
	}
};

///////////////
// Phong shader
///////////////

/**
 * This class represents a classical Phong shader.
 */
class B3_PLUGIN b3ShaderPhong : public b3Shader
{
public:
	/**
	 * This constructor initializes the shader with the given scene.
	 *
	 * @param scene The scene to shade.
	 */
	b3ShaderPhong(b3Scene * scene);

protected:
	void    b3ShadeSurface(b3_surface * surface, b3_count depth);
	void    b3ShadeLight(b3Light * light, b3_light_info * jit, b3_surface * surface);
};

//////////////
// Mork shader
//////////////

/**
 * This class handles a more improved shader called a Mork shader.
 */
class B3_PLUGIN b3ShaderMork : public b3Shader
{
	b3_f32 m_ShadowFactor;

public:
	/**
	 * This constructor initializes the shader with the given scene.
	 *
	 * @param scene The scene to shade.
	 */
	b3ShaderMork(b3Scene * scene);

	void     b3Prepare(b3_preparation_info * prep_info);

protected:
	void     b3ShadeSurface(b3_surface * surface, b3_count depth);
	void     b3ShadeLight(b3Light * light, b3_light_info * jit, b3_surface * surface);
};

///////////////////////
// Enhanced Mork shader
///////////////////////

/**
 * This class handles the enhancement of the Mork shader.
 *
 * @see b3ShaderMork.
 */
class B3_PLUGIN b3ShaderMork2 : public b3Shader
{
	b3_f32 m_ShadowFactor;

public:
	/**
	 * This constructor initializes the shader with the given scene.
	 *
	 * @param scene The scene to shade.
	 */
	b3ShaderMork2(b3Scene * scene);

	void     b3Prepare(b3_preparation_info * prep_info);

protected:
	void     b3ShadeSurface(b3_surface * surface, b3_count depth);
	void     b3ShadeLight(b3Light * light, b3_light_info * jit, b3_surface * surface);

private:
	void     b3ComputeFresnelCoeffs(b3_surface * surface, b3_f32 & refl, b3_f32 & refr);
};

#endif
