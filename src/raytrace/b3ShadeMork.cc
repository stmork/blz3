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

#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3BBox.h" 
#include "blz3/raytrace/b3Animation.h"
#include "blz3/raytrace/b3Special.h"  
#include "blz3/raytrace/b3Scene.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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


b3SceneMork::b3SceneMork(b3_u32 class_type) : b3Scene(sizeof(b3SceneMork), class_type)
{
	b3PrintF(B3LOG_DEBUG,"Using Mork shading...\n");
}

b3SceneMork::b3SceneMork(b3_u32 *src) : b3Scene(src)
{
	b3PrintF(B3LOG_DEBUG,"Using Mork shading...\n");
}

void b3SceneMork::b3Illuminate(
	b3Light       *light,
	b3_light_info *Jit,
	b3_ray_fork   *surface,
	b3Color       &result,
	b3Material    *material)
{
	b3Color aux = b3Color(0,0,0);

	if ((material != null) && material->b3Illuminate(surface,Jit,aux))
	{
		result += aux * 2.0;
	}
	else
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
				if (surface->m_SpecularExp < 100000)
				{
					Factor = log ((
						surface->refl_ray.dir.x * Jit->dir.x +
						surface->refl_ray.dir.y * Jit->dir.y +
						surface->refl_ray.dir.z * Jit->dir.z + 1) * 0.5);

					Factor = exp (Factor * surface->m_SpecularExp) * Jit->LightFrac;
					surface->m_SpecularSum += (light->m_Color * Factor);
				}
			}
			else
			{
				ShapeAngle = 0;
			}

			// surface illumination (diffuse color)
			if ((Factor = ShapeAngle * Jit->LightFrac - m_ShadowFactor) > 0)
			{
				filter = light->m_Color * Factor;
			}
		}

		result += (surface->m_Diffuse * filter);
	}
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
	if ((LightDist = b3Vector::b3QuadLength(&LightDir)) == 0)
	{
		return false;
	}
 	LightDist  = 1.0 / sqrt(LightDist);
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

	Ray->color = Light->m_Color * Factor;
	return true;
}

void b3SceneMork::b3LightFlare (b3_ray_info *Ray)
{
	b3Item    *item;
	b3Light   *Light;
	b3_vector  toLight;
	b3_f64     angle,reverse;

	B3_FOR_BASE(b3GetLightHead(),item)
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
		Ray->color = Light->m_Color * angle + Ray->color * reverse;
	}
}

b3_bool b3SceneMork::b3Shade(
	b3_ray_info *ray,
	b3_count     depth_count)
{
	b3Item      *item;
	b3Light     *light;
	b3BBox      *bbox;
	b3Shape     *shape;
	b3_ray_fork  surface;
	b3_f64       refl,refr,factor;
	b3_bool      transparent;
	b3_bool      result = false;

	surface.m_SpecularSum.b3Init();
	ray->color.b3Init();

	// Normalize incoming ray
	if (b3Vector::b3Normalize(&ray->dir) == 0)
	{
		return false;
	}

	if (b3Intersect(ray))
	{
		b3Material *material = null;

		// Initialize some values
		// Compute intersection point "ipoint"
		// Get hit material
		// Compute normal at ipoint and bump normal
		// Get material values
		bbox  = ray->bbox;
		shape = ray->shape;
		surface.incoming = ray;

		// Compute intersection point
		ray->ipoint.x = ray->pos.x + ray->Q * ray->dir.x;
		ray->ipoint.y = ray->pos.y + ray->Q * ray->dir.y;
		ray->ipoint.z = ray->pos.z + ray->Q * ray->dir.z;

		// Compute rel. box coordinates
		bbox->b3ComputeBoxPolar(&ray->ipoint,&ray->polar.m_BoxPolar);

		// Compute surface values
		material = shape->b3GetSurfaceValues(ray,&surface);
		shape->b3BumpNormal(ray);

		// Where to shoot subsequent rays...
		transparent = b3ComputeOutputRays(&surface);
		if (transparent)
		{
			if (surface.m_Ior == 1)
			{
				surface.refr_ray.inside = false;
				surface.refl_ray.inside = false;
			}
			refr = surface.m_Refraction;
		}
		else
		{
			refr = 0;
		}
		memset(&surface.refr_ray.color,0,sizeof(surface.refr_ray.color));

		if (depth_count <= m_TraceDepth)
		{
			// Reflection
			if (((!ray->inside) || (!transparent)) && (surface.m_Reflection > 0))
			{
				refl = surface.m_Reflection;
				if (!b3Shade(&surface.refl_ray,depth_count + 1))
				{
					b3GetInfiniteColor(&surface.refl_ray);
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
					b3GetInfiniteColor(&surface.refr_ray);
				}
			}
		}
		else
		{
			refl = 0;
		}

		// For each light source
		B3_FOR_BASE(b3GetLightHead(),item)
		{
			light = (b3Light *)item;
			light->b3Illuminate(this,&surface,material);
		}

		// Mix colors
		factor = (1.0 - refl - refr) * 0.5;
		if (factor > 0)
		{
			ray->color *= factor;
		}
		else
		{
			ray->color.b3Init();
		}

		if (refl > 0)
		{
			ray->color += (surface.refl_ray.color * refl);
		}
		if (refr > 0)
		{
			ray->color += (surface.refr_ray.color * refr);
		}

		ray->color += surface.m_SpecularSum;

		if (m_Nebular != null)
		{
			m_Nebular->b3ComputeNebular(ray->color,ray->color,ray->Q);
		}
		result = true;
	}
	else
	{
		if (m_Nebular != null)
		{
			m_Nebular->b3GetNebularColor(ray->color);
			result = true;
		}
		else
		{
			B3_FOR_BASE(b3GetLightHead(),item)
			{
				b3IsPointLightBackground((b3Light *)item,ray);
			}
			result = false;
		}
	}

	return result;
}

void b3SceneMork::b3SetLights(b3RenderContext *context)
{
	b3Item   *item;
	b3Light  *light;
	b3Color   ambient;
	b3Color   diffuse;
	b3Color   black;
	b3_count  count = 0;

	black.b3Init();
	ambient.b3Init(m_ShadowBrightness,m_ShadowBrightness,m_ShadowBrightness);

	context->b3LightNum();
	context->b3LightReset();
	B3_FOR_BASE(b3GetLightHead(),item)
	{
		light = (b3Light *)item;
		if (light->b3IsActive())
		{
			// Use the same color for diffuse and specular
			diffuse = light->m_Color * 0.5;
			if (context->b3LightAdd(
				&light->m_Position,
				light->m_SpotActive ? &light->m_Direction : null,
				b3ComputeSpotExponent(light),
				&diffuse,
				&black,
				&diffuse))
			{
				count++;
			}
		}
	}
	if (count > 0)
	{
		ambient = ambient / count;
		context->b3SetAmbient(ambient);
	}
}
