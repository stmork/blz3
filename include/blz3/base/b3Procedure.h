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
#include "blz3/base/b3Exception.h"

typedef unsigned char b3_noisetype;

enum b3_noise_error
{
	B3_NOISE_ERROR = -1,
	B3_NOISE_OK    =  0,
	B3_NOISE_MEMORY
};

typedef b3Exception<b3_noise_error> b3NoiseException;

class b3Noise : public b3Mem
{
	b3_noisetype *NoiseTable;

public:
	        b3Noise();
	       ~b3Noise();

	b3_f64  b3Turbulence (b3_vector *P);
	b3_f64  b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z);
	void    b3NoiseDeriv (b3_f64  dx,b3_f64  dy,b3_f64  dz,b3_vector *RVec);
	void    b3Marble(b3_vector *d,b3_color *mask);
	void    b3OldMarble (b3_vector *P,b3_color *Color);
	void    b3Wood(b3_vector *d,b3_color *mask);
	void    b3Hell (b3_vector *P,b3_color *Color);
	void    b3Clouds (b3_vector *P,b3_color *Color);
	b3_f64  b3Wave(b3_vector *point);
	b3_f64  b3Water(b3_vector *point,b3_f64 time);
	b3_f64  b3PGauss();

private:
	void         b3InitCurve();
	b3_f64       b3Frac(b3_f64 a,b3_f64 b);
	b3_noisetype b3GetDiff(b3_index xs,b3_index ys,b3_index zs,b3_index k,b3_index i);
	b3_f64       b3GradNoise (b3_f64 x,b3_f64 y,b3_f64 z,b3_index i);
};

extern b3Noise noise_procedures;

#endif
