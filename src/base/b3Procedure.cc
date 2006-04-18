/*
**
**	$Filename:	b3Procedure.c $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Perlin noise
**
**	(C) Copyright 2001, 2002, 2003, 2004, 2005  Steffen A. Mork
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
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/base/b3Spline.h"

#define USE_MORK_NOISE
#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.67  2006/04/18 15:48:59  sm
**	- Extracted from procedure module:
**	  o clouds
**	  o ocean waves
**	  o water
**
**	Revision 1.66  2006/04/17 16:36:34  sm
**	- Importing FFT and ocean waves into Windows project.
**	
**	Revision 1.65  2006/04/17 14:42:46  sm
**	- Completed ocean waves. I see ocean waves. They are not nice but
**	  I can see them!
**	
**	Revision 1.64  2006/04/16 21:05:03  sm
**	- Added FFT module.
**	- Changed ocean waves to FFT creation. Not working yet!
**	
**	Revision 1.63  2006/04/15 20:34:55  sm
**	- Added support for ocean surface bump mapping.
**	
**	Revision 1.62  2006/03/27 10:32:06  smork
**	- Renamed member variables of spline template class.
**	
**	Revision 1.61  2006/03/05 21:22:34  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.60  2005/10/09 14:39:41  sm
**	- Added HDR image processing
**	
**	Revision 1.59  2005/10/08 16:35:44  sm
**	- Fixed rounding problem
**	
**	Revision 1.58  2005/10/02 09:51:12  sm
**	- Added OpenEXR configuration.
**	- Added more excpetion handling.
**	
**	Revision 1.57  2005/09/11 17:45:52  sm
**	- Fixed wrong ordered b3MatGranite read.
**	
**	Revision 1.56  2005/08/13 13:29:02  sm
**	- Refactoring
**	
**	Revision 1.55  2005/08/10 13:20:44  smork
**	- Documentation.
**	
**	Revision 1.54  2005/08/10 12:49:42  smork
**	- Documentation.
**	
**	Revision 1.53  2005/08/10 10:32:08  smork
**	- Documentation.
**	
**	Revision 1.52  2005/08/10 09:01:37  smork
**	- Documentation
**	- Noise improvements.
**	
**	Revision 1.51  2005/07/31 19:34:33  sm
**	- Documentation
**	
**	Revision 1.50  2005/06/10 21:31:42  sm
**	- Vectorization fix for Windows
**	- Read binary from registry returns null when no
**	  entry is in registry.
**	
**	Revision 1.49  2005/05/19 10:27:33  smork
**	- Added improved Perlin noise.
**	
**	Revision 1.48  2005/05/18 10:29:34  smork
**	- Removed b3Mem from b3Array and b3Noise.
**	
**	Revision 1.47  2005/04/27 13:55:01  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**	
**	Revision 1.46  2005/02/02 09:08:25  smork
**	- Fine tuning of epsilon.
**	
**	Revision 1.45  2004/12/30 16:27:38  sm
**	- Removed assertion problem when starting Lines III: The
**	  image list were initialized twice due to double calling
**	  OnInitDialog() of CDialogBar. The CDialogBar::Create()
**	  calls OnInitDialog() automatically sinde MFC 7
**	- Removed many global references from raytrace and base lib
**	- Fixed ticket no. 29: The b3RenderObject::b3Recompute
**	  method checks the vertex maintainer against a null pointer.
**	
**	Revision 1.44  2004/12/22 21:36:37  sm
**	- Changed development environment to Visual C++ .net 2003
**	
**	Revision 1.43  2004/09/23 20:02:25  sm
**	- Introduced VBOs on Windows - with success!
**	
**	Revision 1.42  2004/09/23 09:31:33  sm
**	- Changed b3Runtime into a real singleton.
**	- Added functions for OpenGL extension.
**	- Removed b3PrintF()s from singletons.
**	
**	Revision 1.41  2004/08/10 09:23:13  sm
**	- Noise indexing optimized
**	- Added b3Color::b3Pow() method.
**	
**	Revision 1.40  2004/08/03 19:55:02  sm
**	- Changed thin film animation parameter.
**	
**	Revision 1.39  2004/08/02 13:57:59  sm
**	- Changed thin film animation to closed spline computation.
**	
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
#define NOISESIZE        (NOISEMAX * NOISEMAX * NOISEMAX)

#define INDEX3D(x,y,z)   ((((\
	((z) & NOISEMASK)  << NOISEBITS) +\
	((y) & NOISEMASK)) << NOISEBITS) +\
	((x) & NOISEMASK))

#define MAXVAL           255
#define mSin(x)          ((sin(x) + 1.0) * 0.5)
#define mMod(x)          ((fabs(x) * 0.5 / M_PI) - floor(fabs(x) * 0.5 / M_PI))

/*************************************************************************
**                                                                      **
**                        variables                                     **
**                                                                      **
*************************************************************************/

b3Spline      b3Noise::m_MarbleSpline;
b3_f32        b3Noise::m_MarbleKnots[16];
b3_vector     b3Noise::m_MarbleControls[8] =
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

b3Spline      b3Noise::m_WoodSpline;
b3_f32        b3Noise::m_WoodKnots[16];
b3_vector     b3Noise::m_WoodControls[8] =
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

b3Spline      b3Noise::m_WaveSpline;
b3_f32        b3Noise::m_WaveKnots[14];
b3_vector     b3Noise::m_WaveControls[10];

const b3_u08 b3Noise::m_OM[4][8][3] =
{
	{{1,0,0},{1,0,1},{1,1,0},{1,1,1},{2,0,0},{2,0,1},{2,1,0},{2,1,1}},
	{{0,1,0},{0,1,1},{0,2,0},{0,2,1},{1,1,0},{1,1,1},{1,2,0},{1,2,1}},
	{{0,0,1},{0,0,2},{0,1,1},{0,1,2},{1,0,1},{1,0,2},{1,1,1},{1,1,2}},
	{{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}}
};

/*************************************************************************
**                                                                      **
**                        initialize Noise                              **
**                                                                      **
*************************************************************************/

b3Noise       b3Noise::m_Noise;
b3_noisetype *b3Noise::m_NoiseTable = null;
b3_f64        b3Noise::epsilon      = FLT_EPSILON;

b3Noise::b3Noise () throw(b3NoiseException)
{
	if (m_NoiseTable == null)
	{
		b3_count i,max = NOISEDIM * NOISESIZE;

		m_NoiseTable = (b3_noisetype *)b3MemAccess::b3Alloc(max * sizeof(b3_noisetype));
		if (m_NoiseTable == null)
		{
			B3_THROW(b3NoiseException,B3_NOISE_MEMORY);
		}

		for (i = 0;i < max;i++)
		{
			m_NoiseTable[i] = (b3_noisetype)B3_IRAN(MAXVAL) / MAXVAL;
		}

		// init marble spline
		m_MarbleSpline.m_KnotMax    = sizeof(m_MarbleKnots)    / sizeof(b3_f32);
		m_MarbleSpline.m_ControlMax = sizeof(m_MarbleControls) / sizeof(b3_vector);
		m_MarbleSpline.m_Offset     = 1;
		m_MarbleSpline.m_Knots      = m_MarbleKnots;
		m_MarbleSpline.m_Controls   = m_MarbleControls;
		m_MarbleSpline.b3InitCurve         (3,m_MarbleSpline.m_ControlMax,false);
		m_MarbleSpline.b3ThroughEndControl ();

		// init wood spline
		m_WoodSpline.m_KnotMax     = sizeof(m_WoodKnots)    / sizeof(b3_f32);
		m_WoodSpline.m_ControlMax  = sizeof(m_WoodControls) / sizeof(b3_vector);
		m_WoodSpline.m_Offset      = 1;
		m_WoodSpline.m_Knots       = m_WoodKnots;
		m_WoodSpline.m_Controls    = m_WoodControls;
		m_WoodSpline.b3InitCurve         (3,m_WoodSpline.m_ControlMax,false);
		m_WoodSpline.b3ThroughEndControl ();

		// init wave spline
		m_WaveSpline.m_KnotMax     = sizeof(m_WaveKnots)    / sizeof(b3_f32);
		m_WaveSpline.m_ControlMax  = sizeof(m_WaveControls) / sizeof(b3_vector);
		m_WaveSpline.m_Offset      = 1;
		m_WaveSpline.m_Knots       = m_WaveKnots;
		m_WaveSpline.m_Controls    = m_WaveControls;
		m_WaveSpline.b3InitCurve(3,m_WaveSpline.m_ControlMax,true);
		for (i = 0;i < m_WaveSpline.m_ControlMax;i++)
		{
			m_WaveControls[i].x = cos(M_PI * 2.0 * i / m_WaveSpline.m_ControlMax);
			m_WaveControls[i].y = sin(M_PI * 2.0 * i / m_WaveSpline.m_ControlMax);
			m_WaveControls[i].z = B3_FRAN(M_PI * 2.0) - M_PI;
		}
	}
}

b3Noise::~b3Noise ()
{
	if (m_NoiseTable != null)
	{
		b3MemAccess::b3Free(m_NoiseTable);
		m_NoiseTable = null;
	}
}

/*************************************************************************
**                                                                      **
**                        Improved Perlin noise implementation          **
**                                                                      **
*************************************************************************/

int b3Noise::m_Permutation[512] = 
{
      151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
      140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
      247,120,234, 75,  0, 26,197, 62 ,94,252,219,203,117, 35, 11, 32,
       57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
       74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
       60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
       65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
      200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
       52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
      207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
      119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
      129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
      218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
       81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
      184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
      222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180,
      
      // repeat
      151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
      140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
      247,120,234, 75,  0, 26,197, 62 ,94,252,219,203,117, 35, 11, 32,
       57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
       74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
       60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
       65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
      200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
       52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
      207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
      119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
      129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
      218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
       81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
      184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
      222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180
};

b3_f64 b3Noise::b3SignedImprovedNoise(
	const b3_f32 cx,
	const b3_f32 cy,
	const b3_f32 cz)
{
	int X     = (int)floor(cx) & 255;      // FIND UNIT CUBE THAT
	int Y     = (int)floor(cy) & 255;      // CONTAINS POINT.
	int Z     = (int)floor(cz) & 255;
	b3_f32 x  = cx - floor(cx);            // FIND RELATIVE X,Y,Z
	b3_f32 y  = cy - floor(cy);            // OF POINT IN CUBE.
	b3_f32 z  = cz - floor(cz);
	b3_f32 u  = b3Math::b3Fade(x);         // COMPUTE FADE CURVES
	b3_f32 v  = b3Math::b3Fade(y);         // FOR EACH OF X,Y,Z.
	b3_f32 w  = b3Math::b3Fade(z);
	int A     = m_Permutation[X  ]+Y;
	int AA    = m_Permutation[A]+Z;
	int AB    = m_Permutation[A+1]+Z;      // HASH COORDINATES OF
	int B     = m_Permutation[X+1]+Y;
	int BA    = m_Permutation[B]+Z;
	int BB    = m_Permutation[B+1]+Z;      // THE 8 CUBE CORNERS,

	return
		b3Math::b3Mix(
			b3Math::b3Mix(
				b3Math::b3Mix(
					b3Grad(m_Permutation[AA  ], x  , y  , z   ),       // AND ADD
					b3Grad(m_Permutation[BA  ], x-1, y  , z   ),u),    // BLENDED
				b3Math::b3Mix(
					b3Grad(m_Permutation[AB  ], x  , y-1, z   ),       // RESULTS
					b3Grad(m_Permutation[BB  ], x-1, y-1, z   ),u),v), // FROM  8
			b3Math::b3Mix(
				b3Math::b3Mix(
					b3Grad(m_Permutation[AA+1], x  , y  , z-1 ),       // CORNERS
					b3Grad(m_Permutation[BA+1], x-1, y  , z-1 ),u),    // OF CUBE
				b3Math::b3Mix(
					b3Grad(m_Permutation[AB+1], x  , y-1, z-1 ),
					b3Grad(m_Permutation[BB+1], x-1, y-1, z-1 ),u),v),w);
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

	a = m_NoiseTable[ix];
	b = m_NoiseTable[(ix+1) & NOISEMASK];

	return b3Math::b3Mix(a, b, fx);
}

b3_f64 b3Noise::b3FilteredNoiseScalar(b3_f64 x)
{
	b3_index ix;
	b3_f64   fx,a,b;

	ix = (b3_index)floor(x);
	fx = b3Math::b3Smoothstep(x - ix);

	a = m_NoiseTable[ix];
	b = m_NoiseTable[(ix+1) & NOISEMASK];

	return b3Math::b3Mix(a, b, fx);
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

b3_f64 b3Noise::b3NoiseVector (
	const b3_f64 x,
	const b3_f64 y,
	const b3_f64 z)
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

b3_f64 b3Noise::b3FilteredNoiseVector (
	const b3_f64 x,
	const b3_f64 y,
	const b3_f64 z)
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

void b3Noise::b3NoiseVector (
	const b3_f64     x,
	const b3_f64     y,
	const b3_f64     z,
	      b3_vector *result)
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

void b3Noise::b3FilteredNoiseVector (
	const b3_f64     x,
	const b3_f64     y,
	const b3_f64     z,
	      b3_vector *result)
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
	const b3_index ix,
	const b3_index iy,
	const b3_index iz,
	const b3_f32    fx,
	const b3_f32   fy,
	const b3_f32   fz,
	const b3_index d)
{
	b3_noisetype *NoiseTable = &m_NoiseTable[d * NOISESIZE];
	b3_f32        B3_ALIGN_16 a[4],b[4],c[4];
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
		c[i] = a[i] + fx * (b[i] - a[i]);
	}

	a[0] = c[0];
	b[0] = c[1];
	a[1] = c[2];
	b[1] = c[3];

	for (i = 0;i < 2;i++)
	{
		c[i] = a[i] + fy * (b[i] - a[i]);
	}

	return c[0] + fz * (c[1] - c[0]);
}

inline b3_noisetype b3Noise::b3GetDiff(
	const b3_index xs,
	const b3_index ys,
	const b3_index zs,
	const b3_index k,
	const b3_index i)
{
	return (b3_noisetype)(
		m_NoiseTable[INDEX3D(xs + m_OM[k][i][0],
		                     ys + m_OM[k][i][1],
		                     zs + m_OM[k][i][2])] -
		m_NoiseTable[INDEX3D(xs + m_OM[3][i][0],
		                     ys + m_OM[3][i][1],
		                     zs + m_OM[3][i][2])]);
}

inline b3_f64 b3Noise::b3GradNoise (
	const b3_f32    x,
	const b3_f32    y,
	const b3_f32    z,
	      b3_index  i)
{
	b3_f32  B3_ALIGN_16 a[4],b[4],c[4];
	b3_index ix = (b3_index)floor(x);
	b3_index iy = (b3_index)floor(y);
	b3_index iz = (b3_index)floor(z);
	b3_f32   fx = x   - ix;
	b3_f32   fy = y   - iy;
	b3_f32   fz = z   - iz;
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
		c[l] = a[l] + fx * (b[l] - a[l]);
	}

	a[0] = c[0];
	b[0] = c[1];
	a[1] = c[2];
	b[1] = c[3];

	for (l = 0;l < 2;l++)
	{
		c[l] = a[l] + fy * (b[l] - a[l]);
	}

	return c[0] + fz * (c[1] -c[0]);
}

void b3Noise::b3NoiseDeriv (
	const b3_f64     cdx,
	const b3_f64     cdy,
	const b3_f64     cdz,
	      b3_vector *RVec)
{
	b3_f32 dx = fabs(cdx * NOISEMAX + 15000);
	b3_f32 dy = fabs(cdy * NOISEMAX + 15000);
	b3_f32 dz = fabs(cdz * NOISEMAX + 15000);

	RVec->x = b3GradNoise (dx,dy,dz,0);
	RVec->y = b3GradNoise (dx,dy,dz,1);
	RVec->z = b3GradNoise (dx,dy,dz,2);
}

/*************************************************************************
**                                                                      **
**                        marble procedures                             **
**                                                                      **
*************************************************************************/

const b3Color b3Noise::m_MarbleColors[4] =
{
	b3Color( 0.2f, 0.1f, 0.1f ),
	b3Color( 0.1f, 0.8f, 0.2f ),
	b3Color( 1.0f, 0.9f, 1.0f ),
	b3Color( 0.4f, 0.9f, 0.1f )
};

inline void b3Noise::b3MarbleCurve (
	      b3Spline  *Spline,
	      b3_vector *result,
	const b3_f64     x)
{
	register b3_f32  *knots;
	register b3_f64  q;

	knots = Spline->m_Knots;
	q =         x  * knots[Spline->m_Degree] +
	     (1.0 - x) * knots[Spline->m_ControlNum];
	Spline->b3DeBoorOpened (result,0,q);
}

b3_f64 b3Noise::b3Marble(const b3_vector *d)
{
	b3_vector result;

	b3MarbleCurve(
		&m_MarbleSpline,
		&result,
		mSin(d->x + 0.5 * d->y + 0.3 * d->z + b3Turbulence(d)));

	return result.y;
}

void b3Noise::b3OldMarble (const b3_vector *P,b3Color &Color)
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

	Color = b3Color::b3Mix(m_MarbleColors[e], m_MarbleColors[s], frac);
}

/*************************************************************************
**                                                                      **
**                        wood procedures                               **
**                                                                      **
*************************************************************************/

b3_f64 b3Noise::b3Wood(const b3_vector *d)
{
	b3_vector result;
	b3_f64    s;

	s   = 2 * sqrt(d->x * d->x + d->z * d->z) - 0.15 * d->y;
	b3MarbleCurve(&m_WoodSpline,&result,mMod(s + b3Turbulence(d)));

	return 1.0 - result.y;
}

/*************************************************************************
**                                                                      **
**                        other procedural textures                     **
**                                                                      **
*************************************************************************/

const b3Color b3Noise::m_HellColors[4] =
{
	b3Color(0.8f, 0.2f, 0.1f),
	b3Color(0.8f, 0.9f, 0.2f),
	b3Color(1.0f, 0.9f, 1.0f),
	b3Color(0.7f, 0.2f, 0.0f)
};

void b3Noise::b3Hell (const b3_vector *P,b3Color &Color)
{
	b3_f64    t;
	b3_vector Dir;

	b3Vector::b3Scale(&Dir,P,0.08f);

	t = b3Turbulence (&Dir);	
	if (t >= 1) t = 0.99;
	Color = m_HellColors[(int)(t * 4)];
}

b3_f64 b3Noise::b3Wave(const b3_vector *point)
{
	b3_f64 n,q;
	b3_vector v;

	n = b3ImprovedNoise(point->x * 0.5,point->y,point->z);
	q = b3Math::b3Frac(point->x * 0.5 + n,(b3_f64)m_WaveSpline.m_ControlMax);
	m_WaveSpline.b3DeBoorClosed (&v,0,q);

	return mSin(point->y * 10 + v.z * 2);
}

void b3Noise::b3AnimThinFilm(const b3_f64 t, b3_vector *result)
{
	b3_f64   q,div;

	div = (b3_f64)m_WaveSpline.m_ControlNum;
	q   = b3Math::b3Frac(t * 0.05f,div);
	m_WaveSpline.b3DeBoorClosed (result,0,q);
}

b3_f64 b3Noise::b3Granite(const b3_vector *point,const b3_count octaves)
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
		sum  += b3Noise::b3ImprovedNoise(x,y,z) / freq;
		x    += x;
		y    += y;
		z    += z;
		freq += freq; // = freq *= 2;
	}
	return b3Math::b3Limit(sum);
}
