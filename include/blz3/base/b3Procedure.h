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

/**
 * Tis enumeration lists some noise error states.
 */
enum b3_noise_error
{
	B3_NOISE_ERROR = -1, //!< General error.
	B3_NOISE_OK    =  0, //!< Everything is OK.
	B3_NOISE_MEMORY      //!< Out of memory error.
};

typedef b3Exception<b3_noise_error,'NOI'> b3NoiseException;

/**
 * This class provides some procedural texture methods based Perlin Noise.
 */
class B3_PLUGIN b3Noise
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

	static int                 m_Permutation[512];

	 b3Noise();
	~b3Noise();

public:
	/**
	 * This method returns a linear interpolated pseudo random float based on Perlin noise.
	 * The result is in range [0..1] and is computed from one scalar.
	 *
	 * @param x The input value.
	 * @return The pseudo random value.
	 */
	static        b3_f64        b3NoiseScalar (b3_f64 x);

	/**
	 * This method returns a linear interpolated pseudo random float based on Perlin noise.
	 * The result is in range [-1..1] and is computed from one scalar.
	 *
	 * @param x The input value.
	 * @return The pseudo random value.
	 */
	static inline b3_f64  b3SignedNoiseScalar (b3_f64 x)
	{
		return b3NoiseScalar(x) * 2 - 1;
	}
	
	/**
	 * This method returns a Hermite interpolated pseudo random float based on Perlin noise.
	 * The result is in range [0..1] and is computed from one scalar.
	 *
	 * @param x The input value.
	 * @return The pseudo random value.
	 */
	static        b3_f64        b3FilteredNoiseScalar (b3_f64 x);

	/**
	 * This method returns an Hermite interpolated pseudo random float based on Perlin noise.
	 * The result is in range [-1..1] and is computed from one scalar.
	 *
	 * @param x The input value.
	 * @return The pseudo random value.
	 */
	static inline b3_f64  b3SignedFilteredNoiseScalar (b3_f64 x)
	{
		return b3FilteredNoiseScalar(x) * 2 - 1;
	}
	
	/**
	 * This method returns a linear interpolated pseudo random float based on Perlin noise.
	 * The result is in range [0..1] and is computed from three scalars forming a vector.
	 *
	 * @param x The x input value.
	 * @param y The y input value.
	 * @param z The z input value.
	 * @return The pseudo random value.
	 */
	static        b3_f64        b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);

	/**
	 * This method returns a linear interpolated pseudo random float based on Perlin noise.
	 * The result is in range [-1..1] and is computed from three scalars forming a vector.
	 *
	 * @param x The x input value.
	 * @param y The y input value.
	 * @param z The z input value.
	 * @return The pseudo random value.
	 */
	static inline b3_f64  b3SignedNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return b3NoiseVector(x,y,z) * 2 - 1;
	}
	
	/**
	 * This method returns a Hermite interpolated pseudo random float based on Perlin noise.
	 * The result is in range [0..1] and is computed from three scalars forming a vector.
	 *
	 * @param x The x input value.
	 * @param y The y input value.
	 * @param z The z input value.
	 * @return The pseudo random value.
	 */
	static        b3_f64        b3FilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);

	/**
	 * This method returns an Hermite interpolated pseudo random float based on Perlin noise.
	 * The result is in range [-1..1] and is computed from three scalars forming a vector.
	 *
	 * @param x The x input value.
	 * @param y The y input value.
	 * @param z The z input value.
	 * @return The pseudo random value.
	 */
	static inline b3_f64  b3SignedFilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return b3FilteredNoiseVector(x,y,z) * 2 - 1;
	}

	/**
	 * This method returns a psoeudo random float base on Perlin noise. The value
	 * is in range [0..1]. The algorithm is somewhat optimized in comparison to the
	 * classic version b3NoiseVector().
	 *
	 *
	 * @param x The x input value.
	 * @param y The y input value.
	 * @param z The z input value.
	 * @return The pseudo random value.
	 */
	static inline b3_f64        b3ImprovedNoise    (b3_f64 x,b3_f64 y,b3_f64 z)
	{
		return (b3SignedImprovedNoise(x,y,z) + 1) * 0.5;
	}

	/**
	 * This method returns a psoeudo random float base on Perlin noise. The value
	 * is in range [-1..1]. The algorithm is somewhat optimized in comparison to the
	 * classic version b3NoiseVector().
	 *
	 *
	 * @param x The x input value.
	 * @param y The y input value.
	 * @param z The z input value.
	 * @return The pseudo random value.
	 */
	static        b3_f64  b3SignedImprovedNoise    (b3_f64 x,b3_f64 y,b3_f64 z);
	
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

	/**
	 * This method provides a turbulence function based on Perlin noise.
	 *
	 * @param P The position
	 * @param octaves The number of recursions.
	 * @return The resulting turbulence value.
	 */
	static inline b3_f64  b3Turbulence  (b3_vector *P,b3_count octaves = 10)
	{
		return b3FractionalBrownianMotion(P,octaves,2.0,0.5);
	}

	/**
	 * This method computes fractional brownian motion. The value is computed by adding
	 * a Perlin noise value at each iteration. The used position is reamplified each
	 * iteration. The resulting Perlin value is attenuated. This value is added to a
	 * resulting sum.
	 *
	 * @param p The position.
	 * @param octaves The number of iterations.
	 * @param amplification The amplification of the position for each iteration (= octave).
	 * @param attenuation The attenuation for each iteration (= octave) to the result.
	 * @return The resulting fractional brownian motion.
	 * @see b3FilteredNoiseVector()
	 */
	static inline b3_f64  b3FractionalBrownianMotion(
		b3_vector *p,
		b3_count   octaves,
		b3_f32     amplification,
		b3_f32     attenuation)
	{
		b3_f32 B3_ALIGN_16 v[4];
		b3_f32 B3_ALIGN_16 factor[4];
		b3_f64             sum = 0,n;
		b3_loop            i,k;

		v[0] = 1;    // starting attenuation
		v[1] = p->x;
		v[2] = p->y;
		v[3] = p->z;
		
		factor[0] = attenuation;
		factor[1] = amplification;
		factor[2] = amplification;
		factor[3] = amplification;

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

	/**
	 * This method computes fractional brownian motion. The value is computed by adding
	 * a Perlin noise derivative at each iteration. The used position is reamplified each
	 * iteration. The resulting Perlin value is attenuated. These values is added to a
	 * resulting sum.
	 *
	 * @param p The position.
	 * @param octaves The number of iterations.
	 * @param amplification The amplification of the position for each iteration (= octave).
	 * @param attenuation The attenuation for each iteration (= octave) to the result.
	 * @param result The resulting fractional brownian motion as vector.
	 * @see b3NoiseDeriv()
	 */
	static inline void b3FractionalBrownianMotion(
		b3_vector *p,
		b3_count   octaves,
		b3_f32     amplification,
		b3_f32     attenuation,
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
		
		factor[0] = attenuation;
		factor[1] = amplification;
		factor[2] = amplification;
		factor[3] = amplification;

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

	/**
	 * This method computes a marble pattern from a given position. The pattern
	 * is expressed as simple value between [0..1]. This value can be used for
	 * mixing two or more colors.
	 *
	 * @param d The position.
	 * @return The marble pattern.
	 */
	static b3_f64  b3Marble      (b3_vector *d);

	/**
	 * This method computes a wood pattern from a given position. The pattern
	 * is expressed as simple value between [0..1]. This value can be used for
	 * mixing two or more colors.
	 *
	 * @param d The position.
	 * @return The wood pattern.
	 */
	static b3_f64  b3Wood        (b3_vector *d);

	/**
	 * This method computes a sky cloud pattern from a given position. The pattern
	 * is expressed as interpolated color.
	 *
	 * @param P The position.
	 * @param Color The resulting sky pattern.
	 */
	static void    b3Hell        (b3_vector *P,b3Color &Color);

	/**
	 * This method computes a wave pattern from a given position. The pattern
	 * is expressed as simple value between [0..1]. This value can be used for
	 * mixing two or more colors.
	 *
	 * @param point The position.
	 * @return The wave pattern.
	 */
	static b3_f64  b3Wave        (b3_vector *point);

	/**
	 * This method computes a granite pattern from a given position. The pattern
	 * is expressed as simple value between [0..1]. This value can be used for
	 * mixing two or more colors.
	 *
	 * @param point The position.
	 * @param octaves The number of octaves to use.
	 * @return The granite pattern.
	 */
	static b3_f64  b3Granite     (b3_vector *point,b3_count octaves);
	static void    b3AnimThinFilm(b3_f64 t, b3_vector *result);

private:
	static b3_noisetype b3GetDiff   (b3_index xs,b3_index ys,b3_index zs,b3_index k,b3_index i);
	static b3_f64       b3Interpolate(b3_index ix,b3_index iy,b3_index iz,b3_f32 fx,b3_f32 fy,b3_f32 fz,b3_index d=0);
	static b3_f64       b3GradNoise (b3_f64 x,b3_f64 y,b3_f64 z,b3_index i);

	static void         b3OldMarble   (b3_vector *P,b3Color &Color);
	static void         b3MarbleCurve (b3Spline *Spline,b3_vector *result,b3_f64 x);

	static inline b3_f64 b3Grad(int hash, b3_f64 x, b3_f64 y, b3_f64 z)
	{
		int    h = hash & 15;                   // CONVERT LO 4 BITS OF HASH CODE
		b3_f64 u = h<8 ? x : y;                 // INTO 12 GRADIENT DIRECTIONS.
		b3_f64 v = h<4 ? y : h==12||h==14 ? x : z;

		return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
	}
};

/**
 * This class computes water simulation.
 */
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

/**
 * This class computes cloud simulation.
 */
class b3Clouds
{
	b3_f64    m_EarthRadiusSqr;
	b3_f64    m_CloudRadiusSqr;

public:
	b3_vector m_Anim;        //!< x/y are wind direction, z is time scaling.
	b3_vector m_PosScale;    //!< Position scaling.
	b3_u32    m_Flags;       //!< Some flags.
	b3_f32    m_EarthRadius; //!< The plant radius to use.
	b3_f32    m_CloudHeight; //!< The height of the clouds.
	b3_f32    m_Scaling;     //!< Scaling.
	b3_f32    m_Sharpness;   //!< The sharpness of the cloud border.

public:
	/**
	 * This constructor initializes this instance with default values.
	 */
	b3Clouds();

	/**
	 * This method precomputes some calculation invariant values.
	 */
	void   b3PrepareClouds();

	/**
	 * This method returns a cloud value in range [0..1]. The value 0 means
	 * clear sky and 1 means full cloud.
	 *
	 * @param ray The ray containing the view direction.
	 * @param r The distance to the cloud.
	 * @param time The time point for animation.
	 * @return The cloudiness.
	 */
	b3_f64 b3ComputeClouds(b3_line64 *ray,b3_f64 &r,b3_f64 time);
};

#endif
