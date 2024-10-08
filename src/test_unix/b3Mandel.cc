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
#include "blz3/system/b3Runtime.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Complex64.h"

#include "b3Mandel.h"

#include <cstdio>
#include <cstdlib>
#include <atomic>

// This struct is transfered to the thread procedure
struct mandel_info
{
	b3Display * display;
	b3_count    iter;
	b3_res      xSize;
};

typedef b3Complex64 b3Cmplx;

using b3AtomicCounter = std::atomic_uint64_t;

/*************************************************************************
**                                                                      **
**                        b3MandelRow implementation                    **
**                                                                      **
*************************************************************************/

// Overload one screen row to compute its pixels
class b3MandelRow : public b3Row
{
	static b3_pkd_color    iter_color[64];
	static b3AtomicCounter iter_counter;
	static b3Base<b3Row>   rows;
	static b3Mutex         row_mutex;

	b3_count iter;
	b3_u64   cnt;
	b3_f64   m_fx, m_fy, m_xStep;

public:
	b3MandelRow(
		b3_coord y,
		b3_res   xSize,
		b3_f64   xStart,
		b3_f64   xStep,
		b3_f64   fy,
		b3_count iterations) : b3Row(y, xSize)
	{
		iter    = iterations;
		cnt     = 0;
		m_fx    = xStart;
		m_fy    = fy;
		m_xStep = xStep;
	}

	// This computes the whole row!
	inline void compute()
	{
		b3_coord           x;
		b3_count           count;

		b3Cmplx  a = 0;
		b3Cmplx  f = b3Cmplx(m_fx, m_fy);

		for (x = 0; x < m_xSize; x++)
		{
			// <!-- Snip!
			// This is some computation to compute the Mandelbrot set.

			// Loop
			a     = 0;
			count = 0;
			do
			{
				a *= a;
				a -= f;
				count++;
			}
			while ((count <= iter) && (a.b3SquareLength() < 4));
			// Snap! --!>

			// Fill in color
			m_buffer[x] = b3Color(count >= iter ? B3_BLACK : iter_color[count & 0x3f]);
			f   += m_xStep;
			cnt += count;
		}
	}

public:
	inline static void b3InitColor()
	{
		b3_pkd_color color = B3_BLACK;

		for (b3_loop i = 0; i < 64; i++)
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

	inline static void b3Init()
	{
		iter_counter = 0;
	}

	static bool compute(void * ptr)
	{
		mandel_info * info = (mandel_info *)ptr;
		b3MandelRow * row;

		do
		{
			// Enter critical section
			{
				b3CriticalSection lock(row_mutex);

				if ((row = (b3MandelRow *)rows.First) != nullptr)
				{
					rows.b3Remove(row);
				}
			}
			// Leave critical section

			if (row != nullptr)
			{
				// We can handle the row for its own!
				row->compute();
				info->display->b3PutRow(row);

				// count iterations
				iter_counter += row->cnt;

				// That's it.
				delete row;
			}
		}
		while (row != nullptr);

		// Reach this if the row list ran empty.
		return 0;
	}

	inline static void b3Append(b3MandelRow * row)
	{
		rows.b3Append(row);
	}

	inline static b3_u64 b3GetIterations()
	{
		return iter_counter;
	}
};

b3_pkd_color     b3MandelRow::iter_color[64];
b3AtomicCounter  b3MandelRow::iter_counter;
b3Base<b3Row>    b3MandelRow::rows;
b3Mutex          b3MandelRow::row_mutex;

void b3Mandel::b3Compute(
	b3Display * display,
	b3_f64     xMin,
	b3_f64     xMax,
	b3_f64     yMin,
	b3_f64     yMax,
	b3_count   iter)
{
	b3_f64       xStep, yStep;
	b3_f64       fy;
	b3MandelRow * row;
	b3_res       xSize, ySize;
	b3_count     CPUs, i;
	b3_u64       count;
	b3Thread  *  threads;
	mandel_info * infos;
	b3Time       timepoint;
	b3_f64       tStart, tEnd, tDiff;

	b3PrintF(B3LOG_NORMAL, "Using following values:\n");
	b3PrintF(B3LOG_NORMAL, "Width  %f - %f.\n", xMin, xMax);
	b3PrintF(B3LOG_NORMAL, "Height %f - %f.\n", yMin, yMax);
	b3PrintF(B3LOG_NORMAL, "Max. %d iterations per pixel.\n", iter);

	display->b3GetRes(xSize, ySize);

	// Determine number of CPU's
	CPUs = b3Runtime::b3GetNumCPUs();
	b3PrintF(B3LOG_NORMAL, "Using %d CPU%s.\n",
		CPUs,
		CPUs > 1 ? "'s" : "");

	// Allocate some instances
	infos       = new mandel_info[CPUs];
	threads     = new b3Thread[CPUs];

	// add rows to list
	fy    = yMin;
	xStep = (xMax - xMin) / (double)xSize;
	yStep = (yMax - yMin) / (double)ySize;
	for (i = 0; i < ySize; i++)
	{
		row = new b3MandelRow(i, xSize, xMin, xStep, fy, iter);
		b3MandelRow::b3Append(row);
		fy += yStep;
	}

	b3PrintF(B3LOG_NORMAL, "Starting threads...\n");
	tStart = timepoint;
	b3MandelRow::b3InitColor();
	b3MandelRow::b3Init();
	for (i = 0; i < CPUs; i++)
	{
		infos[i].xSize   = xSize;
		infos[i].iter    = iter;
		infos[i].display = display;

		threads[i].b3Start(b3MandelRow::compute, &infos[i]);
	}

	// Wait for completion
	b3PrintF(B3LOG_NORMAL, "Waiting for threads...\n");
	for (i = 0; i < CPUs; i++)
	{
		threads[i].b3Wait();
	}

	timepoint.b3Now();
	tEnd  = timepoint;
	tDiff = tEnd - tStart;
	count = b3MandelRow::b3GetIterations();

	b3PrintF(B3LOG_NORMAL, "Computing took %3.3fs.\n", tDiff);
	b3PrintF(B3LOG_NORMAL, "Iterations: %lu\n", count);
	b3PrintF(B3LOG_NORMAL, "%3.3fM iterations/s\n", (b3_f64)count / tDiff / 1000000);
	b3PrintF(B3LOG_NORMAL, "%3.3fM complex ops/s\n", count * 3 / tDiff / 1000000);
	b3PrintF(B3LOG_NORMAL, "%3.3fM mults/s\n", count * 5 / tDiff / 1000000);
	b3PrintF(B3LOG_NORMAL, "%3.3fM adds/s\n", count * 6 / tDiff / 1000000);
	b3PrintF(B3LOG_NORMAL, "%3.3fMFlOp/s\n", count * 11 / tDiff / 1000000);

	// Free what we have allocated.
	delete [] threads;
	delete [] infos;
	b3PrintF(B3LOG_NORMAL, "Done.\n");
}
