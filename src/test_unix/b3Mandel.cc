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

#include "blz3/system/b3Log.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Thread.h"

#include "b3Mandel.h"

#include <stdio.h>
#include <stdlib.h>

// This struct is transfered to the thread procedure
typedef struct
{
	b3Display *display;
	b3_count   iter;
	b3_res     xSize;
} mandel_info;

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

// Overload one screen row to compute its pixels
class b3MandelRow : public b3Row
{
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
		b3_f64       Xval,Yval,Xquad,Yquad;
		b3_pkd_color color;

		for (x = 0;x < xSize;x++)
		{
			// <!-- Snip!
			// This is some computation to compute the Mandelbrot set.
			count = 0;
			Xval  = Yval = Xquad = Yquad = 0;
			while ((count < iter) && (Xquad + Yquad < 4.0))
			{
				Yval  = 2 * Xval  * Yval  - fy;
				Xval  =     Xquad - Yquad - fx;
				Xquad =     Xval  * Xval;
				Yquad =     Yval  * Yval;
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
				// Compute pixel color from number of iterations
				switch (count & 0x30)
				{
				case 0x00 : /* blue */
					color = (count & 0x0f) <<  4;
					break;
		
				case 0x10 : /* green */
					color = (count & 0x0f) << 12;
					break;
		
				case 0x20 : /* red */
					color = (count & 0x0f) << 20;
					break;
		
				case 0x30 : /* magenta */
					color = ((count & 0x0f) << 20) | ((count & 0x0f) <<  4);
					break;
				}
			}

			// Fill in color
			buffer[x] = color;
			fx += xStep;
		}
	}
};

static b3CPU               cpu;
static b3Base<b3MandelRow> rows;
static b3Mutex             row_mutex;

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
		if (row = rows.First)
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

	b3PrintF(B3LOG_NORMAL,"Using following values:\n");
	b3PrintF(B3LOG_NORMAL,"Width  %f - %f:\n",xMin,xMax);
	b3PrintF(B3LOG_NORMAL,"Height %f - %f:\n",yMin,yMax);
	b3PrintF(B3LOG_NORMAL,"%lu iterations:\n",iter);

	display->b3GetRes(xSize,ySize);

	// Determine number of CPU's
	CPUs = cpu.b3GetNumCPU();
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

	// Free what we have allocated.
	delete [] threads;
	delete [] infos;
	b3PrintF (B3LOG_NORMAL,"Done.\n");
}
