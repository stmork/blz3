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
#include "blz3/base/b3Spline.h"

typedef b3_f32 b3_noisetype;

enum b3_noise_error
{
	B3_NOISE_ERROR = -1,
	B3_NOISE_OK    =  0,
	B3_NOISE_MEMORY
};

typedef b3Exception<b3_noise_error,'NOI'> b3NoiseException;

class b3Noise
{
	static       b3Spline      m_MarbleSpline;
	static       b3_f32        m_MarbleKnots[16];
	static       b3_vector     m_MarbleControls[8];

	static       b3Spline      m_WoodSpline;
	static       b3_f32        m_WoodKnots[16];
	static       b3_vector     m_WoodControls[8];

	static       b3Spline      m_WaveSpline;
	static       b3_f32        m_WaveKnots[14];
	static       b3_vector     m_WaveControls[10];

	static const b3Color       m_HellColors[4];
	static const b3Color       m_MarbleColors[4];
	static const b3_u08        m_OM[4][8][3];

	static b3Noise             m_Noise;
	static b3_noisetype       *m_NoiseTable;
	static b3_f64              epsilon;

	               b3Noise();

public:
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
	
	static        void          b3NoiseVector         (b3_f64 x,b3_f64 y,b3_f64 z,b3_vector *result);
	static        void          b3FilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z,b3_vector *result);
	
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
			n = b3FilteredNoiseVector(v[1],v[2],v[3]);
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
	static void    b3AnimThinFilm(b3_f64 t, b3_vector *result);
	static b3_f64  b3PGauss      ();

private:
	static b3_noisetype b3GetDiff   (b3_index xs,b3_index ys,b3_index zs,b3_index k,b3_index i);
	static b3_f64       b3Interpolate(b3_index ix,b3_index iy,b3_index iz,b3_f32 fx,b3_f32 fy,b3_f32 fz,b3_index d=0);
	static b3_f64       b3GradNoise (b3_f64 x,b3_f64 y,b3_f64 z,b3_index i);

	static void         b3OldMarble   (b3_vector *P,b3Color &Color);
	static void         b3MarbleCurve (b3Spline *Spline,b3_vector *result,b3_f64 x);
};

class b3Water
{
	b3_f64    m_Factor;

public:
	b3_f32    m_Km;
	b3_count  m_Octaves;
	b3_f32    m_WindAmp;
	b3_f32    m_WindFreq;
	b3_f32    m_MinWind;
	b3_f32    m_ScaleTime;           // time period for wave swing
	b3_vector m_Anim;

public:
	       b3Water();
	void   b3PrepareWater();
	b3_f64 b3ComputeWater(b3_vector *point, b3_f64 time);
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
	void   b3PrepareClouds();
	b3_f64 b3ComputeClouds(b3_line64 *ray,b3_f64 &r,b3_f64 time);
};

#endif
