/*
**
**	$Filename:	b3Shade.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - The default shader
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
  
#include "blz3/b3Config.h" 
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
**                        Lens flare routine and data                   **
**                                                                      **
*************************************************************************/

#define LENSFLARE_LOOP 6
#define LENSFLARE_RING 2

static b3_f64 LensFlare_Distance[LENSFLARE_LOOP] =
{
	0.55,0.0,0.0,0.25,0.45,0.55
};

static b3_f64 LensFlare_ResultWeight[LENSFLARE_LOOP] =
{
	0.9,0.95,0.95,0.6,0.6,0.6
};

static b3_f64 LensFlare_Expon[LENSFLARE_LOOP] =
{
	2.4,1.5,1.5,2.0,7.0,4.0
};

void b3Scene::b3MixLensFlare(b3_ray_info *ray)
{
	b3Light   *light;
	b3_vector  view,central,toLight,nLight,nView;
	b3Color    result;
	b3_f64     factor,distance,weight = 0.6;
	b3_count   i;

	view.x = m_ViewPoint.x - m_EyePoint.x;
	view.y = m_ViewPoint.y - m_EyePoint.y;
	view.z = m_ViewPoint.z - m_EyePoint.z;
	factor = b3Vector::b3Length (&view);
	nView.x = view.y * ray->dir.z - view.z * ray->dir.y;
	nView.y = view.z * ray->dir.x - view.x * ray->dir.z;
	nView.z = view.x * ray->dir.y - view.y * ray->dir.x;

	for (light = b3GetLight();light != null;light = (b3Light *)light->Succ)
	{
		toLight.x = light->m_Position.x - m_ViewPoint.x;
		toLight.y = light->m_Position.y - m_ViewPoint.y;
		toLight.z = light->m_Position.z - m_ViewPoint.z;
		distance  = b3Vector::b3Length (&toLight) / factor;
		central.x = distance * view.x;
		central.y = distance * view.y;
		central.z = distance * view.z;

		b3Vector::b3CrossProduct (&view,&toLight,&nLight);

		for (i = 0;i < LENSFLARE_LOOP;i++)
		{
			b3_f64    cWeight,lWeight,angle,reverse,beta;
#ifdef TX_DISTURB
			b3_vector diff;
#endif

			cWeight  = LensFlare_Distance[i];
			lWeight  = 1.0 - cWeight;
			nLight.x = cWeight * central.x + lWeight * toLight.x;
			nLight.y = cWeight * central.y + lWeight * toLight.y;
			nLight.z = cWeight * central.z + lWeight * toLight.z;

			beta     = (
//	b3AngleOfVectors(&nLight,&ray->dir);
				nLight.x * ray->dir.x +
				nLight.y * ray->dir.y +
				nLight.z * ray->dir.z) / b3Vector::b3Length(&nLight);
			angle    = pow(beta,m_LensFlare->m_Expon * LensFlare_Expon[i]);
			if (i < LENSFLARE_RING)
			{
				if ((angle > 0.5) && (angle < 0.55)) angle = 0.3;
				else angle = 0;
			}
			else
			{
				if (angle > 0.9)  angle  = 0.9;
				else
				{
					if (angle > 0.89) angle = 0.95;
					else angle *= LensFlare_ResultWeight[i];
				}
				angle *= weight;
			}

#ifdef TX_DISTURB
			// disturbe lens flare
			diff.y  = beta;
			diff.x  = sqrt (1.0 - beta * beta);
			diff.z  = 0;
			diff.x *= 20;
			diff.y *= 20;
			if (angle < 0.95)
			{
				angle += ((Turbulence(&diff) - 0.5) * 0.1);
			}
#endif

			reverse  = 1.0 - angle;
			if (i < LENSFLARE_RING)
			{
				result.b3Init(
					ray->color[b3Color::R] * reverse + angle * light->m_Color[b3Color::R] * m_LensFlare->m_Color[b3Color::R],
					ray->color[b3Color::G],
					ray->color[b3Color::B]);
			}
			else
			{
				result = light->m_Color * m_LensFlare->m_Color * angle + ray->color * reverse;
			}
			ray->color = result;
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Some global shading helpers                   **
**                                                                      **
*************************************************************************/

void b3Scene::b3GetBackgroundColor(
	b3_ray_info *ray,
	b3_f64       lx,
	b3_f64       ly)
{
	b3_coord  x,y;
	b3_vector Dir;
	b3_f64    scaling = 5.0;
	b3_f64    sharpness = 10.2;
	b3_f64    hazyness = 0.07;
	b3_f64    r,factor,sight;

	switch (m_BackgroundType)
	{
		case TP_TEXTURE :
			x = (b3_coord)(((lx + 1) * 0.5 * (m_BackTexture->xSize - 1)));
			y = (b3_coord)(((1 - ly) * 0.5 * (m_BackTexture->ySize - 1)));
			if (x < 0)                     x = 0;
			if (x >= m_BackTexture->xSize) x = m_BackTexture->xSize - 1;
			if (y < 0)                     y = 0;
			if (y >= m_BackTexture->ySize) y = m_BackTexture->ySize - 1;

			ray->color = b3Color(m_BackTexture->b3GetValue(x,y));
			break;

		case TP_SKY_N_HELL :
			if (ray->dir.z > 0)
			{
				factor = scaling / ray->dir.z;
				Dir.x = ray->dir.x * factor;
				Dir.y = ray->dir.y * factor;
				Dir.z = 1.0;

				r = 1.0 - pow(b3Noise::b3Turbulence (&Dir),-sharpness);
				if (r < 0)
				{
					r = 0;
				}
				sight = exp(-hazyness * b3Vector::b3Length(&Dir));
				ray->color = b3Color::b3Mix(m_BottomColor,b3Color(r,r,B3_MAX(r,m_TopColor[b3Color::B])),sight);
			}
			else
			{
				ray->color = m_BottomColor;
			}

#ifdef SKY_SLIDE
			ly = ray->color[b3Color::R] * 2.0 - 1.0;
#else
			break;
#endif
		case TP_SLIDE :
			ray->color = m_AvrgColor + m_DiffColor * ly;
			break;

		default:
			ray->color.b3Init(m_ShadowBrightness,m_ShadowBrightness,m_ShadowBrightness);
			break;
	}

	if (m_LensFlare != null)
	{
		b3MixLensFlare (ray);
	}
}

void b3Scene::b3GetInfiniteColor(b3_ray_info *ray)
{
	b3_f64 ly,lx;

	ly =
		ray->dir.x * m_NormHeight.x +
		ray->dir.y * m_NormHeight.y +
		ray->dir.z * m_NormHeight.z;
	lx =
		ray->dir.x * m_NormWidth.x +
		ray->dir.y * m_NormWidth.y +
		ray->dir.z * m_NormWidth.z;

	b3GetBackgroundColor (ray,lx,ly);
}

b3_bool b3Scene::b3ComputeOutputRays(b3_ray_fork *surface)
{
	b3_vector64 *Normal       = &surface->incoming->normal;
	b3_vector64 *incoming_dir = &surface->incoming->dir;
	b3_vector64 *refl_dir     = &surface->refl_ray.dir;
	b3_vector64 *refr_dir     = &surface->refr_ray.dir;
	b3_f64       Factor,cos_a,d,ior;
	b3_bool      transparent = false;
 
	Factor = 2 * (cos_a =
		incoming_dir->x * Normal->x +
		incoming_dir->y * Normal->y +
		incoming_dir->z * Normal->z);
	refl_dir->x = incoming_dir->x - Factor * Normal->x;
	refl_dir->y = incoming_dir->y - Factor * Normal->y;
	refl_dir->z = incoming_dir->z - Factor * Normal->z;

	b3Vector::b3Normalize(refl_dir);
	surface->refl_ray.pos    = surface->incoming->ipoint;
	surface->refl_ray.inside = surface->incoming->inside;
	surface->refl_ray.t      = surface->incoming->t;

	//Use only sharp angles
	if (cos_a >= 0)
	{
		Normal->x = -Normal->x;
		Normal->y = -Normal->y;
		Normal->z = -Normal->z;
		cos_a     = -cos_a;
	}

	if (surface->m_Refraction > 0)
	{
		ior      = surface->incoming->inside ? surface->m_Ior : 1.0 / surface->m_Ior;
		refr_dir = &surface->refr_ray.dir;

		if (fabs(cos_a) < 1)
		{
			d = ior * ior;
			Factor = 1 - d + d * cos_a * cos_a;

			// Test total reflection
			if (Factor >= 0)
			{
				Factor = sqrt(Factor) + ior * cos_a;
				refr_dir->x = Factor * Normal->x - incoming_dir->x * ior;
				refr_dir->y = Factor * Normal->y - incoming_dir->y * ior;
				refr_dir->z = Factor * Normal->z - incoming_dir->z * ior;

				Factor = -1.0 / sqrt(
					refr_dir->x * refr_dir->x +
					refr_dir->y * refr_dir->y +
					refr_dir->z * refr_dir->z);
				refr_dir->x *= Factor;
				refr_dir->y *= Factor;
				refr_dir->z *= Factor;

				surface->refr_ray.pos    =  surface->incoming->ipoint;
				surface->refr_ray.inside = !surface->incoming->inside;
				surface->refr_ray.t      =  surface->incoming->t;
				transparent = true;
			}
		}
		else
		{
			// Perpendicular incoming ray so refracting ray is
			// upside down.
			surface->refr_ray.pos    =  surface->incoming->ipoint;
			surface->refr_ray.dir    =  surface->incoming->dir;
			surface->refr_ray.inside = !surface->incoming->inside;
			surface->refr_ray.t      =  surface->incoming->t;
			transparent = true;
		}
	}
	return transparent;
}

/*************************************************************************
**                                                                      **
**                        Default shader                                **
**                                                                      **
*************************************************************************/

void b3Scene::b3Illuminate(
	b3Light       *light,
	b3_light_info *Jit,
	b3_ray_fork   *surface,
	b3Color       &result,
	b3Material    *material)
{
	b3_f64 ShapeAngle;

	if ((ShapeAngle =
		surface->incoming->normal.x * Jit->dir.x +
		surface->incoming->normal.y * Jit->dir.y +
		surface->incoming->normal.z * Jit->dir.z) >= 0)
	{
		result += surface->m_Diffuse * light->m_Color * ShapeAngle;
	}
}

b3_bool b3Scene::b3Shade(b3_ray_info *ray,b3_count depth_count)
{
	b3Item      *item;
	b3Light     *light;
	b3BBox      *bbox;
	b3Shape     *shape;
	b3_ray_fork  surface;
	b3_f64       refl,refr,factor;
	b3_index     formula = 0;
	b3_bool      transparent;
	b3_bool      result = false;

	// If max raytrace depth is reached leave!
	if (depth_count > m_TraceDepth)
	{
		ray->color.b3Init();
		return false;
	}

	// Normalize incoming ray
	if (b3Vector::b3Normalize(&ray->dir) == 0)
	{
		ray->color.b3Init();
		return false;
	}

	if (b3Intersect(ray))
	{
		b3Material *material;

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
		material = shape->b3GetColors(ray,&surface);
		shape->b3BumpNormal(ray);

		ray->color = surface.m_Ambient;

		transparent = b3ComputeOutputRays(&surface);
		if (transparent)
		{
			if (surface.m_Ior == 1)
			{
				surface.refr_ray.inside = false;
				surface.refl_ray.inside = false;
			}
			refr = surface.m_Refraction;
			if (!b3Shade(&surface.refr_ray,depth_count + 1))
			{
				b3GetInfiniteColor(&surface.refr_ray);
			}
			formula |= 1;
		}
		else
		{
			refr = 0;
		}

		refl = surface.m_Reflection;
		if (refl > 0)
		{
			if (!b3Shade(&surface.refl_ray,depth_count + 1))
			{
				b3GetInfiniteColor(&surface.refl_ray);
			}
			formula |= 2;
		}

		switch(formula)
		{
		case 1:
			// Only refraction
			factor = (1.0 - refr);
			ray->color = surface.refr_ray.color * refr + ray->color * factor;
			break;

		case 2:
			// Only reflection
			factor = (1.0 - refr);
			ray->color = surface.refl_ray.color * refl + ray->color * factor;
			break;

		case 3:
			// Reflection and refraction
			factor = (1.0 - refl - refr);
			ray->color = surface.refl_ray.color * refl + surface.refr_ray.color * refr + ray->color * factor;
			break;
		}

		B3_FOR_BASE(b3GetLightHead(),item)
		{
			light = (b3Light *)item;
			light->b3Illuminate(this,&surface,material);
		}

		if (m_Nebular != null)
		{
			m_Nebular->b3ComputeNebular(ray->color,ray->color,ray->Q);
		}
		result = true;
	}

	if (m_Nebular != null)
	{
		m_Nebular->b3GetNebularColor(ray->color);
		result = true;
	}
	return result;
}
