/*
**
**      $Filename:      b3Mandel.h $
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

#ifndef B3MANDEL_H
#define B3MANDEL_H

#include "blz3/system/b3Display.h"

class b3Mandel
{
public:
	static void b3Compute(b3Display *display,
		b3_f64   xMin,b3_f64 xMax,
		b3_f64   yMin,b3_f64 yMax,
		b3_count iter);
};

#endif
