#include "blz3/system/b3Log.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Thread.h"

#include <stdio.h>
#include <stdlib.h>

// This struct is transfered to the thread procedure
typedef struct
{
	b3Display *display;
	b3_count   iter;
	b3_res     xSize;
} mandel_info;

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
	b3_coord     x,y;
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

int main(int argc,char *argv[])
{
	b3_f64       xMin,yMin;
	b3_f64       xMax,yMax;
	b3_f64       xStep,yStep;
	b3_f64       fy;
	b3Display   *display;
	b3MandelRow *row;
	b3_res       xSize,ySize;
	b3_count     iter,CPUs,i;
	b3Thread    *threads;
	mandel_info *infos;

	b3Log_SetLevel(B3LOG_FULL);
	if (argc != 6)
	{
		printf("USAGE:\n");
		printf("%s xMin yMin xMax yMax iterations\n",argv[0]);
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Multithreaded Mandelbrot computing...\n");
		xMin = atof(argv[1]);
		xMax = atof(argv[2]);
		yMin = atof(argv[3]);
		yMax = atof(argv[4]);
		iter = atoi(argv[5]);
		b3PrintF(B3LOG_NORMAL,"Using following values:\n");
		b3PrintF(B3LOG_NORMAL,"Width  %f - %f:\n",xMin,xMax);
		b3PrintF(B3LOG_NORMAL,"Height %f - %f:\n",yMin,yMax);
		b3PrintF(B3LOG_NORMAL,"%lu iterations:\n",iter);

		try
		{
			display = new b3Display();
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
			fy = yMin;
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

			// We want to see the computed picture until we make input
			// into the display window.
			display->b3Wait();
			delete display;
		}
		catch(b3DisplayException *e)
		{
		}
	}
	return 0;
}
