/*
**
**	$Filename:	b3Clouds.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Clouds
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3Clouds.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2006/04/18 15:48:59  sm
**	- Extracted from procedure module:
**	  o clouds
**	  o ocean waves
**	  o water
**
**
*/

/*************************************************************************
**                                                                      **
**                        Cloud default values                          **
**                                                                      **
*************************************************************************/

#define EARTH_RADIUS_KM 10.0

b3Clouds::b3Clouds()
{
	b3Vector::b3Init(&m_Anim,    0.1f,  0.1f,  0.05f);
	b3Vector::b3Init(&m_PosScale,0.01f, 0.01f, 0.01f);
	m_EarthRadius = EARTH_RADIUS_KM;
	m_CloudHeight =   1.0f;
	m_Scaling     =   5.0f;
	m_Sharpness   =  10.2f;
	m_Flags       =   0;

	b3PrepareClouds();
}

void b3Clouds::b3PrepareClouds()
{
	b3_f64 Rc = m_EarthRadius + m_CloudHeight;

	m_EarthRadiusSqr = m_EarthRadius * m_EarthRadius;
	m_CloudRadiusSqr = Rc * Rc;
}

b3_f64 b3Clouds::b3ComputeClouds(
	const b3_line64 *ray,
	      b3_f64    &r,
	const b3_f64     time)
{
	b3_f64 sight;

	if (ray->dir.z > 0)
	{
		b3_vector Dir;
		b3_f64    p,D,len;
		b3_f32    t;

		p     = ray->dir.z * -m_EarthRadius;
		D     = p * p + m_CloudRadiusSqr - m_EarthRadiusSqr;
		len   = (-p - sqrt(D)) * m_Scaling;

		Dir.x = ray->pos.x * m_PosScale.x + ray->dir.x * len + m_Anim.x * time;
		Dir.y = ray->pos.y * m_PosScale.y + ray->dir.y * len + m_Anim.y * time;
		Dir.z = ray->pos.z * m_PosScale.z + ray->dir.z * len + m_Anim.z * time;

		t = b3Noise::b3Turbulence (&Dir);
		r = 1.0 - pow(t, -m_Sharpness);
		if (r < 0)
		{
			r = 0;
		}
		sight = ray->dir.z;
	}
	else
	{
		r     = 1;
		sight = 0;
	}
	return sight;
}
