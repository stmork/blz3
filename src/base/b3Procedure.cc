/*
**
**	$Filename:	b3Procedure.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
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

#include "blz3/base/b3Math.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/base/b3Spline.h"

#define USE_MORK_NOISE

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.24  2004/04/09 17:30:31  sm
**	- Wood dialog fine tuning.
**
**	Revision 1.23  2004/04/07 16:07:17  sm
**	- Moved wood computing outside b3MatWood for use in its own bump map.
**	
**	Revision 1.22  2004/04/07 11:38:25  sm
**	- Minor bugfix
**	
**	Revision 1.21  2004/04/06 12:17:46  sm
**	- Optimized some noise methods.
**	
**	Revision 1.20  2004/04/05 09:47:58  sm
**	- Some noise optimization
**	
**	Revision 1.19  2004/04/05 09:16:03  sm
**	- Added test wood for Lines wood dialog
**	- Optimized noise a little bit.
**	
**	Revision 1.18  2004/04/04 13:50:38  sm
**	- Optimized noise
**	- Added filtered noise
**	
**	Revision 1.17  2004/04/03 19:25:00  sm
**	- Some other wood
**	
**	Revision 1.16  2004/04/02 08:56:45  sm
**	- Computed more realistic clouds.
**	
**	Revision 1.15  2004/03/21 16:08:35  sm
**	- Moved b3Cbrt from b3Cubic into b3Math and made it inlined.
**	
**	Revision 1.14  2004/02/28 13:51:53  sm
**	- Added Cook/Torrance material. But this is not a material
**	  it is a shader! Further reading is necessary to redesign
**	  Blizzard III to use programmable shader.
**	
**	Revision 1.13  2003/03/04 20:37:36  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.12  2002/12/22 12:07:52  sm
**	- Made some minor accuracy adjustments.
**	
**	Revision 1.11  2002/12/22 11:52:22  sm
**	- Ensure minimum volume for bounding boxes even for plain areas.
**	
**	Revision 1.10  2002/12/11 14:53:39  sm
**	- Made some comments
**	- Changed b3Noise class to s static one.
**	
**	Revision 1.9  2002/08/15 13:56:42  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.8  2002/08/09 13:20:18  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.7  2002/05/08 14:43:30  sm
**	- Changed some output from B3LOG_NORMAL to B3LOG_DEBUG
**	
**	Revision 1.6  2002/03/02 19:52:39  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.5  2002/03/02 19:19:09  sm
**	- Heavily cleaned up BSplines. Now ready!
**	
**	Revision 1.4  2002/03/02 15:24:35  sm
**	- Templetized splines (uhff).
**	- Prepared spline shapes for their creation.
**	  *** And now: Testing! Testing! Testing! ***
**	
**	Revision 1.3  2001/10/11 16:06:32  sm
**	- Cleaning up b3BSpline with including isolated methods.
**	- Cleaning up endian conversion routines and collecting into
**	  b3Endian
**	- Cleaning up some datatypes for proper display in Together.
**	
**	Revision 1.2  2001/10/07 20:41:32  sm
**	- Updating MSVC project status
**	
**	Revision 1.1  2001/10/07 20:17:26  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        definitions                                   **
**                                                                      **
*************************************************************************/

#define NOISEBITS         5
#define NOISEMAX         (1 << (NOISEBITS))
#define NOISEMAXALLOC    (NOISEMAX)
#define NOISEMASK        ((NOISEMAX) - 1)

#define FRAC(Value)      ((Value) - (b3_f32)((long)(Value)))
#define INDEX3D(x,y,z)   ((((\
	((x) & NOISEMASK)  << NOISEBITS) +\
	((y) & NOISEMASK)) << NOISEBITS) +\
	((z) & NOISEMASK))

#define MAXVAL           255
#define mul4b(a,b,c,d)   ((a)*(b)*(c)*(d))
#define scal             (NOISEMAX)

#define mSin(x)          ((sin(x) + 1.0) * 0.5)
#define mMod(x)          ((fabs(x) * 0.5 / M_PI) - floor(fabs(x) * 0.5 / M_PI))

/*************************************************************************
**                                                                      **
**                        variables                                     **
**                                                                      **
*************************************************************************/

static b3Spline      marbleSpline;
static b3_f32        marbleKnots[16];
static b3_vector     marbleControls[8] =
{
	{ 0.0f,0.5f,0},
	{ 1.0f,0.7f,0},
	{ 4.7f,0.9f,0},
	{ 4.9f,0.0f,0},
	{ 5.1f,0.0f,0},
	{ 5.3f,0.8f,0},
	{ 9.0f,0.7f,0},
	{10.0f,0.5f,0}
};

static b3Spline      woodSpline;
static b3_f32        woodKnots[16];
static b3_vector     woodControls[8] =
{
	{ 0.0f,0.4f,0},
	{ 2.0f,0.9f,0},
	{ 4.0f,0.9f,0},
	{ 6.0f,0.7f,0},
	{ 7.0f,0.6f,0},
	{ 8.0f,0.5f,0},
	{ 9.0f,0.4f,0},
	{10.0f,0.4f,0}
};

static b3Spline      waveSpline;
static b3_f32        waveKnots[14];
static b3_vector     waveControls[10];

static b3_u08 oM[4][8][3] =
{
	{{1,0,0},{1,0,1},{1,1,0},{1,1,1},{2,0,0},{2,0,1},{2,1,0},{2,1,1}},
	{{0,1,0},{0,1,1},{0,2,0},{0,2,1},{1,1,0},{1,1,1},{1,2,0},{1,2,1}},
	{{0,0,1},{0,0,2},{0,1,1},{0,1,2},{1,0,1},{1,0,2},{1,1,1},{1,1,2}},
	{{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}}
};

b3_f64  b3Noise::epsilon = 0.005;
b3Noise noise_procedures;

/*************************************************************************
**                                                                      **
**                        initialize Noise                              **
**                                                                      **
*************************************************************************/

b3_noisetype *b3Noise::NoiseTable = null;

b3Noise::b3Noise ()
{
	b3_noisetype *Table;
	b3_coord      x,y,z;
	b3_count      i;

	if (NoiseTable == null)
	{
		b3PrintF(B3LOG_DEBUG,"Initializing noise procedures...\n");
		NoiseTable = Table = (b3_noisetype *)b3Alloc(
			NOISEMAXALLOC *
			NOISEMAXALLOC *
			NOISEMAXALLOC * sizeof(b3_noisetype));
		if (NoiseTable == null)
		{
			B3_THROW(b3NoiseException,B3_NOISE_MEMORY);
		}

		for (x = 0;x < NOISEMAX;x++)
		for (y = 0;y < NOISEMAX;y++)
		for (z = 0;z < NOISEMAX;z++)
		{
			Table[INDEX3D(x,y,z)] = (b3_noisetype)B3_IRAN(MAXVAL) / MAXVAL;
		}

		// init marble spline
		marbleSpline.knot_max    = sizeof(marbleKnots)    / sizeof(b3_f32);
		marbleSpline.control_max = sizeof(marbleControls) / sizeof(b3_vector);
		marbleSpline.offset      = 1;
		marbleSpline.knots       = marbleKnots;
		marbleSpline.controls    = marbleControls;
		marbleSpline.b3InitCurve         (3,marbleSpline.control_max,false);
		marbleSpline.b3ThroughEndControl ();

		// init wood spline
		woodSpline.knot_max     = sizeof(woodKnots)    / sizeof(b3_f32);
		woodSpline.control_max  = sizeof(woodControls) / sizeof(b3_vector);
		woodSpline.offset       = 1;
		woodSpline.knots        = woodKnots;
		woodSpline.controls     = woodControls;
		woodSpline.b3InitCurve         (3,woodSpline.control_max,false);
		woodSpline.b3ThroughEndControl ();

		// init wave spline
		waveSpline.knot_max     = sizeof(waveKnots)    / sizeof(b3_f32);
		waveSpline.control_max  = sizeof(waveControls) / sizeof(b3_vector);
		waveSpline.offset       = 1;
		waveSpline.knots        = waveKnots;
		waveSpline.controls     = waveControls;
		waveSpline.b3InitCurve(3,waveSpline.control_max,true);
		for (i = 0;i < waveSpline.control_max;i++)
		{
			waveControls[i].x = cos(M_PI * 2.0 * i / waveSpline.control_max);
			waveControls[i].y = sin(M_PI * 2.0 * i / waveSpline.control_max);
			waveControls[i].z = B3_FRAN(M_PI * 2.0) - M_PI;
		}
	}
}

b3Noise::~b3Noise ()
{
	NoiseTable = null;
}

/*************************************************************************
**                                                                      **
**                        Perlin noise implementation                   **
**                                                                      **
*************************************************************************/

/*
**  linear interpolation in 3d noise array with random numbers
**  datatype: b3_noisetype (unsigned char)
**
**
**
**                3-------------*--------------7
**                a[3]         /b[1]           b[3]
**                            /
**                           *b[0]
**                          /|
**                         / |
**                        /  |
**                       /   |
**                      /    |
**       1-------------*--------------6
**       a[2]          a[1]  |		  b[2]
**                           |
**       |<-- ox ----->|     |
**                           |
**          -                #a[0]  <== interpolated point
**          ^                |
**          |                |
**         oz                |
**          |     2----------|--*--------------6
**          |     a[1]       | /b[0]           b[1]
**          v                |/
**          -     --         *a[0]
**               /          /
**              /          /
**             /          /
**            oy         /
**           /          /
**       0-------------*--------------4
**		 a[0]          a[0]           b[0]
**
**
*/

b3_f64 b3Noise::b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
{
	b3_index ix,iy,iz;

	ix = (b3_index)floor(x);
	iy = (b3_index)floor(y);
	iz = (b3_index)floor(z);

	return b3Interpolate(ix,iy,iz,
		(b3_f32)x - ix,
		(b3_f32)y - iy,
		(b3_f32)z - iz);
}

b3_f64 b3Noise::b3FilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
{
	b3_index ix,iy,iz;

	ix = (b3_index)floor(x);
	iy = (b3_index)floor(y);
	iz = (b3_index)floor(z);

	return b3Interpolate(ix,iy,iz,
		(b3_f32)(b3Math::b3Smoothstep(x - ix)),
		(b3_f32)(b3Math::b3Smoothstep(y - iy)),
		(b3_f32)(b3Math::b3Smoothstep(z - iz)));
}

inline b3_f64 b3Noise::b3Interpolate(
	b3_index ix,
	b3_index iy,
	b3_index iz,
	b3_f32   fx,
	b3_f32   fy,
	b3_f32   fz)
{
	b3_f32  B3_ALIGN_16 a[4],b[4],c[4];
	b3_f32  rx = 1.0 - fx;
	b3_f32  ry = 1.0 - fy;
	b3_f32  rz = 1.0 - fz;
	b3_loop i;

	a[0] = NoiseTable[INDEX3D(ix  ,iy  ,iz  )];
	b[0] = NoiseTable[INDEX3D(ix+1,iy  ,iz  )];
	a[1] = NoiseTable[INDEX3D(ix  ,iy+1,iz  )];
	b[1] = NoiseTable[INDEX3D(ix+1,iy+1,iz  )];
	a[2] = NoiseTable[INDEX3D(ix  ,iy  ,iz+1)];
	b[2] = NoiseTable[INDEX3D(ix+1,iy  ,iz+1)];
	a[3] = NoiseTable[INDEX3D(ix  ,iy+1,iz+1)];
	b[3] = NoiseTable[INDEX3D(ix+1,iy+1,iz+1)];

	for (i = 0;i < 4;i++)
	{
		c[i] = a[i] * rx + b[i] * fx;
	}

	a[0] = c[0];
	b[0] = c[1];
	a[1] = c[2];
	b[1] = c[3];

	for (i = 0;i < 2;i++)
	{
		c[i] = a[i] * ry + b[i] * fy;
	}

	return c[0] * rz + c[1] * fz;
}

inline b3_noisetype b3Noise::b3GetDiff(
	b3_index xs,
	b3_index ys,
	b3_index zs,
	b3_index k,
	b3_index i)
{
	return (b3_noisetype)(
		NoiseTable[INDEX3D(xs + oM[k][i][0],
		                   ys + oM[k][i][1],
		                   zs + oM[k][i][2])] -
		NoiseTable[INDEX3D(xs + oM[3][i][0],
		                   ys + oM[3][i][1],
		                   zs + oM[3][i][2])]);
}

inline b3_f64 b3Noise::b3GradNoise (
	b3_f64    x,
	b3_f64    y,
	b3_f64    z,
	b3_index  i)
{
	b3_f32  B3_ALIGN_16 a[4],b[4],c[4];
	b3_index ix = (b3_index)floor(x);
	b3_index iy = (b3_index)floor(y);
	b3_index iz = (b3_index)floor(z);
	b3_f32   fx = x   - ix;
	b3_f32   fy = y   - iy;
	b3_f32   fz = z   - iz;
	b3_f32   rx = 1.0 - fx;
	b3_f32   ry = 1.0 - fy;
	b3_f32   rz = 1.0 - fz;
	b3_loop  l;

	a[0] = b3GetDiff(ix,iy,iz,i,0);
	b[0] = b3GetDiff(ix,iy,iz,i,4);
	a[1] = b3GetDiff(ix,iy,iz,i,2);
	b[1] = b3GetDiff(ix,iy,iz,i,6);
	a[2] = b3GetDiff(ix,iy,iz,i,1);
	b[2] = b3GetDiff(ix,iy,iz,i,5);
	a[3] = b3GetDiff(ix,iy,iz,i,3);
	b[3] = b3GetDiff(ix,iy,iz,i,7);

	for (l = 0;l < 4;l++)
	{
		c[l] = a[l] * rx + b[l] * fx;
	}

	a[0] = c[0];
	b[0] = c[1];
	a[1] = c[2];
	b[1] = c[3];

	for (l = 0;l < 2;l++)
	{
		c[l] = a[l] * ry + b[l] * fy;
	}

	return c[0] * rz + c[1] * fz;
}

void b3Noise::b3NoiseDeriv (
	b3_f64     dx,
	b3_f64     dy,
	b3_f64     dz,
	b3_vector *RVec)
{
	dx = fabs(dx * scal + 15000);
	dy = fabs(dy * scal + 15000);
	dz = fabs(dz * scal + 15000);
	RVec->x = b3GradNoise (dx,dy,dz,0);
	RVec->y = b3GradNoise (dx,dy,dz,1);
	RVec->z = b3GradNoise (dx,dy,dz,2);
}

/*************************************************************************
**                                                                      **
**                        marble procedures                             **
**                                                                      **
*************************************************************************/

static b3Color MarbleColors[4] =
{
	b3Color( 0.2f, 0.1f, 0.1f ),
	b3Color( 0.1f, 0.8f, 0.2f ),
	b3Color( 1.0f, 0.9f, 1.0f ),
	b3Color( 0.4f, 0.9f, 0.1f )
};

static inline void marbleCurve (
	b3Spline  *Spline,
	b3_vector *result,
	b3_f64     x)
{
	register b3_f32  *knots;
	register b3_f64  q;

	knots = Spline->knots;
	q =         x  * knots[Spline->degree] +
	     (1.0 - x) * knots[Spline->control_num];
	Spline->b3DeBoorOpened (result,0,q);
}

void b3Noise::b3Marble(b3_vector *d,b3Color &mask)
{
	b3_vector result;

	marbleCurve(&marbleSpline,&result,
		mSin(d->x + 0.5 * d->y + 0.3 * d->z + b3Turbulence(d)));

	mask.b3Init(result.y,result.y,result.y);
}

void b3Noise::b3OldMarble (b3_vector *P,b3Color &Color)
{
	b3_count  s,e;
	b3_f64    t,frac;
	b3_vector Dir;

	Dir.x = P->x * 0.08;
	Dir.y = P->y * 0.08;
	Dir.z = P->z * 0.08;

	t = b3Turbulence (&Dir);
	if (t > 1) t = 1;
	s        = (long)(t * 4)     & 3;
	e        = (long)(t * 4 + 1) & 3;
	frac     = t - s;

	Color = b3Color::b3Mix(MarbleColors[e], MarbleColors[s], frac);
}

/*************************************************************************
**                                                                      **
**                        wood procedures                               **
**                                                                      **
*************************************************************************/

void b3Noise::b3Wood(b3_vector *d,b3Color &mask)
{
	b3_vector result;
	b3_f64    s,r;

	s   = 2 * sqrt(d->x * d->x + d->z * d->z) - 0.15 * d->y;
	marbleCurve(&woodSpline,&result,mMod(s + b3Turbulence(d)));

	r = 1.0 - result.y;
	mask.b3Init(r,r,r);
}

/*************************************************************************
**                                                                      **
**                        other procedural textures                     **
**                                                                      **
*************************************************************************/

static b3Color HellColors[4] =
{
	b3Color(0.8f, 0.2f, 0.1f),
	b3Color(0.8f, 0.9f, 0.2f),
	b3Color(1.0f, 0.9f, 1.0f),
	b3Color(0.7f, 0.2f, 0.0f)
};

void b3Noise::b3Hell (b3_vector *P,b3Color &Color)
{
	b3_f64    t;
	b3_vector Dir;

	b3Vector::b3Scale(&Dir,P,0.08);

	t = b3Turbulence (&Dir);	
	if (t >= 1) t = 0.99;
	Color = HellColors[(int)(t * 4)];
}

static b3Color CloudHaze = b3Color(0.7,0.7,1.0);

void b3Noise::b3Clouds (b3_vector *P,b3Color &result)
{
	b3_vector Dir;
	b3_f64    scaling = 5.0;
	b3_f64    sharpness = 10.2;
	b3_f64    hazyness = 0.07;
	b3_f64    blue = 0.45;
	b3_f64    r,factor,sight;

	if (P->z > 0)
	{
		factor = scaling / P->z;
		Dir.x = P->x * factor;
		Dir.y = P->y * factor;
		Dir.z = 1.0;

		r = 1.0 - pow(b3Turbulence (&Dir),-sharpness);
		if (r < 0)
		{
			r = 0;
		}
		sight = exp(-hazyness * b3Vector::b3Length(&Dir));
		result = b3Color::b3Mix(CloudHaze,b3Color(r,r,B3_MAX(r,blue)),sight);
	}
	else
	{
		result = CloudHaze;
	}
}

inline b3_f64 b3Noise::b3Frac(b3_f64 a,b3_f64 b)
{
	b3_s32 n;

	if (a < 0)
	{
		a = -a;
		n = (b3_s32)(a / b);
		return b - a + n * b;
	}
	else
	{
		n = (b3_s32)(a / b);
		return a - n * b;
	}
}

b3_f64 b3Noise::b3Wave(b3_vector *point)
{
	b3_f64 n,q;
	b3_vector v;

	n = b3NoiseVector(point->x * 0.5,point->y,point->z);
	q = b3Frac(point->x * 0.5 + n,(b3_f64)waveSpline.control_max);
	waveSpline.b3DeBoorClosed (&v,0,q);
	return mSin(point->y * 10 + v.z * 2);
}

b3_f64 b3Noise::b3Water(b3_vector *point,b3_f64 time)
{
	b3_f64 phase,delay;

	phase = b3NoiseVector(
		point->x * 10.0,
		point->y * 20.0,
		point->z * 20.0);
	delay = (time + b3NoiseVector(
		point->x * 3.0,
		point->y * 7.0,
		point->z * 14.0)) * 2.0 * M_PI;

	return mSin(phase * 2.0 * M_PI - M_PI + delay);
}

/*************************************************************************
**                                                                      **
**                        some experiments                              **
**                                                                      **
*************************************************************************/

b3_f64 b3Noise::b3PGauss()
{
	b3_f64 u1,u2,v1,v2,s;

	do
	{
		u1 = B3_FRAN(1.0);
		u2 = B3_FRAN(1.0);
		v1 = u1 + u1 - 1;
		v2 = u2 + u2 - 1;
		s  = v1 * v1 + v2 * v2;
	}
	while (s >= 1.0);
	return v1 * sqrt((-2.0 * log(s)) / s);
}

/*************************************************************************
**                                                                      **
**                        Perlin noise implementation                   **
**                                                                      **
*************************************************************************/

void b3Wood::b3InitWood()
{
	b3Vector::b3Init(&m_Scale,40,40,40);
	m_yRot                   = (b3_f32)(  0.5 * M_PI );
	m_zRot                   = (b3_f32)( -0.5 * M_PI );
	m_Ringy                  =   1;
	m_Grainy                 =   1;
	m_GrainFrequency         =  25;

	m_RingSpacing            =   0.2f;
	m_RingFrequency          =   0.2f;
	m_RingNoise              =   0.2f;
	m_RingNoiseFrequency     =   1;
	m_TrunkWobble            =   0.1f;
	m_TrunkWobbleFrequency   =   0.025f;
	m_AngularWobble          =   0.1f;
	m_AngularWobbleFrequency =   0.9f;
}

void b3Wood::b3PrepareWood()
{
	b3Matrix::b3Move   (null,   &m_Warp,-0.5,-0.5,-0.5);
	b3Matrix::b3Scale  (&m_Warp,&m_Warp,null,m_Scale.x * M_PI,m_Scale.y * M_PI,m_Scale.z * M_PI);
	b3Matrix::b3RotateZ(&m_Warp,&m_Warp,null,m_zRot);
	b3Matrix::b3RotateY(&m_Warp,&m_Warp,null,m_yRot);
}

b3_f64 b3Wood::b3ComputeWood(b3_vector *polar)
{
	b3_vector d;
	b3_vector offset;
	b3_vector Pring;
	b3_vector aux;
	b3_vector Pgrain;
	b3_f64    dPshad = 1; // FIXME
	b3_f64    dPgrain;
	b3_f64    inring;
	b3_f64    grain = 0;
	b3_f64    amp = 1;
	b3_loop   i;

	b3Matrix::b3VMul(&m_Warp,polar,&d,true);
	b3Noise::b3VFBm(&d,dPshad * m_RingNoiseFrequency,2,4,0.5,&offset);

	b3Noise::b3SignedNoiseDeriv(0,0,d.z * m_TrunkWobbleFrequency,&aux);
	Pring.x = d.x + m_RingNoise * offset.x + m_TrunkWobble * aux.x;
	Pring.y = d.y + m_RingNoise * offset.y + m_TrunkWobble * aux.y;
	Pring.z = d.z + m_RingNoise * offset.z;

	// Calculate radius from center
	b3_f64 r2 = Pring.x * Pring.x + Pring.y * Pring.y;
	b3_f64 r  = sqrt(r2) * m_RingFrequency;

	// For unround rings...
	r += m_AngularWobble * b3Math::b3Smoothstep(0,5,r) * b3Noise::b3SignedNoiseVector(
		Pring.x * m_AngularWobbleFrequency,
		Pring.y * m_AngularWobbleFrequency,
		Pring.z * m_AngularWobbleFrequency * 0.1);
	
	// Ensure unequally spaced rings
	r += m_RingSpacing * b3Noise::b3SignedFilteredNoiseVector(0,0,r);

	inring = b3Math::b3SmoothPulse(0.1,0.55,0.7,0.95,fmod(r,1.0));

	Pgrain.x = d.x * m_GrainFrequency;
	Pgrain.y = d.y * m_GrainFrequency;
	Pgrain.z = d.z * m_GrainFrequency * 0.05;
	dPgrain = 1; // FIXME

	for (i = 0;i < 2;i++)
	{
		b3_f64 grain1valid = 1 - b3Math::b3Smoothstep(0.2,0.6,dPgrain);
		if (grain1valid > 0)
		{
			b3_f64 g = grain1valid * b3Noise::b3SignedNoiseVector(Pgrain.x,Pgrain.y,Pgrain.z);

			g *= (0.3 + 0.7 * inring);
			g  = b3Math::b3Limit(0.8 - g,0,1);
			g *= g;
			g  = m_Grainy * b3Math::b3Smoothstep(0.5,1,g);
			if (i == 0)
			{
				inring *= (1 - 0.4 * grain1valid);
			}
			grain = B3_MAX(grain,g);
		}
		Pgrain.x += Pgrain.x;
		Pgrain.y += Pgrain.y;
		Pgrain.z += Pgrain.z;
		dPgrain  += dPgrain;
		amp      *= 0.5;
	}

	return inring * m_Ringy * (1 - grain) + grain;
}
