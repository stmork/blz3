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
**	Revision 1.40  2004/05/28 19:35:39  sm
**	- Added Mork shader enhancement as new extra shader.
**
**	Revision 1.35  2004/05/26 14:30:02  sm
**	- Added Fresnel energy distribution to transparent materials
**	  with index of refraction > 0.
**	
**	Revision 1.34  2004/05/26 12:47:20  sm
**	- Optimized recursive shading
**	- Optimized pow to an integer version (b3Math::b3FastPow)
**	
**	Revision 1.33  2004/05/26 07:20:27  sm
**	- Renamed transparent member.
**	
**	Revision 1.32  2004/05/25 19:17:23  sm
**	- Some reflection spin controls didn't map input.
**	- Divided Fresnel computation and reflection/refraction
**	  mixing into two parts.
**	
**	Revision 1.31  2004/05/25 12:14:48  sm
**	- Compute Fresnel term in separate method.
**	
**	Revision 1.30  2004/05/23 20:52:34  sm
**	- Done some Fresnel formula experiments.
**	
**	Revision 1.29  2004/05/23 15:04:19  sm
**	- Some optimizations
**	
**	Revision 1.28  2004/05/23 13:51:14  sm
**	- Some shader cleanups
**	
**	Revision 1.27  2004/05/22 17:02:56  sm
**	- Decoupled material shader.
**	
**	Revision 1.26  2004/05/22 14:17:31  sm
**	- Merging some basic raytracing structures and gave them some
**	  self explaining names. Also cleaned up some parameter lists.
**	
**	Revision 1.25  2004/05/20 19:10:30  sm
**	- Separated shader from scene. this is easier
**	  to handle.
**	
**	Revision 1.24  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.23  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.22  2004/04/11 19:04:21  sm
**	- Renamed b3Material::b3GetColors into b3Material::b3GetSurfaceValues
**	
**	Revision 1.21  2004/04/11 18:21:36  sm
**	- Raytracer redesign:
**	  o The complete set of surface values moved into
**	    the b3_surface data structure when calling b3GetColors()
**	
**	Revision 1.20  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.19  2004/02/29 18:44:55  sm
**	- Further shader development
**	
**	Revision 1.18  2004/02/28 19:10:13  sm
**	- Cook/Torrance is applicable by use through material
**	  shader.
**	
**	Revision 1.17  2003/03/04 20:37:39  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.16  2002/12/31 16:01:03  sm
**	- Fixed wrong vector multiplication.
**	
**	Revision 1.15  2002/07/26 09:13:33  sm
**	- Found alpha problem: the Linux OpenGL renderer didn't use the
**	  b3RenderContext::b3Init() method! Now everything function very well:-)
**	- The Un*x OpenGL renderer has got a key press interface now.
**	- Corrected spot lights
**	- Textures needn't to be square any more (some less memory usage)
**	
**	Revision 1.14  2002/07/25 13:22:32  sm
**	- Introducing spot light
**	- Optimized light settings when drawing
**	- Further try of stencil maps
**	
**	Revision 1.13  2002/07/21 17:02:36  sm
**	- Finished advanced color mix support (correct Phong/Mork shading)
**	- Added first texture mapping support. Further development on
**	  Windows now...
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
**	Revision 1.7  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.6  2001/10/19 19:43:15  sm
**	- Searching for 5 percent performance lost...
**	
**	Revision 1.5  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.4  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
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

/*************************************************************************
**                                                                      **
**                        b3ShaderMork implementation                   **
**                                                                      **
*************************************************************************/

b3ShaderMork::b3ShaderMork(b3Scene *scene) : b3Shader(scene)
{
}

void b3ShaderMork::b3Prepare()
{
	b3Shader::b3Prepare();
	m_ShadowFactor = m_Scene->m_ShadowBrightness / m_Scene->b3GetLightCount();
}

void b3ShaderMork::b3ShadeLight(
	b3Light       *light,
	b3_light_info *Jit,
	b3_surface    *surface,
	b3Color       &result)
{
	b3_f64   ShapeAngle,Factor;
	b3Color  filter;

	// Real absorption
	result += (surface->m_Diffuse * m_ShadowFactor);

	filter.b3Init();

	// No shadow => surface in light
	if (Jit->shape == null)
	{
		// specular high light
		if ((ShapeAngle =
			surface->incoming->normal.x * Jit->dir.x +
			surface->incoming->normal.y * Jit->dir.y +
			surface->incoming->normal.z * Jit->dir.z) >= 0)
		{
			b3_f64 lambda = b3Vector::b3SMul(&surface->refl_ray.dir,&Jit->dir);

			if ((surface->m_SpecularExp < 100000) && (lambda > 0))
			{
				Factor = b3Math::b3FastPow(lambda, (b3_u32)surface->m_SpecularExp) * Jit->m_LightFrac;
				surface->m_SpecularSum += (light->m_Color * Factor);
			}
		}
		else
		{
			ShapeAngle = 0;
		}

		// surface illumination (diffuse color)
		if ((Factor = ShapeAngle * Jit->m_LightFrac - m_ShadowFactor) > 0)
		{
			filter = light->m_Color * Factor;
		}
	}

	result += (surface->m_Diffuse * filter);
}

void b3ShaderMork::b3ShadeSurface(
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
		refl = surface.m_Reflection;
		refr = surface.m_Refraction;

		b3Shade(&surface.refr_ray,depth_count + 1);
		result = (surface.refr_ray.color * refr);
	}
	else
	{
		refl = surface.m_Reflection;
		refr = 0;
		result.b3Init();
	}

	// Reflection
#if 0
	if (((!ray->inside) || (!surface.m_Transparent)) && (refl > 0))
#else
	if (refl > 0)
#endif
	{
		b3Shade(&surface.refl_ray,depth_count + 1);
		result += (surface.refl_ray.color * refl);
	}
	else
	{
		refl = 0;
	}

	// Mix colors
	factor = (1.0 - refl - refr) * 0.5;
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
		ray->color =
			ray->color * factor +
			result +
			surface.m_SpecularSum;
	}
	else
	{
		ray->color = result;
	}
}
