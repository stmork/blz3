/*
**
**	$Filename:	b3Water.cc $
**	$Release:	Dortmund 2006 $
**	$Revision: 1450 $
**	$Date: 2006-06-28 19:48:22 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Simple water bump map
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
#include "blz3/base/b3Water.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        Water default values                          **
**                                                                      **
*************************************************************************/

b3Water::b3Water()
{
	m_Km        = 1.0f;
	m_Octaves   = 2;
	m_ScaleTime = 1;
	m_WindFreq  = 0.5;
	m_WindAmp   = 0.2f;
	m_MinWind   = 1.0f;
	b3Vector::b3Init(&m_Anim,1.5f,1.5f,4.0f);
	b3PrepareWater(0.0);
}

void b3Water::b3PrepareWater(const b3_f64 t)
{
	m_Factor = 10 * m_WindFreq;
	m_t      = t;
}

b3_f64 b3Water::b3ComputeWater(const b3_vector *point)
{
	b3_vector P;
	b3_f64    offset,turbulence;

	P.x = point->x * m_Factor + m_t * m_Anim.x;
	P.y = point->y * m_Factor + m_t * m_Anim.y;
	P.z = point->z * m_Factor + m_t * m_Anim.z * m_ScaleTime;
	offset = m_Km * b3Noise::b3FractionalBrownianMotion(&P,m_Octaves,2.0,1.0);

	P.x *= 8;
	P.y *= 8;
	P.z *= 8;
	turbulence = b3Noise::b3Turbulence(&P, 3);

	return (m_MinWind + m_WindAmp * turbulence) * offset;
}
