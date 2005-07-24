/*
**
**      $Filename:      b3Mandel.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Mandelbrot set renderer
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
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

#include "blz3/b3Config.h"
#include "blz3/system/b3Display.h"

#include "b3Mandel.h"

#include <stdio.h>
#include <stdlib.h>

// This struct is transfered to the thread procedure
struct mandel_info
{
	b3Display *display;
	b3_count   iter;
	b3_res     xSize;
};

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2005/07/24 14:24:36  sm
**	- Optimized Mandelbrot set.
**
**	Revision 1.7  2005/05/05 16:16:09  sm
**	- Added time measure to mandelbrot set computing.
**	
**	Revision 1.6  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.5  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**	
**	Revision 1.4  2001/10/22 14:47:38  sm
**	- Type correction vor b3Base/b3Link. So fixed a bad behaviour
**	  on Windows.
**	- Some minor fixes.
**	
**	Revision 1.3  2001/10/20 16:15:00  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
**	Revision 1.2  2001/10/19 14:46:58  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.1  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3MandelRow implementation                    **
**                                                                      **
*************************************************************************/

#define X 0
#define Y 1

// Overload one screen row to compute its pixels
class b3MandelRow : public b3Row
{
	static b3_pkd_color iter_color[64];

	b3_count iter;
	b3_f64   fx,fy,xStep;

public:
	b3MandelRow(
		b3_coord y,
		b3_res   xSize,
		b3_f64   xStart,
		b3_f64   xStep,
		b3_f64   fy,
		b3_count iterations) : b3Row(y,xSize)
	{
		iter        = iterations;
		this->fx    = xStart;
		this->fy    = fy;
		this->xStep = xStep;
	}

	// This computes the whole row!
	void compute()
	{
		b3_coord     x;
		b3_count     count;
		b3_f64       val[2],quad[2],f[2];
		b3_pkd_color color = 0;
		b3_loop      k;

		f[Y] = fy;
		for (x = 0;x < m_xSize;x++)
		{
			// <!-- Snip!
			// This is some computation to compute the Mandelbrot set.
			count = 0;

			// Init
			for (k = 0;k < 2;k++)
			{
				val[k] = 0;
				quad[k] = 0;
			}
			f[X] = fx;
			
			// Loop
			while ((count < iter) && ((quad[X] + quad[Y]) < 4.0))
			{
				val[Y]  = 2 * val[X]  * val[Y];
				val[X]  =     quad[X] - quad[Y];
				for (k = 0;k < 2;k++)
				{
					val[k] -= f[k];
					quad[k] = val[k] * val[k];
				}
				count++;
			}
			// Snap! --!>

			// now compute color from iterations needed.
			if (count >= iter)
			{
				// The algorithm doesn't converge...
				color = 0; // black!
			}
			else
			{
				color = iter_color[count & 0x3f];
			}

			// Fill in color
			m_buffer[x] = color;
			fx += xStep;
		}
	}

public:
	static void b3InitColor()
	{
		b3_pkd_color color;

		for (b3_loop i = 0;i < 64;i++)
		{
			// Compute pixel color from number of iterations
			switch (i & 0x30)
			{
			case 0x00 : /* blue */
				color = (i & 0x0f) <<  4;
				break;
		
			case 0x10 : /* green */
				color = (i & 0x0f) << 12;
				break;
	
			case 0x20 : /* red */
				color = (i & 0x0f) << 20;
				break;
	
			case 0x30 : /* magenta */
				color = ((i & 0x0f) << 20) | ((i & 0x0f) <<  4);
				break;
			}
			iter_color[i] = color;
		}
	}
};

b3_pkd_color b3MandelRow::iter_color[64];

static b3Base<b3Row> rows;
static b3Mutex       row_mutex;

static b3_u32 compute(void *ptr)
{
	mandel_info *info = (mandel_info *)ptr;
	b3MandelRow *row;
	b3_res       xSize;
	b3_count     iter;

	xSize = info->xSize;
	iter  = info->iter;
	do
	{
		// Enter critical section
		row_mutex.b3Lock();
		if ((row = (b3MandelRow *)rows.First) != null)
		{
			rows.b3Remove(row);
		}
		row_mutex.b3Unlock();
		// Leave critical section

		if (row != null)
		{
			// We can handle the row for its own!
			row->compute();
			info->display->b3PutRow(row);
			delete row;
		}
	}
	while(row != null);

	// Reach this if the row list ran empty.
	return 0;
}

void b3Mandel::b3Compute(
	b3Display *display,
	b3_f64     xMin,
	b3_f64     xMax,
	b3_f64     yMin,
	b3_f64     yMax,
	b3_count   iter)
{
	b3_f64       xStep,yStep;
	b3_f64       fy;
	b3MandelRow *row;
	b3_res       xSize,ySize;
	b3_count     CPUs,i;
	b3Thread    *threads;
	mandel_info *infos;
	b3Time       timepoint;
	b3_f64       tStart,tEnd;

	b3PrintF(B3LOG_NORMAL,"Using following values:\n");
	b3PrintF(B3LOG_NORMAL,"Width  %f - %f:\n",xMin,xMax);
	b3PrintF(B3LOG_NORMAL,"Height %f - %f:\n",yMin,yMax);
	b3PrintF(B3LOG_NORMAL,"%lu iterations:\n",iter);

	display->b3GetRes(xSize,ySize);

	// Determine number of CPU's
	CPUs = b3Runtime::b3GetNumCPUs();
	b3PrintF (B3LOG_NORMAL,"Using %d CPU%s.\n",
		CPUs,
		CPUs > 1 ? "'s" : "");

	// Allocate some instances
	infos       = new mandel_info[CPUs];
	threads     = new b3Thread[CPUs];

	// add rows to list
	fy    = yMin;
	xStep = (xMax - xMin) / (double)xSize;
	yStep = (yMax - yMin) / (double)ySize;
	for (i = 0;i < ySize;i++)
	{
		row = new b3MandelRow(i,xSize,xMin,xStep,fy,iter);
		rows.b3Append(row);
		fy += yStep;
	}

	b3PrintF (B3LOG_NORMAL,"Starting threads...\n");
	tStart = timepoint;
	b3MandelRow::b3InitColor(); 
	for (i = 0;i < CPUs;i++)
	{
		infos[i].xSize   = xSize;
		infos[i].iter    = iter;
		infos[i].display = display;

		threads[i].b3Start(compute,&infos[i]);
	}

	// Wait for completion
	b3PrintF (B3LOG_NORMAL,"Waiting for threads...\n");
	for (i = 0;i < CPUs;i++)
	{
		threads[i].b3Wait();
	}

	timepoint.b3Now();
	tEnd = timepoint;

	b3PrintF(B3LOG_NORMAL,"Computing took %3.3fs.\n",tEnd - tStart);

	// Free what we have allocated.
	delete [] threads;
	delete [] infos;
	b3PrintF (B3LOG_NORMAL,"Done.\n");
}
