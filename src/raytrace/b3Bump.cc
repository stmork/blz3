/*
**
**      $Filename:      b3Bump.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing bump mapping
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

#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Bump.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.38  2004/08/01 12:47:39  sm
**	- Animated thin film material.
**
**	Revision 1.37  2004/05/26 12:47:20  sm
**	- Optimized recursive shading
**	- Optimized pow to an integer version (b3Math::b3FastPow)
**	
**	Revision 1.36  2004/05/18 13:34:50  sm
**	- Cleaned up water animation
**	
**	Revision 1.35  2004/05/18 10:44:52  sm
**	- Fine tuning animated water.
**	
**	Revision 1.34  2004/05/16 18:50:59  sm
**	- Added new simple image sampler.
**	- We need better water!
**	
**	Revision 1.33  2004/05/14 16:16:52  sm
**	- Modified water
**	- Added some water values to its property dialog
**	
**	Revision 1.32  2004/05/12 19:10:50  sm
**	- Completed bump mapping dialog.
**	
**	Revision 1.31  2004/05/12 14:13:28  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.30  2004/05/09 18:59:52  sm
**	- Fixed bump water.
**	
**	Revision 1.29  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.28  2004/05/08 17:36:39  sm
**	- Unified scaling for materials and bumps.
**	
**	Revision 1.27  2004/04/23 18:46:17  sm
**	- Fixed bump sampler: Now using initialized derivativs
**	
**	Revision 1.26  2004/04/18 16:58:14  sm
**	- Changed definitions for base classes of raytracing objects.
**	- Put wood material and wood bump dialogs into property
**	  pages.
**	
**	Revision 1.25  2004/04/18 09:13:50  sm
**	- Removed hardwired values.
**	- Now we have congruent material and bump oakpank structure.
**	
**	Revision 1.24  2004/04/18 08:53:05  sm
**	- Put temporiraly some hardwired values into the oakplank.
**	
**	Revision 1.23  2004/04/17 19:17:44  sm
**	- Minor changes
**	
**	Revision 1.22  2004/04/17 17:18:33  sm
**	- Made some include adjustments
**	- Added oakplank bump as dialog
**	- Fixed b3BumpWood and b3BumpOakPlank
**	  bump computation
**	
**	Revision 1.21  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.20  2004/04/15 08:57:00  sm
**	- Added b3BumpAokPlank
**	
**	Revision 1.19  2004/04/13 13:44:27  sm
**	- Replaced some divisions by multiplications of their reciprocals.
**	
**	Revision 1.18  2004/04/12 15:41:50  sm
**	- Right computation of normal derivation.
**	
**	Revision 1.17  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.16  2004/04/08 09:57:20  sm
**	- Added bump wood.
**	
**	Revision 1.15  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.14  2003/02/22 17:21:34  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.13  2002/12/11 14:53:39  sm
**	- Made some comments
**	- Changed b3Noise class to s static one.
**	
**	Revision 1.12  2002/12/11 14:31:01  sm
**	- Parse order fixed for water bump mapping
**	
**	Revision 1.11  2002/08/23 15:34:28  sm
**	- Added time support to water animation.
**	- Added multiple file format types to brt3.
**	
**	Revision 1.10  2002/03/03 21:22:22  sm
**	- Added support for creating surfaces using profile curves.
**	- Added simple creating of triangle fields.
**	
**	Revision 1.9  2001/12/30 14:16:57  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.8  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.7  2001/10/25 17:41:32  sm
**	- Documenting stencils
**	- Cleaning up image parsing routines with using exceptions.
**	- Added bump mapping
**	
**	Revision 1.6  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.5  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**
**	Revision 1.4  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**
**	Revision 1.3  2001/09/01 15:54:54  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**
**	Revision 1.2  2001/08/09 15:27:34  sm
**	- Following shapes are newly supported now:
**	  o disk
**	  o cylinder
**	  o cone
**	  o ellipsoid
**	  o torus
**	  o triangles
**	- Done some makefile fixes
**	- Everything is Windozable
**
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Bump::b3Register()
{
	b3PrintF (B3LOG_DEBUG,"Registering bump mapping...\n");
	b3Item::b3Register(&b3BumpNoise::b3StaticInit,    &b3BumpNoise::b3StaticInit,    BUMP_NOISE);
	b3Item::b3Register(&b3BumpMarble::b3StaticInit,   &b3BumpMarble::b3StaticInit,   BUMP_MARBLE);
	b3Item::b3Register(&b3BumpTexture::b3StaticInit,  &b3BumpTexture::b3StaticInit,  BUMP_TEXTURE);
	b3Item::b3Register(&b3BumpWater::b3StaticInit,    &b3BumpWater::b3StaticInit,    BUMP_WATER);
	b3Item::b3Register(&b3BumpWave::b3StaticInit,     &b3BumpWave::b3StaticInit,     BUMP_WAVE);
	b3Item::b3Register(&b3BumpGroove::b3StaticInit,   &b3BumpGroove::b3StaticInit,   BUMP_GROOVE);
	b3Item::b3Register(&b3BumpGlossy::b3StaticInit,   &b3BumpGlossy::b3StaticInit,   BUMP_GLOSSY);
	b3Item::b3Register(&b3BumpWood::b3StaticInit,     &b3BumpWood::b3StaticInit,     BUMP_WOOD);
	b3Item::b3Register(&b3BumpOakPlank::b3StaticInit, &b3BumpOakPlank::b3StaticInit, BUMP_OAKPLANK);
}

/*************************************************************************
**                                                                      **
**                        Bump base class                               **
**                                                                      **
*************************************************************************/

b3Bump::b3Bump(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
	m_Amplitude = 0.2f;
}

b3Bump::b3Bump(b3_u32 class_type) : b3Item(sizeof(b3Bump),class_type)
{
	m_Amplitude = 0.2f;
}

b3Bump::b3Bump(b3_u32 *src) : b3Item(src)
{
}

/*************************************************************************
**                                                                      **
**                        Noise bumping                                 **
**                                                                      **
*************************************************************************/

b3BumpNoise::b3BumpNoise(b3_u32 class_type) : b3Bump(sizeof(b3BumpNoise),class_type)
{
	m_Amplitude = 0.2f;
	b3InitScaling(0.05,B3_SCALE_IPOINT_ORIGINAL);
}

b3BumpNoise::b3BumpNoise(b3_u32 *src) : b3Bump(src)
{
	m_Scale.x   = b3InitFloat() * 1024.0;
	m_Scale.y   = b3InitFloat() * 1024.0;
	m_Scale.z   = b3InitFloat() * 1024.0;
	m_Amplitude = b3InitFloat();
	if (B3_PARSE_INDEX_VALID)
	{
		m_ScaleFlags = (b3_scaling_mode)b3InitInt();
	}
}

void b3BumpNoise::b3Write()
{
	b3StoreFloat(m_Scale.x / 1024.0);
	b3StoreFloat(m_Scale.y / 1024.0);
	b3StoreFloat(m_Scale.z / 1024.0);
	b3StoreFloat(m_Amplitude);
	b3StoreInt(m_ScaleFlags);
}

void b3BumpNoise::b3BumpNormal(b3_ray *ray)
{
	b3_vector n;
	b3_vector point;
	b3_f64    Denom;

	b3Scale(ray,&m_Scale,&point);

	Denom =	ray->normal.x * ray->normal.x +
			ray->normal.y * ray->normal.y +
			ray->normal.z * ray->normal.z;
	if (Denom == 0) return;
	if (Denom != 1) Denom = 1/sqrt(Denom);  

	b3Noise::b3NoiseVector (point.x,point.y,point.z,&n);
	ray->normal.x = ray->normal.x * Denom + n.x * m_Amplitude;
	ray->normal.y = ray->normal.y * Denom + n.y * m_Amplitude;
	ray->normal.z = ray->normal.z * Denom + n.z * m_Amplitude;
}

/*************************************************************************
**                                                                      **
**                        marble like bumping                           **
**                                                                      **
*************************************************************************/

b3BumpMarble::b3BumpMarble(b3_u32 class_type) : b3Bump(sizeof(b3BumpMarble),class_type)
{
	m_Amplitude = 0.3f;
	b3InitScaling(0.1f,B3_SCALE_IPOINT_ORIGINAL);
}

b3BumpMarble::b3BumpMarble(b3_u32 *src) : b3Bump(src)
{
	m_Scale.x   = b3InitFloat() * M_PI;
	m_Scale.y   = b3InitFloat() * M_PI;
	m_Scale.z   = b3InitFloat() * M_PI;
	m_Amplitude = b3InitFloat();
	if (B3_PARSE_INDEX_VALID)
	{
		m_ScaleFlags = (b3_scaling_mode)b3InitInt();
	}
	else
	{
		m_ScaleFlags = B3_SCALE_BOX_POLAR;
	}
}

void b3BumpMarble::b3Write()
{
	b3StoreFloat(m_Scale.x / M_PI);
	b3StoreFloat(m_Scale.y / M_PI);
	b3StoreFloat(m_Scale.z / M_PI);
	b3StoreFloat(m_Amplitude);
	b3StoreInt(m_ScaleFlags);
}

b3_bool b3BumpMarble::b3Prepare()
{
	b3PrepareScaling();
	return true;
}

void b3BumpMarble::b3BumpNormal(b3_ray *ray)
{
	b3_vector point;
	b3_f64    r;

	b3Scale(ray,&m_Scale,&point);

	b3Vector::b3Normalize(&ray->normal);

	r = b3Noise::b3Marble(&point) * m_Amplitude;
	ray->normal.x += r;
	ray->normal.y += r;
	ray->normal.z += r;
}

/*************************************************************************
**                                                                      **
**                        classic bump mapping                          **
**                                                                      **
*************************************************************************/

b3BumpTexture::b3BumpTexture(b3_u32 class_type) : b3Bump(sizeof(b3BumpTexture),class_type)
{
	// Init texture repetition values
	m_xStart =  0;
	m_yStart =  1;
	m_xScale =  1;
	m_yScale = -1;
	m_xTimes =  2;
	m_yTimes =  2;

	// Init texture
	m_Name.b3Empty();
	m_Texture = null;
	m_Flags   = 0;
}

b3BumpTexture::b3BumpTexture(b3_u32 *src) : b3Bump(src)
{
	m_xStart    = b3InitFloat();
	m_yStart    = b3InitFloat();
	m_xScale    = b3InitFloat();
	m_yScale    = b3InitFloat();
	m_xTimes    = b3InitInt();
	m_yTimes    = b3InitInt();
	m_Amplitude = b3InitFloat();
	m_Texture   = (b3Tx *)b3InitNull();
	m_Flags     = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
}

void b3BumpTexture::b3Write()
{
	b3StoreFloat(m_xStart);
	b3StoreFloat(m_yStart);
	b3StoreFloat(m_xScale);
	b3StoreFloat(m_yScale);
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	b3StoreFloat(m_Amplitude);
	b3StoreNull();
	b3StoreInt(m_Flags);
	b3StoreString(m_Name,B3_TEXSTRINGLEN);
}

b3_bool b3BumpTexture::b3Prepare()
{
	return b3Scene::b3CheckTexture(&m_Texture,m_Name);
}

inline b3_bool b3BumpTexture::b3GetNormalDeriv(
	b3_f64     lx,
	b3_f64     ly,
	b3_vector *Deriv)
{
	b3_pkd_color p1,p2,p3;
	b3_coord     x,y;

	if (!m_Texture->b3IsLoaded())
	{
		return false;
	}

	lx = (lx - m_xStart) / m_xScale;
	ly = (ly - m_yStart) / m_yScale;

	if ((ly < 0) || (ly >= m_yTimes) || (lx < 0) || (lx >= m_xTimes))
	{
		return false;
	}

	x = (b3_coord)((lx - (b3_coord)lx) * m_Texture->xSize);
	y = (b3_coord)((ly - (b3_coord)ly) * m_Texture->ySize);

	p1 = m_Texture->b3GetValue(x,y);
	p2 = m_Texture->b3GetValue((x+1) % m_Texture->xSize,y);
	p3 = m_Texture->b3GetValue(x,(y+1) % m_Texture->ySize);

	// extract blue values
	p1 &= 0xff;	/* corner */
	p2 &= 0xff;	/* right */
	p3 &= 0xff;	/* down */

	Deriv->x = ((b3_s32)p2 - (b3_s32)p1) * 0.0039215686;
	Deriv->y = ((b3_s32)p3 - (b3_s32)p1) * 0.0039215686;
	Deriv->z =  (b3_s32)p1               * 0.0039215686;

	return true;
}
void b3BumpTexture::b3BumpNormal(b3_ray *ray)
{
	b3_vector64 *xDeriv = &ray->xDeriv;
	b3_vector64 *yDeriv = &ray->yDeriv;
	b3_vector    Deriv;
	b3_f64       x,y;

	if (b3GetNormalDeriv (
		ray->polar.m_Polar.x,
		ray->polar.m_Polar.y,&Deriv))
	{
		b3Vector::b3Normalize(&ray->normal);
		b3Vector::b3Scale(&Deriv,m_Amplitude);

		x = (m_xScale < 0 ? Deriv.x : -Deriv.x) / b3Vector::b3Length(xDeriv);
		y = (m_yScale < 0 ? Deriv.y : -Deriv.y) / b3Vector::b3Length(yDeriv);
		ray->normal.x += (x * xDeriv->x + y * yDeriv->x);
		ray->normal.y += (x * xDeriv->y + y * yDeriv->y);
		ray->normal.z += (x * xDeriv->z + y * yDeriv->z);
	}
}

/*************************************************************************
**                                                                      **
**                        water surface bumping                         **
**                                                                      **
*************************************************************************/

b3BumpWater::b3BumpWater(b3_u32 class_type) : b3Bump(sizeof(b3BumpWater),class_type)
{
	m_Amplitude = 0.2f;
	b3InitScaling(0.006f,B3_SCALE_IPOINT_ORIGINAL);
}

b3BumpWater::b3BumpWater(b3_u32 *src) : b3Bump(src)
{
	m_ScaleFlags = (b3_scaling_mode)b3InitInt();
	m_Amplitude  = b3InitFloat();
	if (B3_PARSE_INDEX_VALID)
	{
		m_Scale.x   = m_Amplitude;
		m_Scale.y   = b3InitFloat();
		m_Scale.z   = b3InitFloat();
		m_Amplitude = b3InitFloat();
		m_ScaleTime = b3InitFloat();
		if (B3_PARSE_INDEX_VALID)
		{
			m_Km       = b3InitFloat();
			m_Octaves  = b3InitCount(); 
			m_WindAmp  = b3InitFloat();
			m_WindFreq = b3InitFloat();
			m_MinWind  = b3InitFloat();
			if (B3_PARSE_INDEX_VALID)
			{
				b3InitVector(&m_Anim);
			}
		}
	}
	else
	{
		b3InitScaling(1.0,B3_SCALE_BOX_POLAR);
	}
}

void b3BumpWater::b3Write()
{
	b3StoreInt(m_ScaleFlags);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Amplitude);
	b3StoreFloat(m_ScaleTime);
	b3StoreFloat(m_Km);
	b3StoreCount(m_Octaves);
	b3StoreFloat(m_WindAmp);
	b3StoreFloat(m_WindFreq);
	b3StoreFloat(m_MinWind);
	b3StoreVector(&m_Anim);
}

b3_bool b3BumpWater::b3Prepare()
{
	b3PrepareScaling();
	b3PrepareWater();
	return true;
}

void b3BumpWater::b3BumpNormal(b3_ray *ray)
{
	b3_vector point,ox,oy,n;
	b3_f64    Denom,r,time,water;

	time = (m_ScaleTime < 0.0001 ? 0 : ray->t / m_ScaleTime);
	b3Scale(ray,&m_Scale,&point);

	water = b3ComputeWater(&point,ray->t);
	ox.x     = 0.125;
	ox.y     = 0;
	ox.z     = water;
	oy.x     = 0;
	oy.y     = 0.125;
	oy.z     = water;

	point.x += ox.x;
	ox.z    -= b3ComputeWater (&point,time);
	point.x -= ox.x;

	point.y += oy.y;
	oy.z    -= b3ComputeWater (&point,time);

	r   = m_Amplitude;
	n.x = ox.y * oy.z - ox.z - oy.y;
	n.y = ox.z * oy.x - ox.x - oy.z;
	n.z = ox.x * oy.y - ox.y - oy.x;

	Denom =
		ray->normal.x * ray->normal.x +
		ray->normal.y * ray->normal.y +
		ray->normal.z * ray->normal.z;
	if (Denom == 0) Denom = 1;
	if (Denom != 1) Denom = 1 / sqrt(Denom);

	ray->normal.x = ray->normal.x * Denom + n.x * r;
	ray->normal.y = ray->normal.y * Denom + n.y * r;
	ray->normal.z = ray->normal.z * Denom + n.z * r;
}

/*************************************************************************
**                                                                      **
**                        beach ripples bumping                         **
**                                                                      **
*************************************************************************/

b3BumpWave::b3BumpWave(b3_u32 class_type) : b3Bump(sizeof(b3BumpWave),class_type)
{
	m_Amplitude = 0.2f;
}

b3BumpWave::b3BumpWave(b3_u32 *src) : b3Bump(src)
{
	m_ScaleFlags = (b3_scaling_mode)b3InitInt();
	b3InitVector(&m_Scale);
	m_Amplitude = b3InitFloat();
}

void b3BumpWave::b3Write()
{
	b3StoreInt(m_ScaleFlags);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Amplitude);
}

b3_bool b3BumpWave::b3Prepare()
{
	b3PrepareScaling();
	return true;
}

void b3BumpWave::b3BumpNormal(b3_ray *ray)
{
	b3_vector point,ox,oy,n;
	b3_f64    Denom,r,wave;

	b3Scale(ray,&m_Scale,&point);

	wave     = b3Noise::b3Wave(&point);
	ox.x     = 0.125;
	ox.y     = 0;
	ox.z     = wave;
	oy.x     = 0;
	oy.y     = 0.125;
	oy.z     = wave;

	point.x += ox.x;
	ox.z    -= b3Noise::b3Wave (&point);
	point.x -= ox.x;

	point.y += oy.y;
	oy.z    -= b3Noise::b3Wave (&point);

	r   = m_Amplitude;
	n.x = ox.y * oy.z - ox.z - oy.y;
	n.y = ox.z * oy.x - ox.x - oy.z;
	n.z = ox.x * oy.y - ox.y - oy.x;

	Denom =
		ray->normal.x * ray->normal.x +
		ray->normal.y * ray->normal.y +
		ray->normal.z * ray->normal.z;
	if (Denom == 0) Denom = 1;
	if (Denom != 1) Denom = 1 / sqrt(Denom);

	ray->normal.x = ray->normal.x * Denom + n.x * r;
	ray->normal.y = ray->normal.y * Denom + n.y * r;
	ray->normal.z = ray->normal.z * Denom + n.z * r;
}

/*************************************************************************
**                                                                      **
**                        A groovy kind of love                         **
**                                                                      **
*************************************************************************/

b3BumpGroove::b3BumpGroove(b3_u32 class_type) : b3Bump(sizeof(b3BumpGroove),class_type)
{
	m_Amplitude = 0.8f;
}

b3BumpGroove::b3BumpGroove(b3_u32 *src) : b3Bump(src)
{
	m_ScaleFlags = (b3_scaling_mode)b3InitInt();
	b3InitVector(&m_Scale);
	m_Amplitude = b3InitFloat();
}

void b3BumpGroove::b3Write()
{
	b3StoreInt(m_ScaleFlags);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Amplitude);
}

b3_bool b3BumpGroove::b3Prepare()
{
	b3PrepareScaling();
	return true;
}

void b3BumpGroove::b3BumpNormal(b3_ray *ray)
{
	b3_vector point,ox,oy,n;
	b3_f64    Denom,r,groove;

	point.x = 
		ray->polar.m_ObjectPolar.x * m_Scale.x +
		ray->polar.m_ObjectPolar.y * m_Scale.y;
	point.y = 
	point.z = 0;

	groove   = b3Noise::b3Wave(&point);
	ox.x     = 0.125;
	ox.y     = 0;
	ox.z     = groove;
	oy.x     = 0;
	oy.y     = 0.125;
	oy.z     = groove;

	point.x += ox.x;
	ox.z    -= b3Noise::b3Wave (&point);
	point.x -= ox.x;

	point.y += oy.y;
	oy.z    -= b3Noise::b3Wave (&point);

	r   = m_Amplitude;
	n.x = ox.y * oy.z - ox.z - oy.y;
	n.y = ox.z * oy.x - ox.x - oy.z;
	n.z = ox.x * oy.y - ox.y - oy.x;

	Denom =
		ray->normal.x * ray->normal.x +
		ray->normal.y * ray->normal.y +
		ray->normal.z * ray->normal.z;
	if (Denom == 0) Denom = 1;
	if (Denom != 1) Denom = 1.0 / sqrt(Denom);

	ray->normal.x = ray->normal.x * Denom + n.x * r;
	ray->normal.y = ray->normal.y * Denom + n.y * r;
	ray->normal.z = ray->normal.z * Denom + n.z * r;
}

/*************************************************************************
**                                                                      **
**                        glossy surface bumping (needs                 **
**                        distributed raytracing!)                      **
**                                                                      **
*************************************************************************/

b3BumpGlossy::b3BumpGlossy(b3_u32 class_type) : b3Bump(sizeof(b3BumpGlossy),class_type)
{
	m_Flags     = 0;
	m_Amplitude = 0.1f;
}

b3BumpGlossy::b3BumpGlossy(b3_u32 *src) : b3Bump(src)
{
	m_Flags     = b3InitInt();
	m_Amplitude = b3Math::b3Limit(b3InitFloat());
}

void b3BumpGlossy::b3Write()
{
	b3StoreInt(m_Flags);
	b3StoreFloat(m_Amplitude);
}

void b3BumpGlossy::b3BumpNormal(b3_ray *ray)
{
	b3_f64 Denom =
		ray->normal.x * ray->normal.x +
		ray->normal.y * ray->normal.y +
		ray->normal.z * ray->normal.z;
	if (Denom > 0)
	{
		if (Denom != 1)
		{
			Denom = 1 / sqrt(Denom);
		}

		ray->normal.x = ray->normal.x * Denom + (B3_FRAN(0.9) - 0.45) * m_Amplitude;
		ray->normal.y = ray->normal.y * Denom + (B3_FRAN(0.9) - 0.45) * m_Amplitude;
		ray->normal.z = ray->normal.z * Denom + (B3_FRAN(0.9) - 0.45) * m_Amplitude;
	}
}

/*************************************************************************
**                                                                      **
**                        Wodden bump base class                        **
**                                                                      **
*************************************************************************/

b3BumpWooden::b3BumpWooden(b3_size class_size,b3_u32 class_type) : b3Bump(class_size,class_type)
{
	b3InitScaling(1.0,B3_SCALE_IPOINT_ORIGINAL);
}

b3BumpWooden::b3BumpWooden(b3_u32 class_type) : b3Bump(sizeof(b3Bump),class_type)
{
	b3InitScaling(1.0,B3_SCALE_IPOINT_ORIGINAL);
}

b3BumpWooden::b3BumpWooden(b3_u32 *src) : b3Bump(src)
{
}

#define BUMP_dX 0.02
#define BUMP_dY 0.02

/*************************************************************************
**                                                                      **
**                        Wood bump                                     **
**                                                                      **
*************************************************************************/

b3BumpWood::b3BumpWood(b3_u32 class_type) : b3BumpWooden(sizeof(b3BumpWood),class_type)
{
	m_Amplitude = 0.003f;
	m_dX = 1.0 / BUMP_dX;
	m_dY = 1.0 / BUMP_dY;

	b3InitWood();
}

b3BumpWood::b3BumpWood(b3_u32 *src) : b3BumpWooden(src)
{
	m_ScaleFlags     = (b3_scaling_mode)b3InitInt();
	b3InitVector(&m_Scale);
	m_Amplitude              = b3InitFloat();
	m_yRot                   = b3InitFloat();
	m_zRot                   = b3InitFloat();
	m_RingSpacing            = b3InitFloat();
	m_RingFrequency          = b3InitFloat();
	m_RingNoise              = b3InitFloat();
	m_RingNoiseFrequency     = b3InitFloat();
	m_TrunkWobble            = b3InitFloat();
	m_TrunkWobbleFrequency   = b3InitFloat();
	m_AngularWobble          = b3InitFloat();
	m_AngularWobbleFrequency = b3InitFloat();
	m_GrainFrequency         = b3InitFloat();
	m_Grainy                 = b3InitFloat();
	m_Ringy                  = b3InitFloat();
}

void b3BumpWood::b3Write()
{
	b3StoreInt(m_ScaleFlags);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Amplitude);

	// Wood parameter
	b3StoreFloat(m_yRot);
	b3StoreFloat(m_zRot);
	b3StoreFloat(m_RingSpacing);
	b3StoreFloat(m_RingFrequency);
	b3StoreFloat(m_RingNoise);
	b3StoreFloat(m_RingNoiseFrequency);
	b3StoreFloat(m_TrunkWobble);
	b3StoreFloat(m_TrunkWobbleFrequency);
	b3StoreFloat(m_AngularWobble);
	b3StoreFloat(m_AngularWobbleFrequency);
	b3StoreFloat(m_GrainFrequency);
	b3StoreFloat(m_Grainy);
	b3StoreFloat(m_Ringy);
}

b3_bool b3BumpWood::b3Prepare()
{
	b3PrepareWood(&m_Scale);
	b3PrepareScaling();

	m_dX = 1.0 / BUMP_dX;
	m_dY = 1.0 / BUMP_dY;

	return true;
}

void b3BumpWood::b3BumpNormal(b3_ray *ray)
{
	b3_vector   point,n;
	b3_vector   xWood,yWood;
	b3_ray      xRay,yRay;
	b3_f64      Denom,wood,dX,dY,x,y,xLen,yLen;

	b3Scale(ray,null,&point);
	wood = b3Wood::b3ComputeWood(&point);

	// Note: xDeriv and yDeriv are not normalized!
	xLen    = b3Vector::b3Length(&ray->xDeriv);
	x       = 1.0 / (xLen * m_dX);
	xRay.ipoint.x = x * ray->xDeriv.x + ray->ipoint.x;
	xRay.ipoint.y = x * ray->xDeriv.y + ray->ipoint.y;
	xRay.ipoint.z = x * ray->xDeriv.z + ray->ipoint.z;
	ray->bbox->b3ComputeBoxPolar(&xRay);
	b3Scale(&xRay,null,&xWood);
	dX = (b3Wood::b3ComputeWood (&xWood) - wood) * m_dX / xLen;

	yLen    = b3Vector::b3Length(&ray->yDeriv);
	y       = 1.0 / (yLen * m_dY);
	yRay.ipoint.x = y * ray->yDeriv.x + ray->ipoint.x;
	yRay.ipoint.y = y * ray->yDeriv.y + ray->ipoint.y;
	yRay.ipoint.z = y * ray->yDeriv.z + ray->ipoint.z;
	ray->bbox->b3ComputeBoxPolar(&yRay);
	b3Scale(&yRay,null,&yWood);
	dY = (b3Wood::b3ComputeWood (&yWood) - wood) * m_dY / yLen;

	n.x = ray->xDeriv.x * dX + ray->yDeriv.x * dY;
	n.y = ray->xDeriv.y * dX + ray->yDeriv.y * dY;
	n.z = ray->xDeriv.z * dX + ray->yDeriv.z * dY;

	Denom = 1.0 / b3Vector::b3Length(&ray->normal);
	ray->normal.x = ray->normal.x * Denom + n.x * m_Amplitude;
	ray->normal.y = ray->normal.y * Denom + n.y * m_Amplitude;
	ray->normal.z = ray->normal.z * Denom + n.z * m_Amplitude;
}

/*************************************************************************
**                                                                      **
**                        Oak plank bump                                **
**                                                                      **
*************************************************************************/

b3BumpOakPlank::b3BumpOakPlank(b3_u32 class_type) : b3BumpWooden(sizeof(b3BumpOakPlank),class_type)
{
	m_Amplitude = 0.025f;
	b3InitOakPlank();
}

b3BumpOakPlank::b3BumpOakPlank(b3_u32 *src) : b3BumpWooden(src)
{
	// Bump parameter
	m_ScaleFlags     = (b3_scaling_mode)b3InitInt();
	b3InitVector(&m_Scale);
	m_Amplitude = b3InitFloat();

	// Wood parameter
	m_yRot                   = b3InitFloat();
	m_zRot                   = b3InitFloat();
	m_RingSpacing            = b3InitFloat();
	m_RingFrequency          = b3InitFloat();
	m_RingNoise              = b3InitFloat();
	m_RingNoiseFrequency     = b3InitFloat();
	m_TrunkWobble            = b3InitFloat();
	m_TrunkWobbleFrequency   = b3InitFloat();
	m_AngularWobble          = b3InitFloat();
	m_AngularWobbleFrequency = b3InitFloat();
	m_GrainFrequency         = b3InitFloat();
	m_Grainy                 = b3InitFloat();
	m_Ringy                  = b3InitFloat();

	// Oak plank parameter
	m_xTimes  = b3InitInt();
	m_yTimes  = b3InitInt();
	m_xOffset = b3InitFloat();
	m_xScale  = b3InitFloat();
	m_yScale  = b3InitFloat();
	m_Wobble  = b3InitFloat();
}

void b3BumpOakPlank::b3Write()
{
	// Bump parameter
	b3StoreInt(m_ScaleFlags);
	b3StoreVector(&m_Scale);
	b3StoreFloat(m_Amplitude);

	// Wood parameter
	b3StoreFloat(m_yRot);
	b3StoreFloat(m_zRot);
	b3StoreFloat(m_RingSpacing);
	b3StoreFloat(m_RingFrequency);
	b3StoreFloat(m_RingNoise);
	b3StoreFloat(m_RingNoiseFrequency);
	b3StoreFloat(m_TrunkWobble);
	b3StoreFloat(m_TrunkWobbleFrequency);
	b3StoreFloat(m_AngularWobble);
	b3StoreFloat(m_AngularWobbleFrequency);
	b3StoreFloat(m_GrainFrequency);
	b3StoreFloat(m_Grainy);
	b3StoreFloat(m_Ringy);

	// Oak plank parameter
	b3StoreCount(m_xTimes);
	b3StoreCount(m_yTimes);
	b3StoreFloat(m_xOffset);
	b3StoreFloat(m_xScale);
	b3StoreFloat(m_yScale);
	b3StoreFloat(m_Wobble);
}

b3_bool b3BumpOakPlank::b3Prepare()
{
	b3_index x,y;
	b3_f64   fx,fy,wobble;

	b3PrepareOakPlank(&m_Scale);
	b3PrepareScaling();

	m_dX = 1.0 / BUMP_dX;
	m_dY = 1.0 / BUMP_dY;

	m_Amplitudes.b3Clear();
	for (y = 0;y < m_yTimes;y++)
	{
		fy = (b3_f64)y / m_yTimes;
		for (x = 0;x < m_xTimes;x++)
		{
			fx = (b3_f64)x / m_xTimes;

			wobble = b3Noise::b3SignedFilteredNoiseVector(fx,fy,0) * m_Amplitude * m_Wobble;
			m_Amplitudes.b3Add(m_Amplitude + wobble);
		}
	}

	return true;
}

void b3BumpOakPlank::b3BumpNormal(b3_ray *ray)
{
	b3_vector   point,n;
	b3_vector   xWood,yWood;
	b3_ray      xRay,yRay;
	b3_f64      Denom,wood,dX,dY,x,y,xLen,yLen;
	b3_index    index,iX,iY;

	b3Scale(ray,null,&point);
	wood = b3OakPlank::b3ComputeOakPlank(&point,index);

	// Note: xDeriv and yDeriv are not normalized!
	xLen    = b3Vector::b3Length(&ray->xDeriv);
	x       = 1.0 / (xLen * m_dX);
	xRay.ipoint.x = x * ray->xDeriv.x + ray->ipoint.x;
	xRay.ipoint.y = x * ray->xDeriv.y + ray->ipoint.y;
	xRay.ipoint.z = x * ray->xDeriv.z + ray->ipoint.z;
	ray->bbox->b3ComputeBoxPolar(&xRay);
	b3Scale(&xRay,null,&xWood);
	dX = (b3OakPlank::b3ComputeOakPlank (&xWood,iX) - wood) * m_dX / xLen;

	yLen    = b3Vector::b3Length(&ray->yDeriv);
	y       = 1.0 / (yLen * m_dY);
	yRay.ipoint.x = y * ray->yDeriv.x + ray->ipoint.x;
	yRay.ipoint.y = y * ray->yDeriv.y + ray->ipoint.y;
	yRay.ipoint.z = y * ray->yDeriv.z + ray->ipoint.z;
	ray->bbox->b3ComputeBoxPolar(&yRay);
	b3Scale(&yRay,null,&yWood);
	dY = (b3OakPlank::b3ComputeOakPlank (&yWood,iY) - wood) * m_dY / yLen;

	n.x = ray->xDeriv.x * dX + ray->yDeriv.x * dY;
	n.y = ray->xDeriv.y * dX + ray->yDeriv.y * dY;
	n.z = ray->xDeriv.z * dX + ray->yDeriv.z * dY;

	Denom = 1.0 / b3Vector::b3Length(&ray->normal);
	ray->normal.x = ray->normal.x * Denom + n.x * m_Amplitudes[index];
	ray->normal.y = ray->normal.y * Denom + n.y * m_Amplitudes[index];
	ray->normal.z = ray->normal.z * Denom + n.z * m_Amplitudes[index];
}
