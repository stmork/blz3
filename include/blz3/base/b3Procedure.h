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

	static        b3_f64        b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);
	static inline b3_f64  b3SignedNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return b3NoiseVector(x,y,z) * 2 - 1;
	}
	
	static        b3_f64        b3FilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);
	static inline b3_f64  b3SignedFilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return b3NoiseVector(x,y,z) * 2 - 1;
	}
	
	static        void       b3NoiseDeriv (b3_f64 dx,b3_f64 dy,b3_f64 dz,b3_vector *result);
	static inline void b3SignedNoiseDeriv (b3_f64 dx,b3_f64 dy,b3_f64 dz,b3_vector *result)
	{
		b3NoiseDeriv(dx,dy,dz,result);
		
		result->x = result->x * 2 - 1;
		result->y = result->y * 2 - 1;
		result->z = result->z * 2 - 1;
	}

	static b3_f64  b3Turbulence  (b3_vector *P);

	static inline b3_f64  b3FBm(b3_vector &p,b3_f64 width,b3_f64 octaves,b3_f64 lacunarity,b3_f64 gain)
	{
		b3_f64    sum = 0;
		b3_vector pp = p;
		b3_f64    fw = width;
		b3_f64    amp = 1;
		b3_loop   i;

		for (i = 0;i < octaves;i++)
		{
			sum  += amp * b3NoiseVector(pp.x,pp.y,pp.z);
			amp  *= gain;
			pp.x *= lacunarity;
			pp.y *= lacunarity;
			pp.z *= lacunarity;
			fw   *= lacunarity;
		}
		return sum;
	}

	static inline void b3VFBm(b3_vector *p,b3_f64 width,b3_f64 octaves,b3_f64 lacunarity,b3_f64 gain,b3_vector *result)
	{
		b3_vector pp  = *p;
		b3_vector aux;
		b3_f64    fw  = width;
		b3_f64    amp = 1;
		b3_loop   i;

		b3Vector::b3Init(result);
		for (i = 0;i < octaves;i++)
		{
			b3NoiseDeriv(pp.x,pp.y,pp.z,&aux);

			result->x += aux.x * amp;
			result->y += aux.y * amp;
			result->z += aux.z * amp;
			amp       *= gain;
			pp.x *= lacunarity;
			pp.y *= lacunarity;
			pp.z *= lacunarity;
			fw   *= lacunarity;
		}
	}

	static void    b3Marble      (b3_vector *d,b3Color &mask);
	static void    b3Wood        (b3_vector *d,b3Color &mask);
	static void    b3Hell        (b3_vector *P,b3Color &Color);
	static void    b3Clouds      (b3_vector *P,b3Color &Color);
	static b3_f64  b3Wave        (b3_vector *point);
	static b3_f64  b3Water       (b3_vector *point,b3_f64 time);
	static b3_f64  b3PGauss      ();

private:
	static b3_f64       b3Frac      (b3_f64 a,b3_f64 b);
	static b3_noisetype b3GetDiff   (b3_index xs,b3_index ys,b3_index zs,b3_index k,b3_index i);
	static b3_f64       b3Interpolate(b3_index ix,b3_index iy,b3_index iz,b3_f32 fx,b3_f32 fy,b3_f32 fz);
	static b3_f64       b3GradNoise (b3_f64 x,b3_f64 y,b3_f64 z,b3_index i);

	static void         b3OldMarble   (b3_vector *P,b3Color &Color);
};

#endif
