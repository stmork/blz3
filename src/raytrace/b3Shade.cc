/*
**
**	$Filename:	b3Shade.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - The shading dispatcher
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.61  2006/04/29 11:25:49  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**
**	Revision 1.60  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.59  2005/12/19 10:45:18  sm
**	- Some locale specified.
**	
**	Revision 1.58  2005/06/08 14:16:24  smork
**	- Some more vectorization.
**	
**	Revision 1.57  2005/04/27 13:55:02  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**	
**	Revision 1.56  2005/01/03 10:34:30  smork
**	- Rebalanced some floating point comparisons:
**	  a == 0  -> b3Math::b3NearZero
**	  a == b  -> b3Math::b3IsEqual
**	- Removed some very inlikely fp comparisons
**	  in intersection methods.
**	
**	Revision 1.55  2004/10/05 09:29:22  sm
**	- Donw some documentations.
**	
**	Revision 1.54  2004/09/28 15:07:40  sm
**	- Support for car paint is complete.
**	- Made some optimizations concerning light.
**	- Added material dependend possibility for color
**	  mixing instead of mixing inside shader.
**	
**	Revision 1.53  2004/09/17 20:57:53  sm
**	- Material shader add their color components to jit.
**	- Grizzle fix to Mork 2 shader: The reflective and refractive color
**	  is initialized when coefficents are zero.
**	
**	Revision 1.52  2004/09/17 12:53:55  sm
**	- Changed chader signatures to sum on different color
**	  channels (ambient, diffuse and specular). I wanted
**	  to do this for a long time, puh!
**	
**	Revision 1.51  2004/07/27 16:33:50  sm
**	- Added thin film material rendering
**	
**	Revision 1.50  2004/06/23 13:58:07  sm
**	- Changed Fresnel computation decision from real transparent
**	  to real refractive to force Fresnel computation even on
**	  total reflection.
**	
**	Revision 1.49  2004/06/23 11:02:54  sm
**	- Fixed material shader problem in Mork shading model: The half factor
**	  moved into the lighting method.
**	
**	Revision 1.48  2004/05/27 13:13:56  sm
**	- Optimized Mork shader
**	- Removed b3ShadePostMaterial
**	- Removed m_SpecularSum
**	
**	Revision 1.47  2004/05/26 14:30:02  sm
**	- Added Fresnel energy distribution to transparent materials
**	  with index of refraction > 0.
**	
**	Revision 1.46  2004/05/26 12:47:20  sm
**	- Optimized recursive shading
**	- Optimized pow to an integer version (b3Math::b3FastPow)
**	
**	Revision 1.45  2004/05/26 07:20:27  sm
**	- Renamed transparent member.
**	
**	Revision 1.44  2004/05/25 19:17:23  sm
**	- Some reflection spin controls didn't map input.
**	- Divided Fresnel computation and reflection/refraction
**	  mixing into two parts.
**	
**	Revision 1.43  2004/05/25 12:14:48  sm
**	- Compute Fresnel term in separate method.
**	
**	Revision 1.42  2004/05/24 20:25:13  sm
**	- Fresnel experiments
**	
**	Revision 1.41  2004/05/23 20:52:34  sm
**	- Done some Fresnel formula experiments.
**	
**	Revision 1.40  2004/05/23 15:04:19  sm
**	- Some optimizations
**	
**	Revision 1.39  2004/05/23 13:51:14  sm
**	- Some shader cleanups
**	
**	Revision 1.38  2004/05/22 17:02:56  sm
**	- Decoupled material shader.
**	
**	Revision 1.37  2004/05/22 14:17:31  sm
**	- Merging some basic raytracing structures and gave them some
**	  self explaining names. Also cleaned up some parameter lists.
**	
**	Revision 1.36  2004/05/20 19:10:30  sm
**	- Separated shader from scene. this is easier
**	  to handle.
**	
**	Revision 1.35  2004/05/13 16:17:26  sm
**	- Added background clouds as special item.
**	
**	Revision 1.34  2004/05/13 11:22:19  sm
**	- Added animated clouds.
**	
**	Revision 1.33  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.32  2004/04/22 14:35:16  sm
**	- Optimized clouds by making them inline.
**	
**	Revision 1.31  2004/04/22 14:28:44  sm
**	- Adjusted clouds.
**	
**	Revision 1.30  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.29  2004/04/11 19:04:21  sm
**	- Renamed b3Material::b3GetColors into b3Material::b3GetSurfaceValues
**	
**	Revision 1.28  2004/04/11 18:21:36  sm
**	- Raytracer redesign:
**	  o The complete set of surface values moved into
**	    the b3_surface data structure when calling b3GetColors()
**	
**	Revision 1.27  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.26  2004/04/03 14:07:18  sm
**	- Resolved internal compiler error problem of VC++
**	
**	Revision 1.25  2004/04/02 08:56:45  sm
**	- Computed more realistic clouds.
**	
**	Revision 1.24  2004/03/01 14:00:32  sm
**	- Ready to go for Cook/Torrance reflectance model.
**	
**	Revision 1.23  2004/02/28 19:10:13  sm
**	- Cook/Torrance is applicable by use through material
**	  shader.
**	
**	Revision 1.22  2003/03/04 20:37:39  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.21  2002/12/11 14:47:58  sm
**	- Changed noise handling to static
**	- Fixed some error cases when image not loaded.
**	- Added some brt3 flags
**	
**	Revision 1.20  2002/08/23 15:34:28  sm
**	- Added time support to water animation.
**	- Added multiple file format types to brt3.
**	
**	Revision 1.19  2002/02/28 16:58:46  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.18  2002/02/18 17:50:32  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.17  2002/02/17 21:25:07  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.16  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.15  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.14  2001/10/22 14:47:38  sm
**	- Type correction vor b3Base/b3Link. So fixed a bad behaviour
**	  on Windows.
**	- Some minor fixes.
**	
**	Revision 1.13  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.12  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.11  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.10  2001/10/10 17:52:24  sm
**	- Texture loading (only reading into memory) running.
**	- Raytracing without OpenGL must be possible!
**	
**	Revision 1.9  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	Revision 1.8  2001/10/06 19:56:00  sm
**	- Fixing bugs concerning reflection and
**	  refraction computation (both: direction
**	  computation and shading)
**	
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
