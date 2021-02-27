/*
**
**	$Filename:	b3Procedure.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Procedureal components
**
**	(C) Copyright 2001  Steffen A. Mork
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
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/base/b3Wood.h"

/*************************************************************************
**                                                                      **
**                        Wood implementation                           **
**                                                                      **
*************************************************************************/

void b3Wood::b3InitWood()
{
	//	b3Vector::b3Init(&m_Scale,40,40,40);
	m_yRot                   = (b3_f32)(0.5 * M_PI);
	m_zRot                   = (b3_f32)(-0.5 * M_PI);
	m_Ringy                  =   1;
	m_Grainy                 =   1;
	m_GrainFrequency         =  25;

	m_RingSpacing            =   0.3f;
	m_RingFrequency          =   0.2f;
	m_RingNoise              =   0.2f;
	m_RingNoiseFrequency     =   1;
	m_TrunkWobble            =   0.1f;
	m_TrunkWobbleFrequency   =   0.025f;
	m_AngularWobble          =   0.1f;
	m_AngularWobbleFrequency =   0.9f;
}

void b3Wood::b3PrepareWood(b3_vector * scale)
{
	b3Matrix::b3Move(nullptr,   &m_Warp, -0.5, -0.5, -0.5);
	b3Matrix::b3Scale(&m_Warp, &m_Warp, nullptr, scale->x * M_PI, scale->y * M_PI, scale->z * M_PI);
	b3Matrix::b3RotateZ(&m_Warp, &m_Warp, nullptr, m_zRot);
	b3Matrix::b3RotateY(&m_Warp, &m_Warp, nullptr, m_yRot);
}

b3_f64 b3Wood::b3ComputeWood(b3_vector * polar, b3_f64 dist)
{
	b3_vector d;
	b3_vector offset;
	b3_vector Pring;
	b3_vector aux;
	b3_vector Pgrain;
	b3_f64    dPgrain;
	b3_f64    inring;
	b3_f64    grain = 0;
	b3_f64    amp = 1;
	b3_loop   i;

	b3Matrix::b3VMul(&m_Warp, polar, &d, true);
	b3Noise::b3FractionalBrownianMotion(&d, 2, 4, 0.5, &offset);

	b3Noise::b3NoiseVector(0, 0, d.z * m_TrunkWobbleFrequency, &aux);
	Pring.x = d.x + m_RingNoise * offset.x + m_TrunkWobble * (aux.x * 2 - 1);
	Pring.y = d.y + m_RingNoise * offset.y + m_TrunkWobble * (aux.y * 2 - 1);
	Pring.z = d.z + m_RingNoise * offset.z;

	// Calculate radius from center
	b3_f64 r2 = Pring.x * Pring.x + Pring.y * Pring.y;
	b3_f64 r  = sqrt(r2) * m_RingFrequency;

	// For unround rings...
	r += m_AngularWobble * b3Math::b3Smoothstep(0, 5, r) * b3Noise::b3SignedNoiseVector(
			Pring.x * m_AngularWobbleFrequency,
			Pring.y * m_AngularWobbleFrequency,
			Pring.z * m_AngularWobbleFrequency * 0.1);

	// Ensure unequally spaced rings
	r += m_RingSpacing * b3Noise::b3SignedFilteredNoiseScalar(r);

	inring = b3Math::b3SmoothPulse(0.1, 0.55, 0.7, 0.95, b3Math::b3FracOne(r));

	Pgrain.x = d.x * m_GrainFrequency;
	Pgrain.y = d.y * m_GrainFrequency;
	Pgrain.z = d.z * m_GrainFrequency * 0.05;
	dPgrain  = dist / 100;

	for (i = 0; i < 2; i++)
	{
		b3_f64 grain1valid = 1 - b3Math::b3Smoothstep(0.2, 0.6, dPgrain);
		if (grain1valid > 0)
		{
			b3_f64 g = grain1valid * b3Noise::b3SignedNoiseVector(Pgrain.x, Pgrain.y, Pgrain.z);

			g *= (0.3 + 0.7 * inring);
			g  = b3Math::b3Limit(0.8 - g);
			g *= g;
			g  = m_Grainy * b3Math::b3Smoothstep(0.5, 1, g);
			if (i == 0)
			{
				inring *= (1 - 0.4 * grain1valid);
			}
			grain = B3_MAX(grain, g);
		}
		Pgrain.x += Pgrain.x;
		Pgrain.y += Pgrain.y;
		Pgrain.z += Pgrain.z;
		dPgrain  += dPgrain;
		amp      *= 0.5;
	}

	return inring * m_Ringy * (1 - grain) + grain;
}

void b3Wood::b3CopyWobbled(b3Wood * wood, b3_f64 wobble, b3_f64 fx, b3_f64 fy)
{
	m_yRot                   = wood->m_yRot + b3Noise::b3SignedFilteredNoiseVector(fx, 0, 0) * wobble * 7;
	m_zRot                   = wood->m_zRot + b3Noise::b3SignedFilteredNoiseVector(0, fy, 0) * wobble * 5;
	m_Ringy                  = wood->m_Ringy;
	m_Grainy                 = wood->m_Grainy;
	m_GrainFrequency         = wood->m_GrainFrequency;

	m_RingSpacing            = wood->m_RingSpacing;
	m_RingFrequency          = wood->m_RingFrequency;
	m_RingNoise              = wood->m_RingNoise;
	m_RingNoiseFrequency     = wood->m_RingNoiseFrequency;
	m_TrunkWobble            = wood->m_TrunkWobble;
	m_TrunkWobbleFrequency   = wood->m_TrunkWobbleFrequency;
	m_AngularWobble          = wood->m_AngularWobble;
	m_AngularWobbleFrequency = wood->m_AngularWobbleFrequency;
}

/*************************************************************************
**                                                                      **
**                        Oak plank implementation                      **
**                                                                      **
*************************************************************************/

b3OakPlank::b3OakPlank()
{
	m_Planks = nullptr;
}

b3OakPlank::~b3OakPlank()
{
	if (m_Planks != nullptr)
	{
		delete [] m_Planks;
	}
}

void b3OakPlank::b3InitOakPlank()
{
	b3InitWood();
	m_yRot    = (b3_f32)(M_PI * 0.51);
	m_zRot    = 0;
	m_xTimes  = 3;
	m_yTimes  = 7;
	m_xOffset = 0.55f;
	m_xScale  = 0.5f;
	m_yScale  = 0.05f;
	m_Wobble  = 0.1f;
}

void b3OakPlank::b3PrepareOakPlank(b3_vector * scale)
{
	b3_count x, y;
	b3_index index;

	b3PrepareWood(scale);
	if (m_Planks != nullptr)
	{
		delete [] m_Planks;
	}
	m_PlankCount = m_xTimes * m_yTimes;
	m_Planks     = new b3Wood[m_PlankCount];
	m_rxScale    = 1.0 / m_xScale;
	m_ryScale    = 1.0 / m_yScale;
	m_rxTimes    = 1.0 / m_xTimes;
	m_ryTimes    = 1.0 / m_yTimes;

	for (y = 0; y < m_yTimes; y++)
	{
		for (x = 0; x < m_xTimes; x++)
		{
			index = y * m_xTimes + x;
			m_Planks[index].b3CopyWobbled(this, m_Wobble, (b3_f64)x * m_rxTimes, (b3_f64)y * m_ryTimes);
			m_Planks[index].b3PrepareWood(scale);
		}
	}
}

b3_f64 b3OakPlank::b3ComputeOakPlank(b3_vector * polar, b3_f64 distance, b3_index & index)
{
	b3_vector surface;
	b3_index  ix, iy;
	b3_f64    fx, fy;

	fy = polar->y * m_ryScale;
	fx = polar->x * m_rxScale + m_xOffset * floor(fy);
	surface.x = b3Math::b3Frac(fx, (b3_f64)m_xTimes);
	surface.y = b3Math::b3Frac(fy, (b3_f64)m_yTimes);
	surface.z = 0;

	ix  = (b3_index)surface.x;
	iy  = (b3_index)surface.y;
	index = (ix * m_yTimes + iy) % m_PlankCount;
	return m_Planks[index].b3ComputeWood(&surface, distance);
}
