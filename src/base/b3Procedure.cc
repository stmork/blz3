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
#define nscal            (1.0/MAXVAL)

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
b3_f64  b3Math::epsilon = 0.005;
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
			Table[INDEX3D(x,y,z)] = (b3_noisetype)B3_IRAN(MAXVAL);

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
**                        random functions                              **
**                                                                      **
*************************************************************************/

/*
**  linear interpolation in 3d noise array with random numbers
**  datatype: b3_noisetype (unsigned char)
**
**
**
**                3-------------*--------------7
**                             /
**                            /
**                           *
**                          /|
**                         / |
**                        /  |
**                       /   |
**                      /    |
**       1-------------*--------------6
**                           |
**                           |
**       |<-- ox ----->|     |
**                           |
**          -                #  <== interpolated point
**          ^                |
**          |                |
**         oz                |
**          |     2----------|--*--------------6
**          |                | /
**          v                |/
**          -     --         *
**               /          /
**              /          /
**             /          /
**            oy         /
**           /          /
**       0-------------*--------------4
**
**
**
*/

b3_f64 b3Noise::b3NoiseVector (b3_f64 x,b3_f64 y,b3_f64 z)
{
	register b3_f64    n00,n01,n10,n11;
	register b3_f64    ox,oy,oz,n0,n1;
	register b3_noisetype n;
	register long      ix,iy,iz;

	if (NoiseTable == null) return (0.0);

	ix = (long)(x = fabs(x + 15000)); ox = FRAC(x);
	iy = (long)(y = fabs(y + 15000)); oy = FRAC(y);
	iz = (long)(z = fabs(z + 15000)); oz = FRAC(z);


		// trilinear interpolation
		// first dimension
	  n =                NoiseTable[INDEX3D(ix  ,iy  ,iz  )];
	n00 =    n   + ox * (NoiseTable[INDEX3D(ix+1,iy  ,iz  )] - n);
	  n =                NoiseTable[INDEX3D(ix  ,iy  ,iz+1)];
	n01 =    n   + ox * (NoiseTable[INDEX3D(ix+1,iy  ,iz+1)] - n);
	  n =                NoiseTable[INDEX3D(ix  ,iy+1,iz  )];
	n10 =    n   + ox * (NoiseTable[INDEX3D(ix+1,iy+1,iz  )] - n);
	  n =                NoiseTable[INDEX3D(ix  ,iy+1,iz+1)];
	n11 =    n   + ox * (NoiseTable[INDEX3D(ix+1,iy+1,iz+1)] - n);


	// second dimension
	n0  =    n00 + oy * (n10 - n00);
	n1  =    n01 + oy * (n11 - n01);


	// third and last dimension
	return ((n0  + oz * (n1  - n0)) * nscal);
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
	register b3_f64    n00,n01,n10,n11,n0,n1;
	register b3_f64    ox,oy,oz;
	register b3_noisetype n;
	register long      ix,iy,iz;

	if (NoiseTable == null) return (0.0);

	ix = (long)x; ox = FRAC(x);
	iy = (long)y; oy = FRAC(y);
	iz = (long)z; oz = FRAC(z);


		// trilinear interpolation
		// first dimension
	  n =                b3GetDiff(ix,iy,iz,i,0);
	n00 =    n   + ox * (b3GetDiff(ix,iy,iz,i,4) - n);
	  n =                b3GetDiff(ix,iy,iz,i,1);
	n01 =    n   + ox * (b3GetDiff(ix,iy,iz,i,5) - n);
	  n =                b3GetDiff(ix,iy,iz,i,2);
	n10 =    n   + ox * (b3GetDiff(ix,iy,iz,i,6) - n);
	  n =                b3GetDiff(ix,iy,iz,i,3);
	n11 =    n   + ox * (b3GetDiff(ix,iy,iz,i,7) - n);


	// second dimension
	n0  =    n00 + oy * (n10 - n00);
	n1  =    n01 + oy * (n11 - n01);

	// third and last dimension
	return ((n0 + oz * (n1 - n0)) * nscal * 2 - 1);
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

b3_f64 b3Noise::b3Turbulence (b3_vector *P)
{
	b3_f64   x,y,z,s,t;
	b3_count i;

	t = 0;
	s = 1;
	x = P->x;
	y = P->y;
	z = P->z;

	for (i = 0;i < 10;i++)
	{
		t += b3NoiseVector(x,y,z) * s;
		s *= 0.5;
		x += x;
		y += y;
		z += z;
	}
	return t;
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

	Color = MarbleColors[s] * frac + MarbleColors[e] * (1.0 - frac);
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

void b3Noise::b3Clouds (b3_vector *P,b3Color &Color)
{
	b3_vector Dir;
	b3_f64    r;

	b3Vector::b3Scale(&Dir,P,scal * 0.08);
	Dir.x *= 0.3f;

	r = b3Turbulence (&Dir) - 0.45;
	Color.b3Init(r,r,1.0);
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

#ifdef BLIZZARD2_ON_AMIGA

/*
** This is an nice code fragment from Blizzard II which
** was coded some time ago...
*/

/*
#ifdef YESTURBO
long WoodGrain ();

#asm
	public	_WoodGrain
_WoodGrain:
	fmove.d	4(sp),fp0					;u holen
	fmove.d	20(sp),fp1					;w holen
	fmul.s	#"$42800000",fp0			;u *= 64
	fmul.s	#"$42800000",fp1			;w *= 64
	fmove.x	fp0,fp2
	fmove.x	fp1,fp3
	fmul.x	fp2,fp0						;Komponenten multiplizieren
	fmul.x	fp3,fp1
	fadd.x	fp0,fp1						;Komponenten addieren
	fsqrt.x	fp1							;Radius berechnen
	fbne	.nopi
	fmove.s	#"$3fc90fda",fp0			;PI/2 holen 
	bra	.yespi
.nopi	fmove.x	fp3,fp0					;w holen
	fdiv.x	fp2,fp0						;/= u
	fatan.x	fp0							;Bogenmaß berechnen

.yespi	fmove.d	12(sp),fp2				;v holen
	fmul.s	#"$42800000",fp2			;v *= 64
	fmul.s	#"$41a00000",fp0			;atan *= 20
	fdiv.s	#"$43160000",fp2			;v /= 150
	fadd.x	fp0,fp2
	fsin.x	fp2
	fadd.x	fp2,fp2						;*= 2;
	fadd.x	fp2,fp1						;Radius += 2*sin()
	fmove.l	fp1,d1						;nach Integer wandeln
	and.l	#63,d1						;ausmaskieren

	moveq	#3,d0						;Farbe wählen
	cmp.l	#51,d1						;Rest vergleichen
	bge	.wg1L
	moveq	#2,d0
	cmp.l	#38,d1
	bge	.wg1L
	moveq	#1,d0
	cmp.l	#20,d1
	bge	.wg1L
	moveq	#0,d0
.wg1L
	rts
#endasm
#else

long WoodGrain (u,v,w)

register b3_f64 u,v,w;

{
	register b3_f64  radius,angle;
	register long    grain,Modulo = 80;

	u *= 64;
	v *= 64;
	w *= 64;
	radius = sqrt(u * u + w * w);
	if (w==0) angle = PId2;
	else      angle = atan (w/u);
	radius = radius + 2 * sin(20 * angle + v / 150);
	grain = (long)radius % Modulo;
	if (grain > 60) return 2;
	if (grain > 40) return 1;
	else            return 0;
}
#endif
*/

#endif

