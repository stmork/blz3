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
**	Revision 1.38  2004/05/26 12:47:20  sm
**	- Optimized recursive shading
**	- Optimized pow to an integer version (b3Math::b3FastPow)
**
**	Revision 1.37  2004/05/18 16:31:01  sm
**	- Simple valud adjustment.
**	
**	Revision 1.36  2004/05/18 13:34:50  sm
**	- Cleaned up water animation
**	
**	Revision 1.35  2004/05/18 10:44:52  sm
**	- Fine tuning animated water.
**	
**	Revision 1.34  2004/05/16 18:50:59  sm
**	- Added new simple image sampler.
**	- We need better water!
**	
**	Revision 1.33  2004/05/15 10:09:13  sm
**	- Added b3CloudBackground to b3Special item list.
**	
**	Revision 1.32  2004/05/15 07:51:02  sm
**	- Some noise optimizations
**	
**	Revision 1.31  2004/05/14 16:16:52  sm
**	- Modified water
**	- Added some water values to its property dialog
**	
**	Revision 1.30  2004/05/12 14:13:27  sm
**	- Added bump dialogs:
**	  o noise
**	  o marble
**	  o texture
**	  o glossy
**	  o groove
**	  o water
**	  o wave
**	- Setup all bump items with default values.
**	- Found bug 22 which show a camera deletion bug in
**	  toolbar and camera property dialog.
**	- Added bump example bwd
**	- Recounted resource.h (full compile necessary)
**	
**	Revision 1.29  2004/05/10 15:12:09  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.28  2004/04/22 14:35:16  sm
**	- Optimized clouds by making them inline.
**	
**	Revision 1.27  2004/04/22 14:28:44  sm
**	- Adjusted clouds.
**	
**	Revision 1.26  2004/04/10 19:12:46  sm
**	- Splitted up some header/source files:
**	  o b3Wood/b3OakPlank
**	  o b3MaterialSampler
**	- Cleaneup
**	
**	Revision 1.25  2004/04/10 13:45:30  sm
**	- Added wooden oak planks.
**	
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
#define NOISEDIM         3

#define INDEX3D(x,y,z)   ((((\
	((x) & NOISEMASK)  << NOISEBITS) +\
	((y) & NOISEMASK)) << NOISEBITS) +\
	((z) & NOISEMASK))

#define DIMSIZE          (NOISEMAXALLOC * NOISEMAXALLOC * NOISEMAXALLOC)

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

b3_noisetype *b3Noise::m_NoiseTable = null;

b3Noise::b3Noise ()
{
	b3_noisetype *Table;
	b3_coord      x,y,z,d;
	b3_count      i;

	if (m_NoiseTable == null)
	{
		b3PrintF(B3LOG_DEBUG,"Initializing noise procedures...\n");
		m_NoiseTable = (b3_noisetype *)b3Alloc(
			NOISEDIM      *
			NOISEMAXALLOC *
			NOISEMAXALLOC *
			NOISEMAXALLOC * sizeof(b3_noisetype));
		if (m_NoiseTable == null)
		{
			B3_THROW(b3NoiseException,B3_NOISE_MEMORY);
		}

		for (d = 0;d < NOISEDIM;d++)
		for (x = 0;x < NOISEMAX;x++)
		for (y = 0;y < NOISEMAX;y++)
		for (z = 0;z < NOISEMAX;z++)
		{
			Table = &m_NoiseTable[d * DIMSIZE];
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
	m_NoiseTable = null;
}

/*************************************************************************
**                                                                      **
**                        Perlin noise implementation                   **
**                                                                      **
*************************************************************************/

b3_f64 b3Noise::b3NoiseScalar(b3_f64 x)
{
	b3_index ix;
	b3_f64   fx,a,b;

	ix = (b3_index)floor(x);
	fx = (b3_f32)x - ix;

	a = m_NoiseTable[INDEX3D(ix  ,0,0  )];
	b = m_NoiseTable[INDEX3D(ix+1,0,0  )];

	return a * (1.0 - fx) + b * fx;
}

b3_f64 b3Noise::b3FilteredNoiseScalar(b3_f64 x)
{
	b3_index ix;
	b3_f64   fx,a,b;

	ix = (b3_index)floor(x);
	fx = b3Math::b3Smoothstep(x - ix);

	a = m_NoiseTable[INDEX3D(ix  ,0,0  )];
	b = m_NoiseTable[INDEX3D(ix+1,0,0  )];

	return a * (1.0 - fx) + b * fx;
}

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

void b3Noise::b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z,b3_vector *result)
{
	b3_index ix,iy,iz;

	ix = (b3_index)floor(x);
	iy = (b3_index)floor(y);
	iz = (b3_index)floor(z);

	result->x = b3Interpolate(ix,iy,iz,
		(b3_f32)x - ix,
		(b3_f32)y - iy,
		(b3_f32)z - iz,0);
	result->y = b3Interpolate(ix,iy,iz,
		(b3_f32)x - ix,
		(b3_f32)y - iy,
		(b3_f32)z - iz,1);
	result->z = b3Interpolate(ix,iy,iz,
		(b3_f32)x - ix,
		(b3_f32)y - iy,
		(b3_f32)z - iz,2);
}

void b3Noise::b3FilteredNoiseVector (b3_f64 x,b3_f64 y,b3_f64 z,b3_vector *result)
{
	b3_index ix,iy,iz;

	ix = (b3_index)floor(x);
	iy = (b3_index)floor(y);
	iz = (b3_index)floor(z);

	result->x = b3Interpolate(ix,iy,iz,
		(b3_f32)(b3Math::b3Smoothstep(x - ix)),
		(b3_f32)(b3Math::b3Smoothstep(y - iy)),
		(b3_f32)(b3Math::b3Smoothstep(z - iz)),0);
	result->y = b3Interpolate(ix,iy,iz,
		(b3_f32)(b3Math::b3Smoothstep(x - ix)),
		(b3_f32)(b3Math::b3Smoothstep(y - iy)),
		(b3_f32)(b3Math::b3Smoothstep(z - iz)),1);
	result->z = b3Interpolate(ix,iy,iz,
		(b3_f32)(b3Math::b3Smoothstep(x - ix)),
		(b3_f32)(b3Math::b3Smoothstep(y - iy)),
		(b3_f32)(b3Math::b3Smoothstep(z - iz)),2);
}

inline b3_f64 b3Noise::b3Interpolate(
	b3_index ix,
	b3_index iy,
	b3_index iz,
	b3_f32   fx,
	b3_f32   fy,
	b3_f32   fz,
	b3_index d)
{
	b3_noisetype *NoiseTable = &m_NoiseTable[d *DIMSIZE];
	b3_f32        B3_ALIGN_16 a[4],b[4],c[4];
	b3_f32        rx = 1.0 - fx;
	b3_f32        ry = 1.0 - fy;
	b3_f32        rz = 1.0 - fz;
	b3_loop       i;

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
		m_NoiseTable[INDEX3D(xs + oM[k][i][0],
		                     ys + oM[k][i][1],
		                     zs + oM[k][i][2])] -
		m_NoiseTable[INDEX3D(xs + oM[3][i][0],
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

b3_f64 b3Noise::b3Marble(b3_vector *d)
{
	b3_vector result;

	marbleCurve(&marbleSpline,&result,
		mSin(d->x + 0.5 * d->y + 0.3 * d->z + b3Turbulence(d)));

	return result.y;
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

b3_f64 b3Noise::b3Granite(b3_vector *point,b3_count octaves)
{
	b3_loop   i;
	b3_f64    sum = 0;
	b3_f64    freq = 1.0;
	b3_f64    x,y,z;

	x = point->x * 4;
	y = point->y * 4;
	z = point->z * 4;
	for (i = 0;i < octaves;i++)
	{
		sum  += b3Noise::b3FilteredNoiseVector(x,y,z) / freq;
		x    += x;
		y    += y;
		z    += z;
		freq += freq; // = freq *= 2;
	}
	return b3Math::b3Limit(sum);
}

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
	b3PrepareWater();
}

void b3Water::b3PrepareWater()
{
	m_Factor = 10 * m_WindFreq;
}

b3_f64 b3Water::b3ComputeWater(b3_vector *point, b3_f64 time)
{
	b3_vector P;
	b3_f64    offset,turbulence;

	P.x = point->x * m_Factor + time * m_Anim.x;
	P.y = point->y * m_Factor + time * m_Anim.y;
	P.z = point->z * m_Factor + time * m_Anim.z * m_ScaleTime;
	offset = m_Km * b3Noise::b3FractionalBrownianMotion(&P,m_Octaves,2.0,1.0);

	P.x *= 8;
	P.y *= 8;
	P.z *= 8;
	turbulence = b3Noise::b3Turbulence(&P, 3);

	return (m_MinWind + m_WindAmp * turbulence) * offset;
}

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

b3_f64 b3Clouds::b3ComputeClouds(b3_line64 *ray,b3_f64 &r,b3_f64 time)
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
