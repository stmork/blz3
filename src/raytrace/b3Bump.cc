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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

void b3InitBump::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering bump mapping...\n");
	b3Item::b3Register(&b3BumpNoise::b3Init,   &b3BumpNoise::b3Init,   BUMP_NOISE);
	b3Item::b3Register(&b3BumpMarble::b3Init,  &b3BumpMarble::b3Init,  BUMP_MARBLE);
	b3Item::b3Register(&b3BumpTexture::b3Init, &b3BumpTexture::b3Init, BUMP_TEXTURE);
	b3Item::b3Register(&b3BumpWater::b3Init,   &b3BumpWater::b3Init,   BUMP_WATER);
	b3Item::b3Register(&b3BumpWave::b3Init,    &b3BumpWave::b3Init,    BUMP_WAVE);
	b3Item::b3Register(&b3BumpGroove::b3Init,  &b3BumpGroove::b3Init,  BUMP_GROOVE);
	b3Item::b3Register(&b3BumpGlossy::b3Init,  &b3BumpGlossy::b3Init,  BUMP_GLOSSY);
}

b3Bump::b3Bump(b3_size class_size,b3_u32 class_type) : b3Item(class_size,class_type)
{
}

b3Bump::b3Bump(b3_u32 class_type) : b3Item(sizeof(b3Bump),class_type)
{
}

b3Bump::b3Bump(b3_u32 *src) : b3Item(src)
{
}

void b3Bump::b3BumpNormal(b3_ray *ray)
{
}

static b3_f64 timePoint = 0;

/*************************************************************************
**                                                                      **
**                        Noise bumping                                 **
**                                                                      **
*************************************************************************/

b3BumpNoise::b3BumpNoise(b3_u32 class_type) : b3Bump(sizeof(b3BumpNoise),class_type)
{
}

b3BumpNoise::b3BumpNoise(b3_u32 *src) : b3Bump(src)
{
	b3InitVector(&m_Scale);
	m_Size = b3InitFloat();
}

void b3BumpNoise::b3BumpNormal(b3_ray *ray)
{
	b3_vector n;
	b3_f64    u,v,w,Denom;

	u = ray->polar.box_polar.x * m_Scale.x * 1024;
	v = ray->polar.box_polar.y * m_Scale.y * 1024;
	w = ray->polar.box_polar.z * m_Scale.z * 1024;

	Denom =	ray->normal.x * ray->normal.x +
			ray->normal.y * ray->normal.y +
			ray->normal.z * ray->normal.z;
	if (Denom == 0) return;
	if (Denom != 1) Denom = 1/sqrt(Denom);  

	noise_procedures.b3NoiseDeriv (u,v,w,&n);
	ray->normal.x = ray->normal.x * Denom + n.x * m_Size;
	ray->normal.y = ray->normal.y * Denom + n.y * m_Size;
	ray->normal.z = ray->normal.z * Denom + n.z * m_Size;
}

/*************************************************************************
**                                                                      **
**                        marble like bumping                           **
**                                                                      **
*************************************************************************/

b3BumpMarble::b3BumpMarble(b3_u32 class_type) : b3Bump(sizeof(b3BumpMarble),class_type)
{
}

b3BumpMarble::b3BumpMarble(b3_u32 *src) : b3Bump(src)
{
	b3InitVector(&m_Scale);
	m_Size = b3InitFloat();
}

void b3BumpMarble::b3BumpNormal(b3_ray *ray)
{
	b3_f64 u,v,w,i,d,dd;

	u = ray->polar.box_polar.x * m_Scale.x * 1024;
	v = ray->polar.box_polar.y * m_Scale.y * 1024;
	w = ray->polar.box_polar.z * m_Scale.z * 1024;

	i =	ray->normal.x * ray->normal.x +
		ray->normal.y * ray->normal.y +
		ray->normal.z * ray->normal.z;
	if (i == 0) return;
	if (i != 1)
	{ 
		i = 1/sqrt(i);
		ray->normal.x *= i;
		ray->normal.y *= i;
		ray->normal.z *= i;
	}

	d  = (u+15000.0) * 0.02 + 7.0 * noise_procedures.b3NoiseVector(u*0.01,v*0.005,w*0.005);
	dd = (long)d % 17;
	if (dd < 4) i = 0.7 + 0.2   * noise_procedures.b3NoiseVector(u/70.0,v*0.02,w*0.02);
	else
	{
		if ((dd < 9) || (dd >= 12))
		{
			d = fabs (d-(long)(d / 17.0) * 17.0 - 10.5) * 0.1538462;
			i = 0.4 + 0.3 * d + 0.2 * noise_procedures.b3NoiseVector
				(u * 0.01,v * 0.01,w * 0.01);
		}
		else i = 0.2 + 0.2 * noise_procedures.b3NoiseVector (u * 0.01,v * 0.01,w * 0.01);
	}
	i         *= m_Size;
	ray->normal.x += i;
	ray->normal.y += i;
	ray->normal.z += i;
}

/*************************************************************************
**                                                                      **
**                        classic bump mapping                          **
**                                                                      **
*************************************************************************/

b3BumpTexture::b3BumpTexture(b3_u32 class_type) : b3Bump(sizeof(b3BumpTexture),class_type)
{
}

b3BumpTexture::b3BumpTexture(b3_u32 *src) : b3Bump(src)
{
	m_xStart    = b3InitFloat();
	m_yStart    = b3InitFloat();
	m_xScale    = b3InitFloat();
	m_yScale    = b3InitFloat();
	m_xTimes    = b3InitInt();
	m_yTimes    = b3InitInt();
	m_Intensity = b3InitFloat();
	b3InitNull();
	m_Flags     = b3InitInt();
	b3InitString(m_Name,B3_TEXSTRINGLEN);
	m_Texture   = texture_pool.b3LoadTexture(m_Name);
}

void b3BumpTexture::b3BumpNormal(b3_ray *ray)
{
}

/*************************************************************************
**                                                                      **
**                        water surface bumping                         **
**                                                                      **
*************************************************************************/

b3BumpWater::b3BumpWater(b3_u32 class_type) : b3Bump(sizeof(b3BumpWater),class_type)
{
}

b3BumpWater::b3BumpWater(b3_u32 *src) : b3Bump(src)
{
	m_ScaleFlag   = b3InitInt();
	b3InitVector(&m_ScaleIPoint);
	m_ScaleRad    = b3InitFloat();
	if (B3_PARSE_INDEX_VALID)
	{
		m_ScaleTime   = b3InitFloat();
	}
}

void b3BumpWater::b3BumpNormal(b3_ray *ray)
{
	b3_vector point,ox,oy,n;
	b3_f64    Denom,r,time,water;

	time = (m_ScaleTime < 0.0001 ? 0 : timePoint / m_ScaleTime);
	if (m_ScaleFlag & BUMP_IPOINT)
	{
		point.x = ray->ipoint.x * m_ScaleIPoint.x;
		point.y = ray->ipoint.y * m_ScaleIPoint.y;
		point.z = ray->ipoint.z * m_ScaleIPoint.z;
	}
	else
	{
		point.x = ray->polar.box_polar.x * m_ScaleIPoint.x;
		point.y = ray->polar.box_polar.y * m_ScaleIPoint.y;
		point.z = ray->polar.box_polar.z * m_ScaleIPoint.z;
	}

	water = noise_procedures.b3Water(&point,time);
	ox.x     = 0.125;
	ox.y     = 0;
	ox.z     = (m_ScaleFlag & BUMP_U_SUPPRESS_WAVE ? 0 : water);
	oy.x     = 0;
	oy.y     = 0.125;
	oy.z     = (m_ScaleFlag & BUMP_V_SUPPRESS_WAVE ? 0 : water);

	point.x += ox.x;
	ox.z    -= noise_procedures.b3Water (&point,time);
	point.x -= ox.x;

	point.y += oy.y;
	oy.z    -= noise_procedures.b3Water (&point,time);

	r   = m_ScaleRad;
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
}

b3BumpWave::b3BumpWave(b3_u32 *src) : b3Bump(src)
{
	m_Flags     = b3InitInt();
	b3InitVector(&m_Scale);
	m_Amplitude = b3InitFloat();
}

void b3BumpWave::b3BumpNormal(b3_ray *ray)
{
	b3_vector point,ox,oy,n;
	b3_f64    Denom,r,wave;

	if (m_Flags & BUMP_IPOINT)
	{
		point.x = ray->ipoint.x * m_Scale.x;
		point.y = ray->ipoint.y * m_Scale.y;
		point.z = ray->ipoint.z * m_Scale.z;
	}
	else
	{
		point.x = ray->polar.box_polar.x * m_Scale.x;
		point.y = ray->polar.box_polar.y * m_Scale.y;
		point.z = ray->polar.box_polar.z * m_Scale.z;
	}

	wave     = noise_procedures.b3Wave(&point);
	ox.x     = 0.125;
	ox.y     = 0;
	ox.z     = (m_Flags & BUMP_U_SUPPRESS_WAVE ? 0 : wave);
	oy.x     = 0;
	oy.y     = 0.125;
	oy.z     = (m_Flags & BUMP_V_SUPPRESS_WAVE ? 0 : wave);

	point.x += ox.x;
	ox.z    -= noise_procedures.b3Wave (&point);
	point.x -= ox.x;

	point.y += oy.y;
	oy.z    -= noise_procedures.b3Wave (&point);

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
}

b3BumpGroove::b3BumpGroove(b3_u32 *src) : b3Bump(src)
{
	m_Flags     = b3InitInt();
	b3InitVector(&m_Scale);
	m_Amplitude = b3InitFloat();
}

void b3BumpGroove::b3BumpNormal(b3_ray *ray)
{
	b3_vector point,ox,oy,n;
	b3_f64    Denom,r,groove;

	point.x = 
		ray->polar.object_polar.x * m_Scale.x +
		ray->polar.object_polar.y * m_Scale.y;
	point.y = 
	point.z = 0;

	groove   = noise_procedures.b3Wave(&point);
	ox.x     = 0.125;
	ox.y     = 0;
	ox.z     = groove;
	oy.x     = 0;
	oy.y     = 0.125;
	oy.z     = groove;

	point.x += ox.x;
	ox.z    -= noise_procedures.b3Wave (&point);
	point.x -= ox.x;

	point.y += oy.y;
	oy.z    -= noise_procedures.b3Wave (&point);

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
}

b3BumpGlossy::b3BumpGlossy(b3_u32 *src) : b3Bump(src)
{
	m_Flags     = b3InitInt();
	m_Intensity = b3InitFloat();
}

void b3BumpGlossy::b3BumpNormal(b3_ray *ray)
{
	b3_vector n;
	b3_f64    Denom,r;

	r   = m_Intensity;
	if (r <= 0) return;
	if (r >  1) r = 1;
	
	n.x = B3_FRAN(0.9) - 0.45;
	n.y = B3_FRAN(0.9) - 0.45;
	n.z = B3_FRAN(0.9) - 0.45;

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
