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
**	Revision 1.35  2004/09/28 15:07:41  sm
**	- Support for car paint is complete.
**	- Made some optimizations concerning light.
**	- Added material dependend possibility for color
**	  mixing instead of mixing inside shader.
**
**	Revision 1.34  2004/09/17 14:48:12  sm
**	- I have forgotten the area lights. Now sampling is correct by moving
**	  the color sum from surface to Jit (light info).
**	
**	Revision 1.33  2004/09/17 12:53:55  sm
**	- Changed chader signatures to sum on different color
**	  channels (ambient, diffuse and specular). I wanted
**	  to do this for a long time, puh!
**	
**	Revision 1.32  2004/06/23 11:02:54  sm
**	- Fixed material shader problem in Mork shading model: The half factor
**	  moved into the lighting method.
**	
**	Revision 1.31  2004/05/28 14:39:01  sm
**	- Changed spacular exponent to integer value inside shader.
**	
**	Revision 1.30  2004/05/28 14:06:29  sm
**	- Minor optimizations in shader
**	
**	Revision 1.29  2004/05/28 13:15:39  sm
**	- Major optimizations inside shader. But why is the intel brt3
**	  5 minutes slower than the unoptimized version?
**	
**	Revision 1.28  2004/05/27 13:13:56  sm
**	- Optimized Mork shader
**	- Removed b3ShadePostMaterial
**	- Removed m_SpecularSum
**	
**	Revision 1.27  2004/05/26 14:30:02  sm
**	- Added Fresnel energy distribution to transparent materials
**	  with index of refraction > 0.
**	
**	Revision 1.26  2004/05/26 12:47:20  sm
**	- Optimized recursive shading
**	- Optimized pow to an integer version (b3Math::b3FastPow)
**	
**	Revision 1.25  2004/05/26 07:20:27  sm
**	- Renamed transparent member.
**	
**	Revision 1.24  2004/05/23 15:04:19  sm
**	- Some optimizations
**	
**	Revision 1.23  2004/05/22 17:02:56  sm
**	- Decoupled material shader.
**	
**	Revision 1.22  2004/05/22 14:17:31  sm
**	- Merging some basic raytracing structures and gave them some
**	  self explaining names. Also cleaned up some parameter lists.
**	
**	Revision 1.21  2004/05/20 19:10:30  sm
**	- Separated shader from scene. this is easier
**	  to handle.
**	
**	Revision 1.20  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.19  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.18  2004/04/11 19:04:21  sm
**	- Renamed b3Material::b3GetColors into b3Material::b3GetSurfaceValues
**	
**	Revision 1.17  2004/04/11 18:21:36  sm
**	- Raytracer redesign:
**	  o The complete set of surface values moved into
**	    the b3_surface data structure when calling b3GetColors()
**	
**	Revision 1.16  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.15  2004/03/01 14:00:32  sm
**	- Ready to go for Cook/Torrance reflectance model.
**	
**	Revision 1.14  2004/02/28 19:10:13  sm
**	- Cook/Torrance is applicable by use through material
**	  shader.
**	
**	Revision 1.13  2003/03/04 20:37:39  sm
**	- Introducing new b3Color which brings some
**	  performance!
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
**	Revision 1.7  2001/12/23 08:57:21  sm
**	- Fixed recursive calling bug in b3IsObscured(...)
**	- Minor intersection optimazations done.
**	
**	Revision 1.6  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.5  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.4  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
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
		b3_f32 ShapeAngle = b3Vector::b3SMul(&surface->incoming->normal, &Jit->dir);

		if (ShapeAngle >= 0) // test for far side to light
		{
			b3_u32 spec_exp = (b3_u32)surface->m_SpecularExp;

			if (spec_exp < 100000) // test if surface if rough
			{
				b3_f64 lambda = b3Vector::b3SMul(&surface->refl_ray.dir,&Jit->dir);
				b3_f32 factor = b3Math::b3FastPow (lambda, spec_exp) * Jit->m_LightFrac;

				Jit->m_SpecularSum += (surface->m_Specular * light->m_Color * factor);
			}
			Jit->m_DiffuseSum += (surface->m_Diffuse * light->m_Color * ShapeAngle);
		}
	}
}

void b3ShaderPhong::b3ShadeSurface(
	b3_surface *surface,
	b3_count    depth_count)
{
	b3Item      *item;
	b3Light     *light;
	b3_ray      *ray = surface->incoming;
	b3_f32       refl,refr,factor;

	if (surface->m_Transparent)
	{
		if (surface->m_Ior == 1)
		{
			surface->refr_ray.inside = false;
			surface->refl_ray.inside = false;
		}
		refr = surface->m_Refraction;
		b3Shade(&surface->refr_ray,depth_count);
	}
	else
	{
		refr = 0;
	}

	refl = surface->m_Reflection;
	if (refl > 0)
	{
		b3Shade(&surface->refl_ray,depth_count);
	}

	surface->m_DiffuseSum.b3Init();
	surface->m_SpecularSum.b3Init();
	B3_FOR_BASE(m_Scene->b3GetLightHead(),item)
	{
		light = (b3Light *)item;
		light->b3Illuminate(this,surface);
	}

	if (!ray->material->b3MixComponents(surface, refl, refr))
	{
		factor = (1.0 - refl - refr);
		ray->color =
			surface->m_Ambient +
			surface->m_DiffuseSum * factor +
			surface->refl_ray.color * refl +
			surface->refr_ray.color * refr +
			surface->m_SpecularSum;
	}
}
