/*
**
**	$Filename:	b3Procedure.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some procedure proto types for pseudo random noise
**	               functions.
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_NOISE_H
#define B3_BASE_NOISE_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Matrix.h"

typedef b3_f32 b3_noisetype;

enum b3_noise_error
{
	B3_NOISE_ERROR = -1,
	B3_NOISE_OK    =  0,
	B3_NOISE_MEMORY
};

typedef b3Exception<b3_noise_error,'NOI'> b3NoiseException;

class b3Noise : public b3Mem
{
	static b3_noisetype *NoiseTable;
	static b3_f64        epsilon;

public:
	               b3Noise();
	              ~b3Noise();

	static        b3_f64        b3NoiseScalar (b3_f64 x);
	static inline b3_f64  b3SignedNoiseScalar (b3_f64 x)
	{
		return b3NoiseScalar(x) * 2 - 1;
	}
	
	static        b3_f64        b3FilteredNoiseScalar (b3_f64 x);
	static inline b3_f64  b3SignedFilteredNoiseScalar (b3_f64 x)
	{
		return b3FilteredNoiseScalar(x) * 2 - 1;
	}
	
	static        b3_f64        b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);
	static inline b3_f64  b3SignedNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return b3NoiseVector(x,y,z) * 2 - 1;
	}
	
	static        b3_f64        b3FilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);
	static inline b3_f64  b3SignedFilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return b3FilteredNoiseVector(x,y,z) * 2 - 1;
	}
	
	static        void       b3NoiseDeriv (b3_f64 dx,b3_f64 dy,b3_f64 dz,b3_vector *result);
	static inline void b3SignedNoiseDeriv (b3_f64 dx,b3_f64 dy,b3_f64 dz,b3_vector *result)
	{
		b3NoiseDeriv(dx,dy,dz,result);
		
		result->x = result->x * 2 - 1;
		result->y = result->y * 2 - 1;
		result->z = result->z * 2 - 1;
	}

	static inline b3_f64  b3Turbulence  (b3_vector *P,b3_count octaves = 10)
	{
		return b3FractionalBrownianMotion(P,octaves,2.0,0.5);
	}

	static inline b3_f64  b3FractionalBrownianMotion(
		b3_vector *p,
		b3_count   octaves,
		b3_f32     lacunarity,
		b3_f32     gain)
	{
		b3_f32 B3_ALIGN_16 v[4];
		b3_f32 B3_ALIGN_16 factor[4];
		b3_f64             sum = 0,n;
		b3_loop            i,k;

		v[0] = 1;    // amplification
		v[1] = p->x;
		v[2] = p->y;
		v[3] = p->z;
		
		factor[0] = gain;
		factor[1] = lacunarity;
		factor[2] = lacunarity;
		factor[3] = lacunarity;

		for (i = 0;i < octaves;i++)
		{
			n = b3NoiseVector(v[1],v[2],v[3]);
			sum  += (v[0] * n);
			for (k = 0;k < 4;k++)
			{
				v[k] *= factor[k];
			}
		}
		return sum;
	}

	static inline void b3FractionalBrownianMotion(
		b3_vector *p,
		b3_count   octaves,
		b3_f32     lacunarity,
		b3_f32     gain,
		b3_vector *result)
	{
		b3_f32 B3_ALIGN_16 v[4];
		b3_f32 B3_ALIGN_16 factor[4];
		b3_f32 B3_ALIGN_16 aux[4];
		b3_f32 B3_ALIGN_16 sum[4];
		b3_loop   i,k;

		v[0] = 1;    // amplification
		v[1] = p->x;
		v[2] = p->y;
		v[3] = p->z;
		
		factor[0] = gain;
		factor[1] = lacunarity;
		factor[2] = lacunarity;
		factor[3] = lacunarity;

		for (k = 0;k < 4;k++)
		{
			aux[k] = 0;
			sum[k] = 0;
		}
		for (i = 0;i < octaves;i++)
		{
			b3NoiseDeriv(v[1],v[2],v[3],(b3_vector *)&sum[0]);

			for (k = 0;k < 4;k++)
			{
				sum[k] += aux[k] * v[0];
				v[k]   *= factor[k];
			}
		}
		result->x = sum[0];
		result->y = sum[1];
		result->z = sum[2];
	}

	static b3_f64  b3Marble      (b3_vector *d);
	static void    b3Wood        (b3_vector *d,b3Color &mask);
	static void    b3Hell        (b3_vector *P,b3Color &Color);
	static b3_f64  b3Wave        (b3_vector *point);
	static b3_f64  b3Granite     (b3_vector *point,b3_count octaves);
	static b3_f64  b3PGauss      ();

private:
	static b3_f64       b3Frac      (b3_f64 a,b3_f64 b);
	static b3_noisetype b3GetDiff   (b3_index xs,b3_index ys,b3_index zs,b3_index k,b3_index i);
	static b3_f64       b3Interpolate(b3_index ix,b3_index iy,b3_index iz,b3_f32 fx,b3_f32 fy,b3_f32 fz);
	static b3_f64       b3GradNoise (b3_f64 x,b3_f64 y,b3_f64 z,b3_index i);

	static void         b3OldMarble   (b3_vector *P,b3Color &Color);
};

class b3Water
{
public:
	b3_f32    m_Km;
	b3_count  m_Octaves;
	b3_f32    m_WindAmp;
	b3_f32    m_WindFreq;
	b3_f32    m_MinWind;
	b3_f32    m_ScaleTime;           // time period for wave swing
	

public:
	b3Water();

	inline b3_f64 b3ComputeWater(b3_vector *point, b3_f64 time)
	{
		b3_vector P;
		b3_f64    factor  = 10 * m_WindFreq;
		b3_f64    offset,turbulence;

		P.x = point->x * factor;
		P.y = point->y * factor;
		P.z = point->z * factor + time * m_ScaleTime * 3.0;
		offset = m_Km * b3Noise::b3FractionalBrownianMotion(&P,m_Octaves,2.0,1.0);

		P.x *= 8;
		P.y *= 8;
		P.z *= 8;
		turbulence = b3Noise::b3Turbulence(&P, 4);
		return (m_MinWind + m_WindAmp * turbulence) * offset;
	}
};

class b3Clouds
{
	b3_f64    m_EarthRadiusSqr;
	b3_f64    m_CloudRadiusSqr;

public:
	b3_vector m_Anim;     // x/y are wind direction, z is time scaling
	b3_vector m_PosScale;
	b3_u32    m_Flags;
	b3_f32    m_EarthRadius;
	b3_f32    m_CloudHeight;
	b3_f32    m_Scaling;
	b3_f32    m_Sharpness;

public:
	     b3Clouds();
	void b3PrepareClouds();

	inline b3_f64 b3ComputeClouds(b3_line64 *ray,b3_f64 &r,b3_f64 time)
	{
		b3_f64 sight;

		if (ray->dir.z > 0)
		{
			b3_vector Dir;
			b3_f64    p,D,len,t;

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
};

#endif
